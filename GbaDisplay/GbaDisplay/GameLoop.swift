//
//  GameLoop.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import Foundation
import AppKit
import UniformTypeIdentifiers


let VBLANK_TIME_MS : UInt32 = 5
let kMaxRawBytes : Int = 1024 * 1024;

struct SpriteMetadata
{
    var m_num_sprites : Int
    var m_index_buffer_offset : [Int]
    var m_num_pixels : [Int]
}

struct ObjectAttributes
{
    var attr0 : UInt16
    var attr1 : UInt16
    var attr2 : UInt16
    var padding : UInt16
}

func GbaToMetalColor(gba_color : UInt16) -> UInt32
{
    let r : UInt32 = UInt32((gba_color >> 10) & 0b11111) * 256 / 32
    let g : UInt32 = UInt32((gba_color >> 5) & 0b11111) * 256 / 32
    let b : UInt32 = UInt32((gba_color >> 0) & 0b11111) * 256 / 32
    return (r << 24) | (g << 16) | (b << 8)
}

func GetBackgroundShape(control : BGControl) -> (UInt16, UInt16)
{
    switch (control.size)
    {
    case 0b00:
        return (32,32)
    case 0b01:
        return (64,32)
    case 0b10:
        return (32,64)
    case 0b11:
        return (64,64)
    default:
        fatalError("Bad")
    }
}

func GetSpriteShape(attr0 : UInt16, attr1 : UInt16) -> (UInt16, UInt16)
{
    let shape : UInt16 = attr0 >> 14;
    let size : UInt16 = attr1 >> 14;
    switch (shape, size)
    {
    case (0b00, 0b00):
        return (1,1)
    case (0b01, 0b00):
        return (2,1)
    case (0b10, 0b00):
        return (1,2)

    case (0b00, 0b01):
        return (2,2)
    case (0b01, 0b01):
        return (4,1)
    case (0b10, 0b01):
        return (1,4)

    case (0b00, 0b10):
        return (4,4)
    case (0b01, 0b10):
        return (4,2)
    case (0b10, 0b10):
        return (2,4)

    case (0b00, 0b11):
        return (8,8)
    case (0b01, 0b11):
        return (8,4)
    case (0b10, 0b11):
        return (4,8)
        
    default:
        fatalError("Invalid Shape/Size")
    }
}

class GameLoop {
    
    public func load_buffers(
        index_buffer : UnsafeMutablePointer<UInt16>,
        pixel_attr_buffer : UnsafeMutablePointer<PixelAttributes>,
        sprite_attr_buffer : UnsafeMutablePointer<SpriteAttributes>
    )
    {
        out_index_buffer = index_buffer
        out_pixel_attr_buffer = pixel_attr_buffer
        out_sprite_attr_buffer = sprite_attr_buffer
    }
    
    public func start()
    {
        // This has to go before initCpuRunner or else the image comes out weird. Haven't figured out why.
        // in_video_buffer = UnsafeMutablePointer<UInt8>.allocate(capacity : kMaxRawBytes)
        // in_object_attr_buffer = UnsafeMutablePointer<UInt16>.allocate(capacity : kMaxRawBytes)
        // in_palette_buffer = UnsafeMutablePointer<UInt16>.allocate(capacity : kMaxRawBytes)
        
        initCpuRunner()
        timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { [weak self] _ in
            self?.dispatch()
        }
    }
    
    public var sprite_metadata : SpriteMetadata = SpriteMetadata(m_num_sprites: 0, m_index_buffer_offset: [], m_num_pixels: [])
    
    private var timer: Timer?
    
    private var scanLine : uint = 0
    private var triangle_pos : uint = 0
        
    private var out_index_buffer : UnsafeMutablePointer<UInt16>!
    private var out_pixel_attr_buffer : UnsafeMutablePointer<PixelAttributes>!
    private var out_sprite_attr_buffer : UnsafeMutablePointer<SpriteAttributes>!
    
    // From CPU
    private var in_DISPCNT : UInt16 = 0b1 << 6
    private var in_background_registers_0 : Background = Background(ctr: 0, x: 0, y: 0)
    private var in_background_registers_1 : Background = Background(ctr: 0, x: 0, y: 0)
    private var in_background_registers_2 : Background = Background(ctr: 0, x: 0, y: 0)
    private var in_background_registers_3 : Background = Background(ctr: 0, x: 0, y: 0)
    
    private var in_video_buffer : UnsafeMutablePointer<UInt8>!
    private var in_object_attr_buffer : UnsafeMutablePointer<UInt16>!
    private var in_palette_buffer : UnsafeMutablePointer<UInt16>!

    private let CpuRunnerHandle : CpuRunnerHandle = CpuRunner_Create()

    private func initCpuRunner()
    {
        let openPanel = NSOpenPanel()
        openPanel.allowedContentTypes = [UTType(filenameExtension: "gba")!]
        openPanel.allowsMultipleSelection = false
        openPanel.canChooseDirectories = false
        openPanel.canChooseFiles = true

        if openPanel.runModal() == .OK, let url = openPanel.url {
            // Prepare argv with the selected file path
            let args: [String] = [
                "GbaDisplay", // argv[0]
                url.path     // user-selected file path
            ]

            var cArgs = args.map { strdup($0) }
            defer { cArgs.forEach { free($0) } }

            // Pass pointer to first c string pointer
            CpuRunner_Init(CpuRunnerHandle, Int32(cArgs.count), &cArgs)
        } else {
            print("User cancelled or no file selected")
        }
        
        DispatchQueue.global(qos: .userInitiated).async {
            CpuRunner_Run(self.CpuRunnerHandle)
        }

        in_video_buffer = GetByteMemoryPtr(handle: CpuRunnerHandle, address: 0x06000000)
        in_object_attr_buffer = GetHalfWordMemoryPtr(handle: CpuRunnerHandle, address: 0x07000000)
        in_palette_buffer = GetHalfWordMemoryPtr(handle: CpuRunnerHandle, address: 0x05000000)
    }

    private func AddPixelToBuffers(pixel_attribute: PixelAttributes, out_pixel_attr_buffer_size: Int, out_index_buffer_size: Int) -> (Int, Int)
    {
        out_pixel_attr_buffer[out_pixel_attr_buffer_size] = pixel_attribute

        out_index_buffer[out_index_buffer_size + 0] = 0
        out_index_buffer[out_index_buffer_size + 1] = 1
        out_index_buffer[out_index_buffer_size + 2] = 2
        out_index_buffer[out_index_buffer_size + 3] = 0
        out_index_buffer[out_index_buffer_size + 4] = 2
        out_index_buffer[out_index_buffer_size + 5] = 3
        
        return (out_pixel_attr_buffer_size + 1, out_index_buffer_size + 6)
    }
    
    private func GbaToMetal_Mode0() -> SpriteMetadata
    {
        var sprite_metadata : SpriteMetadata = SpriteMetadata(
            m_num_sprites: 0, m_index_buffer_offset: [], m_num_pixels: []
        );
        
        var out_index_buffer_size : Int = 0
        var out_pixel_attr_buffer_size : Int = 0

        // For each background
        let backgrounds = [in_background_registers_0, in_background_registers_1, in_background_registers_2, in_background_registers_3]
        for (background_idx, background) in backgrounds.enumerated()
        {
            if ((in_DISPCNT >> (8 + background_idx)) & 0b1) == 0
            {
                continue
            }
            
            let sprite_id : Int = sprite_metadata.m_num_sprites;
            sprite_metadata.m_num_sprites += 1
                        
            let (tile_width, tile_height) : (UInt16, UInt16) = GetBackgroundShape(control : background.in_REG_BGxCNT)

            // Get number of tiles to processes
            let num_tiles : UInt16 = tile_width * tile_height
            
            // Get tile type
            let bits_per_pixel : Int = !background.in_REG_BGxCNT.is8bpp ? 4 : 8
                        
            // Set up sprite metadata
            sprite_metadata.m_num_pixels.append(Int(num_tiles) * 64)
            sprite_metadata.m_index_buffer_offset.append(out_index_buffer_size)

            // Add entry for sprite attribute buffer
            let sprite_attributes : SpriteAttributes = SpriteAttributes(
                offset_x: -1 * Int16(background.in_REG_BGxHOFS.offset),
                offset_y: -1 * Int16(background.in_REG_BGxVOFS.offset),
                depth: 1,
                tiles_width: tile_width,
                tiles_height: tile_height
            )
            out_sprite_attr_buffer[sprite_id] = sprite_attributes
            
            // Get base index in in_video_buffer
            let in_video_buffer_char_block_base_idx : Int = Int(background.in_REG_BGxCNT.charBlock) * 0x4000
            let in_video_buffer_screen_block_base_idx : Int = Int(background.in_REG_BGxCNT.screenBlock) * 0x800
            
            // For each 4 bits (8 bits if d-tile type) in in_video_buffer, add a pixel to pixel_attr_buffer and a pixel to the index buffer
            let begin_out_pixel_attr_buffer_size : UInt32 = UInt32(out_pixel_attr_buffer_size)
            
            for screen_block_idx in stride(from: in_video_buffer_screen_block_base_idx, to: in_video_buffer_screen_block_base_idx + Int(num_tiles) * 2, by : 2)
            {
                let screen_entry : UInt16 = (UInt16(in_video_buffer[screen_block_idx + 1]) >> 8) | UInt16(in_video_buffer[screen_block_idx])
                let tile_idx : Int = in_video_buffer_char_block_base_idx + Int(screen_entry & 0x1FF)
                
                for i in 0..<64
                {
                    var pixel_attribute : PixelAttributes
                    if (bits_per_pixel == 4)
                    {
                        // 4bpp
                        fatalError("Not Implemented")
                    }
                    else
                    {
                        // 8bpp
                        let palette_idx : UInt8 = in_video_buffer[tile_idx + i]
                        let color : UInt16 = in_palette_buffer[Int(palette_idx)]
                        pixel_attribute = PixelAttributes(
                            color: GbaToMetalColor(gba_color : color),
                            sprite_attribute: UInt32(sprite_id),
                            pixel_buffer_start_offset: begin_out_pixel_attr_buffer_size
                        )
                    }
                    
                    (out_pixel_attr_buffer_size, out_index_buffer_size) = AddPixelToBuffers(
                        pixel_attribute: pixel_attribute,
                        out_pixel_attr_buffer_size: out_pixel_attr_buffer_size,
                        out_index_buffer_size: out_index_buffer_size)
                }
            }
        }

        // For each oam
        for oam_index in stride(from: 0, to: 1024, by : 4)
        {
            let oam_attr = ObjectAttributes(
                attr0 : in_object_attr_buffer[oam_index],
                attr1 : in_object_attr_buffer[oam_index + 1],
                attr2 : in_object_attr_buffer[oam_index + 2],
                padding : in_object_attr_buffer[oam_index + 3]
            )
            
            // Sprite is disabled
            if ((in_DISPCNT >> 12) & 0b1 == 0 || (oam_attr.attr0 >> 8 & 0b11) == 0b10)
            {
                continue
            }
            
            let sprite_id : Int = sprite_metadata.m_num_sprites;
            sprite_metadata.m_num_sprites += 1
                        
            let (tile_width, tile_height) : (UInt16, UInt16) = GetSpriteShape(attr0 : oam_attr.attr0, attr1 : oam_attr.attr1)

            // Get number of tiles to processes
            let num_tiles : UInt16 = tile_width * tile_height
            
            // Get tile type
            let bits_per_pixel : Int = ((oam_attr.attr0 >> 13) & 0b1) == 0 ? 4 : 8
            
            // Calculate number of bytes to process in in_video_buffer
            let bytes_to_process = Int(num_tiles) * 8 * bits_per_pixel;
            
            // Set up sprite metadata
            sprite_metadata.m_num_pixels.append(Int(num_tiles) * 64)
            sprite_metadata.m_index_buffer_offset.append(out_index_buffer_size)

            // Add entry for sprite attribute buffer
            let sprite_attributes : SpriteAttributes = SpriteAttributes(
                offset_x: Int16(oam_attr.attr1) & 0x1FF,
                offset_y: Int16(oam_attr.attr0) & 0x0FF,
                depth: 0,
                tiles_width: tile_width,
                tiles_height: tile_height
            )
            out_sprite_attr_buffer[sprite_id] = sprite_attributes
            
            // Get base index in in_video_buffer
            let in_video_buffer_base_idx : Int = Int(oam_attr.attr2 & 0x3FF)
            
            // For each 4 bits (8 bits if d-tile type) in in_video_buffer, add a pixel to pixel_attr_buffer and a pixel to the index buffer
            let begin_out_pixel_attr_buffer_size : UInt32 = UInt32(out_pixel_attr_buffer_size)
            
            if ((in_DISPCNT >> 6) & 0b1 == 0)
            {
                // 2d Tile Mapping
                fatalError("Not Implemented")
            }
            else
            {
                // 1d Tile Mapping
                for in_video_buffer_idx in stride(from: in_video_buffer_base_idx, to: in_video_buffer_base_idx + bytes_to_process, by : 1)
                {
                    // Add pixel attributes
                    var pixel_attribute : PixelAttributes;
                    if (bits_per_pixel == 4)
                    {
                        // 4bpp
                        fatalError("Not Implemented")
                    }
                    else
                    {
                        // 8bpp
                        let palette_idx : UInt8 = in_video_buffer[0xC000 + in_video_buffer_idx]
                        let color : UInt16 = in_palette_buffer[256 + Int(palette_idx)]
                        pixel_attribute = PixelAttributes(
                            color: GbaToMetalColor(gba_color : color),
                            sprite_attribute: UInt32(sprite_id),
                            pixel_buffer_start_offset: begin_out_pixel_attr_buffer_size
                        )
                    }
                    
                    (out_pixel_attr_buffer_size, out_index_buffer_size) = AddPixelToBuffers(
                        pixel_attribute: pixel_attribute,
                        out_pixel_attr_buffer_size: out_pixel_attr_buffer_size,
                        out_index_buffer_size: out_index_buffer_size)
                }
            }
        }
        
        if out_index_buffer_size >= kMaxRawBytes
        {
            fatalError("out_index_buffer_size exceeded size")
        }
        if out_pixel_attr_buffer_size >= kMaxRawBytes
        {
            fatalError("out_pixel_attr_buffer_size exceeded size")
        }
        if sprite_metadata.m_num_sprites >= kMaxRawBytes
        {
            fatalError("out_sprite_attr_buffer_size exceeded size")
        }
                
        return sprite_metadata
    }
    
    /// The output format is 1d memory mapped tiles.
    /// Each instance in the index buffer is 1 pixel. A tile consists of 64 pixels. A sprite will consist of tile width \* tile length tiles.
    private func GbaToMetal() -> SpriteMetadata
    {
        if (in_DISPCNT == 0)
        {
            return sprite_metadata;
        }
        
        // Mode
        if (in_DISPCNT & 0b111 == 0)
        {
            return GbaToMetal_Mode0()
        }
        else
        {
            fatalError("Unimplemented")
        }
    }
    
    private func createFakeData()
    {
        in_DISPCNT = 0x40
        in_DISPCNT |= (0b1 << 8)
        
        in_background_registers_0.in_REG_BGxCNT.is8bpp = true
        
        var raw_in_video_buffer: [UInt8] = []
        // Load background data
        for _ in 0..<0xC000
        {
            raw_in_video_buffer.append(0)
        }
        
        // Sprite data
        raw_in_video_buffer.append(contentsOf: [
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 2, 1, 1, 2, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 0, 1, 0, 1, 0, 1,  // Tentacles
            0, 1, 0, 1, 0, 1, 0, 1,
            0, 1, 0, 1, 0, 1, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 0
        ])
        
        var raw_in_object_attr_buffer: [UInt16] = []
        
        raw_in_object_attr_buffer.append(0x2001)
        raw_in_object_attr_buffer.append((0b01 << 14) | 0x0000)
        raw_in_object_attr_buffer.append(0x0000)
        raw_in_object_attr_buffer.append(0x0000)
        
        raw_in_object_attr_buffer.append(0x2020)
        raw_in_object_attr_buffer.append((0b01 << 14) | 0x0000)
        raw_in_object_attr_buffer.append(0x0000)
        raw_in_object_attr_buffer.append(0x0000)
        for _ in 4...1024
        {
            // Disable all others.
            raw_in_object_attr_buffer.append(0b10 << 8)
        }
        var raw_palette_buffer: [UInt16] = []
        raw_palette_buffer.append(0x1F)
        for _ in 1..<256
        {
            raw_palette_buffer.append(0)
        }
        raw_palette_buffer.append(0xFF00)
        raw_palette_buffer.append(0x00FF)
        
        in_video_buffer.update(from: raw_in_video_buffer, count: raw_in_video_buffer.count)
        in_object_attr_buffer.update(from: raw_in_object_attr_buffer, count: raw_in_object_attr_buffer.count)
        in_palette_buffer.update(from: raw_palette_buffer, count: raw_palette_buffer.count)
    }

    private func PullGbaData()
    {
        let IO_MEMORY = GetHalfWordMemoryPtr(handle: CpuRunnerHandle, address: 0x04000000)

        in_DISPCNT = IO_MEMORY[0x0/2]
        in_background_registers_0 = Background(ctr: IO_MEMORY[0x8/2], x: IO_MEMORY[0x10/2], y: IO_MEMORY[0x12/2])
        in_background_registers_1  = Background(ctr: IO_MEMORY[0xa/2], x: IO_MEMORY[0x14/2], y: IO_MEMORY[0x16/2])
        in_background_registers_2 = Background(ctr: IO_MEMORY[0xc/2], x: IO_MEMORY[0x18/2], y: IO_MEMORY[0x1a/2])
        in_background_registers_3 = Background(ctr: IO_MEMORY[0xe/2], x: IO_MEMORY[0x1c/2], y: IO_MEMORY[0x1e/2])
    }

    private func SignalUpdateBuffersReady()
    {
        let IO_MEMORY = GetHalfWordMemoryPtr(handle: CpuRunnerHandle, address: 0x04000000)
        let VCOUNT = IO_MEMORY.advanced(by: 0x6 / 2)
        VCOUNT.pointee = 159
    }

    private func SignalDoNotUpdateBuffers()
    {
        let IO_MEMORY = GetHalfWordMemoryPtr(handle: CpuRunnerHandle, address: 0x04000000)
        let VCOUNT = IO_MEMORY.advanced(by: 0x6 / 2)
        VCOUNT.pointee = 0
    }
    
    private func dispatch()
    {
        // Set scanline to indicate to CPU that VRAM can be updated.
        SignalUpdateBuffersReady()
        
        // Wait for time for CPU to update buffers.
        usleep(VBLANK_TIME_MS * 1000);
        
        // Set scanline to indicate to CPU to not update buffers until the next dispatch.
        SignalDoNotUpdateBuffers()
        
        usleep(VBLANK_TIME_MS * 1000);
        
        // Do action
        // createFakeData()
        PullGbaData()
        sprite_metadata = GbaToMetal()
    }
    
}

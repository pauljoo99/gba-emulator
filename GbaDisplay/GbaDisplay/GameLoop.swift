//
//  GameLoop.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import Foundation

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
    
    private var timer: Timer?
    
    var scanLine : uint = 0
    var triangle_pos : uint = 0
    
    var in_video_buffer : UnsafeMutablePointer<UInt8>!
    var in_object_attr_buffer : UnsafeMutablePointer<UInt16>!
    var in_palette_buffer : UnsafeMutablePointer<UInt32>!
    
    var out_index_buffer : UnsafeMutablePointer<UInt16>!
    var out_pixel_attr_buffer : UnsafeMutablePointer<PixelAttributes>!
    var out_sprite_attr_buffer : UnsafeMutablePointer<SpriteAttributes>!
    
    var in_DISPCNT : UInt32 = 0
    
    var sprite_metadata : SpriteMetadata = SpriteMetadata(m_num_sprites: 0, m_index_buffer_offset: [], m_num_pixels: [])

    func load_buffers(
        index_buffer : UnsafeMutablePointer<UInt16>,
        pixel_attr_buffer : UnsafeMutablePointer<PixelAttributes>,
        sprite_attr_buffer : UnsafeMutablePointer<SpriteAttributes>
    )
    {
        out_index_buffer = index_buffer
        out_pixel_attr_buffer = pixel_attr_buffer
        out_sprite_attr_buffer = sprite_attr_buffer
        
        in_video_buffer = UnsafeMutablePointer<UInt8>.allocate(capacity : kMaxRawBytes)
        in_object_attr_buffer = UnsafeMutablePointer<UInt16>.allocate(capacity : kMaxRawBytes)
        in_palette_buffer = UnsafeMutablePointer<UInt32>.allocate(capacity : kMaxRawBytes)
    }
    
    func run()
    {
        timer = Timer.scheduledTimer(withTimeInterval: 1.0 / 60.0, repeats: true) { [weak self] _ in
            self?.dispatch()
        }
    }
    
    func GbatToMetal_Mode0() -> SpriteMetadata
    {
        var sprite_metadata : SpriteMetadata = SpriteMetadata(
            m_num_sprites: 0, m_index_buffer_offset: [], m_num_pixels: []
        );
        
        var out_index_buffer_size : Int = 0
        var out_pixel_attr_buffer_size : Int = 0
                
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
            if ((oam_attr.attr0 >> 8 & 0b11) == 0b10)
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
                offset_x: oam_attr.attr1 & 0x1FF,
                offset_y: oam_attr.attr0 & 0x0FF,
                tiles_width: tile_width,
                tiles_height: tile_height
            )
            out_sprite_attr_buffer[sprite_id] = sprite_attributes
            
            // Get base index in in_video_buffer
            let in_video_buffer_base_idx : Int = Int(oam_attr.attr2 & 0x3FF)
            
            // For each 4 bits (8 bits if d-tile type) in in_video_buffer, add a pixel to pixel_attr_buffer and a pixel to the index buffer
            for in_video_buffer_idx in stride(from: in_video_buffer_base_idx, to: in_video_buffer_base_idx + bytes_to_process, by : 1)
            {
                // Add pixel attributes
                if (bits_per_pixel == 4)
                {
                    fatalError("Not Implemented")
                }
                
                if (bits_per_pixel == 8)
                {
                    let palette_idx : UInt8 = in_video_buffer[in_video_buffer_idx]
                    let color : UInt32 = in_palette_buffer[128 + Int(palette_idx)]
                    let pixel_attribute = PixelAttributes(color: color, sprite_attribute: UInt32(sprite_id))
                    out_pixel_attr_buffer[out_pixel_attr_buffer_size] = pixel_attribute
                    out_pixel_attr_buffer_size += 1
                }
                
                // Add pixel to index buffer
                out_index_buffer[out_index_buffer_size + 0] = 0
                out_index_buffer[out_index_buffer_size + 1] = 1
                out_index_buffer[out_index_buffer_size + 2] = 2
                out_index_buffer[out_index_buffer_size + 3] = 0
                out_index_buffer[out_index_buffer_size + 4] = 2
                out_index_buffer[out_index_buffer_size + 5] = 3
                out_index_buffer_size += 6
            }
        }
        return sprite_metadata
    }
    
    /// The output format is 1d memory mapped tiles.
    /// Each instance in the index buffer is 1 pixel. A tile consists of 64 pixels. A sprite will consist of tile width \* tile length tiles.
    func GbaToMetal(
        in_DISPCNT : UInt32
    ) -> SpriteMetadata
    {
        // Mode
        if (in_DISPCNT & 0b111 == 0)
        {
            // 2-dimensional
            if ((in_DISPCNT >> 6) & 0b1 == 0)
            {
                fatalError("Unimplemented")
            }
            else
            {
                // 1-dimensional
                return GbatToMetal_Mode0()
            }
        }
        else
        {
            fatalError("Unimplemented")
        }
    }
    
    func createFakeData()
    {
        in_DISPCNT = 0x40
        
        var raw_in_video_buffer: [UInt8] = []
        for _ in 0..<64
        {
            // Red tile
            raw_in_video_buffer.append(0x00)
        }
        for _ in 0..<64
        {
            // Green tile
            raw_in_video_buffer.append(0x01)
        }
        for _ in 0..<64
        {
            // Green tile
            raw_in_video_buffer.append(0x01)
        }
        for _ in 0..<32
        {
            // Red tile
            raw_in_video_buffer.append(0x00)
        }
        for _ in 0..<32
        {
            // Green tile
            raw_in_video_buffer.append(0x01)
        }
        var raw_in_object_attr_buffer: [UInt16] = []
        
        raw_in_object_attr_buffer.append(0x2000)
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
        var raw_palette_buffer: [UInt32] = []
        for _ in 0..<128
        {
            raw_palette_buffer.append(0)
        }
        raw_palette_buffer.append(0xFF0000FF)
        raw_palette_buffer.append(0x00FF00FF)
        
        in_video_buffer.update(from: raw_in_video_buffer, count: raw_in_video_buffer.count)
        in_object_attr_buffer.update(from: raw_in_object_attr_buffer, count: raw_in_object_attr_buffer.count)
        in_palette_buffer.update(from: raw_palette_buffer, count: raw_palette_buffer.count)

    }
    
    func dispatch()
    {
        // Set scanline to indicate to CPU that VRAM can be updated.
        scanLine = 160;
        
        // Wait for time for CPU to update buffers.
        usleep(VBLANK_TIME_MS * 1000);
        
        // Set scanline to indicate to CPU to not update buffers until the next dispatch.
        scanLine = 0;
        
        // Do action
        createFakeData()
        
        sprite_metadata = GbaToMetal(
            in_DISPCNT : in_DISPCNT
        )
    }
    
}

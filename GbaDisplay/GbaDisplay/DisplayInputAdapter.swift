//
//  DisplayInputAdapter.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/27/25.
//

import Foundation
import MetalKit

struct Vertex {
    var position_px: SIMD2<Float>
    var texCoord: SIMD2<UInt32>
}

func MakeTileVertexBuffer(device: MTLDevice) -> MTLBuffer!
{
    // A 8x8 quad in pixel space.
    let vertices : [Vertex] = [
        // Position
        Vertex(position_px: [0, 0], texCoord: [0, 0]), // Top-left
        Vertex(position_px: [0, 8], texCoord: [0, 1]), // Bottom-left
        Vertex(position_px: [8, 0], texCoord: [1, 0]), // Top-right
        Vertex(position_px: [8, 8], texCoord: [1, 1])  // Bottom-right
    ]

    return device.makeBuffer(bytes: vertices,
                                     length: kMaxSizeBuffer,
                                     options: [])
    
}

func FillTileTextures(
    handle: CpuRunnerHandle,
    texture: MTLTexture)
{
    let tile_base_byte_ptr = GetByteMemoryPtr(handle: handle, address: 0x06000000)
    let texture_word_ptr = texture.buffer!.contents().assumingMemoryBound(to: UInt32.self)
    
    for tile_i in 0x0..<0x18000
    {
        texture_word_ptr[tile_i] = UInt32(tile_base_byte_ptr[tile_i])
    }
}

func FillPaletteBuffer(handle: CpuRunnerHandle, palette_buffer: MTLBuffer)
{
    let palette_halfword_ptr = GetHalfWordMemoryPtr(handle: handle, address: 0x05000200)
    palette_buffer.contents().copyMemory(from: UnsafeRawPointer(palette_halfword_ptr), byteCount: 0x200)
}

func FillOamAndTileBuffers(
    handle: CpuRunnerHandle,
    index_buffer: MTLBuffer,
    oam_buffer: MTLBuffer,
    oam_id_buffer: MTLBuffer,
    base_tile_instance_id_buffer: MTLBuffer)
{
    let max_num_oams = 0x200 / MemoryLayout<Oam>.size
    let oam_memory_byte_ptr = GetByteMemoryPtr(handle: handle, address: 0x07000000)
    let oam_memory_ptr = UnsafeMutableRawPointer(oam_memory_byte_ptr).bindMemory(to: Oam.self, capacity: 0x400 / MemoryLayout<Oam>.size)
    
    let oam_buffer_ptr = UnsafeMutableRawPointer(oam_buffer.contents()).bindMemory(to: Oam.self, capacity: kMaxRawBytes)
    let base_tile_instance_id_ptr = UnsafeMutableRawPointer(base_tile_instance_id_buffer.contents()).bindMemory(to: UInt16.self, capacity: kMaxRawBytes)
    let oam_id_ptr = UnsafeMutableRawPointer(oam_id_buffer.contents()).bindMemory(to: UInt16.self, capacity: kMaxRawBytes)
    
    let index_buffer_ptr = index_buffer.contents().bindMemory(to: UInt16.self, capacity: kMaxSizeBuffer)
    let quad : [UInt16] = [
        0, 1, 2,
        1, 2, 3,
    ]
    var base_tile : Int = 0
    var num_oams : Int = 0
    for oam_i in 0..<max_num_oams
    {
        // Disabled
        if (oam_memory_ptr[oam_i].mode != 0b10)
        {
            continue;
        }

        // Set oam
        oam_buffer_ptr[num_oams] = oam_memory_ptr[oam_i]
        num_oams += 1
        
        let oam_num_tiles = Int(oam_memory_ptr[oam_i].widthPx) * Int(oam_memory_ptr[oam_i].heightPx) / 64
        for t in base_tile..<oam_num_tiles
        {
            // Set Index Buffer with Tiles
            for q in 0..<6
            {
                index_buffer_ptr[t * 6 + q] = quad[q]
            }
            
            // Set base tile instance for each tile
            base_tile_instance_id_ptr[t] = UInt16(base_tile)
            
            // Set oam id for each tile
            oam_id_ptr[t] = UInt16(oam_i)
        }
        
        base_tile += oam_num_tiles
    }
}

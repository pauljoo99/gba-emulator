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
    var texCoord: SIMD2<Float>
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

func FillOamAndTileBuffers(
    device: MTLDevice,
    handle: CpuRunnerHandle,
    index_buffer: MTLBuffer,
    oam_buffer: MTLBuffer,
    oam_id_buffer: MTLBuffer,
    base_tile_instance_id_buffer: MTLBuffer)
{
    let max_num_oams = 0x200 / MemoryLayout<Oam>.size
    let oam_memory_byte_ptr = GetByteMemoryPtr(handle: handle, address: 0x07000000)
    let oam_memory_ptr = UnsafeMutableRawPointer(oam_memory_byte_ptr).bindMemory(to: Oam.self, capacity: 0x400 / MemoryLayout<Oam>.size)
    
    let index_buffer_ptr = index_buffer.contents().bindMemory(to: UInt16.self, capacity: kMaxSizeBuffer)
    let quad : [UInt16] = [
        0, 1, 2,
        1, 2, 3,
    ]
    var num_tiles : Int = 0
    for oam in 0..<max_num_oams
    {
        // Disabled
        if (oam_memory_ptr[oam].mode != 0b10)
        {
            continue;
        }
        
        num_tiles += Int(oam_memory_ptr[oam].widthPx) * Int(oam_memory_ptr[oam].heightPx) / 64
    }
    for t in 0..<num_tiles
    {
        for q in 0..<6
        {
            index_buffer_ptr[t * 6 + q] = quad[q]
        }
    }
}

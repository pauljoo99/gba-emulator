//
//  TileRenderer.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/25/25.
//

import MetalKit

import AppKit
import UniformTypeIdentifiers


class TileRenderer: NSObject, MTKViewDelegate {
    
    // Device Setup
    private var device: MTLDevice!
    private var commandQueue: MTLCommandQueue!
    private var pipelineState: MTLRenderPipelineState!
    
    // Buffers to be used by the GPU
    private var vertexBuffer: MTLBuffer!
    private var indexBuffer: MTLBuffer!
    private var oamBuffer: MTLBuffer!
    private var oamIdBuffer: MTLBuffer!
    private var baseTileIdBuffer: MTLBuffer!
    
    // Texture
    private var mtlTexture : MTLTexture!
    private var paletteBuffer: MTLBuffer!

    func setupDevice(mtkView: MTKView)
    {
        self.device = MTLCreateSystemDefaultDevice()
        mtkView.device = device
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0)
        mtkView.delegate = self
        commandQueue = device.makeCommandQueue()
    }
    
    func setupRealMemoryDemoBuffers()
    {
        let openPanel = NSOpenPanel()
        openPanel.allowedContentTypes = [UTType(filenameExtension: "bin")!]
        openPanel.allowsMultipleSelection = false
        openPanel.canChooseDirectories = false
        openPanel.canChooseFiles = true

        let memory_data_ptr : UnsafeMutableRawPointer = UnsafeMutableRawPointer.allocate(byteCount: kMaxRawBytes * kMaxRawBytes, alignment: 4)
        if openPanel.runModal() == .OK, let url = openPanel.url {
            do {
                let data = try Data(contentsOf: url)
                data.copyBytes(to: memory_data_ptr.assumingMemoryBound(to: UInt8.self), count: data.count)
            }
            catch
            {
                print("Not copied")
            }
        } else {
            print("User cancelled or no file selected")
        }
        
        let textureDescriptor = MTLTextureDescriptor();
        textureDescriptor.textureType = MTLTextureType.type2DArray
        textureDescriptor.width = 8
        textureDescriptor.height = 8
        textureDescriptor.arrayLength = 128
        textureDescriptor.pixelFormat = MTLPixelFormat.r32Uint
        
        mtlTexture = device.makeTexture(descriptor: textureDescriptor)
        vertexBuffer = device.makeBuffer(length: kMaxSizeBuffer,
                                         options: [])
        oamBuffer = device.makeBuffer(length: kMaxSizeBuffer,
                                         options: [])
        oamIdBuffer = device.makeBuffer(length: kMaxSizeBuffer,
                                         options: [])
        baseTileIdBuffer = device.makeBuffer(length: kMaxSizeBuffer,
                                         options: [])
        paletteBuffer = device.makeBuffer(length: kMaxSizeBuffer,
                                         options: [])
        indexBuffer = device.makeBuffer(length: kMaxSizeBuffer, options: [])

        FillVertexBuffer(vertex_buffer: vertexBuffer)
        FillTileTextures(memory_ptr: memory_data_ptr, texture: mtlTexture)
        FillPaletteBuffer(memory_ptr: memory_data_ptr, palette_buffer: paletteBuffer)
        FillOamAndTileBuffers(memory_ptr: memory_data_ptr, index_buffer: indexBuffer, oam_buffer: oamBuffer, oam_id_buffer: oamIdBuffer, base_tile_instance_id_buffer: baseTileIdBuffer)
    }
    
    func setupDemoBuffers()
    {
        let textureDescriptor = MTLTextureDescriptor();
        textureDescriptor.textureType = MTLTextureType.type2DArray
        textureDescriptor.width = 8
        textureDescriptor.height = 8
        textureDescriptor.arrayLength = 128
        textureDescriptor.pixelFormat = MTLPixelFormat.r32Uint
        
        mtlTexture = device.makeTexture(descriptor: textureDescriptor)
        let red_tile = UnsafeMutableRawPointer.allocate(byteCount: 4 * 64, alignment: 4)
        red_tile.initializeMemory(as: UInt32.self, repeating: 0, count: 64)
        let yellow_tile = UnsafeMutableRawPointer.allocate(byteCount: 4 * 64, alignment: 4)
        yellow_tile.initializeMemory(as: UInt32.self, repeating: 1, count: 64)
        
        let region = MTLRegionMake2D(0, 0, 8, 8)
        
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 0, withBytes: red_tile, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 1, withBytes: yellow_tile, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 2, withBytes: yellow_tile, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        
        // A 8x8 quad in pixel space.
        let pixelVertices : [Vertex] = [
            // Position
            Vertex(position_px: [0, 0], texCoord: [0, 0]), // Top-left
            Vertex(position_px: [0, 8], texCoord: [0, 1]), // Bottom-left
            Vertex(position_px: [8, 0], texCoord: [1, 0]), // Top-right
            Vertex(position_px: [8, 8], texCoord: [1, 1])  // Bottom-right
        ]
        
        // Require one index per quad.
        let indices : [UInt16] = [
            // First quad
            0, 1, 2,
            1, 2, 3,
            
            // Second quad
            0, 1, 2,
            1, 2, 3,
            
            // third quad
            0, 1, 2,
            1, 2, 3,
        ]
        
        // Access by oam id.
        let oams : [UInt64] = [
            CreateOam(x: 120, y: 0, shape: 1, size: 0, tile_id: 0),
            CreateOam(x: 120, y: 80, shape: 0, size: 0, tile_id: 2),
        ]
        
        // Index by absolute tile instance id.
        let oam_ids : [UInt16] = [
            0, 0, 1
        ]
        
        // Index by absolute tile instance id.
        let base_tile_ids : [UInt16] = [
            0, 0, 2
        ]
        
        // Index by palette buffer index in texture data.
        let palette_buffer : [UInt16] = [
            0x00FF, 0xFF00
        ]
        
        vertexBuffer = device.makeBuffer(bytes: pixelVertices,
                                         length: kMaxSizeBuffer,
                                         options: [])
        oamBuffer = device.makeBuffer(bytes: oams,
                                         length: kMaxSizeBuffer,
                                         options: [])
        oamIdBuffer = device.makeBuffer(bytes: oam_ids,
                                         length: kMaxSizeBuffer,
                                         options: [])
        baseTileIdBuffer = device.makeBuffer(bytes: base_tile_ids,
                                         length: kMaxSizeBuffer,
                                         options: [])
        paletteBuffer = device.makeBuffer(bytes: palette_buffer,
                                         length: kMaxSizeBuffer,
                                         options: [])
        indexBuffer = device.makeBuffer(bytes: indices, length: kMaxSizeBuffer, options: [])
    }
    
    func setupPipeline(mtkView: MTKView)
    {
        let library = device.makeDefaultLibrary()
        let vertexFunc = library?.makeFunction(name: "tile_2d_vertex_main")
        let fragFunc = library?.makeFunction(name: "tile_fragment_main")

        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.vertexFunction = vertexFunc
        pipelineDescriptor.fragmentFunction = fragFunc
        pipelineDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat

        pipelineState = try? device.makeRenderPipelineState(descriptor: pipelineDescriptor)
    }
    
    init(mtkView: MTKView) {
        
        // Setup devices
        super.init()
        setupDevice(mtkView : mtkView)
        setupPipeline(mtkView : mtkView)
        setupRealMemoryDemoBuffers()
        // setupDemoBuffers()
    }
    
    func draw(in view: MTKView) {
        guard let drawable = view.currentDrawable,
              let descriptor = view.currentRenderPassDescriptor else { return }

        let commandBuffer = commandQueue.makeCommandBuffer()
        let encoder = commandBuffer?.makeRenderCommandEncoder(descriptor: descriptor)

        encoder?.setRenderPipelineState(pipelineState)
        encoder?.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        encoder?.setVertexBuffer(oamBuffer, offset: 0, index: 1)
        encoder?.setVertexBuffer(oamIdBuffer, offset: 0, index: 2)
        encoder?.setVertexBuffer(baseTileIdBuffer, offset: 0, index: 3)
        encoder?.setFragmentTexture(mtlTexture, index: 0)
        encoder?.setFragmentBuffer(paletteBuffer, offset: 0, index: 0)
        
        var tile_instance_id : Int = 0
        for oam_i in 0..<128
        {
            
            let oam : Oam = oamBuffer.contents().assumingMemoryBound(to: Oam.self)[oam_i]
            
            // Find out how oams that are 0 are ignored.
            if (oam.attr0 == 0 && oam.attr1 == 0)
            {
                continue;
            }
            
            let num_tiles = oam.widthPx * oam.heightPx / 64
            encoder?.drawIndexedPrimitives(type: .triangle,
                                           indexCount: 6,
                                           indexType: .uint16,
                                           indexBuffer: indexBuffer,
                                           indexBufferOffset: oam_i * 6 * 2,
                                           instanceCount:  Int(num_tiles),
                                           baseVertex: 0,
                                           baseInstance: tile_instance_id)
            tile_instance_id += Int(num_tiles);
        }

        encoder?.endEncoding()

        commandBuffer?.present(drawable)
        commandBuffer?.commit()
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    }
}

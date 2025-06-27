//
//  TileRenderer.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/25/25.
//

import MetalKit

struct Vertex {
    var position_px: SIMD2<Float>
    var texCoord: SIMD2<Float>
}

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
    private var mtlSampler : MTLSamplerState!
                
    func setupDevice(mtkView: MTKView)
    {
        self.device = MTLCreateSystemDefaultDevice()
        mtkView.device = device
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0)
        mtkView.delegate = self
        commandQueue = device.makeCommandQueue()
    }
    
    func setupBuffers()
    {
        let textureDescriptor = MTLTextureDescriptor();
        textureDescriptor.textureType = MTLTextureType.type2DArray
        textureDescriptor.width = 8
        textureDescriptor.height = 8
        textureDescriptor.arrayLength = 128
        textureDescriptor.pixelFormat = MTLPixelFormat.rgba8Unorm
        
        mtlTexture = device.makeTexture(descriptor: textureDescriptor)
        let red_tile = UnsafeMutableRawPointer.allocate(byteCount: 4 * 64, alignment: 4)
        red_tile.initializeMemory(as: UInt32.self, repeating: 0xFF0000FF, count: 64)
        let yellow_tile = UnsafeMutableRawPointer.allocate(byteCount: 4 * 64, alignment: 4)
        yellow_tile.initializeMemory(as: UInt32.self, repeating: 0xFF00FFFF, count: 64)
        
        let region = MTLRegionMake2D(0, 0, 8, 8)
        
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 0, withBytes: red_tile, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 1, withBytes: yellow_tile, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 2, withBytes: yellow_tile, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        
        let descriptor = MTLSamplerDescriptor()
        descriptor.minFilter = .nearest
        descriptor.magFilter = .nearest
        descriptor.sAddressMode = .clampToEdge
        descriptor.tAddressMode = .clampToEdge

        mtlSampler = device.makeSamplerState(descriptor: descriptor)

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
        indexBuffer = device.makeBuffer(bytes: indices, length: kMaxSizeBuffer, options: [])
    }
    
    func setupPipeline(mtkView: MTKView)
    {
        let library = device.makeDefaultLibrary()
        let vertexFunc = library?.makeFunction(name: "tile_vertex_main")
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
        setupBuffers()
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
        encoder?.setFragmentSamplerState(mtlSampler, index: 0)
        
//        // for oam in 0..<128
        var tile_instance_id : Int = 0
        for oam in 0..<2
        {
            let oam_value : Oam = oamBuffer.contents().load(fromByteOffset: oam * 8, as: Oam.self)
            let num_tiles = Oam_Get_width_px(oam_value) * Oam_Get_height_px(oam_value) / 64
            encoder?.drawIndexedPrimitives(type: .triangle,
                                           indexCount: 6,
                                           indexType: .uint16,
                                           indexBuffer: indexBuffer,
                                           indexBufferOffset: oam * 6 * 2,
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

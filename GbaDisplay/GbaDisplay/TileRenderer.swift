//
//  TileRenderer.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/25/25.
//

import MetalKit

struct Vertex {
    var position: SIMD2<Float>
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
    private var oamIdBuffer: MTLBuffer!
    
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
        let texture_data = UnsafeMutableRawPointer.allocate(byteCount: 4 * 64, alignment: 4)
        texture_data.initializeMemory(as: UInt32.self, repeating: 0xFF0000FF, count: 64)
        let region = MTLRegionMake2D(0, 0, 8, 8)
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 0, withBytes: texture_data, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        let texture_data2 = UnsafeMutableRawPointer.allocate(byteCount: 4 * 64, alignment: 4)
        texture_data2.initializeMemory(as: UInt32.self, repeating: 0xFF00FFFF, count: 64)
        mtlTexture.replace(region: region, mipmapLevel: 0, slice: 1, withBytes: texture_data2, bytesPerRow: 4 * 8, bytesPerImage: 4 * 8 * 8)
        
        let descriptor = MTLSamplerDescriptor()
        descriptor.minFilter = .nearest
        descriptor.magFilter = .nearest
        descriptor.sAddressMode = .clampToEdge
        descriptor.tAddressMode = .clampToEdge

        mtlSampler = device.makeSamplerState(descriptor: descriptor)

        
        let pixelVertices : [Vertex] = [
            // Position
            Vertex(position: [-1,  1], texCoord: [0, 0]), // Top-left
            Vertex(position: [-1, -1], texCoord: [0, 1]), // Bottom-left
            Vertex(position: [ 1,  1], texCoord: [1, 0]), // Top-right
            Vertex(position: [ 1, -1], texCoord: [1, 1])  // Bottom-right
        ]
        
        let indices : [UInt16] = [
            0, 1, 2,
            1, 2, 3,
        ]
        
        let oam_ids : [UInt16] = [
            0, 1
        ]
        
        vertexBuffer = device.makeBuffer(bytes: pixelVertices,
                                         length: kMaxSizeBuffer,
                                         options: [])
        oamIdBuffer = device.makeBuffer(bytes: oam_ids,
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
        encoder?.setVertexBuffer(oamIdBuffer, offset: 0, index: 1)
        encoder?.setFragmentTexture(mtlTexture, index: 0)
        encoder?.setFragmentSamplerState(mtlSampler, index: 0)
        
        encoder?.drawIndexedPrimitives(type: .triangle,
                                       indexCount: 6,
                                       indexType: .uint16,
                                       indexBuffer: indexBuffer,
                                       indexBufferOffset: 0,
                                       instanceCount:  1)

        encoder?.endEncoding()

        commandBuffer?.present(drawable)
        commandBuffer?.commit()
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    }
}

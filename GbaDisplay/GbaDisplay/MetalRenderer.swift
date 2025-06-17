//
//  MetalRenderer.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import MetalKit

let kMaxSizeBuffer = 1024 * 1024;

class MetalRenderer: NSObject, MTKViewDelegate {
    private var device: MTLDevice!
    private var commandQueue: MTLCommandQueue!
    private var pipelineState: MTLRenderPipelineState!
    private var vertexBuffer: MTLBuffer!
    private var indexBuffer: MTLBuffer!
    private var pixelAttributeBuffer: MTLBuffer!
    private var spriteAttributeBuffer: MTLBuffer!
    
    private var num_sprites : Int!
    private var m_index_buffer_offset : [Int]!
    private var m_num_pixels : [Int]!
    
    private var game_loop: GameLoop = GameLoop()
        
    func setupDevice(mtkView: MTKView)
    {
        self.device = MTLCreateSystemDefaultDevice()
        mtkView.device = device
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0)
        mtkView.delegate = self
        commandQueue = device.makeCommandQueue()
    }
    
    func loadResources()
    {
        let pixelVertices : [SIMD3<Float>] = [
            // Position
            SIMD3<Float>(0.0, 0.0, 0.0),
            SIMD3<Float>(0.0, -1.0, 0.0),
            SIMD3<Float>(1.0, -1.0, 0.0),
            SIMD3<Float>(1.0, 0.0, 0.0),
        ]
        
        vertexBuffer = device.makeBuffer(bytes: pixelVertices,
                                         length: kMaxSizeBuffer,
                                         options: [])
        indexBuffer = device.makeBuffer(length: kMaxSizeBuffer, options: [])
        pixelAttributeBuffer = device.makeBuffer(length: kMaxSizeBuffer, options: [])
        spriteAttributeBuffer = device.makeBuffer(
                                        length: kMaxSizeBuffer,
                                        options: [])

    }
    
    func setupPipeline(mtkView: MTKView)
    {
        let library = device.makeDefaultLibrary()
        let vertexFunc = library?.makeFunction(name: "vertex_main")
        let fragFunc = library?.makeFunction(name: "fragment_main")

        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.vertexFunction = vertexFunc
        pipelineDescriptor.fragmentFunction = fragFunc
        pipelineDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat

        pipelineState = try? device.makeRenderPipelineState(descriptor: pipelineDescriptor)
    }
    
    init(mtkView: MTKView) {
        super.init()
        setupDevice(mtkView : mtkView)
        loadResources()
        setupPipeline(mtkView : mtkView)
        game_loop.load_buffers(
            index_buffer: indexBuffer.contents().bindMemory(to: UInt16.self, capacity: kMaxSizeBuffer),
            pixel_attr_buffer: pixelAttributeBuffer.contents().bindMemory(to: PixelAttributes.self, capacity: kMaxSizeBuffer),
            sprite_attr_buffer: spriteAttributeBuffer.contents().bindMemory(to: SpriteAttributes.self, capacity: kMaxSizeBuffer)
        )
        game_loop.run()
    }
    
    func sendCommand(in view: MTKView)
    {
        guard let drawable = view.currentDrawable,
              let descriptor = view.currentRenderPassDescriptor else { return }

        let commandBuffer = commandQueue.makeCommandBuffer()
        let encoder = commandBuffer?.makeRenderCommandEncoder(descriptor: descriptor)

        encoder?.setRenderPipelineState(pipelineState)
        
        encoder?.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        encoder?.setVertexBuffer(pixelAttributeBuffer, offset: 0, index: 1)
        encoder?.setVertexBuffer(spriteAttributeBuffer, offset: 0, index: 2)
        
        var pixel_base = 0
        for i in stride(from: 0, to: game_loop.sprite_metadata.m_num_sprites, by: 1) {
            encoder?.drawIndexedPrimitives(type: .triangle,
                                           indexCount: 6,
                                           indexType: .uint16,
                                           indexBuffer: indexBuffer,
                                           indexBufferOffset: game_loop.sprite_metadata.m_index_buffer_offset[i],
                                           instanceCount: game_loop.sprite_metadata.m_num_pixels[i],
                                           baseVertex: 0,
                                           baseInstance: pixel_base)
            pixel_base += game_loop.sprite_metadata.m_num_pixels[i]
        }

        encoder?.endEncoding()

        commandBuffer?.present(drawable)
        commandBuffer?.commit()
    }
    
    func draw(in view: MTKView) {
        sendCommand(in : view)
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    }
}

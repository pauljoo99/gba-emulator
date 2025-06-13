//
//  MetalRenderer.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import MetalKit

class MetalRenderer: NSObject, MTKViewDelegate {
    private var device: MTLDevice!
    private var commandQueue: MTLCommandQueue!
    private var pipelineState: MTLRenderPipelineState!
    private var vertexBuffer: MTLBuffer!
    private var indexBuffer: MTLBuffer!
    private var indexOffsetBuffer: MTLBuffer!
    
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
        let vertexData: [Float] = [
            // Position        // Color
            -0.5, -0.5, 0.0,   1.0, 0.0, 0.0, // Vertex 0: Red
             0.5, -0.5, 0.0,   0.0, 1.0, 0.0, // Vertex 1: Green
             0.0,  0.5, 0.0,   0.0, 0.0, 1.0  // Vertex 2: Blue
        ]
        vertexBuffer = device.makeBuffer(bytes: vertexData,
                                         length: vertexData.count * MemoryLayout<Float>.size,
                                         options: [])

        let indexOffsetData: [Float] = [
            0.0, -0.5, 0.0,  // First triangle
            0.0, 0.0, 0.0,  // Second triangle
        ]
        indexOffsetBuffer = device.makeBuffer(bytes: indexOffsetData,
                                        length: indexOffsetData.count * MemoryLayout<Float>.size,
                                        options: [])
        
        let indexData: [UInt16] = [
            0, 1, 2,  // First triangle
            0, 1, 2,  // Second triangle
        ]
        indexBuffer = device.makeBuffer(bytes: indexData,
                                        length: indexData.count * MemoryLayout<UInt16>.size,
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
        game_loop.run()
    }

    func updateBuffers()
    {
        let pointer = indexOffsetBuffer.contents().bindMemory(to: Float.self, capacity: 6)
        pointer[4] = Float(game_loop.triangle_pos) / 100.0
    }
    
    func sendCommand(in view: MTKView)
    {
        guard let drawable = view.currentDrawable,
              let descriptor = view.currentRenderPassDescriptor else { return }

        let commandBuffer = commandQueue.makeCommandBuffer()
        let encoder = commandBuffer?.makeRenderCommandEncoder(descriptor: descriptor)

        encoder?.setRenderPipelineState(pipelineState)
        encoder?.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        encoder?.setVertexBuffer(indexOffsetBuffer, offset: 0, index: 1)
        encoder?.drawIndexedPrimitives(type: .triangle,
                                       indexCount: 3,
                                       indexType: .uint16,
                                       indexBuffer: indexBuffer,
                                       indexBufferOffset: 0,
                                       instanceCount: 2)
        
        encoder?.endEncoding()

        commandBuffer?.present(drawable)
        commandBuffer?.commit()
    }
    
    func draw(in view: MTKView) {
        updateBuffers()
        sendCommand(in : view)
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    }
}

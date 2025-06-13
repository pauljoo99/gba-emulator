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

    init(mtkView: MTKView) {
        super.init()
        self.device = MTLCreateSystemDefaultDevice()
        mtkView.device = device
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0)
        mtkView.delegate = self

        commandQueue = device.makeCommandQueue()

        let vertexData: [Float] = [
            // Position        // Color
            -0.5, -0.5, 0.0,   1.0, 0.0, 0.0, // Vertex 0: Red
             0.5, -0.5, 0.0,   0.0, 1.0, 0.0, // Vertex 1: Green
             0.0,  0.5, 0.0,   0.0, 0.0, 1.0  // Vertex 2: Blue
        ]
        vertexBuffer = device.makeBuffer(bytes: vertexData,
                                         length: vertexData.count * MemoryLayout<Float>.size,
                                         options: [])

        let indexData: [UInt16] = [
            0, 1, 2,  // First triangle
        ]
        indexBuffer = device.makeBuffer(bytes: indexData,
                                        length: indexData.count * MemoryLayout<UInt16>.size,
                                        options: [])

        
        let library = device.makeDefaultLibrary()
        let vertexFunc = library?.makeFunction(name: "vertex_main")
        let fragFunc = library?.makeFunction(name: "fragment_main")

        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.vertexFunction = vertexFunc
        pipelineDescriptor.fragmentFunction = fragFunc
        pipelineDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat

        pipelineState = try? device.makeRenderPipelineState(descriptor: pipelineDescriptor)
    }

    func draw(in view: MTKView) {
        guard let drawable = view.currentDrawable,
              let descriptor = view.currentRenderPassDescriptor else { return }

        let commandBuffer = commandQueue.makeCommandBuffer()
        let encoder = commandBuffer?.makeRenderCommandEncoder(descriptor: descriptor)

        encoder?.setRenderPipelineState(pipelineState)
        encoder?.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        // encoder?.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 3)
        encoder?.drawIndexedPrimitives(type: .triangle,
                                       indexCount: 3,
                                       indexType: .uint16,
                                       indexBuffer: indexBuffer,
                                       indexBufferOffset: 0)
        
        encoder?.endEncoding()

        commandBuffer?.present(drawable)
        commandBuffer?.commit()
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        print("Drawable size changed to: \(size.width) x \(size.height)")

    }
}

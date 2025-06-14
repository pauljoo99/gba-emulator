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

        let spriteAttributeData : [SpriteAttributes] = [
            SpriteAttributes(offset_x: 130, offset_y: 80, tiles_width: 3, tiles_height: 2),
            SpriteAttributes(offset_x: 130, offset_y: 80, tiles_width: 3, tiles_height: 2),
        ]
        
        spriteAttributeBuffer = device.makeBuffer(bytes: spriteAttributeData,
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
        game_loop.run()
    }

    func updateBuffers()
    {
        num_sprites = 1;
        m_index_buffer_offset = [0]
        m_num_pixels = [64 * 4]

        // Place Pixels
        let pixelIndices : [UInt16] = [
            // First Pixel
            0, 1, 2,
            0, 2, 3,
            // Second Pixel
            0, 1, 2,
            0, 2, 3,
        ]
        let indexBufferPtr = indexBuffer.contents().bindMemory(to: UInt16.self, capacity: kMaxSizeBuffer)
        let pixelAttributeBufferPtr = pixelAttributeBuffer.contents().bindMemory(to: PixelAttributes.self, capacity: kMaxSizeBuffer)
        var index_buffer_size = 0;
        var pixel_attr_buffer_size = 0;
        for sprite in stride(from: 0, to: num_sprites, by: 1)
        {
            for _ in stride(from: 0, to: m_num_pixels[sprite], by: 1)
            {
                pixelAttributeBufferPtr[pixel_attr_buffer_size] = PixelAttributes(color : 0xFF0000FF, sprite_attribute: 0);
                pixel_attr_buffer_size += 1;
                for v in pixelIndices
                {
                    indexBufferPtr[index_buffer_size] = v
                    index_buffer_size += 1
                }
            }
        }
        
        // Pixel Attribute determines color
        
        
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
        
        for i in stride(from: 0, to: num_sprites, by: 1) {
            encoder?.drawIndexedPrimitives(type: .triangle,
                                           indexCount: 6,
                                           indexType: .uint16,
                                           indexBuffer: indexBuffer,
                                           indexBufferOffset: m_index_buffer_offset[i],
                                           instanceCount: m_num_pixels[i])
        }

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

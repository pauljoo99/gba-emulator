//
//  Shaders.metal
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

#include <metal_stdlib>
using namespace metal;


struct VertexOut {
    float4 position [[position]];
    float3 color;
};

vertex VertexOut vertex_main(uint vertexID [[vertex_id]],
                             const device float* vertexArray [[buffer(0)]]) {
    VertexOut out;

    // Each vertex has 6 floats: 3 for position, 3 for color
    const uint stride = 6;
    float3 position = float3(vertexArray[vertexID * stride + 0],
                             vertexArray[vertexID * stride + 1],
                             vertexArray[vertexID * stride + 2]);

    float3 color = float3(vertexArray[vertexID * stride + 3],
                          vertexArray[vertexID * stride + 4],
                          vertexArray[vertexID * stride + 5]);

    out.position = float4(position, 1.0);
    out.color = color;
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]]) {
    return float4(in.color, 1.0);
}

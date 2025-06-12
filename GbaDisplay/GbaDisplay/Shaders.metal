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
};

vertex VertexOut vertex_main(uint vertexID [[vertex_id]],
                             const device float3* vertices [[buffer(0)]]) {
    VertexOut out;
    out.position = float4(vertices[vertexID], 1.0);
    return out;
}

fragment float4 fragment_main() {
    return float4(0.0, 0.7, 1.0, 1.0); // Cyan triangle
}

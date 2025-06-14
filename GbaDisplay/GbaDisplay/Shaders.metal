//
//  Shaders.metal
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

#include <metal_stdlib>
#include "Bridging-Header.h"
using namespace metal;


struct VertexOut {
    float4 position [[position]];
    float3 color;
};

vertex VertexOut vertex_main(uint vertexID [[vertex_id]],
                             uint instanceID [[instance_id]],
                             const device float3* vertexArray [[buffer(0)]],
                             const device PixelAttributes* pixelAttributeBuffer [[buffer(1)]],
                             const device SpriteAttributes* spriteAttributeBuffer [[buffer(2)]]) {
    VertexOut out;
    
    SpriteAttributes sprite_attr = spriteAttributeBuffer[pixelAttributeBuffer[instanceID].sprite_attribute];
    uint32_t pos_within_tile[2] = {(instanceID % 64) % 8, (instanceID % 64) / 8};
    uint32_t pos_of_tile[2] = {(instanceID / 64) % sprite_attr.tiles_width, (instanceID / 64) / sprite_attr.tiles_width};
    uint32_t position_offset_px[2] = {pos_within_tile[0] + 8 * pos_of_tile[0], pos_within_tile[1] + 8 * pos_of_tile[1]};
    
    float3 position_px = float3(
                             vertexArray[vertexID][0] + position_offset_px[0] + sprite_attr.offset_x,
                             vertexArray[vertexID][1] + position_offset_px[1] + sprite_attr.offset_y,
                             vertexArray[vertexID][2]);
    float3 position = float3(position_px[0] / 280.0, -1.0 * position_px[1] / 160.0, 0) + float3(-1.0, 1.0, 0.0);
    float3 color = float3(
                          ((pixelAttributeBuffer[instanceID].color & 0xFF000000) >> 24) / 256.0,
                          ((pixelAttributeBuffer[instanceID].color & 0x00FF0000) >> 16) / 256.0,
                          ((pixelAttributeBuffer[instanceID].color & 0x0000FF00) >> 8) / 256.0
    );
    
    out.position = float4(position, 1.0);
    out.color = color;
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]]) {
    return float4(in.color, 1.0);
}

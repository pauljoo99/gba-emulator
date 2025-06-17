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
    
    PixelAttributes pixel_attr = pixelAttributeBuffer[instanceID];
    SpriteAttributes sprite_attr = spriteAttributeBuffer[pixel_attr.sprite_attribute];
    uint32_t pixel_within_sprite = instanceID - pixel_attr.pixel_buffer_start_offset;
    uint32_t pos_within_tile[2] = {(pixel_within_sprite % 64) % 8, (pixel_within_sprite % 64) / 8};
    uint32_t pos_of_tile[2] = {(pixel_within_sprite / 64) % sprite_attr.tiles_width, (pixel_within_sprite / 64) / sprite_attr.tiles_width};
    uint32_t position_offset_px[2] = {pos_within_tile[0] + 8 * pos_of_tile[0] + sprite_attr.offset_x, pos_within_tile[1] + 8 * pos_of_tile[1] + sprite_attr.offset_y};
    
    float3 position_px = float3(
                             vertexArray[vertexID][0] + position_offset_px[0],
                             vertexArray[vertexID][1] + position_offset_px[1],
                             vertexArray[vertexID][2]);
    float3 position = float3(position_px[0] / 280.0, -1.0 * position_px[1] / 160.0, sprite_attr.depth) + float3(-0.5, 0.5, 0.0);
    
    float3 color = float3(
                          ((pixel_attr.color & 0xFF000000) >> 24) / 256.0,
                          ((pixel_attr.color & 0x00FF0000) >> 16) / 256.0,
                          ((pixel_attr.color & 0x0000FF00) >> 8) / 256.0
    );
    
    out.position = float4(position, 1.0);
    out.color = color;
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]]) {
    return float4(in.color, 1.0);
}

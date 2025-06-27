//
//  Shaders.metal
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

#include <metal_stdlib>
#include "Structs.h"
#include "oam_parser.metal"
using namespace metal;

struct VertexIn {
    float2 position_px;
    uint2 texCoord;
};

struct VertexOut {
    float4 position [[position]];
    float3 color;
    int2 texCoord;
    uint16_t tid;
};

//vertex VertexOut vertex_main(uint vertexID [[vertex_id]],
//                             uint instanceID [[instance_id]],
//                             const device float3* vertexArray [[buffer(0)]],
//                             const device PixelAttributes* pixelAttributeBuffer [[buffer(1)]],
//                             const device SpriteAttributes* spriteAttributeBuffer [[buffer(2)]]) {
//    VertexOut out;
//
//    PixelAttributes pixel_attr = pixelAttributeBuffer[instanceID];
//    SpriteAttributes sprite_attr = spriteAttributeBuffer[pixel_attr.sprite_attribute];
//    uint32_t pixel_within_sprite = instanceID - pixel_attr.pixel_buffer_start_offset;
//    uint32_t pos_within_tile[2] = {(pixel_within_sprite % 64) % 8, (pixel_within_sprite % 64) / 8};
//    uint32_t pos_of_tile[2] = {(pixel_within_sprite / 64) % sprite_attr.tiles_width, (pixel_within_sprite / 64) / sprite_attr.tiles_width};
//    uint32_t position_offset_px[2] = {pos_within_tile[0] + 8 * pos_of_tile[0] + sprite_attr.offset_x, pos_within_tile[1] + 8 * pos_of_tile[1] + sprite_attr.offset_y};
//
//    float3 position_px = float3(
//                             vertexArray[vertexID][0] + position_offset_px[0],
//                             vertexArray[vertexID][1] + position_offset_px[1],
//                             vertexArray[vertexID][2]);
//    float3 position = float3(position_px[0] / 280.0, -1.0 * position_px[1] / 160.0, sprite_attr.depth) + float3(-0.5, 0.5, 0.0);
//
//    float3 color = float3(
//                          ((pixel_attr.color & 0xFF000000) >> 24) / 256.0,
//                          ((pixel_attr.color & 0x00FF0000) >> 16) / 256.0,
//                          ((pixel_attr.color & 0x0000FF00) >> 8) / 256.0
//    );
//
//    out.position = float4(position, 1.0);
//    out.color = color;
//    return out;
//}
//
//fragment float4 fragment_main(VertexOut in [[stage_in]]) {
//    return float4(in.color, 1.0);
//}

vertex VertexOut tile_vertex_main(uint vertexID [[vertex_id]],
                             uint instanceID [[instance_id]],
                             const device VertexIn* vertexArray [[buffer(0)]],
                             const device Oam* oams [[buffer(1)]],
                             const device uint16_t* oam_ids [[buffer(2)]],
                             const device uint16_t* base_tile_ids [[buffer(3)]]) {
    uint abs_tile_id = instanceID;
    uint local_tile_id = abs_tile_id - base_tile_ids[instanceID];
    
    uint oam_id = oam_ids[abs_tile_id];
    Oam oam = oams[oam_id];
    
    uint width_px = Oam_Get_width_px(oam);
    uint width_tile = width_px / 8;
    uint local_offset_x = (local_tile_id % width_tile) * 8;
    uint local_offset_y = (local_tile_id / width_tile) * 8;
    
    VertexOut out;
    float2 position_px(
                       vertexArray[vertexID].position_px[0] + Oam_Get_x(oam) + local_offset_x,
                       vertexArray[vertexID].position_px[1] + Oam_Get_y(oam) + local_offset_y);
    out.position = float4(
                          (position_px[0] / 240 * 2) - 1.0,
                          -1 * (position_px[1] / 160 * 2) + 1.0,
                          0.0,
                          1.0
    );
    out.texCoord = int2(vertexArray[vertexID].texCoord[0], vertexArray[vertexID].texCoord[1]);
    out.tid = Oam_Get_tid(oam) + local_tile_id;
    return out;
}

vertex VertexOut tile_2d_vertex_main(uint vertexID [[vertex_id]],
                             uint instanceID [[instance_id]],
                             const device VertexIn* vertexArray [[buffer(0)]],
                             const device Oam* oams [[buffer(1)]],
                             const device uint16_t* oam_ids [[buffer(2)]],
                             const device uint16_t* base_tile_ids [[buffer(3)]]) {
    uint abs_tile_id = instanceID;
    uint local_tile_id = abs_tile_id - base_tile_ids[instanceID];
    
    uint oam_id = oam_ids[abs_tile_id];
    Oam oam = oams[oam_id];
    
    uint width_px = Oam_Get_width_px(oam);
    uint width_tile = width_px / 8;
    uint local_offset_x = (local_tile_id % width_tile) * 8;
    uint local_offset_y = (local_tile_id / width_tile) * 8;
    
    VertexOut out;
    float2 position_px(
                       vertexArray[vertexID].position_px[0] + Oam_Get_x(oam) + local_offset_x,
                       vertexArray[vertexID].position_px[1] + Oam_Get_y(oam) + local_offset_y);
    out.position = float4(
                          (position_px[0] / 240 * 2) - 1.0,
                          -1 * (position_px[1] / 160 * 2) + 1.0,
                          0.0,
                          1.0
    );
    out.texCoord = int2(vertexArray[vertexID].texCoord[0], vertexArray[vertexID].texCoord[1]);
    out.tid = Oam_Get_tid(oam) + (local_offset_y / 8 * 32) + local_offset_x / 8;
    return out;
}


fragment float4 tile_fragment_main(VertexOut in [[stage_in]],
                                   texture2d_array<uint, access::read> tex [[texture(0)]],
                                   const device uint16_t *palette_buffer [[buffer(0)]])
{
    uint4 color_index = tex.read(uint2(in.texCoord), in.tid);
    
    uint16_t rgba = palette_buffer[color_index[0]];
    
    float r = float((rgba >>  0) & 0xFF) / 255.0;
    float g = float((rgba >>  8) & 0xFF) / 255.0;
    float b = float((rgba >> 16) & 0xFF) / 255.0;
    float a = 1.0;

    return float4(r, g, b, a);
}

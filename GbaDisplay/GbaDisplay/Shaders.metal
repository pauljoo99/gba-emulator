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
    float2 texCoord;
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoord;
    uint16_t tid;
    uint16_t oam_color_mode;
    uint16_t oam_palette_bank;
};

vertex VertexOut tile_vertex_main(uint vertexID [[vertex_id]],
                             uint instanceID [[instance_id]],
                             const device VertexIn* vertexArray [[buffer(0)]],
                             const device Oam* oams [[buffer(1)]],
                             const device uint16_t* oam_ids [[buffer(2)]],
                             const device uint16_t* base_tile_inst_ids [[buffer(3)]]) {
    uint abs_tile_inst_id = instanceID;
    uint local_tile_inst_id = abs_tile_inst_id - base_tile_inst_ids[instanceID];
    
    uint oam_id = oam_ids[abs_tile_inst_id];
    Oam oam = oams[oam_id];
    
    uint width_px = Oam_Get_width_px(oam);
    uint width_tile = width_px / 8;
    uint local_offset_x = (local_tile_inst_id % width_tile) * 8;
    uint local_offset_y = (local_tile_inst_id / width_tile) * 8;
    
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
    
    out.texCoord = float2(vertexArray[vertexID].texCoord[0], vertexArray[vertexID].texCoord[1]);
    out.tid = Oam_Get_tid(oam) + local_tile_inst_id;
    return out;
}

vertex VertexOut tile_2d_vertex_main(uint vertexID [[vertex_id]],
                             uint instanceID [[instance_id]],
                             const device VertexIn* vertexArray [[buffer(0)]],
                             const device Oam* oams [[buffer(1)]],
                             const device uint16_t* oam_ids [[buffer(2)]],
                             const device uint16_t* base_tile_inst_ids [[buffer(3)]]) {
    uint abs_tile_inst_id = instanceID;
    uint local_tile_inst_id = abs_tile_inst_id - base_tile_inst_ids[instanceID];
    
    uint oam_id = oam_ids[abs_tile_inst_id];
    Oam oam = oams[oam_id];
    
    uint width_px = Oam_Get_width_px(oam);
    uint width_tile = width_px / 8;
    uint local_offset_x = (local_tile_inst_id % width_tile) * 8;
    uint local_offset_y = (local_tile_inst_id / width_tile) * 8;
    
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
    out.texCoord = float2(vertexArray[vertexID].texCoord[0], vertexArray[vertexID].texCoord[1]);
    
    if (Oam_Get_color_mode(oam) == 1)
    {
        // 8bpp
        out.tid = Oam_Get_tid(oam) + local_offset_y / 8 * 32 + local_offset_x / 8 * 2;
        out.oam_color_mode = 1;
    }
    else
    {
        // 4bpp
        out.tid = Oam_Get_tid(oam) + local_offset_y / 8 * 32 + local_offset_x / 8;
        out.oam_color_mode = 0;
        out.oam_palette_bank = Oam_Get_palette_bank(oam);
    }
    return out;
}


fragment float4 tile_fragment_main(VertexOut in [[stage_in]],
                                   texture2d_array<uint, access::sample> tex4bpp [[texture(0)]],
                                   texture2d_array<uint, access::sample> tex8bppExtension [[texture(1)]],
                                   sampler s [[sampler(0)]],
                                   const device uint16_t *palette_buffer [[buffer(0)]])
{
    uint16_t rgba;
    if (in.oam_color_mode == 1)
    {
        // 8bpp
        uint4 color_index = tex8bppExtension.sample(s, in.texCoord, in.tid);
        rgba = palette_buffer[color_index[0]];
    }
    else
    {
        // 4bpp
        ushort palette_bank = in.oam_palette_bank;
        uint4 color_index_lo = tex4bpp.sample(s, in.texCoord, in.tid);
        rgba = palette_buffer[(palette_bank << 4) | color_index_lo[0]];
    }
        
    float r = 1.0 - float((rgba >>  0) & 0xFF) / 255.0;
    float g = 1.0 - float((rgba >>  8) & 0xFF) / 255.0;
    float b = 1.0 - float((rgba >> 16) & 0xFF) / 255.0;
    float a = 1.0;

    return float4(r, g, b, a);
}

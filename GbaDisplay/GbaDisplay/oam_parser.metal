//
//  oam_parser.metal
//  GbaDisplay
//
//  Created by Paul Joo on 6/27/25.
//

#ifndef OAM_METAL
#define OAM_METAL

#include <metal_stdlib>
#include "Structs.h"

using namespace metal;

inline ushort Oam_Get_x(Oam oam) {
    return oam.attr1 & 0x1FF;
}

inline ushort Oam_Get_y(Oam oam) {
    return oam.attr0 & 0xFF;
}

inline ushort Oam_Get_sh(Oam oam) {
    return oam.attr0 >> 14;
}

inline ushort Oam_Get_sz(Oam oam) {
    return oam.attr1 >> 14;
}

inline ushort Oam_Get_tid(Oam oam) {
    return oam.attr2 & 0x3FF;
}

inline ushort Oam_Get_mode(Oam oam) {
    return (oam.attr0 >> 8) & 0b11;
}

inline ushort Oam_Get_width_px(Oam oam) {
    ushort shape = Oam_Get_sh(oam);
    ushort size = Oam_Get_sz(oam);

    if (shape == 0 && size == 0) return 8;
    else if (shape == 1 && size == 0) return 16;
    else if (shape == 2 && size == 0) return 8;
    else if (shape == 0 && size == 1) return 16;
    else if (shape == 1 && size == 1) return 32;
    else if (shape == 2 && size == 1) return 8;
    else if (shape == 0 && size == 2) return 32;
    else if (shape == 1 && size == 2) return 32;
    else if (shape == 2 && size == 2) return 16;
    else if (shape == 0 && size == 3) return 64;
    else if (shape == 1 && size == 3) return 64;
    else if (shape == 2 && size == 3) return 32;
    return 0;
}

inline ushort Oam_Get_height_px(Oam oam) {
    ushort shape = Oam_Get_sh(oam);
    ushort size = Oam_Get_sz(oam);

    if (shape == 0 && size == 0) return 8;
    else if (shape == 1 && size == 0) return 8;
    else if (shape == 2 && size == 0) return 16;
    else if (shape == 0 && size == 1) return 16;
    else if (shape == 1 && size == 1) return 8;
    else if (shape == 2 && size == 1) return 32;
    else if (shape == 0 && size == 2) return 32;
    else if (shape == 1 && size == 2) return 16;
    else if (shape == 2 && size == 2) return 32;
    else if (shape == 0 && size == 3) return 64;
    else if (shape == 1 && size == 3) return 32;
    else if (shape == 2 && size == 3) return 64;
    return 0;
}

#endif

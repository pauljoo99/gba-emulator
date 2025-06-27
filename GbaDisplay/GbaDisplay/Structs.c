//
//  Structs.c
//  GbaDisplay
//
//  Created by Paul Joo on 6/27/25.
//

#include "Structs.h"

U16 Oam_Get_x(struct Oam oam)
{
    return oam.attr1 & 0x1FF;
}

U16 Oam_Get_y(struct Oam oam)
{
    return oam.attr0 & 0xFF;
}

U16 Oam_Get_sh(struct Oam oam)
{
    return oam.attr0 >> 14;
}

U16 Oam_Get_sz(struct Oam oam)
{
    return oam.attr1 >> 14;
}

U16 Oam_Get_tid(struct Oam oam)
{
    return oam.attr2 & 0x3FF;
}

U16 Oam_Get_mode(struct Oam oam)
{
    return (oam.attr0 >> 8) & 0b11;
}

U16 Oam_Get_width_px(struct Oam oam)
{
    U16 shape = Oam_Get_sh(oam);
    U16 size = Oam_Get_sz(oam);
    if (shape == 0b00 && size == 0b00) {
      return 8;
    } else if (shape == 0b01 && size == 0b00) {
      return 16;
    } else if (shape == 0b10 && size == 0b00) {
      return 8;
    } else if (shape == 0b00 && size == 0b01) {
      return 16;
    } else if (shape == 0b01 && size == 0b01) {
      return 32;
    } else if (shape == 0b10 && size == 0b01) {
      return 8;
    } else if (shape == 0b00 && size == 0b10) {
      return 32;
    } else if (shape == 0b01 && size == 0b10) {
      return 32;
    } else if (shape == 0b10 && size == 0b10) {
      return 16;
    } else if (shape == 0b00 && size == 0b11) {
      return 64;
    } else if (shape == 0b01 && size == 0b11) {
      return 64;
    } else if (shape == 0b10 && size == 0b11) {
        return 32;
    }
    return 0;
}

U16 Oam_Get_height_px(struct Oam oam)
{
    U16 shape = Oam_Get_sh(oam);
    U16 size = Oam_Get_sz(oam);
    if (shape == 0b00 && size == 0b00) {
      return 8;
    } else if (shape == 0b01 && size == 0b00) {
      return 8;
    } else if (shape == 0b10 && size == 0b00) {
      return 16;
    } else if (shape == 0b00 && size == 0b01) {
      return 16;
    } else if (shape == 0b01 && size == 0b01) {
      return 8;
    } else if (shape == 0b10 && size == 0b01) {
      return 32;
    } else if (shape == 0b00 && size == 0b10) {
      return 32;
    } else if (shape == 0b01 && size == 0b10) {
      return 16;
    } else if (shape == 0b10 && size == 0b10) {
      return 32;
    } else if (shape == 0b00 && size == 0b11) {
      return 64;
    } else if (shape == 0b01 && size == 0b11) {
      return 32;
    } else if (shape == 0b10 && size == 0b11) {
      return 64;
    }
    return 0;
}


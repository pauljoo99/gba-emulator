//
//  Structs.h
//  GbaDisplay
//
//  Created by Paul Joo on 6/13/25.
//

#ifndef Structs_h
#define Structs_h

#define U32 unsigned int
#define U16 unsigned short
#define S16 signed short
#define U8  unsigned char

#define I16  signed short

struct Pixel
{
    U32 x;
    U32 y;
};

struct PixelAttributes
{
    U32 color;
    U32 sprite_attribute;
    U32 pixel_buffer_start_offset;
};

struct SpriteAttributes
{
    // Offset x in pixel space. 0,0 is the top left of the screen.
    I16 offset_x;
    // Offset y in pixel space. 0,0 is the top left of the screen.
    I16 offset_y;
    // values from [0, 5). Higher means that it is drawn last.
    U16 depth;
    U16 tiles_width;
    U16 tiles_height;
};

struct Oam
{
    U16 attr0;
    U16 attr1;
    U16 attr2;
    S16 fill;
};

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


#endif /* Structs_h */

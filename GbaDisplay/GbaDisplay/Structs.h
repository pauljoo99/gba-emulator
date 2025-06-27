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

#endif /* Structs_h */

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
#define U8  unsigned char

struct Pixel
{
    U32 x;
    U32 y;
};

struct PixelAttributes
{
    U32 color;
    U32 sprite_attribute;
};

struct SpriteAttributes
{
    U16 offset_x;
    U16 offset_y;
    U16 tiles_width;
    U16 tiles_height;
};

#endif /* Structs_h */

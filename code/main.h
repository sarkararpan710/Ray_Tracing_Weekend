#include<iostream>
#include<stdint.h>
#define internal static //can make the compile time slow down.

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;

#define F32Max FLT_MAX
#define F32Min -FLT_MAX
#define Pi32 3.14159265359f
#define Tau32 6.28318530717958647692f
#define srand48(x) srand((int)(x))
#define drand48() ((double)rand()/RAND_MAX)

#pragma pack(push, 1)
//structure of the bitmap header to set up the ray tracer images to be rendered here
struct bitmap_header
{
	u16 FileType;
	u32 FileSize;
	u16 Reserved1;
	u16 Reserved2;
	u32 BitmapOffset;
	u32 Size;
	s32 Width;
	s32 Height;
	u16 Planes;
	u16 BitsPerPixel;
	u32 Compression;
	u32 SizeOfBitmap;
	s32 HorzResolution;
	s32 VertResolution;
	u32 ColorsUsed;
	u32 ColorsImportant;
	//u32 RedMask;
	//u32 GreenMask;
	//u32 BlueMask;
};
#pragma pack(pop)

//starting to abstract stuff
struct image_32//RGBA image
{
	u32 Width;
	u32 Height;
	u32 *Pixels;
};
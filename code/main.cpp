#include<iostream>
#include<stdint.h>
#include<fstream>
#include<random>
#include "hitableList.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <float.h> //for float_MAX
#include "main.h" //contains our typedef declarations, #defines and struct declarations.
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
/* Will probably use these stb files to implement a png implementation later.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
*/
/*
to get the png image, we need to read the ppm text file to get the pixel data and then 
output to png using the stb_image_write.h. Will implement it later as png file format is more complex. It's a learning process so there you go.
We have been implementing ppm file formats and now testing if we can get a bmp image with the ray tracing implementation. 
*/

//This code now fully implements the write to the bitmap image without the alpha channel for now. So it basically writes the pixel data into a bitmap image.
//we have a main.h that declares the structure for the bitmap header and the Image structure to write the pixel data to.


//Function definition
internal u32 GetTotalPixelSize(image_32 Image)//take image_32 Image
{
	u32 Result = Image.Width*Image.Height * sizeof(u32);
	return(Result);
}

//Function defintion to allocate image size
internal image_32 AllocateImage(u32 width, u32 height)
{
	image_32 Image = {};//create the image object and initialize it.
	Image.Height = height;
	Image.Width = width;
	u32 OutputPixelSize = GetTotalPixelSize(Image);//old version of the code does this line->sizeof(u32)*image.Width*image.Height;
	Image.Pixels = (u32*)malloc(OutputPixelSize);//main source of the initial nullpointer at main *Out.
	return(Image);
}

internal void WriteImage(image_32 Image, const char* OutputFileName)
{
	u32 OutputPixelSize = GetTotalPixelSize(Image);
	bitmap_header Header = {};

	Header.FileType = 0x4D42;
	Header.FileSize = sizeof(Header) + OutputPixelSize;//Need to set it later
													   //Header.Reserved1;//These are reserved and set by the header itself
													   //Header.Reserved2;//These are reserved and set by the header itself
	Header.BitmapOffset = sizeof(Header);
	Header.Size = sizeof(Header) - 14;//also need to set the size of the pixels. Since the header is 50 bytes check wikipedia.
	Header.Width = Image.Width;
	Header.Height = Image.Height;
	Header.Planes = 1;
	Header.BitsPerPixel = 32;
	Header.Compression = 0;
	Header.SizeOfBitmap = OutputPixelSize;//writing bottom part first. Very Important.
	Header.HorzResolution = 0;
	Header.VertResolution = 0;
	Header.ColorsUsed = 0;
	Header.ColorsImportant = 0;

	//Header.RedMask = 0x00FF0000;//LITTLE ENDIAN MACHINES TO SET UP THE COLOR OF THE BUFFER 
	//Header.GreenMask  = 0x0000FF00;
	//Header.BlueMask = 0x000000FF;

	/* Checking the reference image to check the values of the header.
	#if 0
	FILE *InFile = fopen("reference.bmp", "rb");
	bitmap_header RefHeader = {};
	fread(&RefHeader, sizeof(RefHeader), 1, InFile);
	fclose(InFile);
	#endif
	*/
	//performing the standard file procedures to set the output file on the bitmap image set by the structure header to 
	//const char *OutputFileName = "test.bmp";
	FILE *OutFile = fopen(OutputFileName, "wb");

	if (OutFile)
	{
		fwrite(&Header, sizeof(Header), 1, OutFile);//we write it into the header
		fwrite(Image.Pixels, OutputPixelSize, 1, OutFile);
		fclose(OutFile);
	}

	else
	{
		fprintf(stderr, "[Error]Unable to write output file %s. \n", OutputFileName);
	}
}

vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)){
        ray scattered;
        vec3 attenuation;
        if(depth <50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
            return attenuation*color(scattered, world, depth+1);
        }
        else{
            return vec3(0,0,0);
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main()
{    
    printf("Raycasting......");
    /*
    int nx=1280;
    int ny =720;
    */
    u32 ns = 100;
    
    
    image_32 Image = AllocateImage(1920, 1080);

    //std::ofstream outfile;
    //outfile.open("..\\data\\stb_test.txt");  
    //unsigned char *data = stbi_load("..\\data\\stb_test.png", &nx, &ny, &ns, 3);//3 for RGB
    //unsigned char *data = stbi_load("..\\data\\stb_test.png", &nx, &ny, &ns, 0);
    //std::ofstream outfile;
	//outfile.open("..\\data\\another_Test.ppm");
    //const void *data;
    //data = (unsigned char*)"P3\n" + nx + (unsigned char)" " + ny + (unsigned char)"\n255\n";
    //int stride_in_bytes = 8;
    //Output to .ppm file
    //outfile << nx << " " << ny << "\n255\n";
    //std::out<<"P3\n" <<nx <<" " <<ny<<"\n255\n";

    hitable *list[5];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list,4);
    camera cam;
    u32 *Out = Image.Pixels;
    for(u32 y=0 ; y<Image.Height; y++)
    {
        for(u32 x=0; x<Image.Width; x++)
        {
            vec3 col(0, 0, 0);
            
            for(u32 s=0; s < ns; s++)
            {
                float u = float(x+drand48())/float(Image.Width);
                float v = float(y+drand48())/float(Image.Height);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col = col + color(r, world, 0); //col returns a vec3
            }
            col/=float(ns);//average sampling per pixel
            
           vec3 BMPColor = vec3(255*col); //getting bmp color values from raytraced image.
           u32 BMPvalue = BGRPack4x8(BMPColor); //packing the bmp color into an integer to write to the bitmap image.

           *Out++ = BMPvalue;
           /*
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            */
            //std::cout<<ir<<" "<<ig<<" "<<ib<<"\n";
            //data = (unsigned char*)ir + (unsigned char)" " + ig + (unsigned char)" " + ib + (unsigned char)"\n";
            //outfile << ir << " " << ig << " " << ib << "\n";
            //stbi_write_png("\\data\\stb_test.png", ir, ig, ib , data, 8);
        }

        if((y%64) ==0)
		{
			printf("\rRaycasting row %d%%....",100*y / Image.Height);
			fflush(stdout);
		}
    }
    WriteImage(Image, "..\\data\\Hollow_Glass_Sphere.bmp");//getting the raytraced image plane on test.bmp.
    printf("\nDone.....\n");
    //outfile.close();

    
   /* 
   FILE *datafile = fopen("..\\data\\stb_test.txt" , "r");
   if (!datafile) { // success of file open should be tested ALWAYS
     std::cerr << "Cannot open 'stb_test.txt'!\n";
     return -1; // ERROR! (bail out) 
   }

   typedef unsigned char uchar; // for convenience
   std::vector<uchar> pixeldata(nx * ny);
   char Image2[] = "..\\data\\stb_test.bmp";
   for (int i = 0, n = nx * ny; i < n; ++i) {
     if (fscanf(datafile, "%hhx", &pixeldata[i]) < 1) {
       std::cerr << "Failed to read value " << i << " of stb_test.txt!"<<"\n";
       return -1; // ERROR! (bail out) 
     }
   }
   fclose(datafile);
   //stbi_write_png(Image2, nx, ny, 3, pixeldata.data(), 4);
    stbi_write_bmp(Image2, nx, ny, 3, pixeldata.data());
  */
 return 0;


}
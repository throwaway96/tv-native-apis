#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <jpeglib.h>
#include <signal.h>
#include <gm-wayland/gmw_c.h>

#define CRLF() fwrite("\r\n", 1, 2, stdout)

int isrunning = 0;

HAL_GAL_SURFACE_INFO_T *surfinfo;
HAL_GAL_DRAW_SETTINGS_T *settings;
HAL_GAL_RECT_T *rect;
HAL_GAL_DRAW_FLAGS_T *drawflags;
int32_t *palette;

void sighandle(int sig);
int stop();
void NV21_TO_RGB24(unsigned char *yuyv, unsigned char *rgb, int width, int height);
void write_JPEG_stdout(int quality);

void sighandle(int sig){
     fprintf(stderr, "\nTermination-signal recieved. Terminating.. \n");

    int done = 0;
    if(isrunning == 1){
//        done = stop();
        exit(done);
    }else{
//        done = finalize();
        exit(done);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sighandle);
    int done;
    int ex;

     //Pointer of Exitcode from createSurface() 
    drawflags = malloc(sizeof(int)); //default = 0

    settings = malloc(3*sizeof(int));
    settings->blend1 = 2; //default = 2(1-10 possible) - blend? setting
    settings->blend2 = 1;
    settings->blend3 = 0;

    rect = malloc(4*sizeof(int16_t));
    rect->x = 0;
    rect->y = 0;
    rect->w = 1280;
    rect->h = 720;
/*  int32_t offset = 2;
    int32_t physicalAddress = 0; //should be dst framebuffer
    int16_t pitch = 0;
    int16_t bpp = 0;

    int32_t pixelFormat = 0;
    int32_t paletteInfo = 0;
    int32_t property = 0;
    int32_t vendorData = 0; */
    int32_t width = 1280;
    int32_t height = 720;
    palette = malloc(1028);

    surfinfo = malloc(1056);
    surfinfo->paletteInfo = palette;
/*     surfinfo.offset = &offset;
    surfinfo.physicalAddress = &physicalAddress;
    surfinfo.pitch = pitch;
    surfinfo.bpp = bpp;
    surfinfo.width = width;
    surfinfo.height = height;
    surfinfo.pixelFormat = &pixelFormat;
    surfinfo.paletteInfo = &paletteInfo;
    surfinfo.property = &property;
    surfinfo.vendorData = &vendorData;  */


    done = GM_CreateSurface(width, height, 0, surfinfo);
    if(done != 0){
        fprintf(stderr, "GM_CreateSurface failed: %x\nQuitting...\n", done);
        ex = stop();
        return ex;
    }
    fprintf(stderr, "GM_CreateSurface done! Address %p\n", surfinfo->offset);
    fprintf(stderr, "Surfinfo: Offset: %p, physicalAddress: %p, pitch: %d, bpp: %d, width: %d, height: %d, pixelFormat: %p, paletteInfo: %p, property: %d, vendorData: %d\n", surfinfo->offset, surfinfo->physicalAddress, surfinfo->pitch, surfinfo->bpp, surfinfo->width, surfinfo->height, surfinfo->pixelFormat, surfinfo->paletteInfo, surfinfo->property, surfinfo->vendorData);

    done = GM_FillRectangle(surfinfo, rect, 0, drawflags, settings);
    if(done != 0){
        fprintf(stderr, "GM_FillRectangle failed: %x | Settings: %d\nQuitting...\n", done, settings->blend1);
        ex = stop();
        return ex;
    }
    ex = stop();
    free(surfinfo);
    free(rect);
    return ex; 
    
}

int stop()
{
    fprintf(stderr, "-- Quit called! --\n");
    int ex;
    ex = GM_DestroySurface(surfinfo);
    if(ex == 0){
        fprintf(stderr, "Quitting: GM_DestroySurface done!\n");
        fprintf(stderr, "Quitting!\n");
        return 0;
    }
    fprintf(stderr, "Quitting with errors: %x!\n", ex);
    return 1;
}

/*     printf("HTTP/1.1 200 OK");
    CRLF();
    printf("Content-Type: multipart/x-mixed-replace; boundary=myboundary");
    CRLF();
    printf("Cache-Control: no-cache");
    CRLF();
    printf("Connection: close");
    CRLF();
    printf("Pragma: no-cache");
    CRLF();
    CRLF(); */

/*     int comsize;  
    comsize = size0+size1;
    do{
        //Combine two Image Buffers to one and convert to RGB24
        char *combined = (char *) malloc(comsize);
        int rgbsize = sizeof(combined)*w*h*3;
        rgbout = (char *) malloc(rgbsize);
        memcpy(combined, addr0, size0);
        memcpy(combined+size0, addr1, size1);

        NV21_TO_RGB24(combined, rgbout, w, h);

        write_JPEG_stdout(85);

        free(rgbout);
        free(combined);

   
//        FILE * pFile;
//        pFile = fopen ("/tmp/myfile.bin","wb");
//        if (pFile!=NULL)
//        {
//            fwrite(rgbout, rgbsize, sizeof(rgbout), pFile);
//            fclose (pFile);
//        }
    

    }while(1==1);


    done = stop();
    return done;
} */


// //Credits: https://www.programmersought.com/article/18954751423/
// void NV21_TO_RGB24(unsigned char *yuyv, unsigned char *rgb, int width, int height)
// {
//         const int nv_start = width * height ;
//         int  index = 0, rgb_index = 0;
//         uint8_t y, u, v;
//         int r, g, b, nv_index = 0,i, j;
 
//         for(i = 0; i < height; i++){
//             for(j = 0; j < width; j ++){

//                 nv_index = i / 2  * width + j - j % 2;
 
//                 y = yuyv[rgb_index];
//                 u = yuyv[nv_start + nv_index ];
//                 v = yuyv[nv_start + nv_index + 1];
 
//                 r = y + (140 * (v-128))/100;  //r
//                 g = y - (34 * (u-128))/100 - (71 * (v-128))/100; //g
//                 b = y + (177 * (u-128))/100; //b
 
//                 if(r > 255)   r = 255;
//                 if(g > 255)   g = 255;
//                 if(b > 255)   b = 255;
//                 if(r < 0)     r = 0;
//                 if(g < 0)     g = 0;
//                 if(b < 0)     b = 0;
 
//                 index = rgb_index % width + (height - i - 1) * width;

//                 rgb[index * 3+0] = r;
//                 rgb[index * 3+1] = g;
//                 rgb[index * 3+2] = b;
                 
//                 rgb_index++;

//             }
//         }
// }

// //Credits: https://github.com/webosbrew/tv-native-apis/blob/main/samples/vt/capture/capture.c
// void write_JPEG_stdout(int quality)
// {
//     /* This struct contains the JPEG compression parameters and pointers to
//    * working space (which is allocated as needed by the JPEG library).
//    * It is possible to have several such structures, representing multiple
//    * compression/decompression processes, in existence at once.  We refer
//    * to any one struct (and its associated working data) as a "JPEG object".
//    */
//     struct jpeg_compress_struct cinfo;
//     /* This struct represents a JPEG error handler.  It is declared separately
//    * because applications often want to supply a specialized error handler
//    * (see the second half of this file for an example).  But here we just
//    * take the easy way out and use the standard error handler, which will
//    * print a message on stderr and call exit() if compression fails.
//    * Note that this struct must live as long as the main JPEG parameter
//    * struct, to avoid dangling-pointer problems.
//    */
//     struct jpeg_error_mgr jerr;
//     /* More stuff */

//     JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
//     int row_stride;          /* physical row width in image buffer */

//     /* Step 1: allocate and initialize JPEG compression object */

//     /* We have to set up the error handler first, in case the initialization
//    * step fails.  (Unlikely, but it could happen if you are out of memory.)
//    * This routine fills in the contents of struct jerr, and returns jerr's
//    * address which we place into the link field in cinfo.
//    */
//     cinfo.err = jpeg_std_error(&jerr);
//     /* Now we can initialize the JPEG compression object. */
//     jpeg_create_compress(&cinfo);

//     unsigned long jpegSize = 0;
//     unsigned char *jpegBuf = NULL;

//     /* Step 2: specify data destination (eg, a file) */
//     /* Note: steps 2 and 3 can be done in either order. */

//     /* Here we use the library-supplied code to send compressed data to a
//    * stdio stream.  You can also write your own code to do something else.
//    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
//    * requires it in order to write binary files.
//    */
//     jpeg_mem_dest(&cinfo, &jpegBuf, &jpegSize);

//     /* Step 3: set parameters for compression */

//     /* First we supply a description of the input image.
//    * Four fields of the cinfo struct must be filled in:
//    */
//     cinfo.image_width = w; /* image width and height, in pixels */
//     cinfo.image_height = h;
//     cinfo.input_components = 3;          /* # of color components per pixel */
//     cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
//     /* Now use the library's routine to set default compression parameters.
//    * (You must set at least cinfo.in_color_space before calling this,
//    * since the defaults depend on the source color space.)
//    */
//     jpeg_set_defaults(&cinfo);
//     /* Now you can set any non-default parameters you wish to.
//    * Here we just illustrate the use of quality (quantization table) scaling:
//    */
//     jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

//     /* Step 4: Start compressor */

//     /* TRUE ensures that we will write a complete interchange-JPEG file.
//    * Pass TRUE unless you are very sure of what you're doing.
//    */
//     jpeg_start_compress(&cinfo, TRUE);

//     /* Step 5: while (scan lines remain to be written) */
//     /*           jpeg_write_scanlines(...); */

//     /* Here we use the library's state variable cinfo.next_scanline as the
//    * loop counter, so that we don't have to keep track ourselves.
//    * To keep things simple, we pass one scanline per call; you can pass
//    * more if you wish, though.
//    */
//     row_stride = w * 3; /* JSAMPLEs per row in image_buffer */

//     while (cinfo.next_scanline < cinfo.image_height)
//     {
//         /* jpeg_write_scanlines expects an array of pointers to scanlines.
//      * Here the array is only one element long, but you could pass
//      * more than one scanline at a time if that's more convenient.
//      */
//         row_pointer[0] = &rgbout[(h - cinfo.next_scanline - 1) * row_stride];
//         (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
//     }

//     /* Step 6: Finish compression */

//     jpeg_finish_compress(&cinfo);
//     /* After finish_compress, we can close the output file. */

//     printf("--myboundary");
//     CRLF();
//     printf("Content-Type: image/jpeg");
//     CRLF();
//     printf("Content-Length: %d", jpegSize);
//     CRLF();
//     CRLF();
//     fwrite(jpegBuf, 1, jpegSize, stdout);
// //    FILE * pFile;
// //    pFile = fopen ("/tmp/myfile.jpg","wb");
// //    if (pFile!=NULL)
// //    {
// //        fwrite(jpegBuf, 1, jpegSize, pFile);
// //        fclose (pFile);
// //    }

//     CRLF();

//     /* Step 7: release JPEG compression object */

//     /* This is an important step since it will release a good deal of memory. */
//     jpeg_destroy_compress(&cinfo);

//     /* And we're done! */
//     // Free jpeg memory
//     free(jpegBuf);
// }
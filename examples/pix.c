#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "imgproc.h"
#include "./pix.h"

int main(int argc, char * argv[])
{
   int scale = 2;
   char filename[10];
   // initialise the library
   init_imgproc();

   // open the webcam, with a capture resolution of width 320 and height 240
   Camera * cam = camOpen(scale*320, scale*240);

   // capture an image from the webcam   
   Image * img = camGrabImage(cam);

   //save image to .bmp
   sprintf (filename, "img%03d.bmp", 1);
   bmpImage(img, filename);

   // now we will free the memory for the various objects
   imgDestroy(img);

   // finally quit
   camClose(cam);

   // finally we uninitialise the library
   quit_imgproc();
   return 0;
}


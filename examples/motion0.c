#include <stdlib.h>
#include <stdio.h>
#include "imgproc.h"
#include <math.h>

int main(int argc, char * argv[])
{
   int halt = 1;
   int scale = 2;
   int num = 40;
   int scanu = 170;
   int scand = 20;
   int threshold = 20;
   int motionpix = 0;

   init_imgproc(); // initialise the library

   // open the webcam, with a capture resolution of width 320 and height 240
   Camera * cam = camOpen(scale*320, scale*240);

   // create a new viewer of the same resolution with a caption
   Viewer * view = viewOpen(scale*320, scale*240, "First");

   int cnt = 0;
   while(cnt < num){
      cnt++;
      // capture an image from the webcam   
      Image * img0 = camGrabImage(cam);
      //waitTime(100);
      Image * img1 = camGrabImage(cam);
      //waitTime(100);
      //Image * img2 = camGrabImage(cam);
      //waitTime(100);

      unsigned int x, y;
      unsigned int p1, p2;
      for(x = 1; x < img0->width - 1; x++){
         char *pix0, *pix1;
         int int0, int1;
         y = scale * (scanu + scand/2);
         pix0 = imgGetPixel(img0, x, y);
         int0 = pix0[0] + pix0[1] + pix0[2];
         pix1 = imgGetPixel(img1, x, y);
         int1 = pix1[0] + pix1[1] + pix1[2];
         int diffs = 0;
         if (y == scale * (scanu + scand/2) && abs(int0 - int1) > threshold && diffs < 3) { 
            ++diffs; 
            printf("diffs = %d x = %d\n", diffs, p1); 
            p1 = x; } else if (diffs > 0) --diffs;
         if (diffs == 3) { printf("diffs = %d x = %d\n", diffs, p1); break; }
      }

      // display the edge image
      viewDisplayImage(view, img0);
      if (halt ==1)waitTime(2000);
      halt=0;

      // now we will free the memory for the various objects
      imgDestroy(img0);
      imgDestroy(img1);
      //imgDestroy(img2);
   }

   // finally quit
   viewClose(view);
   camClose(cam);

   // finally we uninitialise the library
   quit_imgproc();
   return 0;
}


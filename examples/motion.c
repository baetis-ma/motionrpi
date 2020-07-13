#include <stdlib.h>
#include <stdio.h>
#include "imgproc.h"
#include <math.h>

int main(int argc, char * argv[])
{
   int scale = 2;
   int num = 5000;
   int scan1 = 170;  //out of 240 
   int threshold = 40;

   init_imgproc(); // initialise the library

   // open the webcam, with a capture resolution of width 320 and height 240
   Camera * cam = camOpen(scale*320, scale*240);

   // create a new viewer of the same resolution with a caption
   Viewer * view = viewOpen(scale*320, scale*240, "First");

   int cnt = 0;
   int sum = 0, numframes = 0, p1 = 0, lastp1 = 0, lastcnt = 0;
   while(cnt < num){
      cnt++;
      if (numframes > 0 && cnt - lastcnt > 1 ) { 
         if(numframes > 1 && abs(sum) > 50)
               printf("%5d  %d speed = %6.1fMPH\n", cnt,numframes, 0.003* (float)100*sum/numframes); 
         numframes = 0; sum = 0; }

      // capture an image from the webcam   
      Image * img0 = camGrabImage(cam);
      Image * img1 = camGrabImage(cam);

      unsigned int x, y;
      int diffs = 0;
      for(x = 1; x < img0->width - 1; x++){
         char *pix0, *pix1;
         int int0, int1;
         y = scale * scan1;
         pix0 = imgGetPixel(img0, x, y);
         int0 = pix0[0] + pix0[1] + pix0[2];
         pix1 = imgGetPixel(img1, x, y);
         int1 = pix1[0] + pix1[1] + pix1[2];
         if (y == scale * scan1  && abs(int0 - int1) > threshold && diffs < 3) { 
            ++diffs; 
            //printf("%3d diffs = %d x = %3d\n", cnt, diffs, p1); 
            p1 = x; } else if (diffs > 0) --diffs;
         if (diffs == 3) {
            //printf("%3d %3d  diffs = %d x = %3d    %4d\n", cnt,lastcnt, diffs, p1, p1-lastp1);
            if (p1 > 80 && p1 < 550 && (cnt - lastcnt) == 1) {
                sum = sum + (p1 - lastp1);
                ++numframes;
                //printf("%3d %3d  diffs = %d x = %3d    %4d   %4d  %4d\n", cnt,lastcnt, diffs, p1, p1-lastp1, numframes,sum);
            }  
            lastp1 = p1; 
            lastcnt = cnt;  
            break; 
         } 
      }

      //draw scan line on image
      for(x = 1; x < img0->width - 1; x++) imgSetPixel (img0, x, scale * scan1, 255, 0, 0);

      // display the image
      viewDisplayImage(view, img0);

      // now we will free the memory for the various objects
      imgDestroy(img0);
      imgDestroy(img1);
   }

   // finally quit
   viewClose(view);
   camClose(cam);

   // finally we uninitialise the library
   quit_imgproc();
   return 0;
}


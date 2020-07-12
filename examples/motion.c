#include <stdlib.h>
#include <stdio.h>
#include "imgproc.h"

#include <math.h>


int main(int argc, char * argv[])
{
   int halt = 1;
   int scale = 2;
   int num = 200;
   int scanu = 175;
   int scand = 10;
   int threshold = 20;
   int motionpix = 0;
   // initialise the library
   init_imgproc();

   // open the webcam, with a capture resolution of width 320 and height 240
   Camera * cam = camOpen(scale*320, scale*240);

   // create a new viewer of the same resolution with a caption
   Viewer * view = viewOpen(scale*320, scale*240, "First");

   int cnt = 0;
   while(cnt < num){
      cnt++;
      // capture an image from the webcam   
      Image * img0 = camGrabImage(cam);
      waitTime(100);
      Image * img1 = camGrabImage(cam);
      waitTime(100);
      Image * img2 = camGrabImage(cam);
      waitTime(100);

      // iterators
      unsigned int x, y;
      for(x = 1; x < img0->width - 1; x++){
         for(y = scale * scanu + 1; y < scale * (scanu + scand) - 1; y++){
            char * pixel0;
            char * pixel1;
            char * pixel2;
            int intensity0;
            int intensity1;
            int intensity2;

            pixel0 = imgGetPixel(img0, x, y);
            intensity0 = pixel0[0] + pixel0[1] + pixel0[2];
            pixel1 = imgGetPixel(img1, x, y);
            intensity1 = pixel1[0] + pixel1[1] + pixel1[2];
            pixel2 = imgGetPixel(img2, x, y);
            intensity2 = pixel2[0] + pixel2[1] + pixel2[2];

            // draw the pixel on the edge image
            if (abs(intensity0 - intensity1) < threshold && abs(intensity1 - intensity2) > threshold) {
               if (y == scale * (scanu + scand/2) ) { 
                  //printf(" %d %d\n", x, motionpix); 
                  ++motionpix; 
               }
               imgSetPixel(img2, x, y, 255,0,0);
               if (motionpix > 10) halt = 1;
            }
            else  {
               imgSetPixel(img2, x, y, 0.9 * pixel2[0], 0.9 * pixel2[1], 0.9 * pixel2[2]);
               if (y == scale * (scanu + scand/2) ) { 
                  imgSetPixel(img2, x, y, 0, 0,0);
                  if(motionpix > 10)printf("%2d  motionpix = %d\n", cnt, motionpix); 
                  motionpix = 0;
               }
            }
         }
      }

      // display the edge image
      viewDisplayImage(view, img2);
      if (halt ==1)waitTime(2000);
      halt=0;

      // now we will free the memory for the various objects
      imgDestroy(img0);
      imgDestroy(img1);
      imgDestroy(img2);
   }

   // finally quit
   viewClose(view);
   camClose(cam);

   // finally we uninitialise the library
   quit_imgproc();
   return 0;
}


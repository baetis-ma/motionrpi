#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "imgproc.h"
#include "pix.h"
#include <math.h>

int main(int argc, char * argv[])
{
   int scale = 2;
   int scan1 = 170;  //out of 240 
   int threshold = 60;
   int tsum[32];

   init_imgproc(); // initialise the library

   // open the webcam, with a capture resolution of width 320 and height 240
   Camera * cam = camOpen(scale*320, scale*240);

   // create a new viewer of the same resolution with a caption
   Viewer * view = viewOpen(scale*320, scale*240, "First");

   int cnt = 0, update = 98;
   int sum = 0, numframes = 0, p1 = 0, lastp1 = 0, lastcnt = 0;
   while(1){
      cnt++; update++;
      time_t t = time(NULL);
      char filename[32];
      struct tm tm = *localtime(&t);

      // capture an image from the webcam   
      Image * img0 = camGrabImage(cam);
      Image * img1 = camGrabImage(cam);

      if (numframes > 0 && cnt - lastcnt > 1 ) { 
         if(numframes > 2){
            int avg = sum / numframes;
            //order array
            int nn, n, temp;
            for(nn=1;nn<numframes;nn++) 
               for(n=1;n<numframes;n++) { 
                  if(tsum[n]<tsum[n+1]){temp = tsum[n];tsum[n]=tsum[n+1];tsum[n+1]=temp;};
            }
            //elimate fliers
            int num = 0;
            sum = 0;
            for(nn=1;nn<=numframes;nn++){
               if((float)tsum[nn]/tsum[numframes/2] < 1.5 && (float)tsum[nn]/tsum[numframes/2] > .6){
                  ++num; sum = sum + tsum[nn];
               }
            } 
            float mph = 0.008* (float)100*sum/num;
            if (abs(mph) < 6) mph = (16.0/21.0) * mph;
            if (mph < 0) mph = (26.0/21.0) * mph;
            if(num > 2){printf("%02d:%02d:%02d %6d speed= %7.1fMPH s=%5d  ", 
                    tm.tm_hour, tm.tm_min, tm.tm_sec, cnt, mph, sum); 
               int x; for(x=1; x<=numframes; x++) {printf (" %4d", tsum[x]);if (x>10) break; }
               printf("\n");
            }
            update = 0;
         }
        
         numframes = 0; sum = 0; 
      } 

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
            if (p1 > 150 && p1 < scale*320 - 200 && (cnt - lastcnt) == 1) {
                sum = sum + (p1 - lastp1);
                ++numframes;
                tsum[numframes] = (p1 - lastp1);
                if(numframes == 3) {
                   sprintf (filename, "img%05d.bmp", cnt);
                   bmpImage(img0, filename);
                   viewDisplayImage(view, img0);
                }
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
      if (numframes == 0 && update > 100) {viewDisplayImage(view, img0); update = 0;}

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


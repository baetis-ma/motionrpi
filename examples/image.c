#include <stdlib.h>
#include <stdio.h>
#include "imgproc.h"

#include <math.h>


int main(int argc, char * argv[])
{
        int scale = 2;
	// initialise the library
	init_imgproc();


	// open the webcam, with a capture resolution of width 320 and height 240
	Camera * cam = camOpen(scale*320, scale*240);

	// create a new viewer of the same resolution with a caption
	Viewer * view = viewOpen(scale*320, scale*240, "First");

        int cnt = 0;
	while(cnt < 100){
                cnt++;
		// capture an image from the webcam	
		Image * img = camGrabImage(cam);

		// display the edge image
		viewDisplayImage(view, img);
                printf("%d\n", cnt);

		// now we will free the memory for the various objects
		imgDestroy(img);
	}

	// finally quit
	viewClose(view);
	camClose(cam);

	// finally we uninitialise the library
	quit_imgproc();
	return 0;
}


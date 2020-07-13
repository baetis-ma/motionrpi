
void bmpImage (Image * img, char str[]) {
        FILE *f;
        f = fopen(str, "wb");

        int w = img->width;
        int h = img->height;
        int filesize = 54 + 3*w*h;
        unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
        unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
        unsigned char bmppad[3] = {0,0,0};

        bmpfileheader[ 2] = (unsigned char)(filesize    );
        bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
        bmpfileheader[ 4] = (unsigned char)(filesize>>16);
        bmpfileheader[ 5] = (unsigned char)(filesize>>24);

        bmpinfoheader[ 4] = (unsigned char)(       w    );
        bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
        bmpinfoheader[ 6] = (unsigned char)(       w>>16);
        bmpinfoheader[ 7] = (unsigned char)(       w>>24);
        bmpinfoheader[ 8] = (unsigned char)(       h    );
        bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
        bmpinfoheader[10] = (unsigned char)(       h>>16);
        bmpinfoheader[11] = (unsigned char)(       h>>24);

        fwrite(bmpfileheader,1,14,f);
        fwrite(bmpinfoheader,1,40,f);
        int x,y;
        for(y=h-1; y>0; y--)
        for(x=0; x<w; x++)
        {
            char * pixel = imgGetPixel(img, x, y);
            fprintf(f, "%c", pixel[0]);
            fprintf(f, "%c", pixel[1]);
            fprintf(f, "%c", pixel[2]);
        }

        fclose(f);
}


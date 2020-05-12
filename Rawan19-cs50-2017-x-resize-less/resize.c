/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    int newPadding;
   int z=0,v;
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int n=atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }


    //for the input file: keep the value of old width(bfo) and old height(bih)
    long long oldwidth=bi.biWidth;
    int oldheight=bi.biHeight;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    //determine new width & height
    bi.biWidth*=n;
    bi.biHeight*=n;
    newPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage=((sizeof(RGBTRIPLE)*bi.biWidth)+newPadding)*abs(bi.biHeight);
    bf.bfSize= bi.biSizeImage+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    /// write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines, for each row
    for (int i = 0, biH = abs(oldheight); i < biH; i++)
    {

      for (v=0;v<n-1;v++){
        fpos_t pos;
        fgetpos(inptr,&pos);

         // temporary storage
        RGBTRIPLE triple;

        // iterate over pixels in scanline; for each pixel :
        for (int j = 0; j < oldwidth; j++)
        {
           // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write pixels to outfile, n times"resize horizontally"
            for(z=0;z<n;z++){
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

        }

        //add padding to the end of each line
        for (int k = 0; k < newPadding; k++)
        {
            fputc(0x00, outptr);
       }

       fsetpos(inptr,&pos);
      }

        for (int j = 0; j < oldwidth; j++)
        {

            RGBTRIPLE triple;

           // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write pixels to outfile, n times"resize horizontally"
            for(z=0;z<n;z++){
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
             }

        }
        //add padding to the end of each line
        for (int k = 0; k < newPadding; k++)
        {
            fputc(0x00, outptr);
        }


        //  skip over padding, that is already made for you, in the input file(you don;t what to resize that!)
        fseek(inptr, padding, SEEK_CUR);

    }




    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

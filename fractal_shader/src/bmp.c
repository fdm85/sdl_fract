/*
 * bmp.c
 *
 *  Created on: 27.04.2019
 *      Author: fdm
 */
#include "bmp.h"
#include <stdio.h>
#include <stdbool.h>
typedef struct _FileHeader
{
    uint16_t bfType;
    uint32_t fileSize;
    uint16_t res1;
    uint16_t res2;
    uint32_t offset;
}FileHeader;

typedef struct _InfoHeader
{
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}InfoHeader;


typedef union _BmpHeader
{
    struct
    {
        FileHeader fileHeader;
        InfoHeader infoHeader;
    };
    uint8_t binary[sizeof(FileHeader)+sizeof(InfoHeader)];
}BmpHeader;

const int bytesPerPixel = sizeof(RGB); /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;
void generateBitmapImage(const char * fileName,const Frame * const f);
unsigned char* createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char* createBitmapInfoHeader(int height, int width);
static void fillConstDataToHeader(BmpHeader *const h)
{
    h->fileHeader.bfType = 19778u;
    h->fileHeader.res1 = 0u;
    h->fileHeader.res2 = 0u;
    h->fileHeader.offset = 54u;

    h->infoHeader.biSize = 40u;
    h->infoHeader.biPlanes = 0;
    h->infoHeader.biBitCount = 24u;
    h->infoHeader.biCompression = 0u;
    h->infoHeader.biSizeImage = 0u;
    h->infoHeader.biXPelsPerMeter = 0u;
    h->infoHeader.biYPelsPerMeter = 0u;
    h->infoHeader.biClrUsed = 0u;
    h->infoHeader.biClrImportant = 0u;
}

static void fillBmpFileHeader(const Frame * const f, BmpHeader *const h)
{
    fillConstDataToHeader(h);
    uint8_t padSize  = (4-(f->pWidth*3u)%4u)%4u;
    uint32_t sizeData = f->pWidth* f->pHeight*3 + f->pHeight*padSize;
    h->fileHeader.fileSize = h->fileHeader.offset + sizeData;
}
void saveToBmp(const char * fileName, const Frame * const f)
{
    BmpHeader h;
    FILE *write_ptr;
    uint8_t padSize  = (4-(f->pWidth*3u)%4u)%4u;
    unsigned char padding[3] = {0, 0, 0};
    fillBmpFileHeader(f, &h);
    write_ptr = fopen(fileName, "wb");  // w for write, b for binary

    if(write_ptr == NULL)
    {
        puts("Error on file open");
    }
    else
    {
        fwrite(h.binary, 1u, h.fileHeader.offset,write_ptr);
        for (uint32_t i = 0u; i < f->pHeight; ++i)
        {
           for (uint32_t j = 0u; j < f->pWidth; ++j)
           {
                Pixel *k = &(f->p[(i * f->pWidth) +j]);
                fwrite(&k->color, sizeof(RGB), f->pWidth, write_ptr); //write one pixel line to file
                fwrite(padding, 1, padSize, write_ptr);
           }
        }
        fclose(write_ptr);
    }
    FILE *read_ptr = fopen(fileName, "rb");

}




void generateBitmapImage(const char * fileName,const Frame * const f)
{
    printf ("generateBitmapImage \n");
    fflush(stdout);
    unsigned char padding[3] = {0, 0, 0};
    uint32_t paddingSize = (4 - (f->pWidth*bytesPerPixel) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(f->pHeight, f->pWidth, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(f->pHeight, f->pWidth);

    FILE* imageFile = fopen(fileName, "wb");

    if(imageFile != NULL)
    {
        fwrite(fileHeader, 1, fileHeaderSize, imageFile);
        fwrite(infoHeader, 1, infoHeaderSize, imageFile);
        bool keepGoing = true;
        for(uint32_t i = f->pHeight -1u; keepGoing; --i)
        {
            for(uint32_t j = 0uL; j < f->pWidth && keepGoing; ++j)
            {
                Pixel *k = &(f->p[(i * f->pWidth) + j]);
                uint8_t writeCount;
                writeCount = fwrite(&k->color, 1u, sizeof(RGB), imageFile);
                writeCount += fwrite(padding, 1, paddingSize, imageFile);
                if(writeCount != (sizeof(RGB) + paddingSize) )
                {
                    fprintf(stdout, "Could not write to file i: %d  | j: %d", i, j);
                    fclose(imageFile);
                    keepGoing = false;
                }
            }
            if(i == 0uL)
            {
                break;
            }
        }

        fclose(imageFile);
    }
    else
    {
        puts("Error on file open");
    }
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}

#include <fstream>

#include "Bitmap.h"
using namespace std;

bool Bitmap::Load24(string fileName)
{
    this->loaded = false;
    ifstream file;

    file.open(fileName.c_str(), std::ios::binary);

    if (!file.is_open()) { return this->loaded; }

    // BMP header variables //
    unsigned char b = 0, m = 0;
    unsigned int bmpSize = 0;
    unsigned int offset = 0;

    // DIB header variables //
    unsigned int numBytesHeader = 0;
    unsigned int pixelWidth = 0, pixelHeight = 0;
    unsigned short numColorPlanes = 0;
    unsigned short bitsPerPixel = 0;
    unsigned int BI_RGB = 0;
    unsigned int rawBitmapSize = 0;
    unsigned int printResHoriz = 0;
    unsigned int printResVert = 0;
    unsigned int numColorsInPalette = 0;
    unsigned int importantColors = 0;

#define READ_TO_VAR(out, type) file.read(reinterpret_cast<char*>(&out), sizeof(type))

    // BMP header load //
    READ_TO_VAR(b, unsigned char);
    READ_TO_VAR(m, unsigned char);

    if (b != 'B' || m != 'M') { return this->loaded; }

    READ_TO_VAR(bmpSize, unsigned int);
    file.ignore(4);
    READ_TO_VAR(offset, unsigned int);

    // DIB header load //
    READ_TO_VAR(numBytesHeader, unsigned int);
    READ_TO_VAR(pixelWidth, unsigned int);
    READ_TO_VAR(pixelHeight, unsigned int);
    READ_TO_VAR(numColorPlanes, unsigned short);
    READ_TO_VAR(bitsPerPixel, unsigned short);
    READ_TO_VAR(BI_RGB, unsigned int);
    READ_TO_VAR(rawBitmapSize, unsigned int);
    READ_TO_VAR(printResHoriz, unsigned int);
    READ_TO_VAR(printResVert, unsigned int);
    READ_TO_VAR(numColorsInPalette, unsigned int);
    READ_TO_VAR(importantColors, unsigned int);

    if (bitsPerPixel != 24) { return this->loaded; }

    this->width = pixelWidth;
    this->height = pixelHeight;
    this->bpp = bitsPerPixel;

    // Pixel array load //
    this->bitmap = new RGB[pixelWidth * pixelHeight];

    for (int row = pixelHeight - 1; row >= 0; row--)
    {
        for (int col = 0; col < pixelWidth; col++)
        {
            READ_TO_VAR(this->bitmap[(row * pixelWidth) + col].b, unsigned char);
            READ_TO_VAR(this->bitmap[(row * pixelWidth) + col].g, unsigned char);
            READ_TO_VAR(this->bitmap[(row * pixelWidth) + col].r, unsigned char);
        }
        file.ignore((3 * pixelWidth) % 4);
    }

    file.close();
    this->loaded = true;
    return this->loaded;
}

RGB Bitmap::GetPixel(int x, int y)
{
    if (((x < 0) || x >= width) || ((y < 0) || (y >= height)))
    {
        return { 255, 0, 255 };
    }

    return bitmap[(y * width) + x];
}
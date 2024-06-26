#pragma once

#include <string>
#include <FreeImage.h>
#include <GL/glew.h>
#include <iostream>
//#include <Utilities.h>

namespace sparky
{
    static char* load_image(const char* filename,  GLsizei* width, GLsizei* height, unsigned int* bits)
    {
        FreeImage_Initialise();

        FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
        FIBITMAP *dib = nullptr;
        fif = FreeImage_GetFileType(filename, 0);
        if(fif == FIF_UNKNOWN)
            fif = FreeImage_GetFIFFromFilename(filename);
        if(fif == FIF_UNKNOWN)
            return nullptr;

        if(FreeImage_FIFSupportsReading(fif))
            dib = FreeImage_Load(fif, filename);
        if(!dib)
            return nullptr;

        BYTE* pixels = FreeImage_GetBits(dib);
        *width = FreeImage_GetWidth(dib);
        *height = FreeImage_GetHeight(dib);
        *bits = FreeImage_GetBPP(dib);

        //SwapRedBlue32(dib);

        int size = *width * *height * (*bits / 8);
        char* result = new char[size];
        memcpy(result, pixels, size);
        FreeImage_DeInitialise();
        FreeImage_Unload(dib);

        return result;
    }
}
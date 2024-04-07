#pragma once

#include "../maths/maths.h"

namespace sparky {

        static unsigned int transform_color(float r, float g, float b, float a)
        {
            float red = r;
            float green = g;
            float blue = b;
            float alfa = a;

            unsigned int packedR = (unsigned int)(red * 255);
            unsigned int packedG = (unsigned int)(green * 255) << 8;
            unsigned int packedB = (unsigned int)(blue * 255) << 16;
            unsigned int packedA = (unsigned int)(alfa * 255) << 24;

            return packedR + packedG + packedB + packedA;
        }

}
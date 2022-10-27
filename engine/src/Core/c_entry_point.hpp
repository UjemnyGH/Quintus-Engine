#pragma once
#ifndef __C_ENTRY_POINT_
#define __C_ENTRY_POINT_

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb/stb_image.h"

void Application();

int main() {
    Application();

    return 0;
}

#endif
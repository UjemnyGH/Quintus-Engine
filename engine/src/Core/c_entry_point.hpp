#pragma once
#ifndef __C_ENTRY_POINT_
#define __C_ENTRY_POINT_

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb/stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../../vendor/stb/stb_truetype.h"
#include "../Window/w_window.hpp"
float qe::QE_Window::m_left = -2.0f;
float qe::QE_Window::m_right = 2.0f;
float qe::QE_Window::m_top = 2.0f;
float qe::QE_Window::m_bottom = -2.0f;
float qe::QE_Window::m_fov = 70.0f;
float qe::QE_Window::m_far = 10000.0f;
float qe::QE_Window::m_near = 0.001f;
bool qe::QE_Window::m_orthographic = false;
uint32_t qe::QE_Window::m_width = 800;
uint32_t qe::QE_Window::m_height = 600;

void Application();

int main() {
    Application();

    return 0;
}

#endif
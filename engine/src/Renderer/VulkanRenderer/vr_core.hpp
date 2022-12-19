#pragma once
#ifndef __VR_CORE_
#define __VR_CORE_

#include <vulkan/vulkan.h>
#include <vector>
#include <cstring>
#include "../../Core/c_engine_functions.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>

#ifdef __linux
#define VK_USE_PLATFORM_XLIB_KHW
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef VULKAN_DEBUG
bool enable_validation_layers = true;
#else
bool enable_validation_layers = false;

#endif

#include "vr_instance.hpp"
#include "vr_devices.hpp"
#include "vr_swapchain.hpp"

#endif
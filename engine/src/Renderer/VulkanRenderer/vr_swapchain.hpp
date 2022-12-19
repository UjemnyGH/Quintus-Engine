#pragma once
#ifndef __VR_SWPACHAIN_
#define __VR_SWPACHAIN_

#include "vr_core.hpp"

namespace qe {
    class Swapchain {
    private:
        VkSwapchainKHR m_swapchain;
        Devices *m_devices;

        VkSurfaceFormatKHR chooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR> formats) {
            for(const auto& format : formats) {
                if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return format;
                }
            }

            return formats[0];
        }

        VkPresentModeKHR chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> present_modes) {
            for(const auto& present_mode : present_modes) {
                if(present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return present_mode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
            if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            }
            else {
                int width, height;

                glfwGetFramebufferSize(m_devices->getVulkanInstanceClassPtr()->getWindow(), &width, &height);

                VkExtent2D extent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            
                return extent;
            }
        }

    public:
        Swapchain() = default;

        void CreateSwapchain(Devices *devices) {
            m_devices = devices;

            SwapchainSupportDetails details = querySwapchainSupport(*m_devices->getVulkanPhysicalDevice(), *m_devices->getVulkanInstanceClassPtr()->getVulkanPresentationSurface());

            VkSurfaceFormatKHR swapchain_format = 
        }
    };
}

#endif
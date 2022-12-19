#pragma once
#ifndef __VR_DEVICES_
#define __VR_DEVICES_

#include "vr_core.hpp"

namespace qe {
    struct QueueFamilyIndices {
        std::optional<uint32_t> m_graphics_family;
        std::optional<uint32_t> m_presentation_family;

        bool _is_complete() {
            return m_graphics_family.has_value() && m_presentation_family.has_value();
        }
    };

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_present_modes;
    };

    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice _physical_device, VkSurfaceKHR _surface) {
        SwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physical_device, _surface, &details.m_capabilities);

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &format_count, nullptr);

        if(format_count != 0) {
            details.m_formats.resize(format_count);

            vkGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &format_count, details.m_formats.data());
        }

        uint32_t present_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &present_count, nullptr);

        if(present_count != 0) {
            details.m_present_modes.resize(present_count);

            vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &present_count, details.m_present_modes.data());
        }

        return details;
    }

    QueueFamilyIndices findQueueFamily(VkPhysicalDevice _physical_device, VkSurfaceKHR _surface) {
        QueueFamilyIndices indices;

        uint32_t queue_count;
        vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_properties(queue_count);
        vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_count, queue_properties.data());

        uint32_t i = 0;
        for(const auto& family : queue_properties) {
            VkBool32 presentation_supported = false;

            vkGetPhysicalDeviceSurfaceSupportKHR(_physical_device, i, _surface, &presentation_supported);

            if(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.m_graphics_family = i;
            }

            if(presentation_supported) {
                indices.m_presentation_family = i;
            }

            if(indices._is_complete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    class Devices {
    private:
        VkPhysicalDevice m_physical_device;
        VkDevice m_logical_device;
        Instance *m_instance;
        VkQueue m_graphics_queue;
        VkQueue m_presentation_queue;

        bool _check_device_extension_support(VkPhysicalDevice _physical_device) {
            uint32_t extension_count;
            vkEnumerateDeviceExtensionProperties(_physical_device, nullptr, &extension_count, nullptr);

            std::vector<VkExtensionProperties> extension_properties(extension_count);
            vkEnumerateDeviceExtensionProperties(_physical_device, nullptr, &extension_count, extension_properties.data());

            std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

            for(const auto& extension : extension_properties) {
                required_extensions.erase(extension.extensionName);
            }

            return required_extensions.empty();
        }

        bool _is_device_suitable(VkPhysicalDevice _physical_device) {
            QueueFamilyIndices indices = findQueueFamily(_physical_device, *m_instance->getVulkanPresentationSurface());

            bool extension_supported = _check_device_extension_support(_physical_device);

            bool swapchain_adequate = false;

            if(extension_supported) {
                SwapchainSupportDetails details = querySwapchainSupport(_physical_device, *m_instance->getVulkanPresentationSurface());

                swapchain_adequate = !details.m_formats.empty() && !details.m_present_modes.empty();
            }

            VkPhysicalDeviceFeatures support_features;
            vkGetPhysicalDeviceFeatures(_physical_device, &support_features);

            return indices._is_complete() && extension_supported && swapchain_adequate && support_features.samplerAnisotropy;
        }

    public:
        Devices() = default;

        void CreateDevices(Instance* instance) {
            m_instance = instance;

            uint32_t count;
            vkEnumeratePhysicalDevices(*m_instance->getVulkanInstancePtr(), &count, nullptr);

            if(count == 0) {
                qe::qe_term("Cannot find any physical devices suitable to render graphics by Vulkan!");
            }

            std::vector<VkPhysicalDevice> physical_devices(count);
            vkEnumeratePhysicalDevices(*m_instance->getVulkanInstancePtr(), &count, physical_devices.data());

            for(const auto& physical_device : physical_devices) {
                if(_is_device_suitable(physical_device)) {
                    m_physical_device = physical_device;

                    break;
                }
            }
            
            if(m_physical_device == nullptr) {
                qe::qe_term("Cannot choose physical deivce for Vulkan!");
            }

            QueueFamilyIndices indices = findQueueFamily(m_physical_device, *m_instance->getVulkanPresentationSurface());

            std::vector<VkDeviceQueueCreateInfo> queue_infos;
            std::set<uint32_t> unique_queue_families = {indices.m_graphics_family.value(), indices.m_presentation_family.value()};

            float queue_priority = 1.0f;

            for(uint32_t queue_family : unique_queue_families) {
                VkDeviceQueueCreateInfo dq_info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                dq_info.queueFamilyIndex = queue_family;
                dq_info.queueCount = 1;
                dq_info.pQueuePriorities = &queue_priority;

                queue_infos.push_back(dq_info);
            }

            VkPhysicalDeviceFeatures device_features{};
            device_features.samplerAnisotropy = true;

            VkDeviceCreateInfo device_info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
            device_info.queueCreateInfoCount = queue_infos.size();
            device_info.pQueueCreateInfos = queue_infos.data();
            device_info.pEnabledFeatures = &device_features;
            device_info.enabledExtensionCount = device_extensions.size();
            device_info.ppEnabledExtensionNames = device_extensions.data();

            if(enable_validation_layers) {
                device_info.enabledLayerCount = validation_layers.size();
                device_info.ppEnabledLayerNames = validation_layers.data();
            }
            else {
                device_info.enabledLayerCount = 0;
            }

            if(vkCreateDevice(m_physical_device, &device_info, nullptr, &m_logical_device) != VK_SUCCESS) {
                qe:qe_term("Cannot create Vulkan logical device!");
            }

            vkGetDeviceQueue(m_logical_device, indices.m_graphics_family.value(), 0, &m_graphics_queue);
            vkGetDeviceQueue(m_logical_device, indices.m_presentation_family.value(), 0, &m_presentation_queue);
        }

        Instance* getVulkanInstanceClassPtr() { return m_instance; }

        VkPhysicalDevice* getVulkanPhysicalDevice() { return &m_physical_device; }

        VkDevice* getVulkanLogicalDevice() { return &m_logical_device; }

        VkQueue* getVulkanGraphicsQueue() { return &m_graphics_queue; }

        VkQueue* getVulkanPresentationQueue() { return &m_presentation_queue; }
    };
}

#endif
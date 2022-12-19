#pragma once
#ifndef __VR_INSTANCE_
#define __VR_INSTANCE_

#include "vr_core.hpp"

namespace qe {
    class Instance {
    private:
        VkInstance m_instance;

        VkDebugUtilsMessengerEXT m_debug_messenger;

        VkSurfaceKHR m_presentation_surface;

        GLFWwindow* m_window;

        bool _check_validation_layer_support() {
            uint32_t count;
            vkEnumerateInstanceLayerProperties(&count, nullptr);

            std::vector<VkLayerProperties> properties(count);
            vkEnumerateInstanceLayerProperties(&count, properties.data());

            for(const char* layer_name : validation_layers) {
                bool layer_found = false;

                for(const auto& layer : properties) {
                    if(strcmp(layer_name, layer.layerName)) {
                        layer_found = true;

                        break;
                    }
                }

                if(!layer_found) {
                    return false;
                }
            }

            return true;
        }

        std::vector<const char*> _get_required_extensions() {
            uint32_t count;
            const char** extensions = glfwGetRequiredInstanceExtensions(&count);

            std::vector<const char*> required_extensions(extensions, extensions + count);

            if(enable_validation_layers) {
                required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return required_extensions;
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL _debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
                                                            const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
                                                            void*                                            pUserData) {
            std::cerr << pCallbackData->pMessage << "\n";

            return VK_FALSE;
        }

        void _populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT &debug_messenger) {
            debug_messenger = {};
            debug_messenger.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debug_messenger.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
            
            debug_messenger.messageType =  VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

            debug_messenger.pfnUserCallback = _debug_callback;
        }

        VkResult _create_debug_utils_messanger_ext(VkInstance instance, 
                                                    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                                    const VkAllocationCallbacks* pAllocator,
                                                    VkDebugUtilsMessengerEXT *pDebugMessenger) {
            auto cdume_fn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

            if(cdume_fn != nullptr) {
                return cdume_fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
            }
        }

        void _destroy_debug_utils_messenger_ext(VkInstance instance,
                                                VkDebugUtilsMessengerEXT Messenger,
                                                const VkAllocationCallbacks* pAllocator) {
            auto ddume_fn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

            if(ddume_fn != nullptr) {
                ddume_fn(instance, Messenger, pAllocator);
            }
        }

        void _create_debug_messanger() {
            if(!enable_validation_layers) return;

            VkDebugUtilsMessengerCreateInfoEXT debug_info{};
            _populate_debug_messenger_create_info(debug_info);

            if(_create_debug_utils_messanger_ext(m_instance, &debug_info, nullptr, &m_debug_messenger) != VK_SUCCESS) {
                qe::qe_warn("Cannot create Vulkan debug messanger!");
            }
        }

    public:
        Instance() = default;

        void CreateInstance(uint32_t api_version = VK_API_VERSION_1_3) {
            if(enable_validation_layers && !_check_validation_layer_support()) {
                qe::qe_term("Validation layers are not available!");
            }

            VkApplicationInfo app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO};
            app_info.apiVersion = api_version;
            app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
            app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
            app_info.pApplicationName = "Quintus_App";
            app_info.pEngineName = "Quintus_Engine";

            VkInstanceCreateInfo instance_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
            instance_info.pApplicationInfo = &app_info;

            std::vector<const char*> glfw_extensions = _get_required_extensions();

            instance_info.enabledExtensionCount = glfw_extensions.size();
            instance_info.ppEnabledExtensionNames = glfw_extensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debug_info;

            if(enable_validation_layers) {
                instance_info.enabledLayerCount = validation_layers.size();
                instance_info.ppEnabledLayerNames = validation_layers.data();
            
                _populate_debug_messenger_create_info(debug_info);
                instance_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_info;
            }
            else {
                instance_info.enabledExtensionCount = 0;
            }

            if(vkCreateInstance(&instance_info, nullptr, &m_instance) != VK_SUCCESS) {
                qe::qe_term("Cannot create Vulkan instance!");
            }

            uint32_t extension_count;
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

            std::vector<VkExtensionProperties> extension_properties(extension_count);
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties.data());

            _create_debug_messanger();
        }

        void CreatePresentationSurface(GLFWwindow* window) {
            m_window = window;

            if(glfwCreateWindowSurface(m_instance, window, nullptr, &m_presentation_surface) != VK_SUCCESS) {
                qe::qe_term("Cannot create Vulkan presentation surface!");
            }
        }

        VkInstance* getVulkanInstancePtr() { return &m_instance; }

        VkDebugUtilsMessengerEXT* getVulkanDebugUtilsMessengerEXT() { return &m_debug_messenger; }

        VkSurfaceKHR* getVulkanPresentationSurface() { return &m_presentation_surface; }

        GLFWwindow* getWindow() { return m_window; }
    };
}

#endif
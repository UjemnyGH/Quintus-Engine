#pragma once
#ifndef __VR_SWPACHAIN_
#define __VR_SWPACHAIN_

#include "vr_core.hpp"

namespace qe {
    VkShaderModule LoadSPIRVShader() {
        
    }

    class Swapchain {
    private:
        VkSwapchainKHR m_swapchain;
        Devices *m_devices;
        std::vector<VkImage> m_swapchain_images;
        VkExtent2D m_swapchain_extent;
        VkFormat m_swapchain_format;

        VkImageView m_depth_view;
        VkImage m_depth_image;
        VkDeviceMemory m_depth_memory;
        VkDescriptorSetLayout m_descriptor_set_layout;

        std::vector<VkFramebuffer> m_swapchain_framebuffers;
        std::vector<VkImageView> m_swapchain_image_views;

        VkRenderPass m_render_pass;

        VkSurfaceFormatKHR _choose_swapchain_format(const std::vector<VkSurfaceFormatKHR> formats) {
            for(const auto& format : formats) {
                if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return format;
                }
            }

            return formats[0];
        }

        VkPresentModeKHR _choose_swapchain_present_mode(const std::vector<VkPresentModeKHR> present_modes) {
            for(const auto& present_mode : present_modes) {
                if(present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return present_mode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D _choose_swapchain_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
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

        VkImageView _create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags) {
            VkImageViewCreateInfo image_view_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            image_view_info.image = image;
            image_view_info.format = format;
            image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_info.subresourceRange.aspectMask = aspect_flags;
            image_view_info.subresourceRange.baseMipLevel = 0;
            image_view_info.subresourceRange.levelCount = 1;
            image_view_info.subresourceRange.baseArrayLayer = 0;
            image_view_info.subresourceRange.layerCount = 1;

            VkImageView image_view;

            if(vkCreateImageView(*m_devices->getVulkanLogicalDevice(), &image_view_info, nullptr, &image_view) != VK_SUCCESS) {
                qe::qe_warn("Cannot create image view!");
            }

            return image_view;
        }

        VkFormat _find_supported_format(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features) {
            for(VkFormat format : formats) {
                VkFormatProperties properties;
                vkGetPhysicalDeviceFormatProperties(*m_devices->getVulkanPhysicalDevice(), format, &properties);

                if(tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features)) {
                    return format;
                }
                else if(tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features)) {
                    return format;
                }
            }

            qe::qe_warn("Cannot find right features for format!");
        }

        VkFormat _get_depth_format() {
            return _find_supported_format({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        }

    public:
        Swapchain() = default;

        void CreateSwapchain(Devices *devices) {
            m_devices = devices;

            SwapchainSupportDetails details = querySwapchainSupport(*m_devices->getVulkanPhysicalDevice(), *m_devices->getVulkanInstanceClassPtr()->getVulkanPresentationSurface());

            VkSurfaceFormatKHR swapchain_format = _choose_swapchain_format(details.m_formats);
            VkPresentModeKHR swapchain_present_mode = _choose_swapchain_present_mode(details.m_present_modes);
            VkExtent2D swapchain_extent = _choose_swapchain_extent(details.m_capabilities);

            uint32_t image_count = details.m_capabilities.minImageCount + 1;

            if(details.m_capabilities.maxImageCount > 0 & image_count > details.m_capabilities.maxImageCount) {
                image_count = details.m_capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR swapchain_info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
            swapchain_info.surface = *m_devices->getVulkanInstanceClassPtr()->getVulkanPresentationSurface();
            swapchain_info.minImageCount = image_count;
            swapchain_info.imageFormat = swapchain_format.format;
            swapchain_info.imageColorSpace = swapchain_format.colorSpace;
            swapchain_info.imageExtent = swapchain_extent;
            swapchain_info.imageArrayLayers = 1;
            swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = findQueueFamily(*m_devices->getVulkanPhysicalDevice(), *m_devices->getVulkanInstanceClassPtr()->getVulkanPresentationSurface());

            uint32_t queue_family_indices[] = {indices.m_graphics_family.value(), indices.m_presentation_family.value()};

            if(indices.m_graphics_family != indices.m_presentation_family) {
                swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchain_info.queueFamilyIndexCount = 1;
                swapchain_info.pQueueFamilyIndices = queue_family_indices;
            }
            else {
                swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                swapchain_info.queueFamilyIndexCount = 0;
            }

            swapchain_info.preTransform = details.m_capabilities.currentTransform;
            swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchain_info.presentMode = swapchain_present_mode;
            swapchain_info.clipped = true;

            swapchain_info.oldSwapchain = nullptr;

            if(vkCreateSwapchainKHR(*m_devices->getVulkanLogicalDevice(), &swapchain_info, nullptr, &m_swapchain) != VK_SUCCESS) {
                qe::qe_term("Cannot create swapchain!");
            }

            vkGetSwapchainImagesKHR(*m_devices->getVulkanLogicalDevice(), m_swapchain, &image_count, nullptr);

            m_swapchain_images.resize(image_count);
            vkGetSwapchainImagesKHR(*m_devices->getVulkanLogicalDevice(), m_swapchain, &image_count, m_swapchain_images.data());

            m_swapchain_format = swapchain_format.format;
            m_swapchain_extent = swapchain_extent;
        }

        void CreateRenderPass() {
            VkAttachmentDescription color_attachment{};
            color_attachment.format = m_swapchain_format;
            color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentDescription depth_attachment{};
            depth_attachment.format = _get_depth_format();
            depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference color_attachment_reference{};
            color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            color_attachment_reference.attachment = 0;

            VkAttachmentReference depth_attachment_reference{};
            depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depth_attachment_reference.attachment = 1;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &color_attachment_reference;
            subpass.pDepthStencilAttachment = &depth_attachment_reference;

            VkSubpassDependency dependency{};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            std::array<VkAttachmentDescription, 2> attachments = {color_attachment, depth_attachment};

            VkRenderPassCreateInfo render_pass_info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
            render_pass_info.attachmentCount = attachments.size();
            render_pass_info.pAttachments = attachments.data();
            render_pass_info.subpassCount = 1;
            render_pass_info.pSubpasses = &subpass;
            render_pass_info.dependencyCount = 1;
            render_pass_info.pDependencies = &dependency;

            if(vkCreateRenderPass(*m_devices->getVulkanLogicalDevice(), &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS) {
                qe:qe_term("Cannot create render pass!");
            }
        }

        void CreateDescriptorSetLayout() {
            VkDescriptorSetLayoutBinding ubo_layout_descriptor{};
            ubo_layout_descriptor.binding = 0;
            ubo_layout_descriptor.descriptorCount = 1;
            ubo_layout_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ubo_layout_descriptor.pImmutableSamplers = nullptr;
            ubo_layout_descriptor.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutBinding sampler_layout_descriptor{};
            sampler_layout_descriptor.binding = 1;
            sampler_layout_descriptor.descriptorCount = 1;
            sampler_layout_descriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            sampler_layout_descriptor.pImmutableSamplers = nullptr;
            sampler_layout_descriptor.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            std::array<VkDescriptorSetLayoutBinding, 2> bindings = {ubo_layout_descriptor, sampler_layout_descriptor};

            VkDescriptorSetLayoutCreateInfo layout_set_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
            layout_set_info.bindingCount = bindings.size();
            layout_set_info.pBindings = bindings.data();

            if(vkCreateDescriptorSetLayout(*m_devices->getVulkanLogicalDevice(), &layout_set_info, nullptr, &m_descriptor_set_layout) != VK_SUCCESS) {
                qe::qe_warn("Cannot create descriptor set layout!");
            }
        }

        void CreateGraphicsPipeline() {

        }

        void AddDescriptorSetLayout(uint32_t index, VkDescriptorType type, VkShaderStageFlags flag) {
            VkDescriptorSetLayoutBinding ubo_layout_descriptor{};
            ubo_layout_descriptor.binding = index;
            ubo_layout_descriptor.descriptorCount = 1;
            ubo_layout_descriptor.descriptorType = type;
            ubo_layout_descriptor.pImmutableSamplers = nullptr;
            ubo_layout_descriptor.stageFlags = flag;
        }

        void ClearSwapchain() {
            vkDestroyImageView(*m_devices->getVulkanLogicalDevice(), m_depth_view, nullptr);
            vkDestroyImage(*m_devices->getVulkanLogicalDevice(), m_depth_image, nullptr);
            vkFreeMemory(*m_devices->getVulkanLogicalDevice(), m_depth_memory, nullptr);

            for(auto framebuffer : m_swapchain_framebuffers) {
                vkDestroyFramebuffer(*m_devices->getVulkanLogicalDevice(), framebuffer, nullptr);
            }

            for(auto image_views : m_swapchain_image_views) {
                vkDestroyImageView(*m_devices->getVulkanLogicalDevice(), image_views, nullptr);
            }

            vkDestroySwapchainKHR(*m_devices->getVulkanLogicalDevice(), m_swapchain, nullptr);
        }

        void RecreateSwapchain() {

        }

        void CreateImageViews() {
            m_swapchain_image_views.resize(m_swapchain_images.size());

            for(size_t i = 0; i < m_swapchain_images.size(); i++) {
                m_swapchain_image_views[i] = _create_image_view(m_swapchain_images[i], m_swapchain_format, VK_IMAGE_ASPECT_COLOR_BIT);
            }
        }
    };
}

#endif
#pragma once
#ifndef __R_RENDERER_HELPERS_
#define __R_RENDERER_HELPERS_

#include <glm/glm.hpp>
#include <vector>

namespace qe
{
    struct QE_AxisHelper {
        float position[3];
        float scale[3];
        float rotation[3];

        glm::vec3 pos = glm::vec3(position[0], position[1], position[2]);
        glm::vec3 scl = glm::vec3(scale[0], scale[1], scale[2]);
        glm::vec3 rot = glm::vec3(rotation[0], rotation[1], rotation[2]);
    };

    struct QE_RenderedData {
        std::vector<float> m_vertices;
        std::vector<float> m_color;
        std::vector<float> m_texture_coordinates;
        std::vector<float> m_normals;

        std::vector<uint32_t> m_indices;
    };

    struct QE_Rendered {
        std::vector<QE_RenderedData> m_data;
        std::vector<QE_AxisHelper> m_axis_helper;
        std::vector<uint32_t> m_data_sizes;
        QE_RenderedData m_joined_data;

        uint32_t m_data_sizes_size;
    };

    typedef QE_AxisHelper AxisHelper;
    typedef QE_RenderedData RenderedData;
    typedef QE_Rendered Rendered;
} // namespace qe


#endif
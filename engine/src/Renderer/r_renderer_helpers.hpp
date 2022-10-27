#pragma once
#ifndef __R_RENDERER_HELPERS_
#define __R_RENDERER_HELPERS_

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "../Core/c_algorithms.hpp"

namespace qe
{
    struct QE_AxisHelper {
        float position[3];
        float scale[3];
        float rotation[3];
    };

    struct QE_RenderedData {
        std::vector<float> m_vertices;
        std::vector<float> m_color;
        std::vector<float> m_texture_coordinates;
        std::vector<float> m_normals;

        std::vector<uint32_t> m_indices;

        void Clear() {
            m_vertices.clear();
            m_color.clear();
            m_texture_coordinates.clear();
            m_normals.clear();
            m_indices.clear();
        }
    };

    struct QE_Rendered {
        std::vector<QE_RenderedData> m_data;
        std::vector<QE_AxisHelper> m_axis_helper;
        std::vector<uint32_t> m_data_sizes;
        std::vector<uint32_t> m_data_end;
        QE_RenderedData m_joined_data;

        void JoinData() {
            m_joined_data.Clear();
            m_data_sizes.clear();
            m_data_end.clear();

            for(QE_RenderedData data : m_data) {
                std::copy(data.m_vertices.begin(), data.m_vertices.end(), m_joined_data.m_vertices.end());
                std::copy(data.m_color.begin(), data.m_color.end(), m_joined_data.m_color.end());
                std::copy(data.m_texture_coordinates.begin(), data.m_texture_coordinates.end(), m_joined_data.m_texture_coordinates.end());
                std::copy(data.m_normals.begin(), data.m_normals.end(), m_joined_data.m_normals.end());
                std::copy(data.m_indices.begin(), data.m_indices.end(), m_joined_data.m_indices.end());

                if (m_data_end.size() == 0) {
                    m_data_end.push_back(data.m_vertices.size() / 3);
                }
                else {
                    m_data_end.push_back(data.m_vertices.size() / 3 + m_data_end[m_data_end.size() - 1]);
                }
                
                m_data_sizes.push_back(data.m_vertices.size() / 3);
            }
        }

        void PushData(QE_RenderedData data) {
            m_data.push_back(data);
            m_axis_helper.push_back(QE_AxisHelper{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});

            std::copy(data.m_vertices.begin(), data.m_vertices.end(), m_joined_data.m_vertices.end());
            std::copy(data.m_color.begin(), data.m_color.end(), m_joined_data.m_color.end());
            std::copy(data.m_texture_coordinates.begin(), data.m_texture_coordinates.end(), m_joined_data.m_texture_coordinates.end());
            std::copy(data.m_normals.begin(), data.m_normals.end(), m_joined_data.m_normals.end());
            std::copy(data.m_indices.begin(), data.m_indices.end(), m_joined_data.m_indices.end());

            if (m_data_end.size() == 0) {
                m_data_end.push_back(data.m_vertices.size() / 3);
            }
            else {
                m_data_end.push_back(data.m_vertices.size() / 3 + m_data_end[m_data_end.size() - 1]);
            }

            m_data_sizes.push_back(data.m_vertices.size() / 3);
        }

        void PopData(uint32_t id) {
            m_data.erase(m_data.begin() + id);
            m_axis_helper.erase(m_axis_helper.begin() + id);
            
            JoinData();
        }

        void PopData(QE_RenderedData data) {
            uint32_t index = Search(m_data, data);

            m_data.erase(m_data.begin() + index);
            m_axis_helper.erase(m_axis_helper.begin() + index);
            
            JoinData();
        }
    };

    typedef QE_AxisHelper AxisHelper;
    typedef QE_RenderedData RenderedData;
    typedef QE_Rendered Rendered;
} // namespace qe


#endif
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

        QE_AxisHelper(float p = 0.0f, float s = 1.0f, float r = 0.0f) {
            position[0] = position[1] = position[2] = p;
            scale[0] = scale[1] = scale[2] = s;
            rotation[0] = rotation[1] = rotation[2] = r;
        }
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

        bool operator==(QE_RenderedData &data) {
            return std::equal(this->m_vertices.begin(), this->m_vertices.end(), data.m_vertices.begin(), data.m_vertices.end()) &&
                std::equal(this->m_color.begin(), this->m_color.end(), data.m_color.begin(), data.m_color.end()) &&
                std::equal(this->m_texture_coordinates.begin(), this->m_texture_coordinates.end(), data.m_texture_coordinates.begin(), data.m_texture_coordinates.end()) &&
                std::equal(this->m_normals.begin(), this->m_normals.end(), data.m_normals.begin(), data.m_normals.end()) &&
                std::equal(this->m_indices.begin(), this->m_indices.end(), data.m_indices.begin(), data.m_indices.end());
        }
    };

    struct QE_Rendered {
        std::vector<QE_RenderedData> m_data;
        std::vector<QE_AxisHelper> m_axis_helper;
        std::vector<uint32_t> m_data_sizes;
        std::vector<uint32_t> m_data_end;
        std::vector<float> m_texture_index;
        QE_RenderedData m_joined_data;

        void JoinData() {
            m_joined_data.Clear();
            m_data_sizes.clear();
            m_data_end.clear();

            for(QE_RenderedData data : m_data) {
                std::copy(data.m_vertices.begin(), data.m_vertices.end(), std::back_inserter(m_joined_data.m_vertices));
                std::copy(data.m_color.begin(), data.m_color.end(), std::back_inserter(m_joined_data.m_color));
                std::copy(data.m_texture_coordinates.begin(), data.m_texture_coordinates.end(), std::back_inserter(m_joined_data.m_texture_coordinates));
                std::copy(data.m_normals.begin(), data.m_normals.end(), std::back_inserter(m_joined_data.m_normals));
                std::copy(data.m_indices.begin(), data.m_indices.end(), std::back_inserter(m_joined_data.m_indices));

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
            m_axis_helper.push_back(QE_AxisHelper());

            std::copy(data.m_vertices.begin(), data.m_vertices.end(), std::back_inserter(m_joined_data.m_vertices));
            std::copy(data.m_color.begin(), data.m_color.end(), std::back_inserter(m_joined_data.m_color));
            std::copy(data.m_texture_coordinates.begin(), data.m_texture_coordinates.end(), std::back_inserter(m_joined_data.m_texture_coordinates));
            std::copy(data.m_normals.begin(), data.m_normals.end(), std::back_inserter(m_joined_data.m_normals));
            std::copy(data.m_indices.begin(), data.m_indices.end(), std::back_inserter(m_joined_data.m_indices));

            if (m_data_end.size() == 0) {
                m_data_end.push_back(data.m_vertices.size() / 3);
            }
            else {
                m_data_end.push_back(data.m_vertices.size() / 3 + m_data_end[m_data_end.size() - 1]);
            }

            m_data_sizes.push_back(data.m_vertices.size() / 3);

            m_texture_index.resize(m_texture_index.size() + (data.m_vertices.size() / 3));
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
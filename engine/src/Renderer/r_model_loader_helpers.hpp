#pragma once
#ifndef __R_MODEL_LOADER_HELPERS_
#define __R_MODEL_LOADER_HELPERS_

#include <algorithm>
#include <vector>

namespace qe
{
    /**
     * Non binary formats OBJ, DAE, X3D
     * 
     */

    enum QE_ModelType {
        OBJ = 0,
        GLTF = 1,
        STL = 2,
        DAE = 3,
        PLY = 4,
        X3D = 5,
        FBX = 6,
        ABC = 7,
    };

    /*struct QE_MeshData
    {
        std::vector<float> m_vertices;
        std::vector<float> m_textureCoordinates;
        std::vector<float> m_normals;

        std::vector<unsigned int> m_indices;
        std::vector<unsigned int> m_indicesTexture;
        std::vector<unsigned int> m_indicesNormals;

        QE_BufferMeshData& operator=(const QE_BufferMeshData& m_data)
        {
            this->m_vertices.clear();
            this->m_textureCoordinates.clear();
            this->m_normals.clear();
            this->m_indices.clear();
            this->m_indicesTexture.clear();
            this->m_indicesNormals.clear();

            this->m_vertices = m_data.m_vertices;
            this->m_textureCoordinates = m_data.m_textureCoordinates;
            this->m_normals = m_data.m_normals;
            this->m_indices = m_data.m_indices;
            this->m_indicesTexture = m_data.m_indicesTexture;
            this->m_indicesNormals = m_data.m_indicesNormals;

            return *this;
        }
    };*/

    struct QE_BufferMeshData
    {
        std::vector<float> m_vertices;
        std::vector<float> m_texture_coordinates;
        std::vector<float> m_normals;

        std::vector<uint32_t> m_indices;
        std::vector<uint32_t> m_indicesTex;
        std::vector<uint32_t> m_indicesNor;

        QE_BufferMeshData& operator=(const QE_BufferMeshData& b_data)
        {
            this->m_vertices.clear();
            std::move(b_data.m_vertices.begin(), b_data.m_vertices.end(), std::back_inserter(this->m_vertices));

            this->m_texture_coordinates.clear();
            std::move(b_data.m_texture_coordinates.begin(), b_data.m_texture_coordinates.end(), std::back_inserter(this->m_texture_coordinates));

            this->m_normals.clear();
            std::move(b_data.m_normals.begin(), b_data.m_normals.end(), std::back_inserter(this->m_normals));

            return *this;
        }
    };

    struct QE_MaterialData
    {
        std::vector<float> ambient;
        std::vector<float> diffuse;
        std::vector<float> specular;

        std::vector<float> transparent;
        std::vector<float> transmissionFilter;
        std::vector<float> density;
        std::vector<short> illumination;
    };

    struct GLTF_Material {
        bool m_doubleSided;
        std::string m_name;
        Vector<float> m_baseColor;
        float m_metalicFactor;
        float m_roughnessFactor;
    };

    struct GLTF_Mesh {
        std::string m_name;
        uint32_t m_position;
        uint32_t m_normal;
        uint32_t m_tex_coord;
        uint32_t m_indices;

        GLTF_Material m_material;
    };

    struct GLTF_BufferView {
        uint32_t m_buffer;
        uint32_t m_byte_length;
        uint32_t m_byte_offset;
        uint32_t m_target;
    };

    struct GLTF_Buffer {
        uint32_t m_byte_length;
        std::string m_uri;
    };

    struct GLTF_Accessor {
        GLTF_BufferView* m_buffer_view;
        uint32_t m_component_type;
        uint32_t m_count;
        Vector<float> max;
        Vector<float> min;
        std::string type;
    };

    struct GLTF_Node {
        uint32_t m_id;
        std::string m_name;
    };

    struct GLTF_Scene {
        std::string m_name;
        std::vector<GLTF_Node*> m_nodes;
    };

    struct GLTF_Data {
        uint32_t scene;
        std::vector<GLTF_Scene> m_scenes;
        std::vector<GLTF_Material> m_materials;
        std::vector<GLTF_Mesh> m_meshes;
        std::vector<GLTF_Accessor> m_accessors;
        std::vector<GLTF_BufferView> m_buffer_views;
        std::vector<GLTF_Buffer> m_buffers;
    };

    typedef QE_ModelType ModelType;
    typedef QE_BufferMeshData BufferMeshData;
    typedef QE_MaterialData MaterialData;
} // namespace qe


#endif  
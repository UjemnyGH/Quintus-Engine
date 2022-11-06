#pragma once
#ifndef __R_MODEL_LOADER_
#define __R_MODEL_LOADER_

#include "r_renderer_helpers.hpp"
#include "../Core/c_engine_functions.hpp"
#include "r_model_loader_helpers.hpp"

namespace qe
{
    class QE_ModelLoader {
    private:
        std::vector<RenderedData> m_data;
        bool m_model_reloaded = true;
    
    public:
        QE_ModelLoader() {}

        void LoadModel(const std::string path, const ModelType type = ModelType::OBJ) {
            m_model_reloaded = true;

            switch(type) {
            default:
            case ModelType::OBJ:

                break;

            case ModelType::GLTF:

                break;
            }
        }

        RenderedData GetModelData() {
            m_model_reloaded = false;

            return m_data[0];
        }

        ~QE_ModelLoader() {}
    };

    typedef QE_ModelLoader ModelLoader;
} // namespace qe


#endif
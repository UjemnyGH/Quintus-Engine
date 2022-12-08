#pragma once
#ifndef __C_LAYERS_
#define __C_LAYERS

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace qe {
    class QE_Layer {
    protected:
        std::string m_layer_debug_name;

    public:
        uint32_t m_id;
        static uint32_t m_static_count;
        bool m_auto_start = false;
        bool m_explict_fixed_update_implementation = false;

        QE_Layer(std::string name = "Layer") : m_layer_debug_name(name) {}

        /**
         * @brief Call on start of new layer
         * 
         */
        virtual void Start() {}

        /**
         * @brief Called every frame if needed
         * 
         */
        virtual void Update() {}

        /**
         * @brief Called every fixed time
         * 
         */
        virtual void FixedUpdateExplict() {}

        /**
         * @brief Call on end of layer
         * 
         */
        virtual void End() {}

        /**
         * @brief Get the Name object
         * 
         * @return const std::string 
         */
        const std::string getName() { return m_layer_debug_name; }

        /**
         * @brief Get the Id object
         * 
         * @return const uint32_t 
         */
        const uint32_t getId() { return m_id; }

        ~QE_Layer() {}
    };

    class QE_LayerHandler {
    private:
        std::vector<QE_Layer*> m_layers;
        uint32_t m_layers_count = 0;

    public:
        QE_LayerHandler() {}

        /**
         * @brief Adds layer to layer heap
         * 
         * @param layer 
         */
        void AddLayer(QE_Layer* layer) {
            if (layer != nullptr) {
                m_layers.push_back(layer);

                m_layers_count++;

                if(layer->m_auto_start) {
                    layer->Start();

                    if(m_layers.size() > 0) {
                        layer->m_id = m_layers[m_layers.size() - 1]->m_id + 1;
                    }
                    else {
                        layer->m_id = 0;
                    }
                }

            }
        }

        /**
         * @brief Erase layer from layers heap
         * 
         * @param layer 
         */
        void EraseLayer(QE_Layer* layer) {
            auto iter = std::find(m_layers.begin(), m_layers.end(), layer);

            if(layer != nullptr) {
                m_layers.erase(iter);

                m_layers_count--;

                layer->End();
            }
        }

        /**
         * @brief Automaticly updates all layers on heap
         * 
         */
        void AutoUpdate() {
            for (QE_Layer* layer : m_layers) {
                layer->Update();
            }
        }

        /**
         * @brief Automaticly updates fixed update functions if set
         * 
         */
        void AutoFixedUpdate() {
            for(QE_Layer* layer : m_layers) {
                if(layer->m_explict_fixed_update_implementation) {
                    layer->FixedUpdateExplict();
                }
            }
        }

        /**
         * @brief begin of layers heap
         * 
         * @return const std::vector<QE_Layer*>::iterator 
         */
        const std::vector<QE_Layer*>::iterator begin() { return m_layers.begin(); }

        /**
         * @brief end of layers heap
         * 
         * @return const std::vector<QE_Layer*>::iterator 
         */
        const std::vector<QE_Layer*>::iterator end() { return m_layers.end(); }

        /**
         * @brief const begin of layers heap
         * 
         * @return const std::vector<QE_Layer*>::const_iterator 
         */
        const std::vector<QE_Layer*>::const_iterator cbegin() { return m_layers.cbegin(); }

        /**
         * @brief const end of layers heap
         * 
         * @return const std::vector<QE_Layer*>::const_iterator 
         */
        const std::vector<QE_Layer*>::const_iterator cend() { return m_layers.cend(); }

        /**
         * @brief Ends all layers at the end of program
         * 
         */
        ~QE_LayerHandler() {
            for (QE_Layer* layer : m_layers) {
                #ifdef __DEBUG
                    printf("Layer ( Name: %s ID: %d ) got destroyed!", layer->getName().c_str(), layer->getId());
                #endif
                
                layer->End();
            }
        }
    };

    typedef QE_Layer Layer;
    typedef QE_LayerHandler LayerHandler;
}

#endif
#pragma once
#ifndef __UI_COMPONENTS_
#define __UI_COMPONENTS_

#include "../Core/c_algorithms.hpp"
#include "../Core/c_hard_coded_stuff.hpp"
#include "../Renderer/r_renderer.hpp"

namespace qe {
    namespace ui {
        class UIComponent : public Renderer {
        public:
            UIComponent() {
                Renderer::m_render_with_g_projection = false;
                Renderer::m_render_with_g_view = false;
                Renderer::m_layer_debug_name = "UIComponent";
                Renderer::m_auto_start = true;
            }
        };

        class Button : public UIComponent {
        private:
            float *m_mouse_x;
            float *m_mouse_y;

            Vector<float> m_button_params = Vector<float>(0.0f, 0.0f, 0.1f, 0.1f);

        public:
            Button(float *mouse_x, float *mouse_y) : m_mouse_x(mouse_x), m_mouse_y(mouse_y) { }

            void SetButtonPosition(float x, float y) { m_button_params.x = x; m_button_params.y = y; }
            void SetButtonSize(float x, float y) { m_button_params.z = x; m_button_params.w = y; }

            bool CheckOnDrag() {
                return *m_mouse_x < m_button_params.x + m_button_params.z && *m_mouse_x > m_button_params.x - m_button_params.z && *m_mouse_y < m_button_params.y + m_button_params.w && *m_mouse_y > m_button_params.y - m_button_params.w; 
            }
        };
    }   
}


#endif
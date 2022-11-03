#pragma once
#ifndef __W_WINDOW_
#define __W_WINDOW_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Core/c_layers.hpp"
#include "../Core/c_engine_functions.hpp"
#include "../Core/c_time.hpp"
#include "../Renderer/r_renderer.hpp"

namespace qe {
    class QE_Window {
    private:
        LayerHandler m_layer_handler;
        GLFWwindow* m_window;
        bool m_v_sync = false;
        std::string m_title;

        static float m_left;
        static float m_right;
        static float m_top;
        static float m_bottom;

        static void framebuffer_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);

            m_width = width;
            m_height = height;

            if(!m_orthographic) {
                m_projection = glm::perspectiveFov(m_fov, (float)width, (float)height, m_near, m_far);
            }
            else {
                if(width < height && height > 0) {
                    m_projection = glm::ortho(m_left, m_right, m_bottom * height / width, m_top * height / width, m_near, m_far);
                }
                else if(width >= height && height > 0) {
                    m_projection = glm::ortho(m_left * width / height, m_right * width / height, m_bottom, m_top, m_near, m_far);
                }
                else {
                    m_projection = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
                }
            }
        }

    protected:
        Time time;

    public:
        static float m_fov;
        static float m_far;
        static float m_near;
        static bool m_orthographic;
        static uint32_t m_width;
        static uint32_t m_height;

        /**
         * @brief Construct a new qe window object
         * 
         * @param title 
         * @param width 
         * @param height 
         */
        QE_Window(std::string title = "Window", uint32_t width = 800, uint32_t height = 600) : m_title(title) { 
            QE_Window::m_left = -2.0f;
            QE_Window::m_right = 2.0f;
            QE_Window::m_top = 2.0f;
            QE_Window::m_bottom = -2.0f;
            QE_Window::m_fov = 70.0f;
            QE_Window::m_far = 10000.0f;
            QE_Window::m_near = 0.001f;
            QE_Window::m_orthographic = false;
            QE_Window::m_width = width;
            QE_Window::m_height = height;
        }

        void UpdatePerspective() {
            if(!m_orthographic) {
                m_projection = glm::perspectiveFov(m_fov, (float)m_width, (float)m_height, m_near, m_far);
            }
            else {
                if(m_width < m_height && m_height > 0) {
                    m_projection = glm::ortho(m_left, m_right, m_bottom * m_height / m_width, m_top * m_height / m_width, m_near, m_far);
                }
                else if(m_width >= m_height && m_height > 0) {
                    m_projection = glm::ortho(m_left * m_width / m_height, m_right * m_width / m_height, m_bottom, m_top, m_near, m_far);
                }
                else {
                    m_projection = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
                }
            }
        }

        /**
         * @brief Runs window
         * 
         */
        void run() {
            Awake();

            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

            if(!m_window) {
                qe_term("Window couldn`t be shown!");
            }

            glfwMakeContextCurrent(m_window);

            glewExperimental = true;

            if(glewInit() != GLEW_OK) {
                qe_term("GLEW cannot be initialized!");
            }

            if(!GLEW_VERSION_4_3) {
                qe_term("Your GLEW version doesn`t support that OpenGL version!");
            }

            glEnable(GL_DEPTH_TEST);

            AddLayer(&time);

            glfwSetFramebufferSizeCallback(m_window, framebuffer_callback);

            Start();

            while(!glfwWindowShouldClose(m_window)) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_layer_handler.AutoUpdate();

                Update();

                glfwPollEvents();

                glfwSwapBuffers(m_window);

                glfwSwapInterval((int)m_v_sync);

                LateUpdate();
            }

            glfwTerminate();
        }

        virtual void Framebuffer() {}

        /**
         * @brief Called before main game loop
         * 
         */
        virtual void Start() {}

        /**
         * @brief Called every frame before swapping buffers
         * 
         */
        virtual void Update() {}

        /**
         * @brief Called every frame after swapping buffers(mainly used to physics)
         * 
         */
        virtual void LateUpdate() {}

        /**
         * @brief Called on very beginning of game, before creating window
         * 
         */
        virtual void Awake() {}

        /**
         * @brief Adds layer to layer heap
         * 
         * @param layer 
         */
        void AddLayer(Layer* layer) { m_layer_handler.AddLayer(layer); }

        /**
         * @brief Erase layer from layers heap
         * 
         * @param layer 
         */
        void EraseLayer(Layer* layer) { m_layer_handler.EraseLayer(layer); }

        /**
         * @brief Get the Window Ptr
         * 
         * @return GLFWwindow* 
         */
        GLFWwindow* getWindowPtr() { return m_window; }

        /**
         * @brief Get the Layer Handler Ptr
         * 
         * @return LayerHandler* 
         */
        LayerHandler* getLayerHandlerPtr() { return &m_layer_handler; }

        /**
         * @brief Set the Vertical Sync
         * 
         * @param vsync 
         */
        void setVSync(bool vsync) { m_v_sync = vsync; }
    };

    typedef QE_Window Window;
}

#endif
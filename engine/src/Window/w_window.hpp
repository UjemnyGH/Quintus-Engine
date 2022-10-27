#pragma once
#ifndef __W_WINDOW_
#define __W_WINDOW_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Core/c_layers.hpp"
#include "../Core/c_engine_functions.hpp"

namespace qe {
    class QE_Window {
    private:
        LayerHandler m_layer_handler;
        GLFWwindow* m_window;
        bool m_v_sync = false;

    public:
        /**
         * @brief Construct a new qe window object
         * 
         * @param title 
         * @param width 
         * @param height 
         */
        QE_Window(std::string title = "Window", uint32_t width = 800, uint32_t height = 600) {
            Awake();

            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

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
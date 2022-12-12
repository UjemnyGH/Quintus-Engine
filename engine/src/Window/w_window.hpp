#pragma once
#ifndef __W_WINDOW_
#define __W_WINDOW_

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_vulkan.h"
#include "../../vendor/glad/glad.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include "../Core/c_layers.hpp"
#include "../Core/c_engine_functions.hpp"
#include "../Core/c_time.hpp"
#include "../Renderer/r_renderer.hpp"
#include "../Scripting/s_core.hpp"
#include <thread>
#include <chrono>

namespace qe {
    enum Keys
	{
		_0 = GLFW_KEY_0,
		_1 = GLFW_KEY_1,
		_2 = GLFW_KEY_2,
		_3 = GLFW_KEY_3,
		_4 = GLFW_KEY_4,
		_5 = GLFW_KEY_5,
		_6 = GLFW_KEY_6,
		_7 = GLFW_KEY_7,
		_8 = GLFW_KEY_8,
		_9 = GLFW_KEY_9,
		NUM_0 = GLFW_KEY_KP_0,
		NUM_1 = GLFW_KEY_KP_1,
		NUM_2 = GLFW_KEY_KP_2,
		NUM_3 = GLFW_KEY_KP_3,
		NUM_4 = GLFW_KEY_KP_4,
		NUM_5 = GLFW_KEY_KP_5,
		NUM_6 = GLFW_KEY_KP_6,
		NUM_7 = GLFW_KEY_KP_7,
		NUM_8 = GLFW_KEY_KP_8,
		NUM_9 = GLFW_KEY_KP_9,
		NUM_DEC = GLFW_KEY_KP_DECIMAL,
		NUM_DIV = GLFW_KEY_KP_DIVIDE,
		NUM_MUL = GLFW_KEY_KP_MULTIPLY,
		NUM_SUB = GLFW_KEY_KP_SUBTRACT,
		NUM_ADD = GLFW_KEY_KP_ADD,
		NUM_ENTER = GLFW_KEY_KP_ENTER,
		NUM_EQUAL = GLFW_KEY_KP_EQUAL,
		L_SHIFT = GLFW_KEY_LEFT_SHIFT,
		L_ALT = GLFW_KEY_LEFT_ALT,
		L_CTRL = GLFW_KEY_LEFT_CONTROL,
		L_SUPER = GLFW_KEY_LEFT_SUPER,
		R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		R_ALT = GLFW_KEY_RIGHT_ALT,
		R_CTRL = GLFW_KEY_RIGHT_CONTROL,
		R_SUPER = GLFW_KEY_RIGHT_SUPER,
		MENU = GLFW_KEY_MENU,
		SPACE = GLFW_KEY_SPACE,
		APOSTRO = GLFW_KEY_APOSTROPHE,
		COMMA = GLFW_KEY_COMMA,
		MINUS = GLFW_KEY_MINUS,
		DOT = GLFW_KEY_PERIOD,
		SLASH = GLFW_KEY_SLASH,
		SEMICOLON = GLFW_KEY_SEMICOLON,
		EQUAL = GLFW_KEY_EQUAL,
		L_BRACKET = GLFW_KEY_LEFT_BRACKET,
		R_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		BACKSLASH = GLFW_KEY_BACKSLASH,
		GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
		ESC = GLFW_KEY_ESCAPE,
		ENTER = GLFW_KEY_ENTER,
		TAB = GLFW_KEY_TAB,
		BACKSPACE = GLFW_KEY_BACKSPACE,
		INSERT = GLFW_KEY_INSERT,
		DEL = GLFW_KEY_DELETE,
		RIGHT = GLFW_KEY_RIGHT,
		LEFT = GLFW_KEY_LEFT,
		UP = GLFW_KEY_UP,
		DOWN = GLFW_KEY_DOWN,
		PG_UP = GLFW_KEY_PAGE_UP,
		PG_DOWN = GLFW_KEY_PAGE_DOWN,
		HOME = GLFW_KEY_HOME,
		END = GLFW_KEY_END,
		CAPSLOCK = GLFW_KEY_CAPS_LOCK,
		SCROLLLOCK = GLFW_KEY_SCROLL_LOCK,
		NUMLOCK = GLFW_KEY_NUM_LOCK,
		PRINTSCREEN = GLFW_KEY_PRINT_SCREEN,
		PAUSE = GLFW_KEY_PAUSE,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		W = GLFW_KEY_W,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		Q = GLFW_KEY_Q,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,
		UNKNOWN = GLFW_KEY_UNKNOWN
	};

	enum KeyState
	{
		PRESS = GLFW_PRESS,
		RELEASE = GLFW_RELEASE,
		REPEAT = GLFW_REPEAT
	};

    enum CursorState
	{
		LOCKED = GLFW_CURSOR_DISABLED,
		HIDDEN = GLFW_CURSOR_HIDDEN,
		NORMAL = GLFW_CURSOR_NORMAL
	};

    class QE_Window {
    private:
        LayerHandler m_layer_handler;
        GLFWwindow* m_window;
        bool m_v_sync = false;
        std::string m_title;

        bool m_window_closed = false;

        static float m_left;
        static float m_right;
        static float m_top;
        static float m_bottom;

        static void framebuffer_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);

            m_width = width;
            m_height = height;

            if(!m_orthographic) {
                g_projection = glm::perspectiveFov(m_fov, (float)width, (float)height, m_near, m_far);
            }
            else {
                if(width < height && height > 0) {
                    g_projection = glm::ortho(m_left, m_right, m_bottom * height / width, m_top * height / width, m_near, m_far);
                }
                else if(width >= height && height > 0) {
                    g_projection = glm::ortho(m_left * width / height, m_right * width / height, m_bottom, m_top, m_near, m_far);
                }
                else {
                    g_projection = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
                }
            }
        }

        void fixedUpdateCallback() {
            while(!m_window_closed) {
                if(m_window_closed) {
                    break;
                }

                FixedUpdate();

                m_layer_handler.AutoFixedUpdate();

                std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / m_fixed_update_per_second));
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

        uint64_t m_fixed_update_per_second = 128;

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
                g_projection = glm::perspectiveFov(m_fov, (float)m_width, (float)m_height, m_near, m_far);
            }
            else {
                if(m_width < m_height && m_height > 0) {
                    g_projection = glm::ortho(m_left, m_right, m_bottom * m_height / m_width, m_top * m_height / m_width, m_near, m_far);
                }
                else if(m_width >= m_height && m_height > 0) {
                    g_projection = glm::ortho(m_left * m_width / m_height, m_right * m_width / m_height, m_bottom, m_top, m_near, m_far);
                }
                else {
                    g_projection = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
                }
            }
        }

        /**
         * @brief Runs window
         * 
         */
        void run() {
            __initialize_lua();

            Awake();

            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

            if(!m_window) {
                qe_term("Window couldn`t be shown!");
            }

            glfwMakeContextCurrent(m_window);

            if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                qe_term("No GLAD!");
            }

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            AddLayer(&time);

            glfwSetFramebufferSizeCallback(m_window, framebuffer_callback);

            Start();

            UpdatePerspective();

            std::jthread fixed_update_thread(&QE_Window::fixedUpdateCallback, this);

            fixed_update_thread.detach();

            while(!glfwWindowShouldClose(m_window)) {
                m_window_closed = glfwWindowShouldClose(m_window);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_layer_handler.AutoUpdate();

                Update();

                glfwPollEvents();

                glfwSwapBuffers(m_window);

                glfwSwapInterval((int)m_v_sync);

                LateUpdate();
            }

            fixed_update_thread.request_stop();

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
         * @brief Called in thread. Update m_fixed_update_per_second times per second (by default 128 times per second)
         * 
         */
        virtual void FixedUpdate() {}

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

        /**
         * @brief Get the Key Press
         * 
         * @param key 
         * @param key_state 
         * @return true 
         * @return false 
         */
        bool getKeyPress(Keys key, KeyState key_state) { return glfwGetKey(m_window, key) == key_state; }

        /**
         * @brief Set the Cursor State
         * 
         * @param cursor_state 
         */
        void setCursorState(CursorState cursor_state) { glfwSetInputMode(m_window, GLFW_CURSOR, cursor_state); }

        /**
         * @brief Set the Fullscreen
         * 
         * @param fullscreen 
         */
        void setFullscreen(bool fullscreen) { glfwSetWindowMonitor(m_window, fullscreen == true ? glfwGetPrimaryMonitor() : nullptr, 0, 0, m_width, m_height, GLFW_DONT_CARE); } 

        /**
         * @brief Get the Fixed Update Time Interval In Seconds
         * 
         * @return float 
         */
        float getFixedUpdateTimeIntervalInSec() { return 1.0f / (float)m_fixed_update_per_second; }
    };

    typedef QE_Window Window;
}

#endif
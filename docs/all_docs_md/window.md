# qe::Window class documentation

## qe::Window virtual functions
------------

### qe::Window::Start()

    Description:
        qe::Window::Start() runs after initializing window and just before main program loop. It is defined in qe::Window class as `virtual void Start() {}`, so you need to override it to use it.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::Awake()

    Description:
        qe::Window::Awake() runs on very beginning, before creating window. It is defined in qe::Window class as `virtual void Awake() {}`, so you need to override it to use it.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Awake() override;
        };

        void Game::Awake() {
            // Code here
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::Update()

    Description:
        qe::Window::Update() runs every frame, before rendering happens. It is also defined in qe::Window class as `virtual void Update() {}`, so you also need to override it to actually use it.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Update() override;
        };

        void Game::Update() {
            // Code here
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::FixedUpdate()

    Description:
        qe::Window::FixedUpdate() runs every qe::Window::m_fixed_update_per_second times per second, on separated thread. It is also defined in qe::Window class as `virtual void FixedUpdate() {}`, so you also need to override it to actually use it.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void FixedUpdate() override;
        };

        void Game::FixedUpdate() {
            // Code here
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::LateUpdate()

    Description:
        qe::Window::LateUpdate() runs every frame, after rendering happens. It is also defined in qe::Window class as `virtual void LateUpdate() {}`, so you also need to override it to actually use it.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void LateUpdate() override;
        };

        void Game::LateUpdate() {
            // Code here
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

## qe::Window functions
----------------

### qe::Window.run()

    Description:
        qe::Window.run() runs program with main loop, but you need to create inheritant class out of it to actualy use functions defined there.

    Use example:

       ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            
        };

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::setVSync(bool vsync)

    Description:
        qe::Window::setVSync(bool vsync) sets vertical synchronization, that by default is turned off.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            // To turn on vertical synchronization
            Game::setVSync(true);

            // To turn off vertical synchronization
            Game::setVSync(false);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::UpdatePerspective()
    
    Description:
        qe::Window::UpdatePerspective() update projection matrix to show correct data on screen if 1 of 2 default projection(orthographic or perspective) is used. By default this functions is called after qe::Window::Start() function

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            Game::UpdatePerspective();
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::getWindowPtr()
    
    Description:
        qe::Window::getWindowPtr() returns GLFWwindow pointer. That is it.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            GLFWwindow *windowPtr = Game::getWindowPtr();
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::getLayerHandlerPtr()
    
    Description:
        qe::Window::getLayerHandlerPtr() returns qe::layerHandler pointer. I do not know for what you can use it, but still I wrote that function for some point.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            qe::LayerHandler *windowLayerHandler = Game::getLayerHandlerPtr();
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::AddLayer(Layer *layer)
    
    Description:
        qe::Window::AddLayer([Layer](layers.md) *layer) adds layer to window layer handler. Some class must be added to layering system, like [qe::Renderer](renderer.md) to actually render stuff.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        qe::Renderer renderer1;

        void Game::Start() {
            // Code here

            Game::AddLayer(&renderer1);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::EraseLayer()
    
    Description:
        qe::Window::EraseLayer([Layer](layers.md) *layer) erase layer from window layer handler by finding actuall layer. 

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        qe::Renderer renderer1;

        void Game::Start() {
            // Code here

            Game::AddLayer(&renderer1);
            Game::EraseLayer(&renderer1);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::getKeyPress(Keys key, KeyState key_state)

    Description:
        qe::Window::getKeyPress(Keys key, KeyState key_state) return true or false depending on if key have key_state.

    Use example:
        
        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            bool a_pressed = Game::getKeyPress(qe::A, qe::PRESS);
            bool a_released = Game::getKeyPress(qe::A, qe::RELEASE);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::setCursorState(CursorState cursor_state)

    Description:
        qe::Window::setCursorState(CursorState cursor_state) sets visibility of mouse cursor.

    Use example:
        
        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            Game::setCursorState(qe::NORMAL);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::setFullscreen(bool fullscreen)
    Description:
        qe::Window::setFullscreen(bool fullscreen) sets fullscreen on primary monitor.

    Use example:
        
        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            Game::setFullscreen(true);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

## qe::Window variables to set
---------------

### qe::Window::m_fixed_update_per_second

    Description:
        qe::Window::m_fixed_update_per_second sets how many times per second qe::Window::FixedUpdate() updates. Max value is 1000000000.

### qe::Window::m_fov
    
    Description:
        static float qe::Window::m_fov contain field of view used in projection matrix. Default value is set to 70.0f

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            Game::m_fov = 90.0f;
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::m_near
    
    Description:
        static float qe::Window::m_near contain information about how near camera rendered plane could be than it starts to disappear. It is used in projection matrix. Default value is set to 0.001f

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            Game::m_near = 0.002f;
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::m_far
    
    Description:
        static float qe::Window::m_far contain information about how far from camera rendered plane could be than it starts to disappear. It is used in projection matrix. Default value is set to 10000.0f

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            Game::m_far = 1000.0f;
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::m_orthographic
    
    Description:
        static bool qe::Window::m_orthographic contain information about type of camera projection. It is used to set used method of projection. Default value is set to false, so, by default you are using perspective with fov.

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            // to use orthohraphic projection
            Game::m_orthographic = true;
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

### qe::Window::m_width and qe::Window::m_height
    
    Description:
        static uint32_t qe::Window::m_width and static uint32_t qe::Window::m_height contain information about window width and height. Default values are m_width = 800, m_height = 600. You should not change these values, they are changing when window has been resized!

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Start() override;
        };

        void Game::Start() {
            // Code here

            // You can print them, or use to inform program user about window size
            printf("Window width: %d, height: %d", Game::m_width, Game::m_height);
        }

        void Application() {
            Game game;

            game.run();
        }
       ```

## qe::Window::time class
----------------

Reference to [time class](time.md)
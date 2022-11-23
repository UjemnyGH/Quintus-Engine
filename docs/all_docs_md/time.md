# qe::Time class documentation

qe::Time class is basicly imbedded in [qe::Window](window.md) class, and it is [layered](layers.md), so it is added by default to LayerHandler

## qe::Time functions
-----------------

### qe::Time.GetTime()

    Description:
        qe::Time.GetTime() returns elapsed time in seconds that program runs

    Use example:

        ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Update() override;
        };

        void Game::Update() {
            // Code here

            // GetTime() returns elapsed time
            float time = Game::time.GetTime();
        }

        void Application() {
            Game game;

            game.run();
        }
       ```


### qe::Time.GetDeltaTime()

    Description:
        qe::Time.GetTime() return delta time (time between current and last frame) in seconds

    Use example:

    ```c++
        // Needed to get qe::Window class
        #include "../../../engine/quintus.hpp"

        class Game : public qe::Window {
            virtual void Update() override;
        };

        void Game::Update() {
            // Code here

            // GetDeltaTime() returns elapsed time
            float delta_time = Game::time.GetDeltaTime();
        }

        void Application() {
            Game game;

            game.run();
        }
       ```
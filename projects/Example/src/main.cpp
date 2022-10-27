#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

void Game::Start() {
    // Code here
}

void Game::Update() {
    // Code here
    std::cout << qe::Time::GetDeltaTime() << '\n';
}

void Application() {
    Game game;

    game.run();
}

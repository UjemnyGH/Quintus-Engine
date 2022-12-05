#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

qe::Scene scene1("MainScene");

void Game::Start() {
    // Code here

    scene1.addSceneData(qe::Scene::GeneratePartSceneData(1.0f, "PlayerX"));
    scene1.addSceneData(qe::Scene::GeneratePartSceneData(0.5f, "PlayerY"));
    scene1.addSceneData(qe::Scene::GeneratePartSceneData(0.7434f, "PlayerZ"));

    scene1.GenerateSceneFile();

    qe::qe_req_term("SCENE TEST");
}

void Game::Update() {
    // Code here
}

void Application() {
    Game game;

    game.run();
}

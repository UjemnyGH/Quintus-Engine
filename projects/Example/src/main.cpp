#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

qe::QE_Renderer test1;

void Game::Start() {
    // Code here
    Game::m_orthographic = false;
    Game::AddLayer(&test1);
    test1.AddModel(qe::cube);
    test1.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    test1.AddShader(qe::LoadShader(qe::fragment_shader, qe::ShaderType::fragment));
}

void Game::Update() {
    // Code here

    test1.SetScale(0.1f, 0.1f, 0.1f);
    test1.SetRotation(Game::time.GetTime() * 10.0f, Game::time.GetTime() * 100.0f, 0.0f);
}

void Application() {
    Game game;

    game.run();
}

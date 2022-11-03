#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

qe::Renderer test1;

void Game::Start() {
    // Code here
    Game::m_orthographic = false;
    Game::UpdatePerspective();
    Game::AddLayer(&test1);
    test1.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    test1.AddShader(qe::LoadShader(qe::fragment_shader, qe::ShaderType::fragment));

    for(int y = 0; y < 20; y++) {
        for(int x = 0; x < 20; x++) {
            for(int z = 0; z < 20; z++) {
                test1.AddModel(qe::cube);
                test1.SetPositionByID((y * 400) + (x * 20) + z, (x * 3.0f) - 0.5f, (y * 3.0f) - 0.5f, (z * 3.0f) - 0.5f);
            }
        }
    }

    test1.ForceRejoin();
}

void Game::Update() {
    // Code here

    qe::m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    test1.SetScale(0.1f, 0.1f, 0.1f);
    test1.SetRotation(Game::time.GetTime() * 10.0f, Game::time.GetTime() * 10.0f, Game::time.GetTime() * 10.0f);
}

void Application() {
    Game game;

    game.run();
}

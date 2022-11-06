#include "../../../engine/quintus.hpp"
#include <random>

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

qe::Renderer test1;

void Game::Start() {
    // Code here
    //std::cout << "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" << '\n';
    std::cout << qe::DecodeBase64AsChars("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") << '\n';

    Game::m_orthographic = false;
    Game::UpdatePerspective();
    Game::AddLayer(&test1);
    test1.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    test1.AddShader(qe::LoadShader(qe::fragment_shader, qe::ShaderType::fragment));

    for(int y = 0; y < 10; y++) {
        for(int x = 0; x < 10; x++) {
            for(int z = 0; z < 10; z++) {
                test1.AddModel(qe::cube);
                test1.SetPositionByID((y * 100) + (x * 10) + z, (x * 3.0f) - 0.5f, (y * 3.0f) - 0.5f, (z * 3.0f) - 0.5f);
                test1.SetColorByID((y * 100) + (x * 10) + z, (x * 3.0f) / 20.0f, (y * 3.0f) / 20.0f, (z * 3.0f) / 20.0f, 1.0f);
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

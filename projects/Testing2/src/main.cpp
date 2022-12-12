#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

qe::Renderer rend;

void Game::Start() {
    // Code here

    Game::AddLayer(&rend);
    rend.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    rend.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));

    rend.AddModel(qe::square, "Tree");
    rend.AddModel(qe::square, "Rock");

    rend.AddTexture("data/tree1.png");  // ID: 1 
    rend.AddTexture("data/rock1.png");  // ID: 2

    rend.SetModelTexture(rend.GetIdFromName("Tree"), 1);
    rend.SetModelTexture(rend.GetIdFromName("Rock"), 2);

    rend.SetPositionByID(rend.GetIdFromName("Tree"), -0.5f, 0.0f, 0.0f);
    rend.SetScaleByID(rend.GetIdFromName("Tree"), 0.2f, 0.2f, 1.0f);

    rend.SetPositionByID(rend.GetIdFromName("Rock"), 0.5f, 0.0f, 0.0f);
    rend.SetScaleByID(rend.GetIdFromName("Rock"), 0.2f, 0.2f, 1.0f);

    rend.__debug_texture_indices();
}

void Game::Update() {
    // Code here

    qe::g_view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Application() {
    Game game;

    game.run();
}

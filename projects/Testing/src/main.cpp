#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

/*qe::Scene scene1("MainScene");
qe::Scene scene2;*/

qe::Script player("player", "Player");

void Game::Start() {
    // Code here

    //Game::AddLayer(&player);

    /*scene1.AddStructData("Player");
    float playerP[] = {1.0f, 1.0f, 1.0f};
    scene1.AddPartSceneData(playerP, sizeof(playerP) / sizeof(playerP[0]), "PlayerPosition", qe::v_float);
    scene1.AddPartSceneData(playerP, sizeof(playerP) / sizeof(playerP[0]), "PlayerDirection", qe::v_float);

    scene1.GenerateSceneFile();

    scene2.ReadScene("MainScene");

    std::cout << scene2.getSceneName() << std::endl;

    for(auto n : *scene2.getSceneValues()) {
        std::cout << "Struct name : " << n.m_struct_name  << " : " << std::endl;

        for(auto m : n.m_scene_struct) {
            std::cout << "Variable name : " << m.m_var_name << " Values : " << std::endl;

            for(uint32_t i = 0; i < m.m_scene_values_size; i++) {
                float val_ptr = m.getElement(i);
                std::cout << "\t" << val_ptr << std::endl;
            }
        }  
    }

    qe::qe_req_term("SCENE TEST");*/
}

void Game::Update() {
    // Code here
}

void Application() {
    Game game;

    game.run();
}

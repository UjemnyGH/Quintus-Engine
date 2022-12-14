#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

/*qe::Scene scene1("MainScene");
qe::Scene scene2;*/

qe::Scene scene1;
qe::Scene scene2;

qe::ScriptLua player;
qe::ScriptLua camera;

void Game::Start() {
    // Code here

    player.load_script("player", "Player");
    camera.load_script("camera", "Camera");

    //Game::AddLayer(&player);
    //Game::AddLayer(&camera);

    scene1.BeginScene("MainScene");

    scene1.AddStruct("Player");
    float playerPos[3] = {1.0f, 1.0f, 1.0f};
    float playerPos2[3] = {-1.0f, -1.0f, -1.0f};
    scene1.AddValue("playerPos", playerPos, 3);
    scene1.AddValue("playerDir", playerPos2, 3);
    scene1.GenerateScene();

    scene2.ReadScene("MainScene");

    qe::SceneInfo *info = scene2.GetScenePtr();

    printf("Scene name: %s\n", info->m_scene_name.c_str());

    for(auto i : info->m_scene_structs) {
        printf("Struct: %s\n", i.m_struct_name.c_str());
        for(auto s : i.m_struct_variables) {
            printf("Var name: %s\n", s.first);

            for(auto v : s.second.m_values) {
                printf("Var %s values: %f\n", s.first, v);
            }
        }
    }

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
    qe::qe_req_term("SCENE TEST");
}

void Game::Update() {
    // Code here
}

void Application() {
    Game game;

    game.run();
}

/**
 * @file main.cpp
 * @author Piotr UjemnyGH Plombon
 * @brief 
 * @version 0.1
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 * It need some changes to work!!!!
 */

#include "../../../engine/quintus.hpp"
#include <random>
#include <future>
#include <thread>

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void FixedUpdate() override;
    void input();
};

qe::Renderer skybox;
qe::Renderer bullets;
qe::Renderer gun;
qe::Renderer terrain;

qe::math::Vector<float> playerPos = qe::math::Vector<float>(0.0f);
qe::math::Vector<float> playerDir = qe::math::Vector<float>(0.0f, 0.0f, 1.0f);
qe::math::Vector<float> playerVelocity = qe::math::Vector<float>(0.0f);
float playerVelocityGain = 0.05f;
float playerVelocityMax = 0.5f;
int lastX = 400;
int lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
bool move_press = false;
bool strafe_press = false;
bool camera = false;
const float playerSpeed = 10.0f;

void mouse(GLFWwindow* window, double xpos, double ypos) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if(!camera) {
            glfwSetCursorPos(window, (double)lastX, (double)lastY);

            camera = true;
        }

        float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.01f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = qe::math::clamp(pitch, qe::math::to_radians(-89.99f), qe::math::to_radians(89.99f));

        playerDir.x = cos(pitch) * cos(yaw);
        playerDir.y = sin(pitch);
        playerDir.z = sin(yaw) * cos(pitch);

        playerDir = playerDir.normalize();
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        camera = false;
    }
}

void Game::input() {
    move_press = false;
    strafe_press = false;

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_W) == GLFW_PRESS) {
        if(playerVelocity.z < 0.0f) {
            playerVelocity.z = 0.0f;
        }

        playerVelocity.z += abs(playerVelocity.z) < playerVelocityMax ? playerVelocityGain : 0.0f;
        move_press = true;
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_S) == GLFW_PRESS) {
        if(playerVelocity.z > 0.0f) {
            playerVelocity.z = 0.0f;
        }

        playerVelocity.z -= abs(playerVelocity.z) < playerVelocityMax ? playerVelocityGain : 0.0f;
        move_press = true;
    }

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_D) == GLFW_PRESS) {
        if(playerVelocity.x < 0.0f) {
            playerVelocity.x = 0.0f;
        }

        playerVelocity.x += abs(playerVelocity.x) < playerVelocityMax ? playerVelocityGain : 0.0f;
        strafe_press = true;
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_A) == GLFW_PRESS) {
        if(playerVelocity.x > 0.0f) {
            playerVelocity.x = 0.0f;
        }

        playerVelocity.x -= abs(playerVelocity.x) < playerVelocityMax ? playerVelocityGain : 0.0f;
        strafe_press = true;
    }

    playerPos.z += playerDir.z * playerVelocity.z * Game::time.GetDeltaTime() * playerSpeed;
    playerPos.x += playerDir.x * playerVelocity.z * Game::time.GetDeltaTime() * playerSpeed;

    playerPos += playerDir.normalize().cross(qe::math::Vector<float>(0.0f, 1.0f, 0.0f)).normalize() * (playerVelocity.x * Game::time.GetDeltaTime() * playerSpeed);
    if(!move_press) {
        playerVelocity.z -= playerVelocity.z > 0.0f ? playerVelocityGain : 0.0f;
        playerVelocity.z += playerVelocity.z < 0.0f ? playerVelocityGain : 0.0f;

        if(playerVelocity.z < 0.1f && playerVelocity.z > -0.1f) {
            playerVelocity.z = 0.0f;
        }
    }

    if(!strafe_press) {
        playerVelocity.x -= playerVelocity.x > 0.0f ? playerVelocityGain : 0.0f;
        playerVelocity.x += playerVelocity.x < 0.0f ? playerVelocityGain : 0.0f;

        if(playerVelocity.x < 0.1f && playerVelocity.x > -0.1f) {
            playerVelocity.x = 0.0f;
        }
    }
}

qe::ModelLoader ml;
qe::RenderedData bullet;
qe::RenderedData terrain_ml;

std::vector<qe::Particle> particles;

qe::Particle player_prt;

qe::ParticleForceRegistry player_reg;

qe::ParticleGravity player_grav = qe::ParticleGravity(qe::Vector<float>(0.0f, -1.0f, 0.0f));

std::vector<qe::Collider> terrainCl;

void Game::Start() {
    // Code here
    glfwSetCursorPosCallback(Game::getWindowPtr(), mouse);

    player_reg.Add(&player_prt, &player_grav);

    qe::g_globalLight = glm::vec3(0.0f, 1.0f, 0.0f);

    ml.LoadModelUnindexed("data/models/terrain_test.obj");

    Game::setVSync(true);
    Game::UpdatePerspective();
    Game::m_fixed_update_per_second = 64;

    Game::AddLayer(&terrain);
    terrain.AddShader(LoadShader(qe::vertex_shader, qe::vertex));
    terrain.AddShader(LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    terrain.AddModel(ml.GetModelData(), "Terrain");

    terrain_ml = ml.GetModelData();

    for(uint32_t i = 0; i < terrain_ml.m_vertices.size() / 9; i++) {
        terrainCl.resize(i);

        terrainCl[terrainCl.size() - 1].m_position = qe::Vector<float>(terrain_ml.m_vertices[i * 9 + 0], terrain_ml.m_vertices[i * 9 + 1], terrain_ml.m_vertices[i * 9 + 2]);
        terrainCl[terrainCl.size() - 1].m_size = qe::Vector<float>(terrain_ml.m_vertices[i * 9 + 3], terrain_ml.m_vertices[i * 9 + 4], terrain_ml.m_vertices[i * 9 + 5]);
        terrainCl[terrainCl.size() - 1].m_third_point = qe::Vector<float>(terrain_ml.m_vertices[i * 9 + 6], terrain_ml.m_vertices[i * 9 + 7], terrain_ml.m_vertices[i * 9 + 8]);
    }

    ml.LoadModelUnindexed("data/models/test_cube.obj");

    Game::AddLayer(&gun);
    gun.m_render_with_g_projection = true;
    gun.m_render_with_g_view = false;
    gun.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    gun.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));

    ml.LoadModelUnindexed("data/models/wre-53.obj");

    gun.AddModel(ml.GetModelData(), "Wre-53");
    gun.AddTexture("data/textures/wre-53.png");

    gun.SetPosition(0.5f, -0.5f, -1.8f);
    //gun.SetRotation(qe::math::to_radians(10.0f), qe::math::to_radians(10.0f), qe::math::to_radians(0.0f));

    Game::AddLayer(&bullets);
    bullets.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    bullets.AddShader(qe::LoadShader(qe::color_fragment_shader, qe::fragment));

    ml.LoadModelUnindexed("data/models/test_cube.obj");

    bullet = ml.GetModelData();

    bullets.AddModel(bullet);
    bullets.SetScaleByID(0, 0.0f, 0.0f, 0.0f);
    
    ml.LoadModelUnindexed("data/models/test_cube.obj", qe::ModelType::OBJ);

    Game::AddLayer(&skybox);
    skybox.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    skybox.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::ShaderType::fragment));
    for(int i = 0; i < ml.GetModelVectorData().size(); i++) {
        skybox.AddModel(ml.GetModelVectorData()[i]);
    }
    skybox.AddTexture("data/textures/skybox1.png");
    skybox.SetModelTexture(0, 0);
    skybox.SetScaleByID(0, 1000.0f, 1000.0f, 1000.0f);

    particles.resize(1);

    /*qe::Json json;

    json.LoadJSON("data/models/cube.gltf");

    for(auto j : json.getMap()) {
        std::cout << "Name: " << j.first << " = Value: " << j.second << std::endl;
    }

    std::string node_value = json.findValue("nodes");

    std::cout << "Value of: \"nodes\" = " << node_value << std::endl;

    uint32_t scene_value = json.convertTo<uint32_t>("scene");

    std::cout << "Value of: \"scene\" converted to uint32_t = " << scene_value << std::endl;

    qe::Json group1 = json.findGroup("materials");

    for(auto j : group1.getMap()) {
        std::cout << "Name: " << j.first << " = Value: " << j.second << std::endl;
    }*/
}

bool binding = false;

void SetPos(uint32_t i , qe::Particle particle) {
    bullets.SetPositionByID(i, particle.m_position.x, particle.m_position.y, particle.m_position.z, false);
}

qe::ParticleForceRegistry pfrg;
qe::ParticleGravity pg = qe::ParticleGravity(qe::Vector<float>(0.0f, -1.0f, 0.0f));
qe::Collider player;
qe::Collider player2;

void Game::FixedUpdate() {

    float delay = Game::getFixedUpdateTimeIntervalInSec();

    pfrg.UpdateForces(delay);

    player.m_size = qe::Vector<float>(0.0f, -1.0f, 0.0f);
    player2.m_size = qe::Vector<float>(0.0f, 10.0f, 0.0f);

    player_reg.UpdateForces(delay);

    /*for(auto terr : terrainCl) {
        if(player2.CheckRayToPlane(&terr) || player.CheckRayToPlane(&terr)) {
            player_prt.m_acceleration = qe::Vector<float>(0.0f, 0.1f, 0.0f);
        }
    }*/

    playerPos = player_prt.m_position;

    /*if(player.CheckRayToPlane(&terrainCl, 0.1f) || player2.CheckRayToPlane(&terrainCl, 0.1f)) {
        playerPos.y += 10.0f * delay;
    }
    else {
        playerPos.y -= 10.0f * delay;
    }*/

    //printf("%d\n", player.CheckRayToPlane(&terrainCl, 0.1f));
    
    binding = true;
}

void Game::Update() {
    // Code here
    qe::g_view = glm::lookAt(glm::vec3(playerPos.x, playerPos.y, playerPos.z), glm::vec3(playerPos.x, playerPos.y, playerPos.z) + glm::vec3(playerDir.x, playerDir.y, playerDir.z), glm::vec3(0.0f, 1.0f, 0.0f));

    for(int i = 0; i < bullets.GetModelAmount(); i++) {
        std::future<void> set_pos_async = std::async(SetPos, i, particles[i]);
    }

    if(binding) {
        bullets.BindToBuffers();
        binding = false;
    }

    skybox.SetPosition(playerPos.x, playerPos.y, playerPos.z);
    player.m_position = playerPos;
    player2.m_position = playerPos;
}

bool clicked = false;

void Game::LateUpdate() {

    if(glfwGetMouseButton(Game::getWindowPtr(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS/* && !clicked*/) {
        clicked = true;
        bullets.AddModel(bullet, "__Model__", false);
        bullets.SetScaleByID(particles.size(), 0.01f, 0.01f, 0.01f, false);

        particles.resize(particles.size() + 1);

        particles[particles.size() - 1].m_position = playerPos + (playerDir / 3.0f);
        particles[particles.size() - 1].m_acceleration = qe::Vector<qe::real>(0.0f);
        particles[particles.size() - 1].m_velocity = qe::Vector<qe::real>(0.0f);
        particles[particles.size() - 1].addForce(playerDir * 1000.0f);
        particles[particles.size() - 1].setMass(1.0f);

        pfrg.Add(&particles[particles.size() - 1], &pg);

        if(bullets.GetModelAmount() > 200) {
            bullets.RemoveModel(0, false);
            pfrg.Remove(&particles[0], &pg);
            particles.erase(particles.begin());
        }
    }
    else if(glfwGetMouseButton(Game::getWindowPtr(), GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
        clicked = false;
    }

    input();
}

void Application() {
    Game game;

    game.run();
}

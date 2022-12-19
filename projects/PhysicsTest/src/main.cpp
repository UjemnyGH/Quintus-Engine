/**
 * @file main.cpp
 * @author Piotr UjemnyGH Plombon
 * @brief Physics example and testing. It can crash from seg fault for now!!!
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyleft (c) all wrongs reserved 2022
 * 
 */

#include "../../../engine/quintus.hpp"
#include <future>

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void FixedUpdate() override;
    void Inputs();
};

// Player
qe::Vector<float> player_position = {0.0f, 0.0f, 0.0f, 1.0f};
qe::Vector<float> player_direction = {0.0f, 0.0f, 1.0f};
qe::Vector<float> player_right = {0.0f, 0.0f, 1.0f};

qe::Vector<int> last(400, 300);
qe::Vector<float> camera_axis;
float sensitivity = 0.01f;
bool mouse_on_pos = false;
bool lock_mouse = false;
float fixed_time = 0.0f;

// Bullet physics
qe::ParticleForceRegistry bullets_force_registry;
qe::ParticleGravity bullet_gravity = qe::ParticleGravity({0.0f, -1.0f, 0.0f});

std::vector<qe::Particle> bullets;

// To use more adwance model
// qe::ModelLoader bullets_model;
qe::Renderer bullets_renderer;
bool bullets_binding = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS || lock_mouse) {
        if(!mouse_on_pos) {
            mouse_on_pos = true;

            glfwSetCursorPos(window, (double)last.x, (double)last.y);
        }

        float offset_x = xpos - last.x;
        float offset_y = last.y - ypos;

        last = {(int)xpos, (int)ypos};

        offset_x *= sensitivity;
        offset_y *= sensitivity;

        camera_axis.x += offset_x;
        camera_axis.y += offset_y;

        camera_axis.y = qe::math::clamp(camera_axis.y, qe::math::to_radians(-89.99f), qe::math::to_radians(89.99f));

        player_direction.x = cos(camera_axis.y) * cos(camera_axis.x);
        player_direction.y = sin(camera_axis.y);
        player_direction.z = sin(camera_axis.x) * cos(camera_axis.y);

        player_direction.normalize();

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else {
        mouse_on_pos = false;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Game::Inputs() {
    player_right = player_direction.cross({0.0f, 1.0f, 0.0f}).normalize();

    if(getKeyPress(qe::W, qe::PRESS)) {
        player_position.x += player_position.w * player_direction.x * time.GetDeltaTime();
        player_position.z += player_position.w * player_direction.z * time.GetDeltaTime();
    }
    else if(getKeyPress(qe::S, qe::PRESS)) {
        player_position.x -= player_position.w * player_direction.x * time.GetDeltaTime();
        player_position.z -= player_position.w * player_direction.z * time.GetDeltaTime();
    }

    if(getKeyPress(qe::A, qe::PRESS)) {
        player_position -= player_right * player_position.w * time.GetDeltaTime();
    }
    else if(getKeyPress(qe::D, qe::PRESS)) {
        player_position += player_right * player_position.w * time.GetDeltaTime();
    }

    if(getKeyPress(qe::F1, qe::PRESS) && lock_mouse) {
        lock_mouse = false;
    }
    else if(getKeyPress(qe::F1, qe::PRESS) && !lock_mouse) {
        lock_mouse = true;
    }
}

void Game::Start() {
    // Code here

    glfwSetCursorPosCallback(Game::getWindowPtr(), mouse_callback);

    setVSync(true);

    AddLayer(&bullets_renderer);
    bullets_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    bullets_renderer.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));
    bullets_renderer.AddModel(qe::square, "__Model__");
    bullets.resize(1);
}

void SetBulletsPos(qe::Particle particle, uint32_t id) {
    bullets_renderer.SetPositionByID(id, particle.m_position.x, particle.m_position.y, particle.m_position.z, false);
}

void Game::Update() {
    // Code here

    qe::g_view = glm::lookAt(glm::vec3(player_position.x, player_position.y, player_position.z), glm::vec3(player_position.x, player_position.y, player_position.z) + glm::vec3(player_direction.x, player_direction.y, player_direction.z), glm::vec3(0.0f, 1.0f, 0.0f));

    for(uint32_t i = 0; i < bullets_renderer.GetModelAmount(); i++) {
        std::future<void> bullet_pos_change_async = std::async(SetBulletsPos, bullets[i], i);
        //SetBulletsPos(bullets[i], i);
    }

    if(bullets_binding) {
        bullets_renderer.BindToBuffers();

        bullets_binding = false;
    }
}

void RepositionThreadFn() {
    for(uint32_t i = 0; i < bullets_renderer.GetModelAmount(); i++) {
        std::future<void> bullet_pos_change_async = std::async(SetBulletsPos, bullets[i], i);
        //SetBulletsPos(bullets[i], i);
    }
}

void Game::LateUpdate() {
    std::jthread repos_thread(RepositionThreadFn);

    repos_thread.detach();

    if(glfwGetMouseButton(getWindowPtr(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        bullets_renderer.AddModel(qe::cube, "__Model__", false);
        bullets_renderer.SetScaleByID(bullets_renderer.GetModelAmount() - 1, 0.01f, 0.01f, 0.01f, false);

        bullets.resize(bullets.size() + 1);

        bullets[bullets.size() - 1].m_acceleration = qe::Vector<float>(0.0f);
        bullets[bullets.size() - 1].m_force = qe::Vector<float>(0.0f);
        bullets[bullets.size() - 1].m_velocity = qe::Vector<float>(0.0f);
        bullets[bullets.size() - 1].m_position = player_position + (player_direction * 0.33f);
        bullets[bullets.size() - 1].addForce(player_direction * 1000.0f);
        bullets[bullets.size() - 1].setMass(1.0f);
        bullets[bullets.size() - 1].setDeleteTime(time.GetTime(), 2.0f);
        bullets[bullets.size() - 1].deleteOnTime(true);
        
        //bullets_force_registry.Add(&bullets[bullets.size() - 1], &bullet_gravity);
    }
    
    Inputs();
}

void Game::FixedUpdate() {
    float duration = getFixedUpdateTimeIntervalInSec();

    //bullets_force_registry.UpdateForces(duration);

    for(int i = 0; i < bullets.size(); i++) {
        if(bullets[i].deleteParticle(fixed_time)) {
            //bullets_force_registry.Remove(&bullets[i], &bullet_gravity);
            bullets.erase(bullets.begin() + i);
            bullets_renderer.RemoveModel(i, false);
        }

        bullet_gravity.updateForce(&bullets[i], duration);

        bullets[i].integrate(duration);
    }

    fixed_time += duration;

    bullets_binding = true;
}

void Application() {
    Game game;

    game.run();
}

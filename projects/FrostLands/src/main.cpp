#include "../../../engine/quintus.hpp"
#include "player.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void FixedUpdate() override;
    void input();
};

Player player;
Game game;

qe::Keys forward_key = qe::W;
qe::Keys backward_key = qe::S;
qe::Keys strafe_left_key = qe::A;
qe::Keys strafe_right_key = qe::D;
qe::Keys jump_key = qe::SPACE;
bool move_key_press = false;

qe::Renderer cube[2];

void cursor_callback(GLFWwindow* window, double x, double y) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        player.m_mouse_lock = true;
    } 
    else {
        player.m_mouse_lock = false;
    }

    if(player.m_mouse_released && player.m_mouse_lock) {
        player.m_mouse_released = false;

        glfwSetCursorPos(window, Game::m_width / 2, Game::m_height / 2);
    }

    if(player.m_mouse_lock) {
        game.setCursorState(qe::HIDDEN);
    }
    else {
        game.setCursorState(qe::NORMAL);
    }

    player.update_camera(x, y);
}

void Game::Start() {
    // Code here

    glfwSetCursorPosCallback(getWindowPtr(), cursor_callback);

    AddLayer(&cube[0]);

    cube[0].AddShader(qe::LoadShaderFromPath("data/shaders/light.vert", qe::vertex));
    cube[0].AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    cube[0].AddModel(qe::cube, "Cube1");

    cube[0].SetColorByID(cube[0].GetIdFromName("Cube1"), 0.6f, 0.4f, 0.3f, 1.0f);
    cube[0].SetPositionByID(cube[0].GetIdFromName("Cube1"), 0.0f, 0.0f, 2.9f);

    AddLayer(&cube[1]);

    cube[1].AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    cube[1].AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));
    cube[1].AddModel(qe::cube, "Cube1");
}

void Game::Update() {
    // Code here
    qe::g_view = player.get_view();
}

void Game::input() {
    move_key_press = false;

    if(getKeyPress(forward_key)) {
        player.m_position.addForce(qe::Vector(player.m_direction.x, 0.0f, player.m_direction.z) * time.GetDeltaTime() * player.m_player_speed);
        move_key_press = true;
    }
    else if(getKeyPress(backward_key)) {
        player.m_position.addForce((qe::Vector(player.m_direction.x, 0.0f, player.m_direction.z)  * time.GetDeltaTime() * player.m_player_speed) * -1.0f);
        move_key_press = true;
    }

    if(getKeyPress(strafe_left_key)) {
        player.m_position.addForce((player.m_right * time.GetDeltaTime() * player.m_player_speed) * -1.0f);
        move_key_press = true;
    }
    else if(getKeyPress(strafe_right_key)) {
        player.m_position.addForce(player.m_right * time.GetDeltaTime() * player.m_player_speed);
        move_key_press = true;
    }

    if(getKeyPress(jump_key)) {
        player.m_position.addForce({0.0f, 10.0f, 0.0f});
        move_key_press = true;
    }

    if(!move_key_press) {
        player.m_position.addForce((player.m_position.m_velocity * 0.5f) * -1.0f);
    }
}

void Game::LateUpdate() {
    input();
}

void Game::FixedUpdate() {
    player.m_position.integrate(getFixedUpdateTimeIntervalInSec());
}

void Application() {
    game.run();
}

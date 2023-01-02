#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void LateUpdate() override;
    virtual void Input();
};

qe::Vector<float> player_pos = {0.0f, 0.0f, 0.0f};
qe::Vector<float> player_dir = {0.0f, 0.0f, 1.0f};
qe::Vector<float> player_r = {0.0f, 0.0f, 0.0f};
qe::Vector<float> player_cam = {0.0f, 0.0f, 0.0f};
qe::Vector<int> last = {400, 300};
bool mouse_pressed = false;
bool mouse_lock = false;
float sensitivity = 0.01f;

qe::Particle particles[64];
qe::ParticleGravity gravity;
qe::ParticleBungee bungee[4];

const float rest_len = 1.0f;
const float spring_const = 1.0f;

qe::Renderer rend1;

qe::RenderedData fabric = {
    {
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
    },
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    },
    {
        0.0f
    },
    {
        0.0f
    },
    {
        0
    }
};

Game game;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS || mouse_lock) {
        game.setCursorState(qe::HIDDEN);

        if(!mouse_pressed) {
            mouse_pressed = true;

            glfwSetCursorPos(window, last.x, last.y);
        }

        float offsetX = xpos - last.x;
        float offsetY = last.y - ypos;

        last = {(int)xpos, (int)ypos};

        offsetX *= sensitivity;
        offsetY *= sensitivity;

        player_cam.x += offsetX;
        player_cam.y += offsetY;

        player_cam.y = std::clamp(player_cam.y, qe::math::to_radians(-89.99f), qe::math::to_radians(89.99f));

        player_dir.x = cos(player_cam.y) * cos(player_cam.x);
        player_dir.y = sin(player_cam.y);
        player_dir.z = sin(player_cam.x) * cos(player_cam.y);

        player_dir = player_dir.normalize();

        player_r = player_dir.normalize().cross({0.0f, 1.0f, 0.0f}).normalize();
    }
    else {
        mouse_pressed = false;

        game.setCursorState(qe::NORMAL);
    }
}

void Game::Start() {
    // Code here
    glfwSetCursorPosCallback(getWindowPtr(), mouse_callback);

    fabric.m_color.resize(64 * 4);
    fabric.m_vertices.resize(64 * 3);

    for(int i = 0; i < fabric.m_color.size() / 4; i++) {
        fabric.m_color[i * 4 + 0] = 1.0f;
        fabric.m_color[i * 4 + 1] = 1.0f;
        fabric.m_color[i * 4 + 2] = 1.0f;
        fabric.m_color[i * 4 + 3] = 1.0f;
    }

    AddLayer(&rend1);
    rend1.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    rend1.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));
    rend1.AddModel(fabric);

    gravity.SetData({0.0f, -10.0f, 0.0f});
}

void Game::Update() {
    // Code here
    qe::g_view = glm::lookAt(glm::vec3(player_pos.x, player_pos.y, player_pos.z), glm::vec3(player_pos.x, player_pos.y, player_pos.z) + glm::vec3(player_dir.x, player_dir.y, player_dir.z), glm::vec3(0.0f, 1.0f, 0.0f));

    for(int i = 0; i < fabric.m_vertices.size() / 9; i++) {
        fabric.m_vertices[i * 9 + 0] = particles[i * 3 + 0].m_position.x;
        fabric.m_vertices[i * 9 + 1] = particles[i * 3 + 0].m_position.y;
        fabric.m_vertices[i * 9 + 2] = particles[i * 3 + 0].m_position.z;

        fabric.m_vertices[i * 9 + 3] = particles[i * 3 + 1].m_position.x;
        fabric.m_vertices[i * 9 + 4] = particles[i * 3 + 1].m_position.y;
        fabric.m_vertices[i * 9 + 5] = particles[i * 3 + 1].m_position.z;

        fabric.m_vertices[i * 9 + 6] = particles[i * 3 + 1].m_position.x;
        fabric.m_vertices[i * 9 + 7] = particles[i * 3 + 1].m_position.y;
        fabric.m_vertices[i * 9 + 8] = particles[i * 3 + 1].m_position.z;
    }

    rend1.AddModel(fabric);
    rend1.RemoveModel(0);

    rend1.BindToBuffers();
}

void Game::FixedUpdate() {
    // Code here
    for(int i = 0; i < fabric.m_vertices.size() / 3; i++) {
        if(i - 8 >= 0) {
            bungee[0].SetData(&particles[i - 8], spring_const, rest_len);
            bungee[0].updateForce(&particles[i], getFixedUpdateTimeIntervalInSec());
        }

        if(i + 8 < 64) {
            bungee[1].SetData(&particles[i + 8], spring_const, rest_len);
            bungee[1].updateForce(&particles[i], getFixedUpdateTimeIntervalInSec());
        }

        if(i - 1 >= 0) {
            bungee[2].SetData(&particles[i - 1], spring_const, rest_len);
            bungee[2].updateForce(&particles[i], getFixedUpdateTimeIntervalInSec());
        }

        if(i + 1 < 64) {
            bungee[3].SetData(&particles[i + 1], spring_const, rest_len);
            bungee[3].updateForce(&particles[i], getFixedUpdateTimeIntervalInSec());
        }

        gravity.updateForce(&particles[i == 0 ? 1 : i], getFixedUpdateTimeIntervalInSec());

        particles[i].integrate(getFixedUpdateTimeIntervalInSec());
    }
}

void Game::LateUpdate() {
    // Code here

    Input();
}

void Game::Input() {
    if(getKeyPress(qe::W)) {
        player_pos.x += player_dir.x * time.GetDeltaTime();
        player_pos.z += player_dir.z * time.GetDeltaTime();
    }
    else if(getKeyPress(qe::S)) {
        player_pos.x -= player_dir.x * time.GetDeltaTime();
        player_pos.z -= player_dir.z * time.GetDeltaTime();
    }

    if(getKeyPress(qe::D)) {
        player_pos += player_r * time.GetDeltaTime();
    }
    else if(getKeyPress(qe::A)) {
        player_pos -= player_r * time.GetDeltaTime();
    }

    if(getKeyPress(qe::_1) && !mouse_lock) {
        mouse_lock = true;
    }
    else if(getKeyPress(qe::_1) && mouse_lock) {
        mouse_lock = false;
    }
}

void Application() {
    game.run();
}

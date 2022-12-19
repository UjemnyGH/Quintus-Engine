#define BOOST_UNITS_CGS_TIME_HPP
#define BOOST_UNITS_SI_TIME_HPP
#include "../../../engine/quintus.hpp"
#include <future>

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void LateUpdate() override;
    void Input();
};

qe::Particle player_phys;
qe::Vector<float> player_dir(0.0f, 0.0f, 1.0f);
qe::Vector<float> player_right;
float player_speed_incr_rate = 1.0f;
float max_player_speed = 10.0f;

bool mouse_lock = false;
bool mouse_unlocked = true;
float sens = 0.01f;
qe::Vector<float> cam_end;
qe::Vector<int> last = {400, 300};

qe::Renderer renderer;
qe::Renderer line_renderer;

qe::RenderedData line_data = {
    {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    },
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    },
    {
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
    },
    {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
    },
    {
        0
    }
};

qe::Particle cube1;
qe::Particle cube2;
qe::Particle cube3;

qe::ParticleGravity gravity({0.0f, -10.0f, 0.0f});
qe::ParticleSpring spring(&cube1, 0.2f, 10.0f);
qe::ParticleSpring spring2(&cube2, 0.5f, 5.0f);
qe::ParticleDragFluid drag(2.0f);

Game *ptr;

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS || mouse_lock) {
        ptr->setCursorState(qe::LOCKED);

        if(mouse_unlocked) {
            last.x = ptr->m_width / 2.0;
            last.y = ptr->m_height / 2.0;

            glfwSetCursorPos(window, (double)last.x, (double)last.y);

            mouse_unlocked = false;
        }

        float offset_x = xpos - last.x;
        float offset_y = last.y - ypos;

        last.x = xpos;
        last.y = ypos;

        offset_x *= sens;
        offset_y *= sens;

        cam_end.x += offset_x;
        cam_end.y += offset_y;

        cam_end.y = qe::math::clamp(cam_end.y, qe::math::to_radians(-89.99f), qe::math::to_radians(89.99f));

        player_dir.x = cos(cam_end.y) * cos(cam_end.x);
        player_dir.y = sin(cam_end.y);
        player_dir.z = cos(cam_end.y) * sin(cam_end.x);

        player_dir = player_dir.normalize();

        player_right = player_dir.cross({0.0f, 1.0f, 0.0f}).normalize();
    }
    else {
        ptr->setCursorState(qe::NORMAL);

        mouse_unlocked = true;
    }
}

void Game::Input() {
    if(getKeyPress(qe::W)) {
        player_phys.m_position.x += player_dir.x * max_player_speed * time.GetDeltaTime();
        player_phys.m_position.z += player_dir.z * max_player_speed * time.GetDeltaTime();
    }
    else if(getKeyPress(qe::S)) {
        player_phys.m_position.x -= player_dir.x * max_player_speed * time.GetDeltaTime();
        player_phys.m_position.z -= player_dir.z * max_player_speed * time.GetDeltaTime();
    }

    if(getKeyPress(qe::A)) {
        player_phys.m_position -= player_right * max_player_speed * time.GetDeltaTime();
    }
    else if(getKeyPress(qe::D)) {
        player_phys.m_position += player_right * max_player_speed * time.GetDeltaTime();
    }

    if(getKeyPress(qe::_1, qe::PRESS) && !mouse_lock) {
        mouse_lock = true;
    }
    else if(getKeyPress(qe::_1, qe::PRESS) && mouse_lock) {
        mouse_lock = false;
    }
}

void Game::Start() {
    // Code here
    glfwSetCursorPosCallback(getWindowPtr(), cursor_callback);
    setVSync(true);
    m_fixed_update_per_second = 128;

    AddLayer(&renderer);

    renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    renderer.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));
    renderer.AddModel(qe::cube, "Cube1");
    renderer.AddModel(qe::cube, "Cube2");
    renderer.AddModel(qe::cube, "Cube3");
    renderer.SetColorByID(0, 0.0f, 0.0f, 1.0f, 1.0f);
    renderer.SetScaleByID(1, 1.2f, 1.2f, 1.2f);

    AddLayer(&line_renderer);
    line_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    line_renderer.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));
    line_renderer.AddModel(line_data, "Line1");
    line_renderer.SetColorByID(0, 0.0f, 1.0f, 0.0f, 1.0f);
    line_renderer.m_triangles = false;
    
    player_phys.m_position = {0.0f, 0.0f, -2.0f};

    cube2.m_position = {10.0f, -10.0f, 10.0f};
}

void SetPosByParticle(qe::Particle *part, uint32_t index, qe::Renderer *rend) {
    rend->SetPositionByID(index, part->m_position.x, part->m_position.y, part->m_position.z);
}

void Game::Update() {
    // Code here

    qe::g_view = glm::lookAt(glm::vec3(player_phys.m_position.x, player_phys.m_position.y, player_phys.m_position.z), glm::vec3(player_phys.m_position.x, player_phys.m_position.y, player_phys.m_position.z) + glm::vec3(player_dir.x, player_dir.y, player_dir.z), glm::vec3(0.0f, 1.0f, 0.0f));

    /*for(int i = 0; i < renderer.GetModelAmount(); i++) {
    
    }*/
    
    SetPosByParticle(&cube1, 0, &renderer);
    SetPosByParticle(&cube2, 1, &renderer);
    SetPosByParticle(&cube3, 2, &renderer);

    line_data.m_vertices = {
        cube1.m_position.x,
        cube1.m_position.y,
        cube1.m_position.z,

        cube2.m_position.x,
        cube2.m_position.y,
        cube2.m_position.z,

        cube2.m_position.x,
        cube2.m_position.y,
        cube2.m_position.z,

        cube3.m_position.x,
        cube3.m_position.y,
        cube3.m_position.z,
    };

    line_renderer.RemoveModel(0);
    line_renderer.AddModel(line_data);

    renderer.BindToBuffers();
}

void Game::FixedUpdate() {
    // Code here

    spring.updateForce(&cube2, getFixedUpdateTimeIntervalInSec());
    gravity.updateForce(&cube2, getFixedUpdateTimeIntervalInSec());
    drag.updateForce(&cube2, getFixedUpdateTimeIntervalInSec());

    spring2.updateForce(&cube3, getFixedUpdateTimeIntervalInSec());
    gravity.updateForce(&cube3, getFixedUpdateTimeIntervalInSec());
    drag.updateForce(&cube3, getFixedUpdateTimeIntervalInSec());

    cube1.integrate(getFixedUpdateTimeIntervalInSec());
    cube2.integrate(getFixedUpdateTimeIntervalInSec());
    cube3.integrate(getFixedUpdateTimeIntervalInSec());
}

void Game::LateUpdate() {
    // Code here
    printf("Cube2: %f %f %f\n", cube2.m_position.x, cube2.m_position.y, cube2.m_position.z);

    Input();
}

void Application() {
    Game game;

    ptr = &game;

    game.run();
}

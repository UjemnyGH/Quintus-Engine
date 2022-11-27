#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
};

glm::vec3 camera = glm::vec3(0.0f);
glm::vec2 pos = glm::vec2(0.0f);
qe::Renderer square;
qe::Collider collider[2];

void Game::Start() {
    // Code here
    Game::m_orthographic = true;
    Game::UpdatePerspective();

    Game::AddLayer(&square);

    square.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    square.AddShader(qe::LoadShader(qe::color_fragment_shader, qe::ShaderType::fragment));
    square.m_triangles = false;

    qe::RenderedData ddata = {
        {
            0.5f, 0.6f, 0.0f,
            -0.5f, -0.4f, 0.0f,
            
        },
        {
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
        },
        {
            1.0f, 1.0f,
            1.0f, 1.0f,
        },
        {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
        },
        {
            0, 1
        }
    };

    qe::RenderedData dddata = {
        {
            0.0f, 0.5f, 0.0f,
            -0.3f, -0.4f, 0.0f,
        },
        {
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
        },
        {
            1.0f, 1.0f,
            1.0f, 1.0f,
        },
        {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
        },
        {
            0, 1
        }
    };

    square.AddModel(dddata, "Line1");
    square.AddModel(ddata, "Line2");

    square.ForceRejoin();
}

void Game::Update() {
    // Code here
    qe::g_view = glm::lookAt(camera - glm::vec3(0.0f, 0.0f, 1.0f), camera, glm::vec3(0.0f, 1.0f, 0.0f));
    square.SetPositionByID(square.GetIdFromName("Line1"), pos.x, pos.y, 0.0f);
}

void Game::LateUpdate() {
    collider[0].m_position = qe::math::Vector<float>(pos.x, pos.y + 0.5f, 0.0f);
    collider[0].m_size = qe::math::Vector<float>(pos.x - 0.3f, pos.y - 0.4f, 0.0f);
    collider[1].m_position = qe::math::Vector<float>(0.5f, 0.6f, 0.0f);
    collider[1].m_size = qe::math::Vector<float>(-0.5f, -0.4f, 0.0f);
    
    if(collider[0].CheckLine2DToLine2D(&collider[1])) {
        square.SetColorByID(0, 1.0f, 0.0f, 0.0f, 1.0f);
        square.SetColorByID(1, 1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        square.SetColorByID(0, 0.0f, 1.0f, 0.0f, 1.0f);
        square.SetColorByID(1, 0.0f, 1.0f, 0.0f, 1.0f);
    }

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_W) == GLFW_PRESS) {
        pos.y += 1.0f * Game::time.GetDeltaTime();
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_S) == GLFW_PRESS) {
        pos.y -= 1.0f * Game::time.GetDeltaTime();
    }

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_A) == GLFW_PRESS) {
        pos.x += 1.0f * Game::time.GetDeltaTime();
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_D) == GLFW_PRESS) {
        pos.x -= 1.0f * Game::time.GetDeltaTime();
    }
}

void Application() {
    Game game;

    game.run();
}

#include "../../../engine/quintus.hpp"
#include <random>

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    void input();
};

qe::Renderer test1;
qe::Renderer skybox;
glm::vec3 playerPos = glm::vec3(0.0f);
glm::vec3 playerDir = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 playerVelocity = glm::vec3(0.0f);
float playerVelocityGain = 0.05f;
float playerVelocityMax = 0.5f;
int lastX = 400;
int lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
bool move_press = false;
bool strafe_press = false;

void mouse(GLFWwindow* window, double xpos, double ypos) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.02f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = qe::math::clamp(pitch, glm::radians(-89.99f), glm::radians(89.99f));

        playerDir.x = cos(pitch) * cos(yaw);
        playerDir.y = sin(pitch);
        playerDir.z = sin(yaw) * cos(pitch);

        playerDir = glm::normalize(playerDir);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Game::input() {
    move_press = false;
    strafe_press = false;

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_W) == GLFW_PRESS) {
        playerVelocity.z += abs(playerVelocity.z) < playerVelocityMax ? playerVelocityGain : 0.0f;
        move_press = true;
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_S) == GLFW_PRESS) {
        playerVelocity.z -= abs(playerVelocity.z) < playerVelocityMax ? playerVelocityGain : 0.0f;
        move_press = true;
    }

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_D) == GLFW_PRESS) {
        playerVelocity.x += abs(playerVelocity.x) < playerVelocityMax ? playerVelocityGain : 0.0f;
        strafe_press = true;
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_A) == GLFW_PRESS) {
        playerVelocity.x -= abs(playerVelocity.x) < playerVelocityMax ? playerVelocityGain : 0.0f;
        strafe_press = true;
    }

    playerPos.z += playerDir.z * playerVelocity.z;
    playerPos.x += playerDir.x * playerVelocity.z;

    playerPos += glm::normalize(glm::cross(playerDir, glm::vec3(0.0f, 1.0f, 0.0f))) * playerVelocity.x;

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

    printf("Velocities: %f %f %f\n", playerVelocity.x, playerVelocity.y, playerVelocity.z);
}

void Game::Start() {
    // Code here
    Game::m_far = 10000.0f;
    glfwSetCursorPosCallback(Game::getWindowPtr(), mouse);

    qe::ModelLoader ml;
    ml.m_debug = true;

    ml.LoadModelUnindexed("data/models/test_cube.obj", qe::ModelType::OBJ);

    Game::m_orthographic = false;
    qe::g_globalLight = glm::vec3(40.0f, 100.0f, 0.0f);
    Game::setVSync(true);
    Game::UpdatePerspective();

    Game::AddLayer(&test1);
    Game::AddLayer(&skybox);

    test1.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    test1.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::ShaderType::fragment));
    for(int i = 0; i < ml.GetModelVectorData().size(); i++) {
        test1.AddModel(ml.GetModelVectorData()[i]);
    }
    test1.AddTexture("data/textures/test2k.png");
    test1.SetModelTexture(0, 0);


    skybox.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
    skybox.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::ShaderType::fragment));
    for(int i = 0; i < ml.GetModelVectorData().size(); i++) {
        skybox.AddModel(ml.GetModelVectorData()[i]);
    }
    skybox.AddTexture("data/textures/test_skybox2k.png");
    skybox.SetModelTexture(0, 0);
    skybox.SetScaleByID(0, 1000.0f, 1000.0f, 1000.0f);

    test1.ForceRejoin();
    skybox.ForceRejoin();
}

void Game::Update() {
    // Code here

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_R) == GLFW_PRESS) {
        test1.ClearShaders();
        test1.AddShader(qe::LoadShader(qe::vertex_shader, qe::ShaderType::vertex));
        test1.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::ShaderType::fragment));

        std::cerr << "Reloaded shaders\n";
    }

    qe::g_view = glm::lookAt(playerPos, playerPos + playerDir, glm::vec3(0.0f, 1.0f, 0.0f));

    //test1.SetRotationByID(0, Game::time.GetTime() * 1.0f, Game::time.GetTime() * 1.0f, Game::time.GetTime() * 1.0f);

    skybox.SetPosition(playerPos.x, playerPos.y, playerPos.z);
}

void Game::LateUpdate() {
    input();
}

void Application() {
    Game game;

    game.run();
}

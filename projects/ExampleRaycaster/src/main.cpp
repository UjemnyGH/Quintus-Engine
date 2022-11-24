#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
};

qe::RenderedData frame_data;
qe::Renderer frame_renderer;
const int width_factor = 2;

qe::math::Vector<float> playerPosition = 0.0f;
float playerAngle = 0.0f;

const uint32_t mapX = 10, mapY = 10, mapSize = 100; 

int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

uint32_t iteration = 0;

float pass_delta_time = 0.0f;
float lastpos;

void mouse(GLFWwindow* window, double x, double y) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        float xoff = x - lastpos;

        lastpos = x;

        playerAngle += xoff * 0.1f * pass_delta_time;

        if(playerAngle < 0.0f) {
            playerAngle += M_PI * 2;
        }


        if(playerAngle > M_PI * 2) {
            playerAngle -= 0.0f;
        }

        playerPosition.z = cos(playerAngle) * 2.0f;
        playerPosition.w = sin(playerAngle) * 2.0f;
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Game::Start() {
    // Code here

    glfwSetCursorPosCallback(Game::getWindowPtr(), mouse);

    playerPosition.z = cos(playerAngle) * 2.0f;
    playerPosition.w = sin(playerAngle) * 2.0f;

    Game::AddLayer(&frame_renderer);
    Game::m_orthographic = true;
    Game::setVSync(false);
    frame_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    frame_renderer.AddShader(qe::LoadShader(qe::color_fragment_shader, qe::fragment));
    frame_renderer.AddModel(frame_data);
    frame_renderer.m_triangles = false;
}

void Game::Update() {
    // Code here
    qe::g_projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 10000.0f);

    qe::g_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glLineWidth(static_cast<float>(width_factor));
    
    frame_renderer.AddModel(frame_data);

    if(iteration % 3 == 1) {
        frame_renderer.RemoveModel(0);

        iteration = 0;
    }

    printf("Frames: %f, Pos: %f %f, Angle %f\n", 1.0f / Game::time.GetDeltaTime(), playerPosition.x, playerPosition.y, playerAngle);

    iteration++;

    pass_delta_time = Game::time.GetDeltaTime();
}

void Game::LateUpdate() {
    frame_data.Clear();

    frame_data.m_texture_coordinates.push_back(0.0f);
    frame_data.m_normals.push_back(0.0f);
    frame_data.m_indices.push_back(0);

    int r, mx, my, mp, dof;
    qe::math::Vector<float> ray;
    qe::math::Vector<float> offset;
    float ray_Angle = playerAngle - (M_PI / 180.0 * 40.0), distance_total;

    if(ray_Angle < 0.0f) ray_Angle += 2 * M_PI;
    if(ray_Angle > 2 * M_PI) ray_Angle -= 2 * M_PI;

    for(int i = 0; i < Game::m_width / width_factor; i++) {
        dof = 0;

        float distance_horizntal = FLT_MAX;
        qe::math::Vector<float> horizontal = playerPosition;

        float atg = -1 / tan(ray_Angle);

        if(ray_Angle > M_PI) {
            ray.y = (((int)playerPosition.y >> 6) << 6) - 0.00001;
            ray.x = (playerPosition.y - ray.y) * atg + playerPosition.x;
            offset.y = -mapSize;
            offset.x = -offset.y * atg;
        }

        if(ray_Angle < M_PI) {
            ray.y = (((int)playerPosition.y >> 6) << 6) + mapSize;
            ray.x = (playerPosition.y - ray.y) * atg + playerPosition.x;
            offset.y = mapSize;
            offset.x = -offset.y * atg;
        }

        if(ray_Angle == 0 || ray_Angle == M_PI) {
            ray.x = playerPosition.x;
            ray.y = playerPosition.y;
            dof = mapX;
        }

        while(dof < mapX) {
            mx = (int)(ray.x) >> 6;
            my = (int)(ray.y) >> 6;
            mp = my * mapX + mx;

            if(mp > 0 && mp < mapX * mapY && map[mp] == 1) {
                horizontal = ray;

                distance_horizntal = qe::math::Vector<float>::distance(playerPosition, horizontal);

                dof = 10;
            }
            else {
                ray.x += offset.x;
                ray.y += offset.y;

                dof += 1;
            }
        }

        dof = 0;

        float distance_vertical = FLT_MAX;
        qe::math::Vector<float> vertical = playerPosition;

        atg = -tan(ray_Angle);

        if(ray_Angle > M_PI / 2 && ray_Angle < 3 * M_PI / 2) {
            ray.x = (((int)playerPosition.x >> 6) << 6) - 0.00001;
            ray.y = (playerPosition.x - ray.x) * atg + playerPosition.y;
            offset.x = -mapSize;
            offset.y = -offset.x * atg;
        }

        if(ray_Angle < M_PI / 2 || ray_Angle > 3 * M_PI / 2) {
            ray.x = (((int)playerPosition.x >> 6) << 6) + mapSize;
            ray.y = (playerPosition.x - ray.x) * atg + playerPosition.y;
            offset.x = mapSize;
            offset.y = -offset.x * atg;
        }

        if(ray_Angle == 0 || ray_Angle == M_PI) {
            ray.x = playerPosition.x;
            ray.y = playerPosition.y;
            dof = mapX;
        }

        while(dof < mapX) {
            mx = (int)(ray.x) >> 6;
            my = (int)(ray.y) >> 6;
            mp = my * mapX + mx;

            if(mp > 0 && mp < mapX * mapY && map[mp] == 1) {
                vertical = ray;

                distance_vertical = qe::math::Vector<float>::distance(playerPosition, vertical);

                dof = 10;
            }
            else {
                ray.x += offset.x;
                ray.y += offset.y;

                dof += 1;
            }
        }

        ray_Angle += (M_PI / 180.0) * ((Game::m_width / width_factor) / ((Game::m_width / width_factor) * (20.0f / width_factor)));

        ray = vertical < horizontal ? vertical : horizontal;

        distance_total = distance_horizntal < distance_vertical ? distance_horizntal : distance_vertical;

        float line_height = (mapSize * Game::m_height * 2.0f) / distance_total;

        line_height = qe::math::clamp(line_height, 0.0f, (float)Game::m_height * 2.0f);

        frame_data.m_vertices.push_back(((static_cast<float>(i) / static_cast<float>(Game::m_width / width_factor)) * 4.0f) - 2.0f); 
        frame_data.m_vertices.push_back(2.0f - (line_height / Game::m_height)); 
        frame_data.m_vertices.push_back(-1.0f);


        frame_data.m_vertices.push_back(((static_cast<float>(i) / static_cast<float>(Game::m_width / width_factor)) * 4.0f) - 2.0f); 
        frame_data.m_vertices.push_back(-2.0f + (line_height / (Game::m_height * 2.0f))); 
        frame_data.m_vertices.push_back(-1.0f);

        frame_data.m_color.push_back((distance_total / Game::m_height));
        frame_data.m_color.push_back((distance_total / Game::m_height));
        frame_data.m_color.push_back((distance_total / Game::m_height));
        frame_data.m_color.push_back(1.0f);

        frame_data.m_color.push_back((distance_total / Game::m_height));
        frame_data.m_color.push_back((distance_total / Game::m_height));
        frame_data.m_color.push_back((distance_total / Game::m_height));
        frame_data.m_color.push_back(1.0f);
    }

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_W) == GLFW_PRESS) {
        playerPosition.y += 10.0f * Game::time.GetDeltaTime();
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_S) == GLFW_PRESS) {
        playerPosition.y -= 10.0f * Game::time.GetDeltaTime();
    }

    if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_A) == GLFW_PRESS) {
        playerPosition.x += 10.0f * Game::time.GetDeltaTime();
    }
    else if(glfwGetKey(Game::getWindowPtr(), GLFW_KEY_D) == GLFW_PRESS) {
        playerPosition.x -= 10.0f * Game::time.GetDeltaTime();
    }
}

void Application() {
    Game game;

    game.run();
}

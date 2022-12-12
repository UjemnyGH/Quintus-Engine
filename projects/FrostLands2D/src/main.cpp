#include "../../../engine/quintus.hpp"
#include <random>

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void FixedUpdate() override;
};

enum TileType {
    tile_null = 0,
    tile_grass = 1,
    tile_rock = 2,
    tile_tree = 3,
};

struct Tile {
    qe::Vector<float> position_and_size;
    TileType type;
};

qe::Renderer grass_renderer;
qe::Renderer tree_renderer;
qe::Renderer rock_renderer;
qe::Renderer player_renderer;
//qe::Renderer item_renderer;
qe::Renderer campfire_renderer;
qe::Renderer cold_renderer;

qe::Vector<float> player_position;

std::vector<qe::Collider> tree_colliders;
std::vector<qe::Collider> rock_colliders;

qe::Collider mouse_collider;

const int map_x = 100;
const int map_y = 100;

float cold = 0.0f;
float health = 100.0f;
float player_speed = 1.0f;
float mouse_x, mouse_y;
float view_scale = 0.25f;
float wood = 0.0f;
float rock = 0.0f;

bool campfire_place = false;
bool mouse_pressed = false;
bool generating_snow = true;

std::vector<Tile> rock_tiles;
std::vector<Tile> tree_tiles;

qe::Vector<float> offset = {4.0f, 4.0f};

void load_map() {
    int map_y_counter = 0;
    
    for(int i = 0; i < map_x * map_y; i++) {
        grass_renderer.AddModel(qe::square, "__Model__", false);
        grass_renderer.SetPositionByID(i, (i % map_x * offset.x) - ((map_x * offset.x) / 2), (map_y_counter * offset.y) - ((map_y * offset.y) / 2), -30.0f, false);
        grass_renderer.SetScaleByID(i, offset.x, offset.y, 1.0f, false);

        map_y_counter += i % map_y == 0 ? 1 : 0;
    }

    std::random_device rd;

    for(int i = 0; i < (map_x * map_y) / 2; i++) {
        tree_renderer.AddModel(qe::square, "__Model__", false);
        qe::Vector<float> positions = {(((rd() % map_x * offset.x) - ((map_x * offset.x) / 2))) + ((rd() % 100) / 100.0f), ((rd() % map_y * offset.y) - ((map_y * offset.y) / 2)) + ((rd() % 100) / 100.0f)};
        tree_renderer.SetPositionByID(i, positions.x, positions.y, -7.0f + ((rd() % 100) / 100.0f), false);
        tree_renderer.SetScaleByID(i, 1.0f, 2.0f, 1.0f, false);
        tree_colliders.resize(i);
        tree_colliders[tree_colliders.size() - 1].m_position = positions;
        tree_colliders[tree_colliders.size() - 1].m_size = {1.0f, 2.0f, 100.0f};
        //tree_renderer.SetRotationByID(i, qe::math::to_radians(70.0f), 0.0f, 0.0f, false);
        tree_tiles.push_back({positions, tile_tree});
    }

    for(int i = 0; i < (map_x * map_y) / 2; i++) {
        rock_renderer.AddModel(qe::square, "__Model__", false);
        qe::Vector<float> positions = {(((rd() % map_x * offset.x) - ((map_x * offset.x) / 2))) + ((rd() % 100) / 100.0f), ((rd() % map_y * offset.y) - ((map_y * offset.y) / 2)) + ((rd() % 100) / 100.0f)};

        for(Tile tree : tree_tiles) {
            if(positions == tree.position_and_size) {
                positions = {(((rd() % map_x * offset.x) - ((map_x * offset.x) / 2))) + ((rd() % 100) / 100.0f), ((rd() % map_y * offset.y) - ((map_y * offset.y) / 2)) + ((rd() % 100) / 100.0f)};

                break;
            }
        }

        rock_renderer.SetPositionByID(i, positions.x, positions.y, -28.0f + ((rd() % 100) / 100.0f), false);
        rock_renderer.SetScaleByID(i, 0.5f, 0.5f, 1.0f, false);
        rock_colliders.resize(i);
        rock_colliders[rock_colliders.size() - 1].m_position = positions;
        rock_colliders[rock_colliders.size() - 1].m_size = {0.5f, 0.5f, 100.0f};
        //rock_renderer.SetRotationByID(i, qe::math::to_radians(70.0f), 0.0f, 0.0f, false);
        rock_tiles.push_back({positions, tile_tree});
    }

    generating_snow = false;
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    mouse_x = xpos;
    mouse_y = ypos;
}

void ScrollCallback(GLFWwindow* window, double x, double y) {
    view_scale += y / 100.0f;

    view_scale = qe::math::clamp(view_scale, 0.1f, 10.0f);
}

void Game::Start() {
    // Code here

    glfwSetCursorPosCallback(Game::getWindowPtr(), MouseCallback);
    glfwSetScrollCallback(Game::getWindowPtr(), ScrollCallback);

    Game::m_orthographic = true;
    Game::m_fov = qe::math::to_radians(70.0f);
    Game::UpdatePerspective();

    grass_renderer.m_render = false;
    tree_renderer.m_render = false;
    rock_renderer.m_render = false;

    Game::AddLayer(&grass_renderer);

    grass_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    grass_renderer.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    grass_renderer.m_render_pixels = true;
    grass_renderer.AddTexture("data/textures/snow1.png");

    Game::AddLayer(&tree_renderer);

    tree_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    tree_renderer.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    tree_renderer.m_render_pixels = true;
    tree_renderer.AddTexture("data/textures/tree1.png");

    Game::AddLayer(&rock_renderer);

    rock_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    rock_renderer.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    rock_renderer.m_render_pixels = true;
    rock_renderer.AddTexture("data/textures/rock1.png");

    Game::AddLayer(&player_renderer);

    player_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    player_renderer.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    player_renderer.m_render_pixels = true;
    player_renderer.AddTexture("data/textures/player1.png");
    qe::ModelLoader player_model;
    player_model.LoadModelUnindexed("data/textures/player1.obj");

    player_renderer.AddModel(player_model.GetModelData(), "Player");
    //player_renderer.SetPositionByID(0, 0.0f, -2.0f, -2.0f);
    player_renderer.SetScaleByID(0, 0.2f, 0.2f, 0.2f);

    Game::AddLayer(&campfire_renderer);

    campfire_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    campfire_renderer.AddShader(qe::LoadShaderFromPath("data/shaders/light.frag", qe::fragment));
    campfire_renderer.m_render_pixels = true;
    campfire_renderer.AddTexture("data/textures/campfire1.png");

    /*Game::AddLayer(&item_renderer);

    item_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    item_renderer.AddShader(qe::LoadShader(qe::texture_fragment_shader, qe::fragment));
    item_renderer.m_render_with_g_projection = false;
    item_renderer.m_render_with_g_view = false;
    item_renderer.m_render_pixels = true;*/

    Game::AddLayer(&cold_renderer);

    cold_renderer.AddShader(qe::LoadShader(qe::vertex_shader, qe::vertex));
    cold_renderer.AddShader(qe::LoadShader(qe::color_fragment_shader, qe::fragment));
    cold_renderer.m_render_with_g_projection = false;
    cold_renderer.m_render_with_g_view = false;
    cold_renderer.m_render_pixels = true;
    cold_renderer.AddModel(qe::square, "cold");
    cold_renderer.AddModel(qe::square, "hp");

    cold_renderer.SetPositionByID(0, 0.95f, -0.9f, -2.0f);
    cold_renderer.SetPositionByID(1, -0.96f, -0.9f, -2.0f);
    cold_renderer.SetScaleByID(0, (cold / 100.0f) * 0.3f, 0.01f, 1.0f);
    cold_renderer.SetScaleByID(1, (health / 100.0f) * 0.3f, 0.01f, 1.0f);
    cold_renderer.SetColorByID(0, 0.5f, 0.5f, 1.0f, 1.0f);
    cold_renderer.SetColorByID(1, 1.0f, 0.0f, 0.0f, 1.0f);

    std::jthread load_map_th(load_map);

    load_map_th.detach();
}

void Game::Update() {
    // Code here

    qe::g_view = glm::lookAt(glm::vec3(0.0f, 10.0f, 10.0f) + (glm::vec3(player_position.x, player_position.y, player_position.z) * glm::vec3(view_scale)), glm::vec3(player_position.x, player_position.y, 0.0f) * glm::vec3(view_scale), glm::vec3(0.0f, 1.0f, 0.0f));
    qe::g_view = glm::scale(qe::g_view, glm::vec3(view_scale));

    player_renderer.SetPositionByID(0, player_position.x, player_position.y, player_position.z);
    //player_renderer.SetRotationByID(0, 0.0f, sin((((mouse_x / Game::m_width) - 0.5f) * 2.0f) * M_PI * 2.0f) * cos((((mouse_y / Game::m_height) - 0.5f) * 2.0f) * M_PI * 2.0f), 0.0f);

    cold_renderer.SetPositionByID(0, 0.95f - ((cold / 100.0f) * 0.15f), -0.95f, 2.0f);
    cold_renderer.SetPositionByID(1, -0.95f + ((health / 100.0f) * 0.15f), -0.95f, 2.0f);
    cold_renderer.SetScaleByID(0, (cold / 100.0f) * 0.15f, 0.01f, 1.0f);
    cold_renderer.SetScaleByID(1, (health / 100.0f) * 0.15f, 0.01f, 1.0f);

    if(!generating_snow) {
        grass_renderer.m_render = true;
        tree_renderer.m_render = true;
        rock_renderer.m_render = true;

        grass_renderer.BindToBuffers();
        tree_renderer.BindToBuffers();
        rock_renderer.BindToBuffers();
    }

    mouse_collider.m_position = {player_position.x + ((((mouse_x / Game::m_width) - 0.5f) * 4.0f) / view_scale), player_position.y - ((((mouse_y / Game::m_height) - 0.5f) * 4.0f) / view_scale), 0.0f};
    mouse_collider.m_size = {0.01f, 0.01f, 100.0f};

    if(campfire_place) {
        campfire_renderer.SetPositionByID(campfire_renderer.GetModelAmount() - 1, player_position.x + ((((mouse_x / Game::m_width) - 0.5f) * 4.0f) / view_scale), player_position.y - ((((mouse_y / Game::m_height) - 0.5f) * 4.0f) / view_scale), 0.0f);

        if(wood >= 4.0f && rock >= 3.0f) {
            campfire_renderer.SetColorByID(campfire_renderer.GetModelAmount() - 1, 0.0f, 1.0f, 0.0f, 1.0f);

            if(glfwGetMouseButton(Game::getWindowPtr(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                campfire_place = false;

                campfire_renderer.SetPositionByID(campfire_renderer.GetModelAmount() - 1, player_position.x + ((((mouse_x / Game::m_width) - 0.5f) * 4.0f) / view_scale), player_position.y - ((((mouse_y / Game::m_height) - 0.5f) * 4.0f) / view_scale), 0.0f);

                campfire_renderer.SetColorByID(campfire_renderer.GetModelAmount() - 1, 1.0f, 1.0f, 1.0f, 1.0f);
            }
        }
        else {
            campfire_renderer.SetColorByID(campfire_renderer.GetModelAmount() - 1, 1.0f, 0.0f, 0.0f, 1.0f);

            if(glfwGetMouseButton(Game::getWindowPtr(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                campfire_place = false;

                campfire_renderer.RemoveModel(campfire_renderer.GetModelAmount() - 1);
            }
        }
    }
}

void Game::LateUpdate() {
    cold += 0.1f * Game::time.GetDeltaTime();

    cold = qe::math::clamp(cold, 0.0f, 100.0f);

    if(cold > 80.0f) {
        health -= 1.5f * Game::time.GetDeltaTime();
    }

    if(Game::getKeyPress(qe::W, qe::PRESS)) {
        player_position.y += player_speed * Game::time.GetDeltaTime();
    }
    else if(Game::getKeyPress(qe::S, qe::PRESS)) {
        player_position.y -= player_speed * Game::time.GetDeltaTime();
    }

    if(Game::getKeyPress(qe::D, qe::PRESS)) {
        player_position.x += player_speed * Game::time.GetDeltaTime();
    }
    else if(Game::getKeyPress(qe::A, qe::PRESS)) {
        player_position.x -= player_speed * Game::time.GetDeltaTime();
    }

    if(Game::getKeyPress(qe::_1, qe::PRESS) && !campfire_place) {
        campfire_place = true;
        campfire_renderer.AddModel(qe::square);
    }

    mouse_pressed = glfwGetMouseButton(Game::getWindowPtr(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
}

void Game::FixedUpdate() {
    for(int i = 0; i < tree_colliders.size(); i++) {
        if(mouse_pressed && !campfire_place && tree_colliders[i].CheckAABBToAABB(&mouse_collider)) {
            tree_colliders.erase(tree_colliders.begin() + i);

            tree_renderer.RemoveModel(i, false);

            wood += 2.0f;

            break;
        }
    }

    for(int i = 0; i < rock_colliders.size(); i++) {
        if(mouse_pressed && !campfire_place && rock_colliders[i].CheckAABBToAABB(&mouse_collider)) {
            rock_colliders.erase(rock_colliders.begin() + i);

            rock_renderer.RemoveModel(i, false);

            rock += 2.0f;

            break;
        }
    }
}

void Application() {
    Game game;

    game.run();
}

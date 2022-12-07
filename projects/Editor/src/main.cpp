#include "../../../engine/quintus.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

bool unsaved_data_exist = true;
bool exit_app_call = false;
bool debug_data_show = false;
float last_frames[4096];
uint32_t frame_counter = 0;

void Game::Start() {
    // Code here

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(Game::getWindowPtr(), true);
    ImGui_ImplOpenGL3_Init("#version 450 core");
}

void Game::Update() {
    // Code here

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow((bool*)true);

    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Exit", "CTRL+Q")) {
                if(unsaved_data_exist) {
                    exit_app_call = true;
                }
                else {
                    qe::qe_req_term("USER EXIT REQUEST");
                }
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Editor")) {
            ImGui::MenuItem("Debug data", nullptr, &debug_data_show);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if(exit_app_call)
    if(ImGui::Begin("Save data", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground)) {

        ImGui::Text("Do you want to save your work?");
        if(ImGui::Button("Save", ImVec2(100.0f, 20.0f))) { qe::qe_req_term("USER EXIT REQUEST : SAVE"); }
        ImGui::SameLine();
        if(ImGui::Button("Discard", ImVec2(100.0f, 20.0f))) { qe::qe_req_term("USER EXIT REQUEST : DISCARD"); }
        ImGui::SameLine();
        if(ImGui::Button("Cancel", ImVec2(100.0f, 20.0f))) { exit_app_call = false; }

        ImGui::End();
    }

    if(debug_data_show) {
        if(ImGui::Begin("Debug data", &debug_data_show, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse)) {
            float _fps = 1.0f / Game::time.GetDeltaTime();

            ImGui::PlotLines("FrameRate", last_frames, 4096);
            ImGui::TextColored((_fps > 60.0f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f)), "FPS: %.2f", _fps);

            ImGui::End();
        }
    }

    last_frames[frame_counter < 4096 ? frame_counter : frame_counter = 0] = 1.0f / Game::time.GetDeltaTime();
    frame_counter++;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application() {
    Game game;

    game.run();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

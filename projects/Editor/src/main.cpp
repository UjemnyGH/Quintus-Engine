#include "code_editor.hpp"
#include "editor_object.hpp"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

bool exit_app_call = false;

bool debug_data_show = false;
int debug_location = 0;
float last_frames[4096];

bool unsaved_data_exist = true;

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

CodeEditor code_editor;
EditorObject editor_object;

void Game::Update() {
    // Code here

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow((bool*)true);

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
            ImGui::MenuItem("Code editor", nullptr, &code_editor.show_code_editor);
            ImGui::MenuItem("Objects editor", nullptr, &EditorObject::show_object_list);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if(exit_app_call) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImVec2 work_size = viewport->WorkSize;
        ImVec2 wnd_pos;

        wnd_pos.x = (work_size.x / 2.0f);
        wnd_pos.y = (work_size.y / 2.0f);

        ImGui::SetNextWindowPos(wnd_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        if(ImGui::Begin("Save data", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {

            ImGui::Text("Do you want to save your work?");
            if(ImGui::Button("Save", ImVec2(100.0f, 20.0f))) { qe::qe_req_term("USER EXIT REQUEST : SAVE"); }
            ImGui::SameLine();
            if(ImGui::Button("Discard", ImVec2(100.0f, 20.0f))) { qe::qe_req_term("USER EXIT REQUEST : DISCARD"); }
            ImGui::SameLine();
            if(ImGui::Button("Cancel", ImVec2(100.0f, 20.0f))) { exit_app_call = false; }

            ImGui::End();
        }
    }

    if(debug_data_show) {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;

        window_pos.x =(work_pos.x + work_size.x - PAD);
        window_pos.y =(work_pos.y + work_size.y - PAD);
        window_pos_pivot.x = 1.0f;
        window_pos_pivot.y = 1.0f;

        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Example: Simple overlay", &debug_data_show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove)) {
            float _fps = 1.0f / Game::time.GetDeltaTime();

            ImGui::Text("Debug data\n");
            ImGui::Separator();

            ImGui::PlotLines("FrameRate", last_frames, 4096);
            ImGui::TextColored((_fps > 60.0f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f)), "FPS: %.2f", _fps);

            if (ImGui::BeginPopupContextWindow()) {
                if (debug_data_show && ImGui::MenuItem("Close")) debug_data_show = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    code_editor.code_editor();
    EditorObject::object_list();

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
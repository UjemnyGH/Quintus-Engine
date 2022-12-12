#pragma once
#include "../../../engine/quintus.hpp"

class CodeEditor {
private:
    std::string file_name;
    std::string code_buffer;
    int code_chars = 0;

    bool w_open_file = false;
    bool w_save_file = false;
    bool w_save_as_file = false;

public:
    bool show_code_editor = false;

    void code_editor() {
        if(show_code_editor) {
            ImGuiViewport *viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);

            if(ImGui::Begin("Code editor", &show_code_editor, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings)) {
                if(ImGui::BeginMenuBar()) {
                    if(ImGui::BeginMenu("File")) {
                        if(ImGui::MenuItem("Open", "Ctrl+O", &w_open_file)) {}
                        if(ImGui::MenuItem("Save", "Ctrl+S")) { save_file(); }
                        if(ImGui::MenuItem("Save as", "Ctrl+Shift+S", &w_save_as_file)) {}

                        ImGui::EndMenu();
                    }

                    ImGui::EndMenuBar();
                }
                
                ImGui::InputTextMultiline("  Code", code_buffer.data(), code_buffer.size(), ImVec2(ImGui::GetWindowSize().x - 15.0f, ImGui::GetWindowSize().y - 55.0f));

                //code_buffer.erase(code_buffer.begin() + code_buffer.find_first_of((char)0), code_buffer.end());

                code_buffer.resize(code_buffer.size() + 10);

                ImGui::End();
            }

            open_file();
            save_as_file();
        }
    }

    void open_file() {
        if(w_open_file) {
            code_buffer.clear();

            ImGui::SetNextWindowSize(ImVec2(400.0f, 70.0f));

            if(ImGui::Begin("Open file", &w_open_file, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings)) {
                ImGui::InputTextWithHint("File name", "File", file_name.data(), file_name.size() + 100);

                if(ImGui::Button("Open")) {
                    std::ifstream f(file_name);

                    if(f.fail()) {
                        w_open_file = false;
                        w_save_as_file = true;

                        ImGui::End();

                        return;
                    }

                    f.seekg(0, std::ios::end);

                    code_buffer.resize(f.tellg());

                    f.seekg(0, std::ios::beg);

                    f.read(code_buffer.data(), code_buffer.size() - 1);

                    f.close();

                    w_open_file = false;
                }

                ImGui::SameLine();

                if(ImGui::Button("Cancel")) {
                    w_open_file = false;
                }
                
                ImGui::End();
            }
        }
    }

    void open_file_from_path(std::string path) {
        file_name = path;

        code_buffer.clear();

        std::ifstream f(file_name);

        if(f.fail()) {
            w_open_file = false;
            w_save_as_file = true;

            return;
        }

        f.seekg(0, std::ios::end);

        code_buffer.resize(f.tellg());

        f.seekg(0, std::ios::beg);

        f.read(code_buffer.data(), code_buffer.size() - 1);

        f.close();
    }

    void save_file() {
        std::ofstream f(file_name);

        if(f.fail()) {
            w_save_as_file = true;

            return;
        }

        f.write(code_buffer.c_str(), code_buffer.size());

        f.close();
    }

    void save_as_file() {
        if(w_save_as_file) {
            ImGui::SetNextWindowSize(ImVec2(400.0f, 70.0f));

            if(ImGui::Begin("Save as file", &w_save_as_file, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings)) {
                ImGui::InputTextWithHint("File name", "File", file_name.data(), file_name.size() + 100);

                if(ImGui::Button("Save as")) {
                    std::ofstream f(file_name);

                    f.write(code_buffer.data(), code_buffer.size() - 1);

                    f.close();

                    w_save_as_file = false;
                }

                ImGui::SameLine();

                if(ImGui::Button("Cancel")) {
                    w_save_as_file = false;
                }
                
                ImGui::End();
            }
        }
    }
};
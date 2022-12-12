#pragma once
#include "../../../engine/quintus.hpp"

class EditorObject {
private:
    std::string name;
    std::string model_path;
    std::string texture_path;
    qe::Vector<float> color;
    uint32_t model_index;
    uint32_t renderer_index;
    qe::ModelLoader model_data;

    static void __show_object__(EditorObject* object) {
        // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        bool node_open = ImGui::TreeNode(object->name.c_str(), object->name.c_str());
        /*ImGui::TableSetColumnIndex(1);
        ImGui::Text("my sailor is rich");*/

        if (node_open)
        {
            object->name.resize(255);
            object->model_path.resize(1023);
            object->texture_path.resize(1023);

            ImGui::InputTextWithHint("Object name", "Name", object->name.data(), 255);
            ImGui::InputTextWithHint("Model path", "Path", object->model_path.data(), 1023);
            ImGui::InputTextWithHint("Texture path", "Path", object->texture_path.data(), 1023);

            ImGui::Text("Model index: %d", object->model_index);
            ImGui::Text("Renderer index: %d", object->renderer_index);

            qe::Vector<float> position = static_renderers[object->renderer_index].GetModelPosition(object->model_index);
            qe::Vector<float> scale = static_renderers[object->renderer_index].GetModelScale(object->model_index);
            qe::Vector<float> rotation = static_renderers[object->renderer_index].GetModelRotation(object->model_index);

            float pos[3] = {position.x, position.y, position.z};
            float scl[3] = {scale.x, scale.y, scale.z};
            float rot[3] = {rotation.x, rotation.y, rotation.z};
            float col[4] = {object->color.x, object->color.y, object->color.z, object->color.w};

            ImGui::DragFloat3("Position", pos);
            ImGui::DragFloat3("Scale", scl);
            ImGui::DragFloat3("Rotation", rot);
            ImGui::ColorPicker4("Color", col);

            if(ImGui::Button("Reload model")) {
                object->model_data.LoadModelUnindexed(object->model_path);
                static_renderers[object->renderer_index].AddModel(object->model_data.GetModelData());
            }

            static_renderers[object->renderer_index].SetPositionByID(object->model_index, pos[0], pos[1], pos[2]);
            static_renderers[object->renderer_index].SetScaleByID(object->model_index, scl[0], scl[1], scl[2]);
            static_renderers[object->renderer_index].SetRotationByID(object->model_index, rot[0], rot[1], rot[2]);
            static_renderers[object->renderer_index].SetColorByID(object->model_index, col[0] / 255.0f, col[1] / 255.0f, col[2] / 255.0f, col[3] / 255.0f);

            /*static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
            for (int i = 0; i < 8; i++)
            {
                ImGui::PushID(i); // Use field index as identifier.
                if (i < 2)
                {
                    ShowPlaceholderObject("Child", 424242);
                }
                else
                {
                    // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                    ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (i >= 5)
                        ImGui::InputFloat("##value", &placeholder_members[i], 1.0f);
                    else
                        ImGui::DragFloat("##value", &placeholder_members[i], 0.01f);
                    ImGui::NextColumn();
                }
                ImGui::PopID();
            }*/
            ImGui::TreePop();
        }
    }

public:
    static bool show_object_list;

    static std::vector<qe::Renderer> static_renderers;
    static std::vector<EditorObject*> static_objects;

    /*EditorObject() {
        static_objects.push_back(this);
    }*/

    static void object_list() {
        if(show_object_list) {
            if(ImGui::Begin("Objects", &show_object_list, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Popup | ImGuiWindowFlags_MenuBar)) {
                if(ImGui::BeginPopupContextWindow()) {
                    if(ImGui::MenuItem("Add")) { 
                        static_objects.resize(static_objects.size() + 1);
                        static_objects[static_objects.size() - 1]->renderer_index = static_renderers.size() - 1;
                        static_objects[static_objects.size() - 1]->model_index = static_renderers[static_renderers.size() - 1].GetModelAmount();
                    }

                    ImGui::EndPopup();
                }

                if(ImGui::BeginTable("Objects", 1)) {
                    for(auto object : static_objects) {
                        __show_object__(object);
                    }

                    ImGui::EndTable();
                }
            }
        }
    }
};

bool EditorObject::show_object_list = false;

std::vector<qe::Renderer> EditorObject::static_renderers = std::vector<qe::Renderer>();
std::vector<EditorObject*> EditorObject::static_objects = std::vector<EditorObject*>();
#pragma once
#include "../../../engine/quintus.hpp"

class Player {
private:
    qe::Vector<float> m_cam;
    qe::Vector<int> m_last = {400, 300};

public:
    qe::Vector<float> m_right;
    qe::Particle m_position;
    qe::Vector<float> m_direction = {0.0f, 0.0f, 1.0f};
    bool m_mouse_released = true;
    bool m_mouse_lock = false;
    float m_sensitivity = 0.01f;
    float m_player_speed = 100.0f;

    void update_camera(double x, double y) {
        if(m_mouse_lock) {
            float offset_x = x - m_last.x;
            float offset_y = m_last.y - y;

            m_last = {(int)x, (int)y};

            offset_x *= m_sensitivity;
            offset_y *= m_sensitivity;

            m_cam.x += offset_x;
            m_cam.y += offset_y;

            m_cam.y = qe::math::clamp(m_cam.y, qe::math::to_radians(-89.99f), qe::math::to_radians(89.99f));

            m_direction.x = cos(m_cam.y) * cos(m_cam.x);
            m_direction.y = sin(m_cam.y);
            m_direction.z = sin(m_cam.x) * cos(m_cam.y);

            m_direction = m_direction.normalize();
            m_right = m_direction.cross({0.0f, 1.0f, 0.0f}).normalize();
        }
        else {
            m_mouse_released = true;
        }
    }

    glm::mat4 get_view() {
        return glm::lookAt(glm::vec3(m_position.m_position.x, m_position.m_position.y, m_position.m_position.z), glm::vec3(m_position.m_position.x, m_position.m_position.y, m_position.m_position.z) + glm::vec3(m_direction.x, m_direction.y, m_direction.z), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
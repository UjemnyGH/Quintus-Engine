#pragma once
#ifndef __P_COLLIDER_
#define __P_COLLIDER_

#include <glm/glm.hpp>
#include "../Core/c_algorithms.hpp"
#include <map>
#include <unordered_map>

namespace qe {
    class ColliderComponent {
    protected:
        qe::Vector<float> m_position = qe::Vector<float>(0.0f);
        qe::Vector<float> m_size = qe::Vector<float>(1.0f);

        std::string m_collider_type_name = "___ColliderComponent___";
    
    public:
        bool m_collision = false;

        std::string get_type() { return m_collider_type_name; }

        qe::Vector<float> get_position() { return m_position; }

        void set_position(qe::Vector<float> const &position) { m_position = position; }

        qe::Vector<float> get_size() { return m_size; }
        
        void set_size(qe::Vector<float> const &size) { m_size = size; }

        qe::Vector<float> get_radiuses() { return m_size; }

        void set_radiuses(qe::Vector<float> _radiuses) { m_size = _radiuses; }

        virtual bool is_colliding(ColliderComponent &_component) { return false;}
    };

    class AABB : public ColliderComponent {
    public:
        AABB(qe::Vector<float> _position = qe::Vector<float>(0.0f), qe::Vector<float> _size = qe::Vector<float>(1.0f)) {
            m_position = _position;
            m_size = _size;

            m_collider_type_name = "_AABB";
        }

        virtual bool is_colliding(ColliderComponent &_aabb) override {
            return (m_position - m_size) + (m_size * 2.0f) >= (_aabb.get_position() - _aabb.get_size()) && (_aabb.get_position() - _aabb.get_size()) + (_aabb.get_size() * 2.0f) >= (m_position - m_size);
        }
    };

    class BallToBall : public ColliderComponent {
    public:
        BallToBall(qe::Vector<float> _position = qe::Vector<float>(0.0f), qe::Vector<float> _radiuses = qe::Vector<float>(1.0f)) {
            set_radiuses(_radiuses);
            set_position(_position);

            m_collider_type_name = "_BallToBall";
        }

        virtual bool is_colliding(ColliderComponent &_sphere) {
            bool collX = abs((m_position.y - _sphere.get_position().y) * (m_position.y - _sphere.get_position().y) + (m_position.z - _sphere.get_position().z) * (m_position.z - _sphere.get_position().z)) <= (get_radiuses().x + _sphere.get_radiuses().x) * (get_radiuses().x + _sphere.get_radiuses().x);
            bool collY = abs((m_position.x - _sphere.get_position().x) * (m_position.x - _sphere.get_position().x) + (m_position.z - _sphere.get_position().z) * (m_position.z - _sphere.get_position().z)) <= (get_radiuses().y + _sphere.get_radiuses().y) * (get_radiuses().y + _sphere.get_radiuses().y);
            bool collZ = abs((m_position.y - _sphere.get_position().y) * (m_position.y - _sphere.get_position().y) + (m_position.x - _sphere.get_position().x) * (m_position.x - _sphere.get_position().x)) <= (get_radiuses().z + _sphere.get_radiuses().z) * (get_radiuses().z + _sphere.get_radiuses().z);
        
            return collX && collY && collZ;
        }
    };

    class ColliderComponentHandler {
    private:
        std::unordered_map<std::string, ColliderComponent*> m_collider_components;

    public:
        ColliderComponentHandler() = default;

        void add_collider(ColliderComponent* _collider_component, std::string name = "__ColliderComponent__") {
            m_collider_components.insert(std::make_pair(name, _collider_component));
        }

        void remove_collider(std::string name) {
            m_collider_components.erase(name);
        }

        void check_collision() {

            for(auto collider : m_collider_components) {
                for(auto coll : m_collider_components) {
                    if(collider.second != coll.second) {
                        if(collider.second->get_type() == coll.second->get_type()) {
                            collider.second->m_collision = collider.second->is_colliding(*coll.second);
                        }
                    }
                }
            }
        }
    };

    class Collider {
    public:
        math::Vector<float> m_position = math::Vector<float>(0.0f);
        math::Vector<float> m_size = math::Vector<float>(1.0f);
        math::Vector<float> m_third_point = math::Vector<float>(0.0f);

        /**
         * @brief It use m_size and m_position as references
         * 
         * @param collider 
         * @return true 
         * @return false 
         */
        bool CheckAABBToAABB(Collider* collider) {
            math::Vector<float> Position1 = m_position - m_size;
            math::Vector<float> Scale1 = m_size * math::Vector<float>(2.0f, 2.0f, 2.0f);

            math::Vector<float> Position2 = collider->m_position - collider->m_size;
            math::Vector<float> Scale2 = collider->m_size * math::Vector<float>(2.0f, 2.0f, 2.0f);

            bool collisionX = Position1.x + Scale1.x >= Position2.x &&
                Position2.x + Scale2.x >= Position1.x;

            bool collisionY = Position1.y + Scale1.y >= Position2.y &&
                Position2.y + Scale2.y >= Position1.y;

            bool collisionZ = Position1.z + Scale1.z >= Position2.z &&
                Position2.z + Scale2.z >= Position1.z;

            return collisionX && collisionY && collisionZ;
        }

        /**
         * @brief It use m_size(x, y and z axis, if you want perfect ball collision you need to set x, y and z to the same value) and m_position s references
         * 
         * @param collider 
         * @return true 
         * @return false 
         */
        bool CheckBallToBall(Collider* collider) {
            bool collX = abs((m_position.y - collider->m_position.y) * (m_position.y - collider->m_position.y) + (m_position.z - collider->m_position.z) * (m_position.z - collider->m_position.z)) <= (m_size.x + collider->m_size.x) * (m_size.x + collider->m_size.x);
            bool collY = abs((m_position.x - collider->m_position.x) * (m_position.x - collider->m_position.x) + (m_position.z - collider->m_position.z) * (m_position.z - collider->m_position.z)) <= (m_size.y + collider->m_size.y) * (m_size.y + collider->m_size.y);
            bool collZ = abs((m_position.y - collider->m_position.y) * (m_position.y - collider->m_position.y) + (m_position.x - collider->m_position.x) * (m_position.x - collider->m_position.x)) <= (m_size.z + collider->m_size.z) * (m_size.z + collider->m_size.z);

            return collX && collY && collZ;
        }

        bool CheckLineToLine(Collider* collider) {
            bool collX = false;//(m_size.x > collider->m_size.x && m_size.x < collider->m_position.x) || (m_position.x > collider->m_size.x && m_position.x < collider->m_position.x) || (m_size.x > collider->m_position.x && m_size.x < collider->m_size.x) || (m_position.x > collider->m_position.x && m_position.x < collider->m_size.x);
            bool collY = false;//(m_size.y > collider->m_size.y && m_size.y < collider->m_position.y) || (m_position.y > collider->m_size.y && m_position.y < collider->m_position.y) || (m_size.y > collider->m_position.y && m_size.y < collider->m_size.y) || (m_position.y > collider->m_position.y && m_position.y < collider->m_size.y);
            bool collZ = false;//(m_size.z > collider->m_size.z && m_size.z < collider->m_position.z) || (m_position.z > collider->m_size.z && m_position.z < collider->m_position.z) || (m_size.z > collider->m_position.z && m_size.z < collider->m_size.z) || (m_position.z > collider->m_position.z && m_position.z < collider->m_size.z);



            return collX && collY && collZ;
        }

        bool CheckCapsuleToCapluse(Collider* collider) {
            return false;   
        }

        /**
         * @brief Point to line 2D collisions
         * 
         * @param collider 
         * @param error_margin 
         * @param this_object_as_point 
         * @return true 
         * @return false 
         */
        bool CheckPointToLine2D(Collider* collider, float error_margin = 0.1f, bool this_object_as_point = true) {
            if(this_object_as_point) {
                math::Vector<float> highest = math::Vector<float>::highest_on_y({collider->m_position, collider->m_size}, true);
                math::Vector<float> lowest = math::Vector<float>::lowest_on_y({collider->m_position,collider-> m_size}, true);

                math::Vector<float> high_x = math::Vector<float>::highest_on_x({collider->m_position, collider->m_size}, true);
                math::Vector<float> low_x = math::Vector<float>::lowest_on_x({collider->m_position, collider->m_size}, true);

                float length = collider->m_position.distance(collider->m_size);

                float alpha = abs(collider->m_position.x - collider->m_size.x) / abs(length);

                float highest_point_length = abs(highest.x - m_position.x);

                math::Vector<float> hit_point = math::Vector<float>(m_position.x, highest.y - sqrt(pow(highest_point_length / alpha, 2) - pow(highest_point_length, 2)));
            
                if(m_position > hit_point - error_margin && m_position < hit_point + error_margin && m_position.y <= highest.y && m_position.y >= lowest.y && hit_point.x >= low_x.x && hit_point.x <= high_x.x) {
                    return true;
                }
            }
            else {
                math::Vector<float> highest = math::Vector<float>::highest_on_y({m_position, m_size}, true);
                math::Vector<float> lowest = math::Vector<float>::lowest_on_y({m_position, m_size}, true);

                math::Vector<float> high_x = math::Vector<float>::highest_on_x({m_position, m_size}, true);
                math::Vector<float> low_x = math::Vector<float>::lowest_on_x({m_position, m_size}, true);

                float length = m_position.distance(m_size);

                float alpha = abs(m_position.x - m_size.x) / abs(length);

                float highest_point_length = abs(highest.x - collider->m_position.x);

                math::Vector<float> hit_point = math::Vector<float>(collider->m_position.x, highest.y - sqrt(pow(highest_point_length / alpha, 2) - pow(highest_point_length, 2)));
            
                if(collider->m_position > hit_point - error_margin && collider->m_position < hit_point + error_margin && collider->m_position.y <= highest.y && collider->m_position.y >= lowest.y && hit_point.x >= low_x.x && hit_point.x <= high_x.x) {
                    return true;
                }
            }

            return false;
        }

        bool CheckLine2DToLine2D(Collider* collider) {
            math::Vector<float> collider_high_y = math::Vector<float>::highest_on_y({collider->m_position, collider->m_size}, true);
            math::Vector<float> collider_low_y = math::Vector<float>::lowest_on_y({collider->m_position, collider->m_size}, true);

            math::Vector<float> collider_high_x = math::Vector<float>::highest_on_x({collider->m_position, collider->m_size}, true);
            math::Vector<float> collider_low_x = math::Vector<float>::lowest_on_x({collider->m_position, collider->m_size}, true);

            math::Vector<float> this_high_y = math::Vector<float>::highest_on_y({m_position, m_size}, true);
            math::Vector<float> this_low_y = math::Vector<float>::lowest_on_y({m_position, m_size}, true);

            math::Vector<float> this_high_x = math::Vector<float>::highest_on_x({ m_position, m_size}, true);
            math::Vector<float> this_low_x = math::Vector<float>::lowest_on_x({m_position, m_size}, true);

            Vector<float> intersection_point_2d = Vector<float>(((collider->m_position.x * collider->m_size.y - collider->m_position.y * collider->m_size.x) * (m_position.x - m_size.x) - (collider->m_position.x - collider->m_size.x) * (m_position.x * m_size.y - m_position.y * m_size.x)) / ((collider->m_position.x - collider->m_size.x) * (m_position.y - m_size.y) - (collider->m_position.y - collider->m_size.y) * (m_position.x - m_size.x)),
                                                                ((collider->m_position.x * collider->m_size.y - collider->m_position.y * collider->m_size.x) * (m_position.y - m_size.y) - (collider->m_position.y - collider->m_size.y) * (m_position.x * m_size.y - m_position.y * m_size.x)) / ((collider->m_position.x - collider->m_size.x) * (m_position.y - m_size.y) - (collider->m_position.y - collider->m_size.y) * (m_position.x - m_size.x)));
        
            if(intersection_point_2d.y <= collider_high_y.y && intersection_point_2d.y >= collider_low_y.y && intersection_point_2d.x >= collider_low_x.x && intersection_point_2d.x <= collider_high_x.x &&
                intersection_point_2d.y <= this_high_y.y && intersection_point_2d.y >= this_low_y.y && intersection_point_2d.x >= this_low_x.x && intersection_point_2d.x <= this_high_x.x) {
                return true;
            }

            return false;
        }

        bool CheckRayToPlane(Collider* collider, float error_margin = 0.01f, bool this_object_as_point = true) {
            if(this_object_as_point) {
                Vector<float> high_x = Vector<float>::highest_on_x({collider->m_position, collider->m_size, collider->m_third_point}, true);
                Vector<float> high_y = Vector<float>::highest_on_y({collider->m_position, collider->m_size, collider->m_third_point}, true);
                Vector<float> high_z = Vector<float>::highest_on_z({collider->m_position, collider->m_size, collider->m_third_point}, true);

                Vector<float> low_x = Vector<float>::lowest_on_x({collider->m_position, collider->m_size, collider->m_third_point}, true);
                Vector<float> low_y = Vector<float>::lowest_on_y({collider->m_position, collider->m_size, collider->m_third_point}, true);
                Vector<float> low_z = Vector<float>::lowest_on_z({collider->m_position, collider->m_size, collider->m_third_point}, true);

                Vector<float> normal = math::find_plane_normal(collider->m_position, collider->m_size, collider->m_third_point);
                
                float D = m_position.dot(m_size);

                float numer = normal.dot(m_position);
                float denom = normal.dot(m_size);

                float t = -(numer / denom);

                Vector<float> collision_point = m_position + m_size * t;

                if(collision_point < high_x && collision_point < high_z && collision_point < high_y && collision_point > low_x && collision_point > low_z && collision_point > low_y && m_position > collision_point - error_margin && m_position < collision_point + error_margin) {
                    return true;
                }

                if(m_position > collision_point - error_margin && m_position < collision_point + error_margin) {
                    return true;
                }
            }
            else {
                Vector<float> high_x = Vector<float>::highest_on_x({m_position, m_size, m_third_point}, true);
                Vector<float> high_y = Vector<float>::highest_on_y({m_position, m_size, m_third_point}, true);
                Vector<float> high_z = Vector<float>::highest_on_z({m_position, m_size, m_third_point}, true);

                Vector<float> low_x = Vector<float>::lowest_on_x({m_position, m_size, m_third_point}, true);
                Vector<float> low_y = Vector<float>::lowest_on_y({m_position, m_size, m_third_point}, true);
                Vector<float> low_z = Vector<float>::lowest_on_z({m_position, m_size, m_third_point}, true);

                Vector<float> normal = math::find_plane_normal(m_position, m_size, m_third_point);
                
                float D = m_position.dot(collider->m_size);

                float numer = normal.dot(collider->m_position);
                float denom = normal.dot(collider->m_size);

                float t = -(numer / denom);

                Vector<float> collision_point = collider->m_position + collider->m_size * t;

                if(collision_point < high_x && collision_point < high_z && collision_point < high_y && collision_point > low_x && collision_point > low_z && collision_point > low_y && collider->m_position > collision_point - error_margin && collider->m_position < collision_point + error_margin) {
                    return true;
                }

                if(collider->m_position > collision_point - error_margin && collider->m_position < collision_point + error_margin) {
                    return true;
                }
            }

            return false;
        }

        /**
         * @brief Point to line 2D collisions
         * 
         * @param collider 
         * @param error_margin 
         * @param this_object_as_point 
         * @return true 
         * @return false 
         */
        /*bool CheckPointToPlane(Collider* collider, float error_margin = 0.1f, bool this_object_as_point = true) {
            if(this_object_as_point) {
                math::Vector<float> highest = math::Vector<float>::highest_on_y({collider->m_position, collider->m_size, collider->m_third_point}, true);
                math::Vector<float> lowest = math::Vector<float>::lowest_on_y({collider->m_position,collider-> m_size, collider->m_third_point}, true);

                math::Vector<float> high_x = math::Vector<float>::highest_on_x({collider->m_position, collider->m_size, collider->m_third_point}, true);
                math::Vector<float> low_x = math::Vector<float>::lowest_on_x({collider->m_position, collider->m_size, collider->m_third_point}, true);

                float lengths[3] = {collider->m_position.distance(collider->m_size), collider->m_size.distance(collider->m_third_point), collider->m_position.distance(collider->m_third_point)};

                float alphas[3] = {abs(collider->m_position.x - collider->m_size.x) / abs(lengths[0]), abs(collider->m_size.x - collider->m_third_point.x) / abs(lengths[1]), abs(collider->m_position.x - collider->m_third_point.x) / abs(lengths[2])};

                float highest_point_length_x = abs(highest.x - m_position.x);
                float highest_point_length_z = abs(highest.z - m_position.z);

                math::Vector<float> hit_point = math::Vector<float>(m_position.x, highest.y - sqrt(pow(highest_point_length_x / alphas[0], 2) - pow(highest_point_length_x, 2)), m_position.z);
            
                if(m_position > hit_point - error_margin && m_position < hit_point + error_margin && m_position.y <= highest.y && m_position.y >= lowest.y && hit_point.x >= low_x.x && hit_point.x <= high_x.x) {
                    return true;
                }
            }
            else {
                math::Vector<float> highest = math::Vector<float>::highest_on_y({m_position, m_size}, true);
                math::Vector<float> lowest = math::Vector<float>::lowest_on_y({m_position, m_size}, true);

                math::Vector<float> high_x = math::Vector<float>::highest_on_x({m_position, m_size}, true);
                math::Vector<float> low_x = math::Vector<float>::lowest_on_x({m_position, m_size}, true);

                float length = m_position.distance(m_size);

                float alpha = abs(m_position.x - m_size.x) / abs(length);

                float highest_point_length = abs(highest.x - collider->m_position.x);

                math::Vector<float> hit_point = math::Vector<float>(collider->m_position.x, highest.y - sqrt(pow(highest_point_length / alpha, 2) - pow(highest_point_length, 2)));
            
                if(collider->m_position > hit_point - error_margin && collider->m_position < hit_point + error_margin && collider->m_position.y <= highest.y && collider->m_position.y >= lowest.y && hit_point.x >= low_x.x && hit_point.x <= high_x.x) {
                    return true;
                }
            }

            return false;
        }*/
    };

    class ColliderHandler  {
    private:
        std::unordered_map<std::string, Collider> m_collider_map;

    public:
        ColliderHandler() {}

        /**
         * @brief Add collider to map
         * 
         * @param name 
         */
        void AddCollider(std::string name = "__Collider__") {
            if(name == "__Collider__") {
                name += std::to_string(m_collider_map.size());
            }

            m_collider_map.insert(std::make_pair(name, Collider()));
        }

        /**
         * @brief Erase Collider from map
         * 
         * @param name 
         */
        void EraseCollider(std::string name) {
            m_collider_map.erase(name);
        }

        /**
         * @brief Get the Collider Ptr object
         * 
         * @param name 
         * @return Collider* 
         */
        Collider* GetColliderPtr(std::string name) { return &m_collider_map[name]; }

        /**
         * @brief Checks collidion all collidion with collider pulled by name and return collider names that it collides with
         * 
         * @param name 
         * @return std::vector<std::string> 
         */
        std::vector<std::string> CheckWithEverythingLine2DToLine2D(std::string name) {
            std::vector<std::string> collider_names;
            
            for(auto collider : m_collider_map) {
                if(collider.first != name) {
                    if(m_collider_map[name].CheckLine2DToLine2D(&collider.second)) {
                        collider_names.push_back(collider.first);
                    }
                }
            }

            return collider_names;
        }

        /**
         * @brief Get the Collider Map Ptr object
         * 
         * @return std::unordered_map<std::string, Collider>* 
         */
        std::unordered_map<std::string, Collider>* GetColliderMapPtr() { return &m_collider_map; }

        std::unordered_map<std::string, Collider>::iterator begin() { return m_collider_map.begin(); }
        std::unordered_map<std::string, Collider>::iterator end() { return m_collider_map.end(); }
        std::unordered_map<std::string, Collider>::const_iterator cbegin() { return m_collider_map.cbegin(); }
        std::unordered_map<std::string, Collider>::const_iterator cend() { return m_collider_map.cend(); }
    };
}

#endif
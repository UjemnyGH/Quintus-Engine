#pragma once
#ifndef __P_COLLIDER_
#define __P_COLLIDER_

#include <glm/glm.hpp>
#include "../Core/c_algorithms.hpp"

namespace qe {
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

        bool CheckPointToPlane(Collider* collider, float error_margin = 0.1f, bool this_object_as_point = true, math::Vector<float> direction = math::Vector<float>(0.0f, -1.0f, 0.0f)) {
            if(this_object_as_point) {
                
            }
            else {

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

        // TODO: Point to plane collisions
    };
}

#endif
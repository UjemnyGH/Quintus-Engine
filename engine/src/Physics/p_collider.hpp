#pragma once
#ifndef __P_COLLIDER_
#define __P_COLLIDER_

#include <glm/glm.hpp>
#include "../Core/c_algorithms.hpp"

namespace qe {
    class Collider {
    public:
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_size = glm::vec3(1.0f);
        glm::vec3 m_third_point = glm::vec3(0.0f);

        /**
         * @brief It use m_size and m_position as references
         * 
         * @param collider 
         * @return true 
         * @return false 
         */
        bool CheckAABBToAABB(Collider* collider) {
            glm::vec3 Position1 = m_position - m_size;
            glm::vec3 Scale1 = m_size * glm::vec3(2.0f, 2.0f, 2.0f);

            glm::vec3 Position2 = collider->m_position - collider->m_size;
            glm::vec3 Scale2 = collider->m_size * glm::vec3(2.0f, 2.0f, 2.0f);

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

        // TODO: Point to plane collisions
        bool CheckPointToPlane(Collider* collider, bool this_objects_as_aabb = true) {
            bool collX, collY, collZ;
            
            if(this_objects_as_aabb) {
                //  closest x, y and z point to dot                
                glm::vec3 closest = glm::vec3(closestVector(m_position.x, {collider->m_position.x, collider->m_size.x, collider->m_third_point.x}), 
                                                closestVector(m_position.y, {collider->m_position.y, collider->m_size.y, collider->m_third_point.y}), 
                                                closestVector(m_position.z, {collider->m_position.z, collider->m_size.z, collider->m_third_point.z}));

                //glm::vec3 dist = glm::distance(closest, m_position);

                // lenghts between 2 points on x, y and z
                glm::vec3 lenghts[3] = { glm::vec3(abs(collider->m_position.x - collider->m_size.x), abs(collider->m_position.y - collider->m_size.y), abs(collider->m_position.z - collider->m_size.z)),
                                            glm::vec3(abs(collider->m_size.x - collider->m_third_point.x), abs(collider->m_size.y - collider->m_third_point.y), abs(collider->m_size.z - collider->m_third_point.z)),
                                            glm::vec3(abs(collider->m_position.x - collider->m_third_point.x), abs(collider->m_position.y - collider->m_third_point.y), abs(collider->m_position.z - collider->m_third_point.z))};

                // Pitagoras of line in every direction
                glm::vec3 lengths_in_directions = glm::vec3(sqrtf((lenghts[0].y * lenghts[0].y) + (lenghts[0].z * lenghts[0].z)), sqrtf((lenghts[1].x * lenghts[1].x) + (lenghts[1].z * lenghts[1].z)), sqrtf((lenghts[2].y * lenghts[2].y) + (lenghts[2].x * lenghts[2].x)));

                // sin alpha later used to get hypotenuse of triangle
                glm::vec3 sin_alpha = glm::vec3(lenghts[0].x / lengths_in_directions.x, lenghts[1].y / lengths_in_directions.y, lenghts[2].z / lengths_in_directions.z);
                
                // hypotenuse on every dimmension
                glm::vec3 lenghts_of_c = glm::vec3(abs(m_position.x - closest.x) / sin_alpha.x, abs(m_position.y - closest.y) / sin_alpha.y, abs(m_position.z - closest.z) / sin_alpha.z);
                
                // predicted hitpoint
                glm::vec3 hit_point = glm::vec3(closest.x - lenghts_of_c.x, closest.y - lenghts_of_c.y, closest.z - lenghts_of_c.z);
            }
            else {
                glm::vec3 closest = glm::vec3(closestVector(collider->m_position.x, {m_position.x, m_size.x, m_third_point.x}), 
                                                closestVector(collider->m_position.y, {m_position.y, m_size.y, m_third_point.y}), 
                                                closestVector(collider->m_position.z, {m_position.z, m_size.z, m_third_point.z}));
            }
        }
    };
}

#endif
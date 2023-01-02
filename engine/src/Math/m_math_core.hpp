#pragma once
#ifndef __M_MATH_CORE_
#define __M_MATH_CORE_

#include "m_vector.hpp"
//#include "m_quaterion.hpp"

namespace qe {
    namespace math {
        /**
         * @brief It clamp value between low and high
         * 
         * @tparam T 
         * @param val 
         * @param low 
         * @param high 
         * @return T 
         */
        template<class T>
        T clamp(T val, T low, T high) {
            if(val > low && val < high) {
                return val;
            }
            else if(val < low) {
                return low;
            }
            else if(val > high) {
                return high;
            }

            return val;
        }

        /**
         * @brief original fast inverse sqrt from Quake III Arena slightly modified
         * 
         * @param number 
         * @return float 
         */
        template<typename T>
        T Q_invsqrt( T number )
        {
            long i;
            T x2, y;
            const T threehalfs = static_cast<T>(1.5);

            x2 = number * static_cast<T>(0.5);
            y  = number;
            i  = * ( long * ) &y;                       // evil floating point bit level hacking
            i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
            y  = * ( float * ) &i;
            y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
            // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

            return y;
        }

        /**
         * @brief Convert deegrees to radians
         * 
         * @tparam T 
         * @param degrees 
         * @return T 
         */
        template<typename T>
        T to_radians(T degrees) {
            return degrees * static_cast<T>(M_PI / 180.0);
        }

        /**
         * @brief Convert radians to deegrees
         * 
         * @tparam T 
         * @param degrees 
         * @return T 
         */
        template<typename T>
        T to_degrees(T radians) {
            return radians * static_cast<T>(180.0 / M_PI);
        }
    }
}

#endif
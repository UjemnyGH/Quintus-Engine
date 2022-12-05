#pragma once
#ifndef __M_VECTOR_
#define __M_VECTOR_

#include <cmath>

namespace qe {
        namespace math {

        /**
         * @brief Absolute value
         * 
         * @tparam T 
         * @param x 
         * @return T 
         */
        template<typename T>
        T absolute(T x) {
            return x >= static_cast<T>(0) ? x : -x;
        }

        // TODO: Dynamic Matrix Struct

        template<typename T>
        struct Vector {
            union { T x, r, s; };
            union { T y, g, t; };
            union { T z, b, p; };
            union { T w, a, q; };

            constexpr Vector<T>() { x = y = z = w = static_cast<T>(0); }
            constexpr Vector<T>(T _x, T _y, T _z = 0.0f, T _w = 0.0f) { x = _x; y = _y; z = _z; w = _w; }
            constexpr Vector<T>(T val) { x = y = z = w = val; }
            constexpr Vector<T>(Vector<T> const &vec) { x = vec.x; y = vec.y; z = vec.z; w = vec.w; }

            constexpr Vector<T> operator=(Vector<T> const &vec) { x = vec.x; y = vec.y; z = vec.z; w = vec.w; return *this; }

            constexpr Vector<T> operator+(Vector<T> const &vec) { return Vector<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
            constexpr Vector<T> operator-(Vector<T> const &vec) { return Vector<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
            constexpr Vector<T> operator*(Vector<T> const &vec) { return Vector<T>(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
            constexpr Vector<T> operator/(Vector<T> const &vec) { return Vector<T>(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }

            constexpr Vector<T> operator+(T scalar) { return Vector<T>(x + scalar, y + scalar, z + scalar, w + scalar); }
            constexpr Vector<T> operator-(T scalar) { return Vector<T>(x - scalar, y - scalar, z - scalar, w - scalar); }
            constexpr Vector<T> operator*(T scalar) { return Vector<T>(x * scalar, y * scalar, z * scalar, w * scalar); }
            constexpr Vector<T> operator/(T scalar) { return Vector<T>(x / scalar, y / scalar, z / scalar, w / scalar); }

            constexpr Vector<T> operator+=(Vector<T> const &vec) { *this = *this + vec; return *this; }
            constexpr Vector<T> operator-=(Vector<T> const &vec) { *this = *this - vec; return *this; }
            constexpr Vector<T> operator*=(Vector<T> const &vec) { *this = *this * vec; return *this; }
            constexpr Vector<T> operator/=(Vector<T> const &vec) { *this = *this / vec; return *this; }

            constexpr Vector<T> operator+=(T scalar) { *this = *this + Vector<T>(scalar); return *this; }
            constexpr Vector<T> operator-=(T scalar) { *this = *this - Vector<T>(scalar); return *this; }
            constexpr Vector<T> operator*=(T scalar) { *this = *this * Vector<T>(scalar); return *this; }
            constexpr Vector<T> operator/=(T scalar) { *this = *this / Vector<T>(scalar); return *this; }

            constexpr Vector<T> operator++() { return Vector<T>(x + static_cast<T>(1), y + static_cast<T>(1), z + static_cast<T>(1), w + static_cast<T>(1)); }
            constexpr Vector<T> operator--() { return Vector<T>(x - static_cast<T>(1), y - static_cast<T>(1), z - static_cast<T>(1), w - static_cast<T>(1)); }

            constexpr Vector<T> operator%(Vector<T> const &vec) { return Vector<T>(x % vec.x, y % vec.y, z % vec.z, w % vec.w); }
            constexpr Vector<T> operator&(Vector<T> const &vec) { return Vector<T>(x & vec.x, y & vec.y, z & vec.z, w & vec.w); }
            constexpr Vector<T> operator|(Vector<T> const &vec) { return Vector<T>(x | vec.x, y | vec.y, z | vec.z, w | vec.w); }
            constexpr Vector<T> operator^(Vector<T> const &vec) { return Vector<T>(x ^ vec.x, y ^ vec.y, z ^ vec.z, w ^ vec.w); }
            constexpr Vector<T> operator<<(Vector<T> const &vec) { return Vector<T>(x << vec.x, y << vec.y, z << vec.z, w << vec.w); }
            constexpr Vector<T> operator>>(Vector<T> const &vec) { return Vector<T>(x >> vec.x, y >> vec.y, z >> vec.z, w >> vec.w); }

            constexpr Vector<T> operator%(T scalar) { return Vector<T>(x % scalar, y % scalar, z % scalar, w % scalar); }
            constexpr Vector<T> operator&(T scalar) { return Vector<T>(x & scalar, y & scalar, z & scalar, w & scalar); }
            constexpr Vector<T> operator|(T scalar) { return Vector<T>(x | scalar, y | scalar, z | scalar, w | scalar); }
            constexpr Vector<T> operator^(T scalar) { return Vector<T>(x ^ scalar, y ^ scalar, z ^ scalar, w ^ scalar); }
            constexpr Vector<T> operator<<(T scalar) { return Vector<T>(x << scalar, y << scalar, z << scalar, w << scalar); }
            constexpr Vector<T> operator>>(T scalar) { return Vector<T>(x >> scalar, y >> scalar, z >> scalar, w >> scalar); }


            constexpr Vector<T> operator%=(Vector<T> const &vec) { *this = *this % vec; return *this;}
            constexpr Vector<T> operator&=(Vector<T> const &vec) { *this = *this & vec; return *this;}
            constexpr Vector<T> operator|=(Vector<T> const &vec) { *this = *this | vec; return *this;}
            constexpr Vector<T> operator^=(Vector<T> const &vec) { *this = *this ^ vec; return *this;}
            constexpr Vector<T> operator<<=(Vector<T> const &vec) { *this = *this << vec; return *this;}
            constexpr Vector<T> operator>>=(Vector<T> const &vec) { *this = *this >> vec; return *this;}

            constexpr Vector<T> operator%=(T scalar) { *this = *this % scalar; return *this;}
            constexpr Vector<T> operator&=(T scalar) { *this = *this & scalar; return *this;}
            constexpr Vector<T> operator|=(T scalar) { *this = *this | scalar; return *this;}
            constexpr Vector<T> operator^=(T scalar) { *this = *this ^ scalar; return *this;}
            constexpr Vector<T> operator<<=(T scalar) { *this = *this << scalar; return *this;}
            constexpr Vector<T> operator>>=(T scalar) { *this = *this >> scalar; return *this;}

            constexpr bool operator==(Vector<T> const &vec) { return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w); }
            constexpr bool operator!=(Vector<T> const &vec) { return (x != vec.x) && (y != vec.y) && (z != vec.z) && (w != vec.w); }
            constexpr bool operator>(Vector<T> const &vec) { return (x > vec.x) && (y > vec.y) && (z > vec.z) && (w > vec.w); }
            constexpr bool operator<(Vector<T> const &vec) { return (x < vec.x) && (y < vec.y) && (z < vec.z) && (w < vec.w); }
            constexpr bool operator>=(Vector<T> const &vec) { return (x >= vec.x) && (y >= vec.y) && (z >= vec.z) && (w >= vec.w); }
            constexpr bool operator<=(Vector<T> const &vec) { return (x <= vec.x) && (y <= vec.y) && (z <= vec.z) && (w <= vec.w); }

            /**
             * @brief Cross function
             * 
             * @param vec 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> cross(Vector<T> const &vec) { return Vector<T>(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }
            
            /**
             * @brief Static cross function
             * 
             * @param vec 
             * @param vec2 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> cross(Vector<T> const &vec, Vector<T> const &vec2) { return Vector<T>(vec.y * vec2.z - vec.z * vec2.y, vec.z * vec2.x - vec.x * vec2.z, vec.x * vec2.y - vec.y * vec2.x); }
            
            /**
             * @brief Dot function
             * 
             * @param vec 
             * @return constexpr T 
             */
            constexpr T dot(Vector<T> const &vec) { return x * vec.x + y * vec.y + z * vec.z + w * vec.w; }

            /**
             * @brief Static dot function
             * 
             * @param vec 
             * @param vec2 
             * @return constexpr T 
             */
            constexpr static T dot(Vector<T> const &vec, Vector<T> const &vec2) { return vec2.x * vec.x + vec2.y * vec.y + vec2.z * vec.z + vec2.w * vec.w; }
            
            /**
             * @brief Length function
             * 
             * @return constexpr T 
             */
            constexpr T length() { return sqrt(dot(*this)); }

            /**
             * @brief Static length function
             * 
             * @param vec 
             * @return constexpr T 
             */
            constexpr static T length(Vector<T> const &vec) { return sqrt(Vector<T>::dot(vec, vec)); }

            /**
             * @brief Normalize function
             * 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> normalize() { return *this * (static_cast<T>(1) / sqrt(dot(*this))); }

            /**
             * @brief Static normalize function
             * 
             * @param vec 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> normalize(Vector<T> const &vec) { return vec * (static_cast<T>(1) / sqrt(Vector<T>::dot(vec, vec))); }

            /**
             * @brief Distance function
             * 
             * @param vec 
             * @return constexpr T 
             */
            constexpr T distance(Vector<T> const &vec) { return Vector<T>::length(*this - vec); } 

            /**
             * @brief Static distance function
             * 
             * @param vec 
             * @param vec2 
             * @return constexpr T 
             */
            constexpr static T distance(Vector<T> &vec, Vector<T> &vec2) { return Vector<T>::length(vec2 - vec); } 

            /**
             * @brief Inverts vector
             * 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> invert() { return Vector<T>(-x, -y, -z, -w); }

            /**
             * @brief Static vector invert
             * 
             * @param vec 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> invert(Vector<T> const &vec) { return Vector<T>(-vec.x, -vec.y, -vec.z, -vec.w); }

            /**
             * @brief Absolute value of vector
             * 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> abs() { return Vector<T>(qe::math::absolute(x), qe::math::absolute(y), qe::math::absolute(z), qe::math::absolute(w)); }

            /**
             * @brief Static absolute value of vector
             * 
             * @param vec 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> abs(Vector<T> const &vec) { return Vector<T>(qe::math::absolute(vec.x), qe::math::absolute(vec.y), qe::math::absolute(vec.z), qe::math::absolute(vec.w)); }

            /**
             * @brief Get closest vector to heap of vectors
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> closest(std::vector<Vector<T>> const &vecs) { 
                Vector<T> result = vecs[0];
                
                for(Vector<T> vec : vecs) {
                    if(Vector<T>::abs(*this - vec) < Vector<T>::abs(*this - result)) {
                        result = vec;
                    }
                }

                return result;
            }

            /**
             * @brief Static get closest vector to heap of vectors
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> closest(Vector<T> const &vec_val, std::vector<Vector<T>> const &vecs) { 
                Vector<T> result = vecs[0];
                
                for(Vector<T> vec : vecs) {
                    if(Vector<T>::abs(vec_val - vec) < Vector<T>::abs(vec_val - result)) {
                        result = vec;
                    }
                }

                return result;
            }

            /**
             * @brief Return highest y vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> highest_on_y(std::vector<Vector<T>> const &vecs) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.y > result.y) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return highest y vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> highest_on_y(std::vector<Vector<T>> const &vecs, bool st) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.y > result.y) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return lowest y vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> lowest_on_y(std::vector<Vector<T>> const &vecs) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.y < result.y) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return lowest y vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> lowest_on_y(std::vector<Vector<T>> const &vecs, bool st) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.y < result.y) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return highest x vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> highest_on_x(std::vector<Vector<T>> const &vecs) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.x > result.x) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return highest x vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> highest_on_x(std::vector<Vector<T>> const &vecs, bool st) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.x > result.x) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return lowest x vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> lowest_on_x(std::vector<Vector<T>> const &vecs) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.x < result.x) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return lowest x vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> lowest_on_x(std::vector<Vector<T>> const &vecs, bool st) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.x < result.x) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return highest z vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> highest_on_z(std::vector<Vector<T>> const &vecs) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.z > result.z) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return highest z vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> highest_on_z(std::vector<Vector<T>> const &vecs, bool st) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.z > result.z) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return lowest z vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> lowest_on_z(std::vector<Vector<T>> const &vecs) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.z < result.z) {
                        result = v;
                    }
                }

                return result;
            }

            /**
             * @brief Return lowest z vector
             * 
             * @param vecs 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> lowest_on_z(std::vector<Vector<T>> const &vecs, bool st) {
                Vector<T> result = vecs[0];

                for(Vector<T> v : vecs) {
                    if(v.z < result.z) {
                        result = v;
                    }
                }

                return result;
            }
        };

        /**
         * @brief Returns normal from 3 points(plane)
         * 
         * @tparam T 
         * @param a 
         * @param b 
         * @param c 
         * @param first_to_last 
         * @return constexpr Vector<T> 
         */
        template<typename T>
        constexpr Vector<T> find_plane_normal(Vector<T> a, Vector<T> b, Vector<T> c) {
            Vector<float> V1 = b - a;
            Vector<float> V2 = c - a;

            return V1.cross(V2);
        }
    }

    using math::Vector;
}

#endif
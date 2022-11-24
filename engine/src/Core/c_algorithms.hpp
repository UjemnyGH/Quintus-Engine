#pragma once
#ifndef __C_ALGORITHMS_
#define __C_ALGORITHMS_

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace qe {
    /**
     * @brief It swaps 2 values
     * 
     * @tparam T 
     * @param a 
     * @param b 
     */
    template<class T>
    void swapValues(T* a, T* b) {
        T t = *a;
        *a = *b;
        *b = t;
    }

    /**
     * @brief Quick sort algorithm
     *
     * @tparam T typename
     * @tparam Arr data to sort
     * @param _begin first index
     * @param _end last index
     * @param arr data to sort
     * @return T* 
     */
    template<class T>
    void quickSort(T *arr, size_t _begin, size_t _end) {
        if (_begin < _end) {
            T pivot = arr[_end];
            T i = (_begin - 1);

            for (int j = _begin; j <= _end; j++)
            {
                if (arr[j] < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }

            swap(&arr[i + 1], &arr[_end]);

            T part = i + 1;

            quickSort(arr, _begin, part - 1);
            quickSort(arr, part + 1, _end);
        }
    }

    /**
     * @brief Quick sort algorithm
     *
     * @tparam T typename
     * @tparam Arr data to sort
     * @param _begin first index
     * @param _end last index
     * @param arr data to sort
     * @return T* 
     */
    template<class T>
    void quickSort(std::vector<T>* arr, size_t _begin, size_t _end) {
        if (_begin < _end) {
            T pivot = arr[_end];
            T i = (_begin - 1);

            for (int j = _begin; j <= _end; j++)
            {
                if (arr[j] < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }

            swap(&arr[i + 1], &arr[_end]);

            T part = i + 1;

            quickSort(arr, _begin, part - 1);
            quickSort(arr, part + 1, _end);
        }
    }

    /**
     * @brief Quick sort algorithm
     *
     * @tparam T typename
     * @tparam Arr data to sort
     * @param _begin first index
     * @param _end last index
     * @param arr data to sort
     * @return T* 
     */
    template<class T>
    std::vector<T> quickSort(std::vector<T> arr, size_t _begin, size_t _end) {
        if (_begin < _end) {
            T pivot = arr[_end];
            T i = (_begin - 1);

            for (int j = _begin; j <= _end; j++)
            {
                if (arr[j] < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }

            swap(&arr[i + 1], &arr[_end]);

            T part = i + 1;

            quickSort(arr, _begin, part - 1);
            quickSort(arr, part + 1, _end);
        }
        else {
            return arr;
        }
    }

    /**
     * @brief Binary search algorithm
     *
     * @tparam T typename
     * @param _first first element
     * @param _last last element
     * @param arr data to research (sorted!!!!!!)
     * @param value value to be founded bt address
     * @return size_t
     */
    template<class T>
    size_t binSearch(size_t _first, size_t _last, std::vector<T> arr, T value) {
        if (_last >= _first) {
            int pos = _first + (_last - _first) / 2;

            if (arr[pos] == value)
                return pos;
            if (arr[pos] > value)
                return binSearch(_first, pos - 1, arr, value);

            return binSearch(pos + 1, _last, arr, value);
        }

        return -1;
    }

    /**
     * @brief Quick and dirty search algorithm that search from middle to left and right at the same time
     * 
     * @tparam T 
     * @param arr 
     * @param value 
     * @param optional_return_val 
     * @return size_t 
     */
    template<class T>
    size_t Search(std::vector<T> arr, T value, size_t optional_return_val = -1) {
        size_t beg = arr.size() / 2;
        size_t end = arr.size() / 2;

        for(size_t i = 0; i < arr.size() / 2; i++) {
            if(arr[end] == value) {
                return end;
            }

            if(arr[beg] == value) {
                return beg;
            }
            
            beg--;
            end++;
        }

        return optional_return_val;
    }

    /**
     * @brief Returns closest of 2 values
     * 
     * @tparam T 
     * @param val 
     * @param a 
     * @param b 
     * @return T 
     */
    template<typename T>
    T closest(T val, T a, T b) {
        return abs(val - a) > abs(val - b) ? b : a;
    }

    /**
     * @brief Return closest from vector
     * 
     * @tparam T 
     * @param val 
     * @param data 
     * @return T 
     */
    template<typename T>
    T closestVector(T val, std::vector<T> data) {
        uint32_t closest_index = 0;

        for(uint32_t i = 0; i < data.size(); i++) {
            if(abs(val - data[i]) < abs(val - data[closest_index])) {
                closest_index = i;
            }
        }

        return data[closest_index];
    }

    /**
     * @brief Return index of closest from vector
     * 
     * @tparam T 
     * @param val 
     * @param data 
     * @return uint32_t 
     */
    template<typename T>
    uint32_t closestVectorIndex(T val, std::vector<T> data) {
        uint32_t closest_index = 0;

        for(uint32_t i = 0; i < data.size(); i++) {
            if(abs(val - data[i]) < abs(val - data[closest_index])) {
                closest_index = i;
            }
        }

        return closest_index;
    }

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
         * @brief Cross function
         * 
         * @param a 
         * @param b 
         * @return glm::vec3 
         */
        glm::vec3 cross(glm::vec3 a, glm::vec3 b) {
            return glm::vec3((a.y * b.z) - (a.z * b.y), (a.x * b.z) - (a.z * b.x), (a.y * b.x) - (a.x * b.y));
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

            constexpr Vector<T> operator=(Vector<T> const &vec) { this->x = vec.x; this->y = vec.y; this->z = vec.z; this->w = vec.w; return *this; }

            constexpr Vector<T> operator+(Vector<T> const &vec) { return Vector<T>(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w); }
            constexpr Vector<T> operator-(Vector<T> const &vec) { return Vector<T>(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w); }
            constexpr Vector<T> operator*(Vector<T> const &vec) { return Vector<T>(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w); }
            constexpr Vector<T> operator/(Vector<T> const &vec) { return Vector<T>(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w); }

            constexpr Vector<T> operator+(T scalar) { return Vector<T>(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar); }
            constexpr Vector<T> operator-(T scalar) { return Vector<T>(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar); }
            constexpr Vector<T> operator*(T scalar) { return Vector<T>(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar); }
            constexpr Vector<T> operator/(T scalar) { return Vector<T>(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar); }

            constexpr Vector<T> operator+=(Vector<T> const &vec) { *this = *this + vec; return *this; }
            constexpr Vector<T> operator-=(Vector<T> const &vec) { *this = *this - vec; return *this; }
            constexpr Vector<T> operator*=(Vector<T> const &vec) { *this = *this * vec; return *this; }
            constexpr Vector<T> operator/=(Vector<T> const &vec) { *this = *this / vec; return *this; }

            constexpr Vector<T> operator+=(T scalar) { *this = *this + Vector<T>(scalar); return *this; }
            constexpr Vector<T> operator-=(T scalar) { *this = *this - Vector<T>(scalar); return *this; }
            constexpr Vector<T> operator*=(T scalar) { *this = *this * Vector<T>(scalar); return *this; }
            constexpr Vector<T> operator/=(T scalar) { *this = *this / Vector<T>(scalar); return *this; }

            constexpr Vector<T> operator++() { return Vector<T>(this->x + static_cast<T>(1), this->y + static_cast<T>(1), this->z + static_cast<T>(1), this->w + static_cast<T>(1)); }
            constexpr Vector<T> operator--() { return Vector<T>(this->x - static_cast<T>(1), this->y - static_cast<T>(1), this->z - static_cast<T>(1), this->w - static_cast<T>(1)); }

            constexpr Vector<T> operator%(Vector<T> const &vec) { return Vector<T>(this->x % vec.x, this->y % vec.y, this->z % vec.z, this->w % vec.w); }
            constexpr Vector<T> operator&(Vector<T> const &vec) { return Vector<T>(this->x & vec.x, this->y & vec.y, this->z & vec.z, this->w & vec.w); }
            constexpr Vector<T> operator|(Vector<T> const &vec) { return Vector<T>(this->x | vec.x, this->y | vec.y, this->z | vec.z, this->w | vec.w); }
            constexpr Vector<T> operator^(Vector<T> const &vec) { return Vector<T>(this->x ^ vec.x, this->y ^ vec.y, this->z ^ vec.z, this->w ^ vec.w); }
            constexpr Vector<T> operator<<(Vector<T> const &vec) { return Vector<T>(this->x << vec.x, this->y << vec.y, this->z << vec.z, this->w << vec.w); }
            constexpr Vector<T> operator>>(Vector<T> const &vec) { return Vector<T>(this->x >> vec.x, this->y >> vec.y, this->z >> vec.z, this->w >> vec.w); }

            constexpr Vector<T> operator%(T scalar) { return Vector<T>(this->x % scalar, this->y % scalar, this->z % scalar, this->w % scalar); }
            constexpr Vector<T> operator&(T scalar) { return Vector<T>(this->x & scalar, this->y & scalar, this->z & scalar, this->w & scalar); }
            constexpr Vector<T> operator|(T scalar) { return Vector<T>(this->x | scalar, this->y | scalar, this->z | scalar, this->w | scalar); }
            constexpr Vector<T> operator^(T scalar) { return Vector<T>(this->x ^ scalar, this->y ^ scalar, this->z ^ scalar, this->w ^ scalar); }
            constexpr Vector<T> operator<<(T scalar) { return Vector<T>(this->x << scalar, this->y << scalar, this->z << scalar, this->w << scalar); }
            constexpr Vector<T> operator>>(T scalar) { return Vector<T>(this->x >> scalar, this->y >> scalar, this->z >> scalar, this->w >> scalar); }


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

            constexpr bool operator==(Vector<T> const &vec) { return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z) && (this->w == vec.w); }
            constexpr bool operator!=(Vector<T> const &vec) { return (this->x != vec.x) && (this->y != vec.y) && (this->z != vec.z) && (this->w != vec.w); }
            constexpr bool operator>(Vector<T> const &vec) { return (this->x > vec.x) && (this->y > vec.y) && (this->z > vec.z) && (this->w > vec.w); }
            constexpr bool operator<(Vector<T> const &vec) { return (this->x < vec.x) && (this->y < vec.y) && (this->z < vec.z) && (this->w < vec.w); }
            constexpr bool operator>=(Vector<T> const &vec) { return (this->x >= vec.x) && (this->y >= vec.y) && (this->z >= vec.z) && (this->w >= vec.w); }
            constexpr bool operator<=(Vector<T> const &vec) { return (this->x <= vec.x) && (this->y <= vec.y) && (this->z <= vec.z) && (this->w <= vec.w); }

            /**
             * @brief Cross function
             * 
             * @param vec 
             * @return constexpr Vector<T> 
             */
            constexpr Vector<T> cross(Vector<T> const &vec) { return Vector<T>(this->y * vec.z - this->z * vec.y, this->x * vec.z - this->z * vec.x, this->x * vec.y - this->y * vec.x); }
            
            /**
             * @brief Static cross function
             * 
             * @param vec 
             * @param vec2 
             * @return constexpr Vector<T> 
             */
            constexpr static Vector<T> cross(Vector<T> const &vec, Vector<T> const &vec2) { return Vector<T>(vec.y * vec2.z - vec.z * vec2.y, vec.x * vec2.z - vec.z * vec2.x, vec.x * vec2.y - vec.y * vec2.x); }
            
            /**
             * @brief Dot function
             * 
             * @param vec 
             * @return constexpr T 
             */
            constexpr T dot(Vector<T> const &vec) { return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w; }

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
            constexpr T length() { return sqrt(this->dot(*this)); }

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
            constexpr Vector<T> normalize() { return *this * (static_cast<T>(1) / sqrt(this->dot(*this))); }

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
            constexpr Vector<T> invert() { return Vector<T>(-this->x, -this->y, -this->z, -this->w); }

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
            constexpr Vector<T> abs() { return Vector<T>(qe::math::absolute(this->x), qe::math::absolute(this->y), qe::math::absolute(this->z), qe::math::absolute(this->w)); }

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
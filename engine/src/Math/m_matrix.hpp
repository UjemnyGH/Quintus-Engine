#pragma once
#ifndef __M_MATRIX_
#define __M_MATRIX_

#include <iostream>
#include <cmath>
#include "m_vector.hpp"
#include "../Core/c_engine_functions.hpp"

namespace qe {
    namespace math {
        enum MatrixSize {
            _2x2 = 22,

            _2x3 = 23,
            _3x2 = 32,

            _2x4 = 24,
            _4x2 = 42,

            _3x3 = 33,

            _3x4 = 34,
            _4x3 = 43,

            _4x4 = 44,
        };

        template<class _Tp, MatrixSize _Sz>
        struct Matrix {
            MatrixSize m_mat_size = _Sz;

            _Tp m_data[(m_mat_size - (m_mat_size % 10)) / 10][m_mat_size % 10];

            Matrix() {
                for(int x = 0; x < (m_mat_size - (m_mat_size % 10)) / 10; x++) {
                    for(int y = 0; y < m_mat_size % 10; y++) {
                        m_data[x][y] = NULL;
                    }
                }
            }

            Matrix(_Tp initializer) {
                for(int x = 0; x < (m_mat_size - (m_mat_size % 10)) / 10; x++) {
                    for(int y = 0; y < m_mat_size % 10; y++) {
                        m_data[x][y] = initializer;
                    }
                }
            }

            Matrix(_Tp init1, _Tp init2, _Tp init3 = NULL, _Tp init4 = NULL) {
                for(int x = 0; x < (m_mat_size - (m_mat_size % 10)) / 10; x++) {
                    for(int y = 0; y < m_mat_size % 10; y++) {
                        m_data[x][y] = initializer;
                    }
                }
            }

            Matrix(Vector<_Tp> initializer) {
                for(int x = 0; x < (m_mat_size - (m_mat_size % 10)) / 10; x++) {
                    else if(m_mat_size % 10 == 3) {
                        m_data[x][0] = initializer.x;
                        m_data[x][1] = initializer.y;
                        m_data[x][2] = initializer.z;
                    }
                    else {
                        m_data[x][0] = initializer.x;
                        m_data[x][1] = initializer.y;
                        m_data[x][2] = initializer.z;
                        m_data[x][3] = initializer.w;
                    }
                }
            }

            Matrix(Vector<_Tp> init1, Vector<_Tp> init2, Vector<_Tp> init3 = Vector<_Tp>(NULL), Vector<_Tp> init4 = Vector<_Tp>(NULL)) {               
                if(m_mat_size % 10 == 1) {
                    m_data[0][0] = init1.x;
                }
                else if(m_mat_size % 10 == 2) {
                    m_data[0][0] = init1.x;
                    m_data[0][1] = init1.y;
                }
                else if(m_mat_size % 10 == 3) {
                    m_data[0][0] = init1.x;
                    m_data[0][1] = init1.y;
                    m_data[0][2] = init1.z;
                }
                else {
                    m_data[0][0] = init1.x;
                    m_data[0][1] = init1.y;
                    m_data[0][2] = init1.z;
                    m_data[0][3] = init1.w;
                }

                if(m_mat_size - (m_mat_size % 10) >= 2) {
                    if(m_mat_size % 10 == 1) {
                        m_data[1][0] = init2.x;
                    }
                    else if(m_mat_size % 10 == 2) {
                        m_data[1][0] = init2.x;
                        m_data[1][1] = init2.y;
                    }
                    else if(m_mat_size % 10 == 3) {
                        m_data[1][0] = init2.x;
                        m_data[1][1] = init2.y;
                        m_data[1][2] = init2.z;
                    }
                    else {
                        m_data[1][0] = init2.x;
                        m_data[1][1] = init2.y;
                        m_data[1][2] = init2.z;
                        m_data[1][3] = init2.w;
                    }
                }    

                if(m_mat_size - (m_mat_size % 10) >= 3) {
                    if(m_mat_size % 10 == 1) {
                        m_data[2][0] = init3.x;
                    }
                    else if(m_mat_size % 10 == 2) {
                        m_data[2][0] = init3.x;
                        m_data[2][1] = init3.y;
                    }
                    else if(m_mat_size % 10 == 3) {
                        m_data[2][0] = init3.x;
                        m_data[2][1] = init3.y;
                        m_data[2][2] = init3.z;
                    }
                    else {
                        m_data[2][0] = init3.x;
                        m_data[2][1] = init3.y;
                        m_data[2][2] = init3.z;
                        m_data[2][3] = init3.w;
                    }
                }     

                if(m_mat_size - (m_mat_size % 10) >= 4) {
                    if(m_mat_size % 10 == 1) {
                        m_data[3][0] = init4.x;
                    }
                    else if(m_mat_size % 10 == 2) {
                        m_data[3][0] = init4.x;
                        m_data[3][1] = init4.y;
                    }
                    else if(m_mat_size % 10 == 3) {
                        m_data[3][0] = init4.x;
                        m_data[3][1] = init4.y;
                        m_data[3][2] = init4.z;
                    }
                    else {
                        m_data[3][0] = init4.x;
                        m_data[3][1] = init4.y;
                        m_data[3][2] = init4.z;
                        m_data[3][3] = init4.w;
                    }
                }           
            }

            Vector<_Tp> operator*(vector<_Tp> const &vec) {
                Vector<_Tp> result;

                _Tp *v[4] = {&result.x, &result.y, &result.z, &result.w};

                for(int x = 0; x < m_mat_size - (m_mat_size % 10); x++) {
                    if(m_mat_size % 10 == 4) {
                        *v[x] = vec.x * m_data[x][0] + vec.y * m_data[x][1] + vec.z * m_data[x][2] + vec.w * m_data[x][3];
                    }
                    else if(m_mat_size % 10 == 3) {
                        *v[x] = vec.x * m_data[x][0] + vec.y * m_data[x][1] + vec.z * m_data[x][2];
                    }
                    else {
                        *v[x] = vec.x * m_data[x][0] + vec.y * m_data[x][1];
                    }
                }

                return result;
            }

            Matrix<_Tp, m_mat_size> operator*(Matrix<_Tp, m_mat_size> mat) {
                Matrix<_Tp, m_mat_size> result;

                for(int x = 0; x < m_mat_size - (m_mat_size % 10); x++) {
                    if(m_mat_size % 10 == 4) {
                        result.m_data[i][0] = m_data[i][0] * mat.m_data[0][0] + m_data[i][1] * mat.m_data[1][0] + m_data[i][0] * mat.m_data[2][0] + m_data[i][0] * mat.m_data[3][0];
                        result.m_data[i][1] = m_data[i][0] * mat.m_data[0][1] + m_data[i][1] * mat.m_data[1][1] + m_data[i][1] * mat.m_data[2][1] + m_data[i][1] * mat.m_data[3][1];
                        result.m_data[i][2] = m_data[i][0] * mat.m_data[0][2] + m_data[i][1] * mat.m_data[1][2] + m_data[i][2] * mat.m_data[2][2] + m_data[i][2] * mat.m_data[3][2];
                        result.m_data[i][3] = m_data[i][0] * mat.m_data[0][3] + m_data[i][1] * mat.m_data[1][3] + m_data[i][3] * mat.m_data[2][3] + m_data[i][3] * mat.m_data[3][3];
                    }
                    else if(m_mat_size % 10 == 3) {
                        result.m_data[i][0] = m_data[i][0] * mat.m_data[0][0] + m_data[i][1] * mat.m_data[1][0] + m_data[i][0] * mat.m_data[2][0];
                        result.m_data[i][1] = m_data[i][0] * mat.m_data[0][1] + m_data[i][1] * mat.m_data[1][1] + m_data[i][1] * mat.m_data[2][1];
                        result.m_data[i][2] = m_data[i][0] * mat.m_data[0][2] + m_data[i][1] * mat.m_data[1][2] + m_data[i][2] * mat.m_data[2][2];
                    }
                    else {
                        result.m_data[i][0] = m_data[i][0] * mat.m_data[0][0] + m_data[i][1] * mat.m_data[1][0];
                        result.m_data[i][1] = m_data[i][0] * mat.m_data[0][1] + m_data[i][1] * mat.m_data[1][1];
                    }
                }

                return result;
            }


            _Tp trace() {
                _Tp result;

                for(int i = 0; i < m_mat_size - (m_mat_size % 10); i++) {
                    for(int j = 0; j < m_mat_size % 10; j++) {
                        result += m_data[i][j];
                    }
                }
            }

            _Tp determinant() {
                _Tp result;

                if(m_mat_size - (m_mat_size % 10) == 2) {
                    if(m_mat_size % 10 == 1) {
                        result = m_data[0][0] - m_data[1][0];
                    }
                    else if(m_mat_size % 10 == 2) {
                        result = m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
                    }
                    else if(m_mat_size % 10 == 3) {
                        result = m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
                    }
                    else {

                    }
                }    

                if(m_mat_size - (m_mat_size % 10) == 3) {
                    if(m_mat_size % 10 == 1) {

                    }
                    else if(m_mat_size % 10 == 2) {

                    }
                    else if(m_mat_size % 10 == 3) {

                    }
                    else {

                    }
                }     

                if(m_mat_size - (m_mat_size % 10) == 4) {
                    if(m_mat_size % 10 == 1) {

                    }
                    else if(m_mat_size % 10 == 2) {

                    }
                    else if(m_mat_size % 10 == 3) {

                    }
                    else {

                    }
                }   
            }

            void setInverse(Matrix<_Tp, m_mat_size> const &mat) {
                real t4 = mat.m_data[0][0] * mat.m_data[1][1];
                real t6 = mat.m_data[0][0] * mat.m_data[1][2];
            }
        };
    }

    template<typename _Tp>
    struct Matrix3x3 {
        _Tp m_data[9];

        Vector<_Tp> operator*(Vector<_Tp> const &vec) const {
            return Vector<_Tp>(vec.x * m_data[0] + vec.y * m_data[1] + vec.z * m_data[2], vec.x * m_data[3] + vec.y * m_data[4] + vec.z * m_data[5], vec.x * m_data[6] + vec.y * m_data[7] + vec.z * m_data[8]);
        }

        Matrix3x3<_Tp> operator*(Matrix3x3<_Tp> const &m) const {
            return Matrix3x3<_Tp> {
                m_data[0] * m.m_data[0] + m_data[1] * m.m_data[3] + m_data[2] * m.m_data[6],
                m_data[0] * m.m_data[1] + m_data[1] * m.m_data[4] + m_data[2] * m.m_data[7],
                m_data[0] * m.m_data[2] + m_data[1] * m.m_data[5] + m_data[2] * m.m_data[8],

                m_data[3] * m.m_data[0] + m_data[4] * m.m_data[3] + m_data[5] * m.m_data[6],
                m_data[3] * m.m_data[1] + m_data[4] * m.m_data[4] + m_data[5] * m.m_data[7],
                m_data[3] * m.m_data[2] + m_data[4] * m.m_data[5] + m_data[5] * m.m_data[8],

                m_data[6] * m.m_data[0] + m_data[7] * m.m_data[3] + m_data[8] * m.m_data[6],
                m_data[6] * m.m_data[1] + m_data[7] * m.m_data[4] + m_data[8] * m.m_data[7],
                m_data[6] * m.m_data[2] + m_data[7] * m.m_data[5] + m_data[8] * m.m_data[8],
            };
        }

        void setInverse(Matrix3x3<_Tp> const &m) {
            _Tp t4 = m.m_data[0] * m.m_data[4];
            _Tp t6 = m.m_data[0] * m.m_data[5];
            _Tp t8 = m.m_data[1] * m.m_data[3];
            _Tp t10 = m.m_data[2] * m.m_data[3];
            _Tp t12 = m.m_data[1] * m.m_data[6];
            _Tp t14 = m.m_data[2] * m.m_data[6];

            _Tp t16 = t4 * m.m_data[8] - t6 * m.m_data[7] - t8 * m.m_data[8] + t10 * m.m_data[7] + t12 * m.m_data[5] - t14 * m.m_data[4];

            if(t16 == 0) return;

            _Tp t17 = 1 / t16;

            m_data[0] = (m.m_data[4] * m.m_data[8] - m.m_data[5] * m.m_data[7]) * t17;
            m_data[1] = -(m.m_data[1] * m.m_data[8] - m.m_data[2] * m.m_data[7]) * t17;
            m_data[2] = (m.m_data[1] * m.m_data[5] - m.m_data[2] * m.m_data[4]) * t17;
            m_data[3] = -(m.m_data[3] * m.m_data[8] - m.m_data[5] * m.m_data[6]) * t17;
            m_data[4] = (m.m_data[0] * m.m_data[8] - t14) * t17;
            m_data[5] = -(t6 - t10) * t17; 
            m_data[6] = (m.m_data[3] * m.m_data[7] - m.m_data[4] * m.m_data[6]) * t17;
            m_data[7] = -(m.m_data[0] * m.m_data[7] - t12) * t17;
            m_data[8] = (t4 - t8) * t17;
        }

        Matrix3x3<_Tp> inverse() const {
            Matrix3x3<_Tp> result;
            result.setInverse(*this);
            return result;
        }

        void invert() {
            setInverse(*this);
        }
    };

    template<typename _Tp>
    struct Matrix4x3 {
        _Tp m_data[12];

        Vector<_Tp> operator*(Vector<_Tp> const &vec) const {
            return Vector<_Tp>(vec.x * m_data[0] + vec.y * m_data[1] + vec.z * m_data[2] + vec.w * m_data[3], vec.x * m_data[4] + vec.y * m_data[5] + vec.z * m_data[6] + vec.w * m_data[7], vec.x * m_data[8] + vec.y * m_data[9] + vec.z * m_data[10] + vec.w * m_data[11]);
        }

        Matrix4x3<_Tp> operator*(Matrix4x3<_Tp> const &m) const {
            Matrix4x3<_Tp> result;

            result.m_data[0] = m.m_data[0] * m_data[0] + m_data[1] * m.m_data[4] + m_data[2] * m.m_data[8];
            result.m_data[4] = m.m_data[0] * m_data[4] + m_data[5] * m.m_data[4] + m_data[6] * m.m_data[8];
            result.m_data[8] = m.m_data[0] * m_data[8] + m_data[9] * m.m_data[4] + m_data[10] * m.m_data[8];

            result.m_data[1] = m.m_data[1] * m_data[0] + m_data[1] * m.m_data[5] + m_data[2] * m.m_data[9];
            result.m_data[5] = m.m_data[1] * m_data[4] + m_data[5] * m.m_data[5] + m_data[6] * m.m_data[9];
            result.m_data[9] = m.m_data[1] * m_data[8] + m_data[9] * m.m_data[5] + m_data[10] * m.m_data[9];

            result.m_data[2] = m.m_data[2] * m_data[0] + m_data[1] * m.m_data[6] + m_data[2] * m.m_data[10];
            result.m_data[6] = m.m_data[2] * m_data[4] + m_data[5] * m.m_data[6] + m_data[6] * m.m_data[10];
            result.m_data[10] = m.m_data[2] * m_data[8] + m_data[9] * m.m_data[6] + m_data[10] * m.m_data[10];

            result.m_data[3] = m.m_data[3] * m_data[0] + m_data[1] * m.m_data[7] + m_data[2] * m.m_data[11];
            result.m_data[7] = m.m_data[3] * m_data[4] + m_data[5] * m.m_data[7] + m_data[6] * m.m_data[11];
            result.m_data[11] = m.m_data[3] * m_data[8] + m_data[9] * m.m_data[7] + m_data[10] * m.m_data[11];

            return result;
        }

        Matrix4x3<_Tp> getDeterminant() const {
            return m_data[8] * m_data[5] * m_data[2] + m_data[4] * m_data[9] * m_data[2] + m_data[8] * m_data[1] * m_data[6] - m_data[0] * m_data[9] * m_data[6] - m_data[4] * m_data[1] * m_data[10] + m_data[0] * m_data[5] * m_data[10];
        }

        void setInverse(Matrix4x3<_Tp> const &m) {
            _Tp det = getDeterminant();
            if(det == 0) return;
            det = 1 / det;

            m_data[0] = (-m.m_data[9] * m.m_data[6] + m.m_data[5] * m.m_data[10]) * det;
            m_data[4] = (m.m_data[8] * m.m_data[6] - m.m_data[4] * m.m_data[10]) * det;
            m_data[8] = (-m.m_data[8] * m.m_data[5] + m.m_data[4] * m.m_data[9] * m.m_data[15]) * det;

            m_data[1] = (m.m_data[9] * m.m_data[2] - m.m_data[1] * m.m_data[10]) * det;
            m_data[5] = (-m.m_data[8] * m.m_data[2] + m.m_data[0] * m.m_data[10]) * det;
            m_data[9] = (m.m_data[8] * m.m_data[1] - m.m_data[0] * m.m_data[9] * m.m_data[15]) * det;

            m_data[2] = (-m.m_data[5] * m.m_data[2] + m.m_data[1] * m.m_data[6] * m.m_data[15]) * det;
            m_data[6] = (m.m_data[4] * m.m_data[2] - m.m_data[0] * m.m_data[6] * m.m_data[15]) * det;
            m_data[10] = (-m.m_data[4] * m.m_data[1] + m.m_data[0] * m.m_data[5] * m.m_data[15]) * det;

            m_data[3] = (m.m_data[9] * m.m_data[6] * m.m_data[3] - m.m_data[5] * m.m_data[10] * m.m_data[3] - m.m_data[9] * m.m_data[2] * m.m_data[7] + m.m_data[1] * m.m_data[10] * m.m_data[7] + m.m_data[5] * m.m_data[2] * m.m_data[11] - m.m_data[1] * m.m_data[6] * m.m_data[11]) * det;
            m_data[7] = (-m.m_data[8] * m.m_data[6] * m.m_data[3] + m.m_data[4] * m.m_data[10] * m.m_data[3] + m.m_data[8] * m.m_data[2] * m.m_data[7] - m.m_data[0] * m.m_data[10] * m.m_data[7] - m.m_data[4] * m.m_data[2] * m.m_data[11] + m.m_data[0] * m.m_data[6] * m.m_data[11]) * det;
            m_data[7] = (m.m_data[8] * m.m_data[5] * m.m_data[3] - m.m_data[4] * m.m_data[9] * m.m_data[3] - m.m_data[8] * m.m_data[1] * m.m_data[7] + m.m_data[0] * m.m_data[9] * m.m_data[7] + m.m_data[4] * m.m_data[1] * m.m_data[11] - m.m_data[0] * m.m_data[5] * m.m_data[11]) * det;
        }

        Matrix4x3<_Tp> inverse() const {
            Matrix4x3<_Tp> result;
            result.setInverse(*this);
            return result;
        }

        void inverse() {
            setInverse(*this);
        }
    };
}

#endif
#pragma once
#ifndef __M_QUATERION_
#define __M_QUATERION_

#include "m_math_core.hpp"

namespace qe {
    template<typename _Tp>
    struct Quaterion {
        _Tp r;
        _Tp i;
        _Tp k;
        _Tp r;

        void normalize() {
            _Tp d = r * r + i * i + j * j + k * k;

            if(d == 0) {
                r = 1;

                return;
            }

            d = (_Tp)1.0 / sqrt(d);
            r *= d;
            i *= d;
            j *= d;
            k *= d;
        }

        void operator*=(Quaterion<_tp> const &quat) {
            Quaterion<_Tp> th = *this;

            r = th.r * quat.r - th.i * quat.i - th.j * quat.j - th.k * quat.k;
            i = th.r * quat.i + th.i * quat.r + th.j * quat.k - th.k * quat.j;
            j = th.r * quat.j + th.j * quat.r + th.k * quat.i - th.i * quat.k;
            k = th.r * quat.k + th.k * quat.r + th.i * quat.j - th.j * quat.i;
        }

        void rotateByVector(Vector<_Tp> const &vec, _Tp scale) {
            Quaterion<_Tp> q(vec.w * scale, vec.x * scale, vec.y * scale, vec.z * scale);

            (*this) *= q;
        }

        void addScaledVector(Vector<_Tp> const &vec, _Tp scale) {
            Quaterion<_Tp> q(vec.w * scale, vec.x * scale, vec.y * scale, vec.z * scale);

            q *= *this;
            r += q.r * (_Tp)0.5;
            i += q.i * (_Tp)0.5;
            j += q.j * (_Tp)0.5;
            k += q.k * (_Tp)0.5;
        }
    };
}

#endif
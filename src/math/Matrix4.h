#pragma once
#include "Vector3.h"
#include <cmath>
#include <cstring>

struct Matrix4 {
    float m[16];

    Matrix4() {
        identity();
    }

    void identity() {
        memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i * 4 + j] =
                    m[i * 4 + 0] * other.m[0 * 4 + j] +
                    m[i * 4 + 1] * other.m[1 * 4 + j] +
                    m[i * 4 + 2] * other.m[2 * 4 + j] +
                    m[i * 4 + 3] * other.m[3 * 4 + j];
            }
        }
        return result;
    }

    Matrix4 transpose() const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[j * 4 + i] = m[i * 4 + j];
            }
        }
        return result;
    }

    static Matrix4 perspective(float fov, float aspect, float nearPlane, float farPlane) {
        Matrix4 result;
        memset(result.m, 0, sizeof(result.m));
        float tanHalfFov = std::tan(fov * 0.5f);

        result.m[0] = 1.0f / (aspect * tanHalfFov);
        result.m[5] = 1.0f / tanHalfFov;
        result.m[10] = farPlane / (farPlane - nearPlane);
        result.m[11] = 1.0f;
        result.m[14] = -(farPlane * nearPlane) / (farPlane - nearPlane);

        return result;
    }

    static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
        // left-handed coordinate system for DirectX
        Vector3 f = (center - eye).normalize(); // Forward
        Vector3 s = up.cross(f).normalize();    // Right
        Vector3 u = f.cross(s);                 // Up

        Matrix4 result;
        result.identity();

        // this creates a column-major matrix in our row-major array
        // which is what the rest of the engine expects before transposing
        result.m[0] = s.x;
        result.m[1] = u.x;
        result.m[2] = f.x;
        result.m[3] = 0.0f;

        result.m[4] = s.y;
        result.m[5] = u.y;
        result.m[6] = f.y;
        result.m[7] = 0.0f;

        result.m[8] = s.z;
        result.m[9] = u.z;
        result.m[10] = f.z;
        result.m[11] = 0.0f;

        result.m[12] = -s.dot(eye);
        result.m[13] = -u.dot(eye);
        result.m[14] = -f.dot(eye);
        result.m[15] = 1.0f;

        return result;
    }

    static Matrix4 translate(const Vector3& translation) {
        Matrix4 result;
        result.identity();
        result.m[12] = translation.x;
        result.m[13] = translation.y;
        result.m[14] = translation.z;
        return result;
    }

    static Matrix4 scale(const Vector3& scaling) {
        Matrix4 result;
        result.identity();
        result.m[0] = scaling.x;
        result.m[5] = scaling.y;
        result.m[10] = scaling.z;
        return result;
    }

    static Matrix4 rotateY(float angle) {
        Matrix4 result;
        result.identity();
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        result.m[0] = cosA;
        result.m[2] = -sinA;
        result.m[8] = sinA;
        result.m[10] = cosA;

        return result;
    }

    static Matrix4 rotateX(float angle) {
        Matrix4 result;
        result.identity();
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        result.m[5] = cosA;
        result.m[6] = sinA;
        result.m[9] = -sinA;
        result.m[10] = cosA;

        return result;
    }
};

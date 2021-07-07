#pragma once

#include <math.h>
#include <stdbool.h>

#define PI (3.14159265359f)
#define DEG2RAD(d) ((d)*PI/180.f)

struct vec3
{
    float x, y, z;
};

struct mat3
{
    float v[9];
};

static void mat3_rot_x(struct mat3* mat, float degrees)
{
    const float rad = DEG2RAD(degrees);

    mat->v[0] = 1.0f;
    mat->v[1] = 0.0f;
    mat->v[2] = 0.0f;

    mat->v[3] = 0.0f;
    mat->v[4] = cosf(rad);
    mat->v[5] = sinf(rad);

    mat->v[6] = 0.0f;
    mat->v[7] = sinf(rad);
    mat->v[8] = cosf(rad);
}

static void mat3_rot_y(struct mat3* mat, float degrees)
{
    const float rad = DEG2RAD(degrees);

    mat->v[0] = cosf(rad);
    mat->v[1] = 0.0f;
    mat->v[2] = -sinf(rad);

    mat->v[3] = 0.0f;
    mat->v[4] = 1.0f;
    mat->v[5] = 0.0f;

    mat->v[6] = sinf(rad);
    mat->v[7] = 0.0f;
    mat->v[8] = cosf(rad);
}

static void mat3_rot_z(struct mat3* mat, float degrees)
{
    const float rad = DEG2RAD(degrees);

    mat->v[0] = cosf(rad);
    mat->v[1] = sinf(rad);
    mat->v[2] = 0.0f;

    mat->v[3] = -sinf(rad);
    mat->v[4] = cosf(rad);
    mat->v[5] = 0.0f;

    mat->v[6] = 0.0f;
    mat->v[7] = 0.0f;
    mat->v[8] = 1.0f;
}

static void mat3_mult_mat3(struct mat3* lhs, struct mat3* rhs, struct mat3* out)
{
    for (size_t i = 0; i < 9; ++i) {
        const size_t row = i % 3;
        const size_t col = i / 3;
        out->v[i] = 0.0f;
        for (size_t j = 0; j < 3; ++j)
            out->v[i] += lhs->v[row + j * 3] * rhs->v[col * 3 + j];
    }
}

// It's okay for rhs and out to be the same pointer
static void mat3_mult_vec3(const struct mat3* lhs, const struct vec3* rhs, struct vec3* out)
{
    const float rx = rhs->x;
    const float ry = rhs->y;
    const float rz = rhs->z;
    out->x = rx * lhs->v[0] + ry * lhs->v[1] + rz * lhs->v[2];
    out->y = rx * lhs->v[3] + ry * lhs->v[4] + rz * lhs->v[5];
    out->z = rx * lhs->v[6] + ry * lhs->v[7] + rz * lhs->v[8];
}

// yaw +0 forward, +90 right, +180 back, +270 left
// pitch +90 up, -90 down, 0 forward
static void applyViewAnglesToAxes(float pitch, float yaw, struct vec3* up,
    struct vec3* left, struct vec3* forward)
{
    struct mat3 rotYaw, rotPitch, rotBoth;
    mat3_rot_z(&rotYaw, yaw); // Here I'm assuming +z is up and +y is left.
    mat3_rot_y(&rotPitch, pitch); // You may need to change that assumption.
    mat3_mult_mat3(&rotPitch, &rotYaw, &rotBoth);
    mat3_mult_vec3(&rotBoth, forward, forward);
    mat3_mult_vec3(&rotBoth, left, left);
    mat3_mult_vec3(&rotBoth, up, up);
}

// out x,y is screen coords. z is distance
static bool worldToScreen(struct vec3* viewPos, struct vec3* objPos,
    float fovX, float fovY, float width, float height,
    struct vec3* forward, struct vec3* left,
    struct vec3* up, struct vec3* screen)
{
    /*const*/ struct vec3 transform = {

    };

    transform.x = objPos->x - viewPos->x;
    transform.y = objPos->y - viewPos->y;
    transform.z = objPos->z - viewPos->z;

    const float dotForward = forward->x * transform.x + forward->y * transform.y + forward->z * transform.z;
    if (dotForward <= 0.05f)
        return false;

    const float dotLeft = left->x * transform.x + left->y * transform.y + left->z * transform.z;
    const float dotUp = up->x * transform.x + up->y * transform.y + up->z * transform.z;

    const float tanFovX = tanf(DEG2RAD(fovX) / 2.0f);
    const float tanFovY = tanf(DEG2RAD(fovY) / 2.0f);
    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;

    screen->x = halfWidth - dotLeft * halfWidth / (dotForward * tanFovX);
    screen->y = halfHeight - dotUp * halfHeight / (dotForward * tanFovY);
    screen->z = sqrtf(transform.x * transform.x + transform.y * transform.y + transform.z * transform.z);
    return true;
}
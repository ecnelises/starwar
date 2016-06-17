#ifndef BALL_TYPE_H_
#define BALL_TYPE_H_

struct SunBall {
    float linearDamping = 6.0f;
    float mass          = 80.0f;
    float radius        = 35.0f;
    float density       = 1.0f;
    float restitution   = 1.2f;
    float friction      = 1.0f;
    const char * file   = "texture/sun.png";
    int   tag           = 0x11;
};

struct EarthBall {
    float linearDamping = 5.3f;
    float mass          = 40.0f;
    float radius        = 25.0f;
    float density       = 1.0f;
    float restitution   = 1.2f;
    float friction      = 1.0f;
    const char * file   = "texture/earth.png";
    int   tag           = 0x12;
};

struct MoonBall {
    float linearDamping = 4.8f;
    float mass          = 20.0f;
    float radius        = 16.0f;
    float density       = 1.0f;
    float restitution   = 1.2f;
    float friction      = 1.0f;
    const char * file   = "texture/moon.png";
    int   tag           = 0x13;
};

#endif // BALL_TYPE_H_
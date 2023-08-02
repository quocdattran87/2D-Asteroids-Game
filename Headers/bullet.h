#ifndef BULLET_CLASS_H
#define BULLET_CLASS_H

#include "struct.h"

class Bullet {
    public:
        Bullet(float xPos, float yPos, float rotation, float size, bool isTracking);

        void move(float deltaTime);
        void track(Vector asteroid, float deltaTime);
        void render();
        Vector getPosition();

    private:
        float speed;
        float rotation;
        float size;
        bool isTracking;
        Vector position;
};
#endif
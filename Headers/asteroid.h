#ifndef ASTEROID_CLASS_H
#define ASTEROID_CLASS_H

#include <vector>
#include "struct.h"

class Asteroid {
    public:
        Asteroid(int width, int height, Vector shipPosition, int level, int n, Vector parentPosition, float radius, bool inArena, int health, float rotation, int m);

        void move(float deltaTime, int width, int height);
        void rotate(float deltaTime);
        void render();

        float getRadius();
        Vector getPosition();
        void verticalBounce();
        void horizontalBounce();
        int getLevel();
        void reduceHealth();
        int getHealth();
        bool isInArena();
        void collisionWithOtherAsteroid();
        float getRotation();

    private:
        float radius;
        float speed;
        float rotationSpeed;
        float rotation;
        float rotation2;
        float rotation3;
        Vector position;
        Vector direction;

        bool inArena;
        int size;
        int side;
        int level;
        int health;
        int rotationDirection;
        std::vector<float> randomEdges;

        void updateDirection();
        void aimAtPlayer(float x, float y, Vector shipPosition);
};

#endif
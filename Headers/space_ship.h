#ifndef SPACE_SHIP_CLASS_H
#define SPACE_SHIP_CLASS_H

#include "struct.h"


class SpaceShip {
    public:
        SpaceShip();

        void accelerate(float deltaTime);
        void decelerate(float deltaTime);
        void rotateCounterClockwise(float deltaTime);
        void rotateClockwise(float deltaTime);
        void switchDoubleGun();
        bool isDoubleGun();
        void switchHomingGun();
        bool isHomingGun();
        Vector getPosition();
        Vector getEnginePosition();
        void setPosition(Vector position);
        float getRotation();
        float getEngineDirection();
        Vector getSpeed();
        int getLives();
        void setLives(int lives);
        void reset();
        void render(bool moveIsPressed);
        void droneMode(Vector asteroid, float deltaTime);

    private:
        int lives;
        float maxSpeed;
        float acceleration;
        float rotationSpeed;
        float rotation;
        bool doubleGun;
        bool homingGun;
        bool decelerating;
        bool drone;

        Vector position;
        Vector direction;
        Vector speed;

        void updateDirection();
};
#endif
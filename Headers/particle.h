#ifndef PARTICLE_CLASS_H
#define PARTICLE_CLASS_H

#include "struct.h"

class Particle {
    public:
        Particle(Vector position);
        void move(float deltaTime);
        bool maxDistanceTravelled();
        void render();

    private:
        Vector origin;
        Vector position;
        float speed;
        float direction;
        float maxDistance;
        float size;
        float decay;
};
#endif
#ifndef ENGINE_PARTICLE_CLASS_H
#define ENGINE_PARTICLE_CLASS_H

#include "struct.h"

class EngineParticle {
    public:
        EngineParticle(Vector position, float direction);
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
        float colour;
        float decay;
};
#endif
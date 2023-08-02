#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include "Headers/engine_particle.h"

EngineParticle::EngineParticle(Vector position, float direction) {
    EngineParticle::origin = position;
    EngineParticle::position = position;
    EngineParticle::direction = direction;
    speed = 0.03 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(0.2 - 0.03)));
    maxDistance = 0.005 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(0.04 - 0.005)));
    decay = 2.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(4.0 - 2.0)));
    colour = 0.5;
    size = 5.0;
}

void EngineParticle::move(float deltaTime) {
    position.x += speed * sin(direction * PI / 180) * deltaTime;
    position.y += speed * -cos(direction * PI / 180) * deltaTime;
    size -= decay * deltaTime;
    if (size <= 0.0) {
        size = 0.000001;
    }
    colour += 1.5 * deltaTime;
    if (colour >= 1.0) {
        colour = 1.0;
    }
}

void EngineParticle::render() {
    glPushMatrix();
        glLoadIdentity();
        glPointSize(size);
        glColor3f(1.0, colour, 0.0);
        glBegin(GL_POINTS);
        glVertex3f(position.x, position.y, position.z);
        glEnd();
    glPopMatrix();
}

bool EngineParticle::maxDistanceTravelled() {
	float x = origin.x - position.x;
	float y = origin.y - position.y;

    if (((x * x) + (y * y)) >= (maxDistance * maxDistance)) {
        return true;
    }
    return false;
}
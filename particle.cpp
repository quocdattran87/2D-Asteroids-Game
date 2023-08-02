#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include "Headers/particle.h"

Particle::Particle(Vector position) {
    Particle::origin = position;
    Particle::position = position;
    direction = static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(360.0)));
    maxDistance = 0.04 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(0.12 - 0.04)));
    decay = 1.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(3.0 - 1.0)));
    speed = 0.03 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(0.2 - 0.03)));
    size = 6.0;
}

void Particle::move(float deltaTime) {
    position.x += speed * sin(direction * PI / 180) * deltaTime;
    position.y += speed * -cos(direction * PI / 180) * deltaTime;
    size -= decay * deltaTime;
    if (size <= 0.0) {
        size = 0.000001;
    }
}

void Particle::render() {
    glPushMatrix();
    glLoadIdentity();
    glPointSize(size);
    glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
    glVertex3f(position.x, position.y, 0.0);
    glEnd();
    glPopMatrix();
}

bool Particle::maxDistanceTravelled() {
	float x = origin.x - position.x;
	float y = origin.y - position.y;

    if (((x * x) + (y * y)) >= (maxDistance * maxDistance)) {
        return true;
    }
    return false;
}
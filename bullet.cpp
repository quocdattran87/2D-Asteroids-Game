#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include "Headers/bullet.h"

Bullet::Bullet(float xPos, float yPos, float rotation, float size, bool isTracking) {
   position.x = xPos;
   position.y = yPos;
   position.z = 0.0;
   Bullet::rotation = rotation;
   Bullet::size = size;
   speed = 1.0;
   Bullet::isTracking = isTracking;
}

void Bullet::move(float deltaTime) {
    position.x += speed * sin(rotation * PI / 180) * deltaTime;
    position.y += speed * -cos(rotation * PI / 180) * deltaTime;
}

void Bullet::track(Vector asteroid, float deltaTime) {
    if (isTracking) {
        if (position.x < asteroid.x && position.y > asteroid.y) {
            rotation = atan((asteroid.x - position.x) / (position.y - asteroid.y)) * 180 / PI;
        } else if (position.x > asteroid.x && position.y > asteroid.y) {
            rotation = -atan((position.x - asteroid.x) / (position.y - asteroid.y)) * 180 / PI;
        } else if (position.x < asteroid.x && position.y < asteroid.y) {
            rotation = 90.0 + (atan((asteroid.y - position.y) / (asteroid.x - position.x)) * 180 / PI);
        } else if (position.x > asteroid.x && position.y < asteroid.y) {
            rotation = 270.0 - (atan((asteroid.y - position.y) / (position.x - asteroid.x)) * 180 / PI);
        } else if (position.x == asteroid.x && position.y > asteroid.y) {
            rotation = 0.0;
        } else if (position.x == asteroid.x && position.y < asteroid.y) {
            rotation = 180.0;
        } else if (position.x > asteroid.x && position.y == asteroid.y) {
            rotation = 270.0;
        } else if (position.x < asteroid.x && position.y == asteroid.y) {
            rotation = 90.0;
        }
    }
}

Vector Bullet::getPosition() {
    return position;
}

void Bullet::render() {
    glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
    // Front
	glVertex3f(position.x + size * sin(rotation * PI / 180), position.y - size * cos(rotation * PI / 180), 0.0);
	// Back left corner
	glVertex3f(position.x + size * sin((rotation - 140) * PI / 180), position.y - size * cos((rotation - 140) * PI / 180), 0.0);
    // Back right corner
    glVertex3f(position.x + size * sin((rotation + 140) * PI / 180), position.y - size * cos((rotation + 140) * PI / 180), 0.0);
	glEnd();
}
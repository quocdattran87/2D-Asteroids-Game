#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>

#include "Headers/asteroid.h"

Asteroid::Asteroid(int width, int height, Vector shipPosition, int level, int n, Vector parentPosition, float radius, bool inArena, int health, float rotation, int m) {
    Asteroid::radius = radius;
    Asteroid::inArena = inArena;
    side = rand() % 4 + 1;

    speed = 0.08 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(0.21-0.08)));
    rotationSpeed = rand() % 60 + 20;
    rotation2 = 0.0;
    rotation3 =0.0;
    rotationDirection = rand() % 2;

    size = 32; // make random
    for (int i = 0; i < size; i++) {
        randomEdges.push_back(0.9 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(1.1 - 0.9))));
    }

    Asteroid::health = health;
    Asteroid::level = level;

    if (n == 1 && width > height) {
        // New asteroids at start of level
        if (side == 1) { // Top
            position.x = -(float)width/height - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(2*(float)width/height + 4*radius)));
            position.y = 1 + 2*radius;
        } else if (side == 2) { // Bottom
                position.x = -(float)width/height - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(2*(float)width/height + 4*radius)));
                position.y = -1 - 2*radius;
        } else if (side == 3) { // Left
                position.x = -(float)width/height - 2*radius;
                position.y = (-1.0 - 2*radius) + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(2.0 + 4*radius)));
        } else if (side == 4) { // Right
                position.x = (float)width/height + 2*radius;
                position.y = (-1.0 - 2*radius) + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(2.0 + 4*radius)));
        }
        aimAtPlayer(position.x, position.y, shipPosition);
    }
    if (n == 0) {
        // Asteroids from splitting
        if (m == 0) { // Left asteroid
            position.x = parentPosition.x + radius * sin((rotation + 45) * PI / 180);
            position.y = parentPosition.y - radius * cos((rotation + 45) * PI / 180);
        } else { // Right asteroid
            position.x = parentPosition.x + radius * sin((rotation - 45) * PI / 180);
            position.y = parentPosition.y - radius * cos((rotation - 45) * PI / 180);
        }
        if (inArena) {
            if (rotation < 0) {
                Asteroid::rotation = rotation + 360.0;
            } else if (rotation > 360) {
                Asteroid::rotation = rotation - 360.0;
            } else {
                Asteroid::rotation = rotation;
            }
        } else {
            aimAtPlayer(position.x, position.y, shipPosition);
        }
    }
}

void Asteroid::aimAtPlayer(float x, float y, Vector shipPosition) {
    if (x < shipPosition.x && y > shipPosition.y) {
        rotation = atan((shipPosition.x - x) / (y - shipPosition.y)) * 180 / PI;
    } else if (x > shipPosition.x && y > shipPosition.y) {
        rotation = -atan((x - shipPosition.x) / (y - shipPosition.y)) * 180 / PI;
    } else if (x < shipPosition.x && y < shipPosition.y) {
        rotation = 90.0 + (atan((shipPosition.y - y) / (shipPosition.x - x)) * 180 / PI);
    } else if (x > shipPosition.x && y < shipPosition.y) {
        rotation = 270.0 - (atan((shipPosition.y - y) / (x - shipPosition.x)) * 180 / PI);
    } else if (x == shipPosition.x && y > shipPosition.y) {
        rotation = 0.0;
    } else if (x == shipPosition.x && y < shipPosition.y) {
        rotation = 180.0;
    } else if (x > shipPosition.x && y == shipPosition.y) {
        rotation = 270.0;
    } else if (x < shipPosition.x && y == shipPosition.y) {
        rotation = 90.0;
    }
}

void Asteroid::move(float deltaTime, int width, int height) {
    position.x += speed * sin(rotation * PI / 180) * deltaTime;
    position.y += speed * -cos(rotation * PI / 180) * deltaTime;

    if (!inArena && position.y < 1.0 - radius && position.x > -(float)width/height + radius && position.x < (float)width/height - radius
    && position.y > -1.0 + radius && position.x) {
        inArena = true;
    }
}

void Asteroid::rotate(float deltaTime) {
    if (rotationDirection == 0) {
        rotation2 += rotationSpeed * deltaTime;
        if (rotation2 >= 360.0) {
            rotation2 -= 360.0;
        }
    } else {
        rotation2 -= rotationSpeed * deltaTime;
        if (rotation2 <= 0.0) {
            rotation2 += 360.0;
        }
    }
    updateDirection();
}

void Asteroid::verticalBounce() {
    if (inArena) {
        rotation = 180.0 - rotation;
        if (rotation <= 0.0) {
            rotation += 360.0;
        }
    }
}

void Asteroid::horizontalBounce() {
    if (inArena) {
        rotation = 360.0 - rotation;
    }
}

float Asteroid::getRadius() {
    return radius;
}

Vector Asteroid::getPosition() {
    return position;
}

int Asteroid::getLevel() {
    return level;
}

int Asteroid::getHealth() {
    return health;
}

bool Asteroid::isInArena() {
    return inArena;
}

void Asteroid::reduceHealth() {
    health -= 1;
}

float Asteroid::getRotation() {
    return rotation;
}

void Asteroid::collisionWithOtherAsteroid() {
    // std::cout << "Asteroids collide" << std::endl;
    //     if (inArena) {
    //     rotation -= 180;
    //     if (rotation <= 0.0) {
    //         rotation += 360.0;
    //     }
    // }
}

void Asteroid::updateDirection() {
    float x = sin(rotation2 * PI / 180);
    float y = -cos(rotation2 * PI / 180);
    float x_normalised = x / sqrt((x * x) + (y * y));
    float y_normalised = y / sqrt((x * x) + (y * y));
    
    direction.x = x_normalised;
    direction.y = y_normalised;
}

void Asteroid::render() {
	float x, y;
    if (health == 3) {
        glColor3f(1.0, 1.0, 0.0);
    } else if (health == 2) {
        glColor3f(1.0, 0.5, 0.0);
    } else if (health == 1) {
        glColor3f(1.0, 0.0, 0.0);
    } else {
        glColor3f(1.0, 1.0, 1.0);
    }
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
    float angle = 360.0 / size;
	for (int i = 0; i < size; i++) {
        rotation3 += angle;
		x = position.x + randomEdges[i] * radius * sin((rotation3+rotation2) * PI / 180);
		y = position.y - randomEdges[i] * radius * cos((rotation3+rotation2) * PI / 180);
		glVertex2f(x, y);
	}
	glEnd();  
}
#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>

#include "Headers/space_ship.h"

SpaceShip::SpaceShip() {
    lives = 3;
    speed.x = 0.0;
    speed.y = 0.0;
    maxSpeed = 0.34;
    acceleration = 0.006;
    rotationSpeed = 300.0;
    rotation = 180.0;
    doubleGun = false;
    homingGun = false;
    decelerating = false;
    drone = false;
    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;
    updateDirection();
}

void SpaceShip::accelerate(float deltaTime) {
    updateDirection();
    speed.x += direction.x * 0.5 * deltaTime;
    speed.y += direction.y * 0.5 * deltaTime;

    if (speed.x > maxSpeed) {
        speed.x = maxSpeed;
    }
    if (speed.x < -maxSpeed) {
        speed.x = -maxSpeed;
    }
    if (speed.y > maxSpeed) {
        speed.y = maxSpeed;
    }
    if (speed.y < -maxSpeed) {
        speed.y = -maxSpeed;
    }

    position.x += speed.x * deltaTime;
    position.y += speed.y * deltaTime;
}

void SpaceShip::decelerate(float deltaTime) {
    decelerating = true;
    if (speed.x > 0 && speed.y > 0) {
        speed.x -= acceleration;
        speed.y -= acceleration;
        if (speed.x < 0) {
            speed.x = 0;
        }
        if (speed.y < 0) {
            speed.y = 0;
        }
    }
    if (speed.x < 0 && speed.y < 0) {
        speed.x += acceleration;
        speed.y += acceleration;
        if (speed.x > 0) {
            speed.x = 0;
        }
        if (speed.y > 0) {
            speed.y = 0;
        }
    }
    if (speed.x < 0 && speed.y > 0) {
        speed.x += acceleration;
        speed.y -= acceleration;
        if (speed.x > 0) {
            speed.x = 0;
        }
        if (speed.y < 0) {
            speed.y = 0;
        }
    }
    if (speed.x > 0 && speed.y < 0) {
        speed.x -= acceleration;
        speed.y += acceleration;
        if (speed.x < 0) {
            speed.x = 0;
        }
        if (speed.y > 0) {
            speed.y = 0;
        }
    }
    if (speed.x == 0 && speed.y > 0) {
        speed.y -= acceleration;
        if (speed.y < 0) {
            speed.y = 0;
        }
    }
    if (speed.x == 0 && speed.y < 0) {
        speed.y += acceleration;
        if (speed.y > 0) {
            speed.y = 0;
        }
    }
    if (speed.y == 0 && speed.x < 0) {
        speed.x += acceleration;
        if (speed.x > 0) {
            speed.x = 0;
        }
    }
    if (speed.y == 0 && speed.x > 0) {
        speed.x -= acceleration;
        if (speed.x < 0) {
            speed.x = 0;
        }
    }
    if (speed.x == 0 && speed.y == 0) {
        decelerating = false;
    }

    position.x += speed.x * deltaTime;
    position.y += speed.y * deltaTime;
}

void SpaceShip::rotateCounterClockwise(float deltaTime) {
    rotation += rotationSpeed * deltaTime;
    if (rotation >= 360.0) {
        rotation -= 360.0;
    }
    if (!decelerating) {
        updateDirection();
    }
}

void SpaceShip::rotateClockwise(float deltaTime) {
    rotation -= rotationSpeed * deltaTime;
    if (rotation <= 0.0) {
        rotation += 360.0f;
    }
    if (!decelerating) {
        updateDirection();
    }
}

void SpaceShip::updateDirection() {
    float x = sin(rotation * PI / 180);
    float y = -cos(rotation * PI / 180);
    float x_normalised = x / sqrt((x * x) + (y * y));
    float y_normalised = y / sqrt((x * x) + (y * y));
    
    direction.x = x_normalised;
    direction.y = y_normalised;
}

void SpaceShip::switchDoubleGun() {
    doubleGun = !doubleGun;
}
bool SpaceShip::isDoubleGun() {
    return doubleGun;
}

void SpaceShip::switchHomingGun() {
    homingGun = !homingGun;
}

bool SpaceShip::isHomingGun() {
    return homingGun;
}

Vector SpaceShip::getPosition() {
    return position;
}

Vector SpaceShip::getEnginePosition() {
    Vector engine = {(float)(position.x + (-0.04) * sin(rotation * PI / 180)),(float)(position.y - (-0.04) * cos(rotation * PI / 180)),  0.0f};
    return engine;
}

void SpaceShip::setPosition(Vector position) {
    SpaceShip::position.x = position.x;
    SpaceShip::position.y = position.y;
    SpaceShip::position.z = position.z;
}

float SpaceShip::getRotation() {
    return rotation;
}

float SpaceShip::getEngineDirection() {
    int angle = rand() % 60 + 150;
    float engineDirection = (float)rotation - (float)angle;
    if (engineDirection >= 360.0) {
        engineDirection -= 360.0;
    }
    return engineDirection;
}

Vector SpaceShip::getSpeed() {
    return speed;
}

int SpaceShip::getLives() {
    return lives;
}

void SpaceShip::setLives(int lives) {
    SpaceShip::lives = lives;
}

void SpaceShip::reset() {
    rotation = 180;
    speed.x = 0.0;
    speed.y = 0.0;
    doubleGun = false;
    homingGun = false;
    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;
    updateDirection();
}

void SpaceShip::droneMode(Vector asteroid, float deltaTime) {
    drone = true;
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

void SpaceShip::render(bool moveIsPressed) {
    glColor3f(1.0, 1.0, 1.0); // Spaceship
	glBegin(GL_LINE_LOOP); // Outline
	glVertex3f(position.x + 0.04 * sin(rotation * PI / 180), position.y - 0.04 * cos(rotation * PI / 180), 0.0); // Front
	glVertex3f(position.x + 0.04 * sin((rotation - 140) * PI / 180), position.y - 0.04 * cos((rotation - 140) * PI / 180),  0.0); // Back left corner
    glVertex3f(position.x + (-0.02) * sin(rotation * PI / 180), position.y - (-0.02) * cos(rotation * PI / 180), 0.0); // Back
    glVertex3f(position.x + 0.04 * sin(rotation * PI / 180), position.y - 0.04 * cos(rotation * PI / 180), 0.0); // Front
    glVertex3f(position.x + 0.04 * sin((rotation + 140) * PI / 180), position.y - 0.04 * cos((rotation + 140) * PI / 180), 0.0); // Back right corner
    glVertex3f(position.x + (-0.02) * sin(rotation * PI / 180), position.y - (-0.02) * cos(rotation * PI / 180), 0.0); // Back
    glEnd();
    if (drone) {
        glColor3f(0.0, 0.5, 1.0);
    } else {
        glColor3f(0.0, 0.8, 0.0);
    }
    glBegin(GL_POLYGON); // Fill
	glVertex3f(position.x + 0.04 * sin(rotation * PI / 180), position.y - 0.04 * cos(rotation * PI / 180), 0.0); // Front
	glVertex3f(position.x + 0.04 * sin((rotation - 140) * PI / 180), position.y - 0.04 * cos((rotation - 140) * PI / 180),  0.0); // Back left corner
    glVertex3f(position.x + (-0.02) * sin(rotation * PI / 180), position.y - (-0.02) * cos(rotation * PI / 180), 0.0); // Back
    glVertex3f(position.x + 0.04 * sin((rotation + 140) * PI / 180), position.y - 0.04 * cos((rotation + 140) * PI / 180), 0.0); // Back right corner
	glEnd();
    glColor3f(1.0, 1.0, 1.0); // Engine
	glBegin(GL_LINE_LOOP); // Outline
    glVertex3f(position.x + (-0.02) * sin(rotation * PI / 180), position.y - (-0.02) * cos(rotation * PI / 180), 0.0); // Back
	glVertex3f(position.x + 0.028 * sin((rotation - 145) * PI / 180), position.y - 0.028 * cos((rotation - 145) * PI / 180),  0.0); // Upper left corner
    glVertex3f(position.x + 0.035 * sin((rotation - 160) * PI / 180), position.y - 0.035 * cos((rotation - 160) * PI / 180),  0.0); // Lower left corner
	glVertex3f(position.x + 0.035 * sin((rotation + 160) * PI / 180), position.y - 0.035 * cos((rotation + 160) * PI / 180),  0.0); // Lower right corner
    glVertex3f(position.x + 0.028 * sin((rotation + 145) * PI / 180), position.y - 0.028 * cos((rotation + 145) * PI / 180),  0.0); // Upper right corner
	glEnd();
    glColor3f(0.4, 0.4, 0.4); 
    glBegin(GL_POLYGON); // Fill
    glVertex3f(position.x + (-0.02) * sin(rotation * PI / 180), position.y - (-0.02) * cos(rotation * PI / 180), 0.0); // Back
	glVertex3f(position.x + 0.028 * sin((rotation - 145) * PI / 180), position.y - 0.028 * cos((rotation - 145) * PI / 180),  0.0); // Upper left corner
    glVertex3f(position.x + 0.035 * sin((rotation - 160) * PI / 180), position.y - 0.035 * cos((rotation - 160) * PI / 180),  0.0); // Lower left corner
	glVertex3f(position.x + 0.035 * sin((rotation + 160) * PI / 180), position.y - 0.035 * cos((rotation + 160) * PI / 180),  0.0); // Lower right corner
    glVertex3f(position.x + 0.028 * sin((rotation + 140) * PI / 180), position.y - 0.028 * cos((rotation + 145) * PI / 180),  0.0); // Upper right corner
	glEnd();

    if (moveIsPressed) {
        glColor3f(1.0, 0.0, 0.0); // Flame
        glBegin(GL_POLYGON);
        glVertex3f(position.x + 0.035 * sin((rotation - 160) * PI / 180), position.y - 0.035 * cos((rotation - 160) * PI / 180),  0.0); // Upper left corner
        glVertex3f(position.x + 0.035 * sin((rotation + 160) * PI / 180), position.y - 0.035 * cos((rotation + 160) * PI / 180),  0.0); // Upper right corner
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(position.x + 0.042 * sin((rotation + 168) * PI / 180), position.y - 0.042 * cos((rotation + 168) * PI / 180),  0.0); // Lower right corner
        glVertex3f(position.x + 0.042 * sin((rotation - 168) * PI / 180), position.y - 0.042 * cos((rotation - 168) * PI / 180),  0.0); // Lower left corner
        glEnd();
    }
    if (doubleGun || homingGun) {
        glColor3f(1.0, 1.0, 1.0); // Gun outline
        glBegin(GL_LINE_LOOP); // Left gun barrel
        glVertex3f(position.x + 0.015 * sin((rotation + 38) * PI / 180), position.y - 0.015 * cos((rotation + 38) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.033 * sin((rotation + 17) * PI / 180), position.y - 0.033 * cos((rotation + 17) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.034 * sin((rotation + 22) * PI / 180), position.y - 0.034 * cos((rotation + 22) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.013 * sin((rotation + 83) * PI / 180), position.y - 0.013 * cos((rotation + 83) * PI / 180),  0.0); 
        glEnd();
        glBegin(GL_LINE_LOOP); // Right gun barrel
        glVertex3f(position.x + 0.015 * sin((rotation - 38) * PI / 180), position.y - 0.015 * cos((rotation - 38) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.033 * sin((rotation - 17) * PI / 180), position.y - 0.033 * cos((rotation - 17) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.034 * sin((rotation - 22) * PI / 180), position.y - 0.034 * cos((rotation - 22) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.013 * sin((rotation - 83) * PI / 180), position.y - 0.013 * cos((rotation - 83) * PI / 180),  0.0); 
        glEnd();
        glColor3f(0.4, 0.4, 0.4); // Gun fill
        glBegin(GL_POLYGON); // Left gun barrel
        glVertex3f(position.x + 0.015 * sin((rotation + 38) * PI / 180), position.y - 0.015 * cos((rotation + 38) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.033 * sin((rotation + 17) * PI / 180), position.y - 0.033 * cos((rotation + 17) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.034 * sin((rotation + 22) * PI / 180), position.y - 0.034 * cos((rotation + 22) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.013 * sin((rotation + 83) * PI / 180), position.y - 0.013 * cos((rotation + 83) * PI / 180),  0.0); 
        glEnd();
        glBegin(GL_POLYGON); // Right gun barrel
        glVertex3f(position.x + 0.015 * sin((rotation - 38) * PI / 180), position.y - 0.015 * cos((rotation - 38) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.033 * sin((rotation - 17) * PI / 180), position.y - 0.033 * cos((rotation - 17) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.034 * sin((rotation - 22) * PI / 180), position.y - 0.034 * cos((rotation - 22) * PI / 180),  0.0); 
        glVertex3f(position.x + 0.013 * sin((rotation - 83) * PI / 180), position.y - 0.013 * cos((rotation - 83) * PI / 180),  0.0); 
        glEnd();

        if (doubleGun) {
            glColor3f(1.0, 1.0, 1.0); // Outline
            glBegin(GL_LINE_LOOP); // Left gun design
            glVertex3f(position.x + 0.034 * sin((rotation + 22) * PI / 180), position.y - 0.034 * cos((rotation + 22) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.013 * sin((rotation + 90) * PI / 180), position.y - 0.013 * cos((rotation + 90) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.026 * sin((rotation + 55) * PI / 180), position.y - 0.026 * cos((rotation + 55) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.021 * sin((rotation + 28) * PI / 180), position.y - 0.021 * cos((rotation + 28) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.025 * sin((rotation + 47) * PI / 180), position.y - 0.025 * cos((rotation + 47) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation + 40) * PI / 180), position.y - 0.034 * cos((rotation + 40) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation + 21) * PI / 180), position.y - 0.034 * cos((rotation + 21) * PI / 180),  0.0); 
            glEnd();
            glBegin(GL_LINE_LOOP); // Right gun design
            glVertex3f(position.x + 0.034 * sin((rotation - 22) * PI / 180), position.y - 0.034 * cos((rotation - 22) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.013 * sin((rotation - 90) * PI / 180), position.y - 0.013 * cos((rotation - 90) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.026 * sin((rotation - 55) * PI / 180), position.y - 0.026 * cos((rotation - 55) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.021 * sin((rotation - 28) * PI / 180), position.y - 0.021 * cos((rotation - 28) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.025 * sin((rotation - 47) * PI / 180), position.y - 0.025 * cos((rotation - 47) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation - 40) * PI / 180), position.y - 0.034 * cos((rotation - 40) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation - 21) * PI / 180), position.y - 0.034 * cos((rotation - 21) * PI / 180),  0.0); 
            glEnd();
            glColor3f(0.0, 0.5, 0.0); // Fill
            glBegin(GL_POLYGON); // Left gun design
            glVertex3f(position.x + 0.034 * sin((rotation + 22) * PI / 180), position.y - 0.034 * cos((rotation + 22) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.013 * sin((rotation + 90) * PI / 180), position.y - 0.013 * cos((rotation + 90) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.026 * sin((rotation + 55) * PI / 180), position.y - 0.026 * cos((rotation + 55) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.021 * sin((rotation + 28) * PI / 180), position.y - 0.021 * cos((rotation + 28) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.025 * sin((rotation + 47) * PI / 180), position.y - 0.025 * cos((rotation + 47) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation + 40) * PI / 180), position.y - 0.034 * cos((rotation + 40) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation + 21) * PI / 180), position.y - 0.034 * cos((rotation + 21) * PI / 180),  0.0); 
            glEnd();
            glBegin(GL_POLYGON); // Right gun design
            glVertex3f(position.x + 0.034 * sin((rotation - 22) * PI / 180), position.y - 0.034 * cos((rotation - 22) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.013 * sin((rotation - 90) * PI / 180), position.y - 0.013 * cos((rotation - 90) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.026 * sin((rotation - 55) * PI / 180), position.y - 0.026 * cos((rotation - 55) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.021 * sin((rotation - 28) * PI / 180), position.y - 0.021 * cos((rotation - 28) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.025 * sin((rotation - 47) * PI / 180), position.y - 0.025 * cos((rotation - 47) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation - 40) * PI / 180), position.y - 0.034 * cos((rotation - 40) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.034 * sin((rotation - 21) * PI / 180), position.y - 0.034 * cos((rotation - 21) * PI / 180),  0.0); 
            glEnd();
        }
        if (homingGun) {
            glColor3f(1.0, 1.0, 1.0); // Gun outline
            glBegin(GL_LINES); // Left design
            glVertex3f(position.x + 0.033 * sin((rotation + 17) * PI / 180), position.y - 0.033 * cos((rotation + 17) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.04 * sin(rotation * PI / 180), position.y - 0.04 * cos(rotation * PI / 180), 0.0);
            glEnd();
            glBegin(GL_LINES); // Right design
            glVertex3f(position.x + 0.033 * sin((rotation - 17) * PI / 180), position.y - 0.033 * cos((rotation - 17) * PI / 180),  0.0); 
            glVertex3f(position.x + 0.04 * sin(rotation * PI / 180), position.y - 0.04 * cos(rotation * PI / 180), 0.0);
            glEnd();
        }
    }
}
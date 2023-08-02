#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "Headers/boundary.h"


Boundary::Boundary() {
}

void Boundary::render_warning(int width, int height, float ship_xPos, float ship_yPos) {
    float w_ratio = (float)width/height;
    float h_ratio = (float)height/width;
    glColor3f(1.0, 0.0, 0.0);
    // Left boundary
    glBegin(GL_POLYGON);
    if (width > height && ship_xPos <= -w_ratio * 0.75) {
        glVertex3f(-w_ratio, 1, 0.0); // Top left
        glVertex3f(-w_ratio + 0.03, 1, 0.0); // Top right
        glVertex3f(-w_ratio + 0.03, -1, 0.0); // Bottom right
        glVertex3f(-w_ratio, -1, 0.0); // Bottom left
    } else if (width <= height && ship_xPos <= -1 * 0.75) {
        glVertex3f(-1, h_ratio, 0.0); // Top Left
        glVertex3f(-1 + 0.03, h_ratio, 0.0); // Top right
        glVertex3f(-1 + 0.03, -h_ratio, 0.0); // Bottom right
        glVertex3f(-1, -h_ratio, 0.0); // Bottom left
    }       
    glEnd();
    // Right boundary
    glBegin(GL_POLYGON);
    if (width > height && ship_xPos >= w_ratio * 0.75) {
        glVertex3f(w_ratio, 1, 0.0); // Top right
        glVertex3f(w_ratio - 0.03, 1, 0.0); // Top left
        glVertex3f(w_ratio - 0.03, -1, 0.0); // Bottom left
        glVertex3f(w_ratio, -1, 0.0); // Bottom right
    } else if (width <= height && ship_xPos >= 1 * 0.75) {
        glVertex3f(1, h_ratio, 0.0); // Top right
        glVertex3f(1 - 0.03, h_ratio, 0.0); // Top left
        glVertex3f(1 - 0.03, -h_ratio, 0.0); // Bottom left
        glVertex3f(1, -h_ratio, 0.0); // Bottom right
    }
    glEnd();
    // Top boundary
    glBegin(GL_POLYGON);
    if (height > width && ship_yPos >= h_ratio * 0.75) {
        glVertex3f(-1, h_ratio, 0.0); // Top Left
        glVertex3f(1, h_ratio, 0.0); // Top right
        glVertex3f(1, h_ratio - 0.03, 0.0); // Bottom left
        glVertex3f(-1, h_ratio - 0.03, 0.0); // Bottom right
    } else if (height <= width && ship_yPos >= 1 * 0.75) {
        glVertex3f(-w_ratio, 1, 0.0); // Top Left
        glVertex3f(w_ratio, 1, 0.0); // Top right
        glVertex3f(w_ratio, 0.97, 0.0); // Bottom right
        glVertex3f(-w_ratio, 0.97, 0.0); // Bottom left
    }
    glEnd();
    // Bottom boundary
    glBegin(GL_POLYGON);
    if (height > width && ship_yPos <= -h_ratio * 0.75) {
        glVertex3f(-1, -h_ratio, 0.0); // Bottom Left
        glVertex3f(1, -h_ratio, 0.0); // Bottom right
        glVertex3f(1, -h_ratio + 0.03, 0.0); // Top right
        glVertex3f(-1, -h_ratio + 0.03, 0.0); // Top left
    } else if (height <= width && ship_yPos <= -1 * 0.75) {
        glVertex3f(-w_ratio, -1, 0.0); // Bottom Left
        glVertex3f(w_ratio, -1, 0.0); // Bottom right
        glVertex3f(w_ratio, -0.97, 0.0); // Top right
        glVertex3f(-w_ratio, -0.97, 0.0); // Top left
    }
	glEnd();
}

void Boundary::delete_bullets(std::vector<Bullet*> &bullets, int width, int height) {
	for (int i = 0; i < bullets.size(); i++) {
		if ((width > height) && 
			((bullets[i]->getPosition().x >= (float)width/height) ||
			(bullets[i]->getPosition().x <= -(float)width/height) ||
			(bullets[i]->getPosition().y >= 1 || (bullets[i]->getPosition().y <= -1)))) {
			bullets.erase(bullets.begin() + i);
		}
		else if ((width < height) && 
			((bullets[i]->getPosition().x >= 1) || (bullets[i]->getPosition().x <= -1) ||
			(bullets[i]->getPosition().y >= (float)height/width || 
			(bullets[i]->getPosition().y <= -(float)height/width)))) {
			bullets.erase(bullets.begin() + i);
		}
		else if ((width == height) && 
			((bullets[i]->getPosition().x >= 1) || (bullets[i]->getPosition().x <= -1) ||
			(bullets[i]->getPosition().y >= 1 || (bullets[i]->getPosition().y <= -1)))) {
			bullets.erase(bullets.begin() + i);
		}
	}
}

void Boundary::check_asteroids_bounce(std::vector<Asteroid*> &asteroids, int width, int height) {
    for (int i = 0; i < asteroids.size(); i++) {
		if (width > height) {
            if ((asteroids[i]->getPosition().y + asteroids[i]->getRadius() >= 1) ||
            (asteroids[i]->getPosition().y - asteroids[i]->getRadius() <= -1)) {
                asteroids[i]->verticalBounce();
            }
            if ((asteroids[i]->getPosition().x + asteroids[i]->getRadius() >= (float)width/height) ||
            (asteroids[i]->getPosition().x - asteroids[i]->getRadius() <= -(float)width/height)) {
                asteroids[i]->horizontalBounce();
            }
        }
    }
}

bool Boundary::check_space_ship_crash(SpaceShip* spaceShip, int width, int height) {
	if ((width > height) && 
			((spaceShip->getPosition().x >= (float)width/height) ||
			(spaceShip->getPosition().x <= -(float)width/height) ||
			(spaceShip->getPosition().y >= 1 || (spaceShip->getPosition().y <= -1)))) {
        return true;
	}
	else if ((width < height) && 
			((spaceShip->getPosition().x >= 1 || (spaceShip->getPosition().x <= -1 ||
			(spaceShip->getPosition().y >= (float)height/width) || 
			(spaceShip->getPosition().y <= -(float)height/width))))) {
        return true;
	}
	else if ((width == height) && 
			((spaceShip->getPosition().x >= 1) || (spaceShip->getPosition().x <= -1) ||
			(spaceShip->getPosition().y >= 1) || (spaceShip->getPosition().y <= -1))) {
        return true;
	}
    return false;
}
#ifndef BOUNDARY_CLASS_H
#define BOUNDARY_CLASS_H

#include <vector>
#include "bullet.h"
#include "space_ship.h"
#include "asteroid.h"


class Boundary {
    public:
        Boundary();
        void render_warning(int width, int height, float ship_xPos, float ship_yPos);
        void delete_bullets(std::vector<Bullet*> &bullets, int width, int height);
        void check_asteroids_bounce(std::vector<Asteroid*> &asteroids, int width, int height);
        bool check_space_ship_crash(SpaceShip* spaceShip, int width, int height);
};
#endif

#include <math.h>
#include <iostream>
#include <vector>


#include "Headers/space_ship.h"
#include "Headers/bullet.h"
#include "Headers/boundary.h"
#include "Headers/asteroid.h"
#include "Headers/particle.h"
#include "Headers/engine_particle.h"

#define GL_SILENCE_DEPRECATION

#if _WIN32
#include <windows.h>
#endif
#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define GAME_TITLE "Asteroid Arena"
#define KEY_ESC 27


typedef struct {
	int width, height, x_pos, y_pos;
	bool is_fullscreen;
} window_t;

window_t g_mainwin;
SpaceShip* spaceShip;
Boundary* boundary;
std::vector<Bullet*> bullets;
std::vector<Asteroid*> asteroids;
std::vector<SpaceShip*> drones;
std::vector<Particle*> explosionParticles;
std::vector<EngineParticle*> engineParticles;

int level = 0;
int score = 0;
float gameTimer;
int minutes, seconds = 0;
float deltaTime;
float previousTime;
float dronePreviousTimes[2] = {};
float droneTimer[2] = {};
bool setUp = true;
bool newGame = false;
bool oneTime = true;
bool isPlaying = false;
bool gameOver = false;
bool W_IsPressed = false;
bool D_IsPressed = false;
bool A_IsPressed = false;
bool left_mouse_IsPressed = false;


void end_app()
{
	delete spaceShip;
	delete boundary;
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	for (int i = 0; i < drones.size(); i++) {
		delete drones[i];
	}
	for (int i = 0; i < explosionParticles.size(); i++) {
		delete explosionParticles[i];
	}
	for (int i = 0; i < engineParticles.size(); i++) {
		delete engineParticles[i];
	}
	exit(EXIT_SUCCESS);
}

void createAsteroids() {
	for (int i = 0; i < level; i++) {
		asteroids.push_back(new Asteroid(g_mainwin.width, g_mainwin.height, spaceShip->getPosition(),3,1,spaceShip->getPosition(),0.2,false,3,0.0,0));
	}
}

void removeAsteroids() {
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids.erase(asteroids.begin());
	}
	if (asteroids.size() == 0) {
		setUp = false;
		newGame = true;
	}
}

void render_string(float x, float y, void *font, const char *string)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Set the projection - use this to render text
	gluOrtho2D(0, g_mainwin.width, 0, g_mainwin.height);
	glMatrixMode(GL_MODELVIEW);

	glRasterPos2f(x, y);
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}

	glMatrixMode(GL_PROJECTION);

	// Restore the original projection matrix for rendering everything else
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

float calculate_deltaTime() {
	float ct, dt;
	static float last_t = 0.0;
	ct = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	dt = ct - last_t;
	last_t = ct;

	return dt;
}

bool collision(float distance, Vector v1, Vector v2) {
	float x = v1.x - v2.x;
	float y = v1.y - v2.y;
	return distance * distance >= (x * x) + (y * y);
}

float distance(Vector v1, Vector v2) {
	float x = v1.x - v2.x;
	float y = v1.y - v2.y;
	return (x * x) + (y * y);
}

Vector closest_asteroid(std::vector<Asteroid*> &asteroids, Vector position) {
	Vector closest_asteroid = {0.0, 0.0, 0.0};
	float closest_distance = 100.0;

	for (int i = 0; i < asteroids.size(); i++) {
		if (distance(position, asteroids[i]->getPosition()) < closest_distance) {
			closest_distance = distance(position, asteroids[i]->getPosition());
			closest_asteroid = asteroids[i]->getPosition();
		}
	}
	return closest_asteroid;
}

void explosion(Vector position) {
	int numberOfParticles = rand() % 50 + 100; // 100 to 150
	for (int k = 0; k < numberOfParticles; k++) {
		explosionParticles.push_back(new Particle(position));
	}
}

void check(std::vector<Bullet*> &bullets, std::vector<Asteroid*> &asteroids) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->render();
		bullets[i]->move(deltaTime);
		if (spaceShip->isHomingGun()) {
			bullets[i]->track(closest_asteroid(asteroids, bullets[i]->getPosition()), deltaTime);
		}
	}
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i]->render();
		asteroids[i]->rotate(deltaTime);
		asteroids[i]->move(deltaTime, g_mainwin.width, g_mainwin.height);
	}
	for (int j = 0; j < asteroids.size(); j++) {
		if (collision(asteroids[j]->getRadius() + 0.04, asteroids[j]->getPosition(), spaceShip->getPosition())) {
			// Asteroids collide with spaceShip
			explosion(spaceShip->getPosition());
			spaceShip->setLives(spaceShip->getLives()-1);
			isPlaying = false;
			if (spaceShip->getLives() > 0) {
				setUp = true;
			}
			return;
		}
		for (int i = 0; i < drones.size(); i++) {
			if (collision(asteroids[j]->getRadius() + 0.04, asteroids[j]->getPosition(), drones[i]->getPosition())) {
				// Asteroid collides with drone
				explosion(drones[i]->getPosition());
				drones.erase(drones.begin() + i);
				if (i == 0) {
					droneTimer[0] = droneTimer[1];
				}
			}
		}
		for (int i = 0; i < asteroids.size(); i++) {
			if (j != i && collision(asteroids[j]->getRadius() + asteroids[i]->getRadius(), asteroids[j]->getPosition(), asteroids[i]->getPosition())) {
				// Asteroids collide together
				asteroids[j]->collisionWithOtherAsteroid();
				asteroids[i]->collisionWithOtherAsteroid();
			}
		}
		for (int i = 0; i < bullets.size(); i++) {
			if (collision(asteroids[j]->getRadius(), bullets[i]->getPosition(), asteroids[j]->getPosition())) {
				// Asteroid collides with bullet
				bullets.erase(bullets.begin() + i);
				score += 10;
				asteroids[j]->reduceHealth();
				if (asteroids[j]->getHealth() <= 0) {
					score += 20;
					explosion(asteroids[j]->getPosition());
					if (asteroids[j]->getLevel() > 1) {
						asteroids.push_back(new Asteroid(g_mainwin.width, g_mainwin.height, spaceShip->getPosition(),asteroids[j]->getLevel() - 1,0,asteroids[j]->getPosition(),(float)asteroids[j]->getRadius()/2,asteroids[j]->isInArena(),asteroids[j]->getLevel()-1,asteroids[j]->getRotation()+45.0,0));
						asteroids.push_back(new Asteroid(g_mainwin.width, g_mainwin.height, spaceShip->getPosition(),asteroids[j]->getLevel() - 1,0,asteroids[j]->getPosition(),(float)asteroids[j]->getRadius()/2,asteroids[j]->isInArena(),asteroids[j]->getLevel()-1,asteroids[j]->getRotation()-45.0,1));
					}
					asteroids.erase(asteroids.begin() + j);
				}
			}
		}	
	}
}

void render_frame() {
	deltaTime = calculate_deltaTime();
	for (int i = 0; i < explosionParticles.size(); i++) {
		explosionParticles[i]->render();
		explosionParticles[i]->move(deltaTime);
		if (explosionParticles[i]->maxDistanceTravelled()) {
			explosionParticles.erase(explosionParticles.begin() + i);
		}
	}
	if (gameOver) {
		std::string temp = std::to_string(score).c_str();
		char const *score_as_char = temp.c_str();
		glColor3f(1.0, 1.0, 1.0);
		render_string(g_mainwin.width/2, g_mainwin.height/2, GLUT_BITMAP_TIMES_ROMAN_24, "Score: ");
		render_string(g_mainwin.width/2 + 40, g_mainwin.height/2, GLUT_BITMAP_TIMES_ROMAN_24, score_as_char);
		render_string(g_mainwin.width/2, g_mainwin.height/2 - 20, GLUT_BITMAP_TIMES_ROMAN_24, "Play Again?");
		render_string(g_mainwin.width/2, g_mainwin.height/2 - 40, GLUT_BITMAP_TIMES_ROMAN_24, "Y - New Game");
		render_string(g_mainwin.width/2, g_mainwin.height/2 - 60, GLUT_BITMAP_TIMES_ROMAN_24, "Esc - Quit");
		for (int i = 0; i < asteroids.size(); i++) {
			asteroids[i]->render();
			asteroids[i]->rotate(deltaTime);
			asteroids[i]->move(deltaTime, g_mainwin.width, g_mainwin.height);
		}
	}
	if (setUp) {
		if (asteroids.size() > 0) {
			asteroids.erase(asteroids.begin());
		}
		if (bullets.size() > 0) {
			bullets.erase(bullets.begin());
		}
		if (asteroids.size() == 0 && bullets.size() == 0) {
			spaceShip->reset();
			setUp = false;
			newGame = true;
			gameTimer = glutGet(GLUT_ELAPSED_TIME) / 1000;
			minutes = 0;
			seconds = 0;
		}
	}
	if (newGame) {
		if (oneTime) {
			glColor3f(1.0, 1.0, 1.0);
			render_string(g_mainwin.width/2-50, g_mainwin.height/2 - 20, GLUT_BITMAP_TIMES_ROMAN_24, "ASTEROID ARENA");
			render_string(g_mainwin.width/2-50, g_mainwin.height/2 - 40, GLUT_BITMAP_TIMES_ROMAN_24, "left click to start..");
		} else {
			createAsteroids();
			newGame = false;
			isPlaying = true;
		}
	}
	if (isPlaying) {
		seconds = int((glutGet(GLUT_ELAPSED_TIME) / 1000) - gameTimer);
		if (seconds >= 60) {
			gameTimer = int(glutGet(GLUT_ELAPSED_TIME) / 1000);
			minutes += 1;
		}
		check(bullets, asteroids);
		std::string temp = std::to_string(score).c_str();
		char const *score_as_char = temp.c_str();
		std::string temp2 = std::to_string(spaceShip->getLives()).c_str();
		char const *lives_as_char = temp2.c_str();
		std::string temp3 = std::to_string(level).c_str();
		char const *level_as_char = temp3.c_str();
		std::string temp4 = std::to_string(seconds).c_str();
		char const *seconds_as_char = temp4.c_str();
		std::string temp5 = std::to_string(minutes).c_str();
		char const *minutes_as_char = temp5.c_str();
		glColor3f(1.0, 1.0, 1.0);
		render_string(50, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, "Lives: ");
		render_string(90, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, lives_as_char);
		render_string(160, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, "Score: ");
		render_string(200, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, score_as_char);
		render_string(50, g_mainwin.height -80, GLUT_BITMAP_TIMES_ROMAN_24, "Level: ");
		render_string(90, g_mainwin.height -80, GLUT_BITMAP_TIMES_ROMAN_24, level_as_char);
		render_string(g_mainwin.width -170, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, "Time: ");
		render_string(g_mainwin.width -130, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, minutes_as_char);
		render_string(g_mainwin.width -120, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, ":");
		render_string(g_mainwin.width -110, g_mainwin.height -50, GLUT_BITMAP_TIMES_ROMAN_24, seconds_as_char);
		boundary->render_warning(g_mainwin.width, g_mainwin.height, spaceShip->getPosition().x, spaceShip->getPosition().y);
		spaceShip->render(W_IsPressed);
		for (int i = 0; i < drones.size(); i++) {
			drones[i]->render(false);
			drones[i]->droneMode(closest_asteroid(asteroids, drones[i]->getPosition()), deltaTime);
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - dronePreviousTimes[i]  > 0.34) {
				bullets.push_back(new Bullet(drones[i]->getPosition().x + 0.04 * sin(drones[i]->getRotation() * PI / 180), drones[i]->getPosition().y - 0.04 * cos(drones[i]->getRotation() * PI / 180), drones[i]->getRotation(), 0.006, false));
				dronePreviousTimes[i] = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - droneTimer[i] > 10.0) {
				drones.erase(drones.begin() + i);
				droneTimer[0] = droneTimer[1];
			}
		}
		for (int i = 0; i < engineParticles.size(); i++) {
			engineParticles[i]->render();
			engineParticles[i]->move(deltaTime);
			if (engineParticles[i]->maxDistanceTravelled()) {
				engineParticles.erase(engineParticles.begin() + i);
			}
		}
	}
}

void on_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	render_frame();

	int err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("error: %s\n", gluErrorString(err));
	}

	glutSwapBuffers();
}

void update_game_state() {
	if (spaceShip->getLives() <= 0) {
		isPlaying = false;
		gameOver = true;
	}
	if (asteroids.size() <= 0 && isPlaying) {
		isPlaying = false;
		setUp = true;
		level += 1;
	}
}

void on_idle() {
	update_game_state();
	if (boundary->check_space_ship_crash(spaceShip, g_mainwin.width, g_mainwin.height)) {
		explosion(spaceShip->getPosition());
		spaceShip->reset();
		isPlaying = false;
		setUp = true;
		spaceShip->setLives(spaceShip->getLives()-1);
	}
	boundary->check_asteroids_bounce(asteroids, g_mainwin.width, g_mainwin.height);
	boundary->delete_bullets(bullets, g_mainwin.width, g_mainwin.height);
	glutPostRedisplay();

	if (W_IsPressed) {
		spaceShip->accelerate(deltaTime);
		engineParticles.push_back(new EngineParticle(spaceShip->getEnginePosition(),spaceShip->getEngineDirection()));
		//explosionParticles.push_back(new Particle(position, 1, 0.0));
	}
	if (!W_IsPressed && (spaceShip->getSpeed().x != 0 || spaceShip->getSpeed().y != 0)) {
		spaceShip->decelerate(deltaTime);
	}
	if (A_IsPressed) {
		spaceShip->rotateCounterClockwise(deltaTime);
	}
	if (D_IsPressed) {
		spaceShip->rotateClockwise(deltaTime);
	}
	if (left_mouse_IsPressed) {
		if (spaceShip->isDoubleGun()) {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.5) {
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin((spaceShip->getRotation() + 28) * PI / 180), spaceShip->getPosition().y - 0.04 * cos((spaceShip->getRotation() + 28) * PI / 180), spaceShip->getRotation(), 0.006, false));
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin((spaceShip->getRotation() - 28) * PI / 180), spaceShip->getPosition().y - 0.04 * cos((spaceShip->getRotation() - 28) * PI / 180), spaceShip->getRotation(), 0.006, false));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		} else if (spaceShip->isHomingGun()) {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.8) {
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin(spaceShip->getRotation() * PI / 180), spaceShip->getPosition().y - 0.04 * cos(spaceShip->getRotation() * PI / 180), spaceShip->getRotation(), 0.012, true));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		} else {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.34) {
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin(spaceShip->getRotation() * PI / 180), spaceShip->getPosition().y - 0.04 * cos(spaceShip->getRotation() * PI / 180), spaceShip->getRotation(), 0.006, false));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		}
	}
}

void on_key_press(unsigned char key, int x, int y) {	
	if (key == 'w' || key == 'W') {
		W_IsPressed = true;
	}
	if (key == 'a' || key == 'A') {
		A_IsPressed = true;
	}
	if (key == 'd' || key == 'D') {
		D_IsPressed = true;
	}
	if (key == 'z' || key == 'Z') {
		if (spaceShip->isDoubleGun()) {
			spaceShip->switchDoubleGun();
			spaceShip->switchHomingGun();
		} else if (spaceShip->isHomingGun()) {
			spaceShip->switchHomingGun();
		} else {
			spaceShip->switchDoubleGun();
		}
	}
	if ((key == 'x' || key == 'X') && isPlaying) {
		if (drones.size() < 2 && score >= 100) {
			SpaceShip* drone = new SpaceShip();
			drone->setPosition(spaceShip->getPosition());
			drones.push_back(drone);
			droneTimer[drones.size()-1] = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			score -= 100;
		}
	}
	if ((key == 'y' || key == 'Y') && gameOver) {
		score = 0;
		level = 1;
		spaceShip->reset();
		spaceShip->setLives(3);
		setUp = true;
		gameOver = false;
	}
	if (key == KEY_ESC) {
		std::cout << "Goodbye Astroid!" << std::endl;
		end_app();
	}
}

void on_key_release(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'W') {
		W_IsPressed = false;
	}
	if (key == 'a' || key == 'A') {
		A_IsPressed = false;
	}
	if (key == 'd' || key == 'D') {
		D_IsPressed = false;
	}
}

void on_mouse_button(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isPlaying) {
		left_mouse_IsPressed = true;
		if (spaceShip->isDoubleGun()) {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.5) {
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin((spaceShip->getRotation() + 28) * PI / 180), spaceShip->getPosition().y - 0.04 * cos((spaceShip->getRotation() + 28) * PI / 180), spaceShip->getRotation(), 0.006, false));
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin((spaceShip->getRotation() - 28) * PI / 180), spaceShip->getPosition().y - 0.04 * cos((spaceShip->getRotation() - 28) * PI / 180), spaceShip->getRotation(), 0.006, false));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		} else if (spaceShip->isHomingGun()) {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.8) {
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin(spaceShip->getRotation() * PI / 180), spaceShip->getPosition().y - 0.04 * cos(spaceShip->getRotation() * PI / 180), spaceShip->getRotation(), 0.012, true));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		} else {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.34) {
				bullets.push_back(new Bullet(spaceShip->getPosition().x + 0.04 * sin(spaceShip->getRotation() * PI / 180), spaceShip->getPosition().y - 0.04 * cos(spaceShip->getRotation() * PI / 180), spaceShip->getRotation(), 0.006, false));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		}
	}
	if (state == GLUT_UP) {
		left_mouse_IsPressed = false;
		if (newGame) {
			createAsteroids();
			newGame = false;
			oneTime = false;
			isPlaying = true;
		}
	}
}

void on_reshape(int w, int h) {
	g_mainwin.width = w;
	g_mainwin.height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) {
		float aspect = (float)h / (float)w;
		glOrtho(-1.0, 1.0, -1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	} else {
		float aspect = (float)w / (float)h;
		glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init_app(int *argcp, char **argv, window_t *mainwinp) {
	// GLUT & OpenGL setup
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// Display related setup
	glutInitWindowPosition(mainwinp->x_pos, mainwinp->y_pos);
	glutInitWindowSize(mainwinp->width, mainwinp->height);
	glutCreateWindow(GAME_TITLE);

	if (mainwinp->is_fullscreen == true) {
		glutFullScreen();
	}

	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);

	// Keyboard and Mouse related setup
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardUpFunc(on_key_release);
	glutKeyboardFunc(on_key_press);
	// glutSpecialFunc(on_special_key_press);
	// glutSpecialUpFunc(on_special_key_release);
	glutMouseFunc(on_mouse_button);
	// glutPassiveMotionFunc(on_mouse_move);
	// glutMotionFunc(on_mouse_drag);

	// Define the idle function
	glutIdleFunc(on_idle);
}

void load_config(int *argcp, char **argv, window_t *mainwin_p) {
	mainwin_p->width = 1440;
	mainwin_p->height = 900;
	mainwin_p->is_fullscreen = true;
}

int main(int argc, char** argv) {
	srand((int) time(0));
	std::cout << "Hello Astroid!" << std::endl;
	spaceShip = new SpaceShip();
	boundary = new Boundary();

	load_config(&argc, argv, &g_mainwin);
	init_app(&argc, argv, &g_mainwin);
	glutMainLoop();
}
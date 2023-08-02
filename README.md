# Third Year University Assignment
By Quoc Tran (s3827826)

### How to build:
    Install C/C++ Compiler and OpenGL Libraries
    Run the command 'bash build.sh'

### Controls:
    W - Move forward
    A - Rotate counter clockwise
    D - Rotate clockwise 
    Left Click - Shoot 
    Z - Change guns 
    X - Spawn friendly drone 

## Features Implemented:
### LEVEL 1:
    1.1 - Screen Mode
    1.2 - The Arena
    1.3 - Ready Player 1
    1.4 - Starting Location
    1.5 - Player Movement
    1.6 - Hit The Wall
    1.7 - Shooting
    1.8 - Math Structs/Classes and Functions
    1.9 - Code Quality
### LEVEL 2:
    2.1 - Launch Position
    2.2 - Asteroid Launch
    2.3 - Gun Rendering & Shooting
    2.4 - Asteroid/ Ship/ Bullet Collision
    2.5 - Multiple Asteroids
### LEVEL 3:
    3.1 - Procedurally Generated Asteroids
    3.2 - Rotation Asteroids
    3.3 - Gun Switching (There are three gun modes. Normal, double, and homing)
    3.4 - Hit Points (Large asteroid 3hp, medium asteroid 2hp, small asteroid 1hp)
    3.5 - Time and Score (Couldn't figure out how to pad single digit seconds with a 0)
    3.6 - Game Over, Man (Start game - "right click to play", Game over - "Y to play again, Esc to quit")
### LEVEL 4:
    4.1 - Bouncies
    4.3 - Do The Splits (Large asteroid splits into two medium asteroids. Medium asteroid splits into two small asteroids)
    4.4 - Earth Shattering Kaboom (Explosion particle effects and spaceship engine particle effects)
### BONUS FEATURES:
    B.1 - Better Ship Movement
    B.2 - Spawn Drone (Drone lasts 10 seconds and costs 100 points to create. Maximum of 2 drones)

## Features not successfully implemented:
### 4.2 - Bouncies 2
    _main.cpp line 205 (Checking for collision) 
    asteroid.cpp line 157 (Function to handle collision)
    Was planning to work out each case asteroids could collide and work out the angle of reflection.
    It became too long winded in the end. 
    Final intuition is it work out the perpendicular tangent of the two asteroids and reflect the angle against that.

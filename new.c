#include <SDL.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

#include "new.h"

Uint32 t_end, t_start, t_projectile, startTick;

Ship ship1;

void initWindow() {
    	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not be initialized: %s\n", SDL_GetError());
        }
        else {
            printf("SDL video system is ready to go\n");
        }
        game.running = 1;
		game.window = SDL_CreateWindow( "GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		game.renderer =  SDL_CreateRenderer(game.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );
		game.screenSurface = SDL_GetWindowSurface(game.window);
}

TTF_Font* initFont() {
	if(TTF_Init() == -1){
			printf("Could not initailize SDL2_ttf, error: %s\n", TTF_GetError());
	}
	else {
		printf("SDL2_ttf system ready to go!\n");
	}

	// Load our font file and set the font size
	TTF_Font* myFont = TTF_OpenFont("./fonts/Orbitron-Medium.ttf", 128);
	// Confirm that it was loaded
	if (myFont == NULL) {
		printf("Could not load font\n");
		return NULL;
	}
	return myFont;
}

void renderCircles() {
    SDL_SetRenderDrawColor(game.renderer,0,0,255,0);
    for (int i = 0;i<level.numOfProjectiles;i++) {
        const int32_t diameter = level.projectiles[i].radius * 2;

        int32_t x = (level.projectiles[i].radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        int a;
        while (x >= y) {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x + x, level.projectiles[i].y - y);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x + x, level.projectiles[i].y + y);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x - x, level.projectiles[i].y - y);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x - x, level.projectiles[i].y + y);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x + y, level.projectiles[i].y - x);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x + y, level.projectiles[i].y + x);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x - y, level.projectiles[i].y - x);
            SDL_RenderDrawPoint(game.renderer, level.projectiles[i].x - y, level.projectiles[i].y + x);

            if (error <= 0) {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0) {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }

	SDL_SetRenderDrawColor(game.renderer,255,255,0,0);
    for (int i = 0;i<ship1.numOfRenderParticles;i++) {
        const int32_t diameter = ship1.particles[i].radius * 2;

        int32_t x = (ship1.particles[i].radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        int a;

        while (x >= y) {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x + x, ship1.particles[i].y - y);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x + x, ship1.particles[i].y + y);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x - x, ship1.particles[i].y - y);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x - x, ship1.particles[i].y + y);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x + y, ship1.particles[i].y - x);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x + y, ship1.particles[i].y + x);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x - y, ship1.particles[i].y - x);
            SDL_RenderDrawPoint(game.renderer, ship1.particles[i].x - y, ship1.particles[i].y + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }
}

void render() {
	SDL_SetRenderDrawColor(game.renderer,180,180,180,0);
	SDL_RenderClear(game.renderer);
	
    //printf("FIRST X = %i\n", ship1.trianglePoints[0].x);
	SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 100);
    SDL_RenderDrawLines(game.renderer, ship1.trianglePoints, 4);
    SDL_RenderDrawRect(game.renderer, &level.floor);
	//SDL_RenderDrawRect(game.renderer, &Player1);

    renderCircles();

    SDL_RenderCopy(game.renderer,game.textureText,NULL,&game.messageRect);
	
	SDL_RenderPresent(game.renderer);
}

void updateShipRotation() {
    ship1.trianglePoints[0].x = ship1.x + ship1.r*cos(ship1.thrust_angle * 3.14/180);
    ship1.trianglePoints[0].y = ship1.y - ship1.r*sin(ship1.thrust_angle * 3.14/180);

    ship1.trianglePoints[1].x = ship1.x + ship1.r*cos((ship1.thrust_angle + 120) * 3.14/180);
    ship1.trianglePoints[1].y = ship1.y - ship1.r*sin((ship1.thrust_angle + 120) * 3.14/180); 

    ship1.trianglePoints[2].x = ship1.x + ship1.r*cos((ship1.thrust_angle + 240) * 3.14/180);
    ship1.trianglePoints[2].y = ship1.y - ship1.r*sin((ship1.thrust_angle + 240) * 3.14/180);

    ship1.trianglePoints[3].x = ship1.x + ship1.r*cos(ship1.thrust_angle * 3.14/180);
    ship1.trianglePoints[3].y = ship1.y - ship1.r*sin(ship1.thrust_angle * 3.14/180);
}

void updateShipLocation() {
    if (ship1.thrust_angle <= 90 && ship1.thrust_angle > 0) {
        ship1.x = ship1.trianglePoints[0].x - ship1.r*cos(ship1.thrust_angle * 3.14/180);
        ship1.y = ship1.trianglePoints[0].y + ship1.r*sin(ship1.thrust_angle * 3.14/180);
    }
    else if (ship1.thrust_angle >= 90 && ship1.thrust_angle < 180) {
        ship1.x = ship1.trianglePoints[0].x + ship1.r*cos((180 - ship1.thrust_angle) * 3.14/180);
        ship1.y = ship1.trianglePoints[0].y + ship1.r*sin((180 - ship1.thrust_angle) * 3.14/180);
    }
    else if (ship1.thrust_angle >= 180 && ship1.thrust_angle < 270) {
        ship1.x = ship1.trianglePoints[0].x + ship1.r*sin((270 - ship1.thrust_angle) * 3.14/180);
        ship1.y = ship1.trianglePoints[0].y - ship1.r*cos((270 - ship1.thrust_angle) * 3.14/180);
    }
    else {
        ship1.x = ship1.trianglePoints[0].x - ship1.r*cos((360 - ship1.thrust_angle) * 3.14/180);
        ship1.y = ship1.trianglePoints[0].y - ship1.r*sin((360 - ship1.thrust_angle) * 3.14/180);
    }
}

float getShipThrustX(Ship* ship) {
    if (ship->thrust_angle <= 90 && ship->thrust_angle > 0) {
        return ship->F_thrust*cos(ship->thrust_angle * 3.14/180);
    }
    else if (ship->thrust_angle <= 180 && ship->thrust_angle > 90) {
        return -ship->F_thrust*cos((180 - ship->thrust_angle) * 3.14/180);
    }
    else if (ship->thrust_angle > 180 && ship->thrust_angle <= 270) {
        return -ship->F_thrust*sin((270 - ship->thrust_angle) * 3.14/180);
    }
    else {
        return ship->F_thrust*sin((360 - ship->thrust_angle) * 3.14/180);
    }  
}
    
float getShipThrustY(Ship* ship) {
    if (ship->thrust_angle <= 90 && ship->thrust_angle > 0) {
        return ship->F_thrust*sin(ship->thrust_angle * 3.14/180);
    }
    else if (ship->thrust_angle <= 180 && ship->thrust_angle > 90) {
        return ship->F_thrust*sin((180 - ship->thrust_angle) * 3.14/180);
    }
    else if (ship->thrust_angle > 180 && ship->thrust_angle <= 270) {
        return -ship->F_thrust*cos((270 - ship->thrust_angle) * 3.14/180);
    }
    else {
        return -ship->F_thrust*cos((360 - ship->thrust_angle) * 3.14/180);
    }  
}

void updateProjectileLocations() {
    for (int i = 0;i<level.numOfProjectiles;i++) {
        if (level.projectiles[i].angle <= 90 && level.projectiles[i].angle > 0) {
            level.projectiles[i].x += level.projectiles[i].v*cos(level.projectiles[i].angle * 3.14/180);
            level.projectiles[i].y -= level.projectiles[i].v*sin(level.projectiles[i].angle * 3.14/180);
        }
        else if (level.projectiles[i].angle <= 180 && level.projectiles[i].angle > 90) {
            level.projectiles[i].x -= level.projectiles[i].v*cos((180 - level.projectiles[i].angle) * 3.14/180);
            level.projectiles[i].y -= level.projectiles[i].v*sin((180 - level.projectiles[i].angle) * 3.14/180);
        }
        else if (level.projectiles[i].angle > 180 && level.projectiles[i].angle <= 270) {
            level.projectiles[i].x -= level.projectiles[i].v*sin((270 - level.projectiles[i].angle) * 3.14/180);
            level.projectiles[i].y += level.projectiles[i].v*cos((270 - level.projectiles[i].angle) * 3.14/180);
        }
        else {
            level.projectiles[i].x += level.projectiles[i].v*cos((360 - level.projectiles[i].angle) * 3.14/180);
            level.projectiles[i].y += level.projectiles[i].v*sin((360 - level.projectiles[i].angle) * 3.14/180);
        }
    }
}

void updateParticleLocations() {
    for (int i = 0;i<ship1.numOfRenderParticles;i++) {
        if (ship1.particles[i].angle <= 90 && ship1.particles[i].angle > 0) {
            ship1.particles[i].x += ship1.particles[i].v*cos(ship1.particles[i].angle * 3.14/180);
            ship1.particles[i].y -= ship1.particles[i].v*sin(ship1.particles[i].angle * 3.14/180);
        }
        else if (ship1.particles[i].angle <= 180 && ship1.particles[i].angle > 90) {
            ship1.particles[i].x -= ship1.particles[i].v*cos((180 - ship1.particles[i].angle) * 3.14/180);
            ship1.particles[i].y -= ship1.particles[i].v*sin((180 - ship1.particles[i].angle) * 3.14/180);
        }
        else if (ship1.particles[i].angle > 180 && ship1.particles[i].angle <= 270) {
            ship1.particles[i].x -= ship1.particles[i].v*sin((270 - ship1.particles[i].angle) * 3.14/180);
            ship1.particles[i].y += ship1.particles[i].v*cos((270 - ship1.particles[i].angle) * 3.14/180);
        }
        else {
            ship1.particles[i].x += ship1.particles[i].v*cos((360 - ship1.particles[i].angle) * 3.14/180);
            ship1.particles[i].y += ship1.particles[i].v*sin((360 - ship1.particles[i].angle) * 3.14/180);
        }
    }
}

void applyForces() {
    updateShipRotation();

    t_end = SDL_GetTicks();
    double delta_s = ((double)t_end - (double)t_start) / 1000;

    if (!ship1.on_ground) {
        ship1.F_x_total = getShipThrustX(&ship1);
        ship1.F_y_total = getShipThrustY(&ship1) - GRAVITY;

        ship1.v_x = ship1.v_x + ship1.F_x_total*delta_s;
        ship1.v_y = ship1.v_y + ship1.F_y_total*delta_s;
    }
    else {
        ship1.F_y_total = getShipThrustY(&ship1) - GRAVITY + NORMAL_FORCE;
        ship1.F_x_total = getShipThrustX(&ship1);

        ship1.v_x = ship1.v_x + ship1.F_x_total*delta_s;
        ship1.v_y = ship1.F_y_total*delta_s;
    }
    
    ship1.s_x = (int)ship1.v_x*delta_s;    
    ship1.s_y = (int)ship1.v_y*delta_s;


    for (int i = 0;i<4;i++) {
        ship1.trianglePoints[i].x += ship1.s_x;
        ship1.trianglePoints[i].y -= ship1.s_y;
    }

    updateShipLocation();

    updateProjectileLocations();

    updateParticleLocations();
    
    /* BROKEN
    
    sprintf(game.textStr, "v_y = %.2f, F_y_t = %.2f, s_y = %.2f, phi = %.2f", ship1.v_y, ship1.F_y_total, ship1.s_y, ship1.thrust_angle);
    game.surfaceText = TTF_RenderText_Solid(game.myFont, game.textStr, game.textColor);

    // Setup the texture
    game.textureText = SDL_CreateTextureFromSurface(game.renderer,game.surfaceText);

    // Free the surface
    // We are done with it after we have uploaded to
    // the texture
    SDL_FreeSurface(game.surfaceText); 
    
    */

    t_start = SDL_GetTicks();
}

float normalizeThrustAngle(float thrust_angle) {
    float angle;
    if (thrust_angle < 0) {
        angle = thrust_angle + 360;
    }
    else {
        angle = thrust_angle;
    }
    return angle;

}

void spawnProjectile(int x, int y, int radius, float angle, float v) {
    level.numOfProjectiles++;
    level.projectiles[level.numOfProjectiles - 1].x = x;
    level.projectiles[level.numOfProjectiles - 1].y = y;
    level.projectiles[level.numOfProjectiles - 1].radius = radius;
    level.projectiles[level.numOfProjectiles - 1].angle = angle;
    level.projectiles[level.numOfProjectiles - 1].v = v;
}

void checkProjectileCollision() {
    for (int i = 0;i<level.numOfProjectiles;i++) {
        // check wall collisions
        // right wall
        if (level.projectiles[i].x + level.projectiles[i].radius >= 800) {
            if (level.projectiles[i].angle < 90) {
                level.projectiles[i].angle = 180 - level.projectiles[i].angle;
            }
            else {
                level.projectiles[i].angle = 540 - level.projectiles[i].angle;
            }
        }
        // left wall
        else if (level.projectiles[i].x - level.projectiles[i].radius <= 0) {
            if (level.projectiles[i].angle < 180) {
                level.projectiles[i].angle = 180 - level.projectiles[i].angle;
                level.projectiles[i].x = level.projectiles[i].radius;
            }
            else {
                level.projectiles[i].angle = 540 - level.projectiles[i].angle;
                level.projectiles[i].x = level.projectiles[i].radius;
            }
        }
        // check floor and roof collisions
        // roof
        else if (level.projectiles[i].y - level.projectiles[i].radius <= 0) {
            level.projectiles[i].angle = 360 - level.projectiles[i].angle;
            level.projectiles[i].y = level.projectiles[i].radius;
        }
        // floor
        else if (level.projectiles[i].y + level.projectiles[i].radius >= 800) {
            level.projectiles[i].angle = 360 - level.projectiles[i].angle;
            level.projectiles[i].y = 800 - level.projectiles[i].radius;
        }
    }
}

spawnParticle(Ship* ship, int radius, float v) {
    ship->numOfParticles += 2;
    ship->particles[(ship->numOfParticles - 1) % 10].x = ship->trianglePoints[1].x;
    ship->particles[(ship->numOfParticles - 1) % 10].y = ship->trianglePoints[1].y;
    ship->particles[(ship->numOfParticles - 1) % 10].radius = radius;
    ship->particles[(ship->numOfParticles - 1) % 10].angle = ship->thrust_angle + 180;
    ship->particles[(ship->numOfParticles - 1) % 10].v = v;

    ship->particles[(ship->numOfParticles) % 10].x = ship->trianglePoints[2].x;
    ship->particles[(ship->numOfParticles) % 10].y = ship->trianglePoints[2].y;
    ship->particles[(ship->numOfParticles) % 10].radius = radius;
    ship->particles[(ship->numOfParticles) % 10].angle = ship->thrust_angle + 180;
    ship->particles[(ship->numOfParticles) % 10].v = v;
}

resetParticles(Ship* ship) {
    for (int i = 0;i<ship->numOfRenderParticles;i++) {
        ship->particles[i].radius = 0;
    }
}

int main( int argc, char* args[]) {
	game.initWindow = initWindow;
    game.initFont = initFont;
    game.render = render;

    ship1.r = 10;
    ship1.thrust_angle = 90;

    ship1.x = 100;
    ship1.y = 100;

    SDL_Point p1 = {ship1.x + ship1.r*cos(ship1.thrust_angle * 3.14/180), ship1.y + ship1.r*sin(ship1.thrust_angle * 3.14/180)};
    SDL_Point p2 = {ship1.x + ship1.r*cos((ship1.thrust_angle + 120) * 3.14/180), ship1.y + ship1.r*sin((ship1.thrust_angle + 120) * 3.14/180)};
    SDL_Point p3 = {ship1.x + ship1.r*cos((ship1.thrust_angle + 240) * 3.14/180), ship1.y + ship1.r*sin((ship1.thrust_angle + 240) * 3.14/180)};
    SDL_Point pointArr[] = {p1, p2, p3, p1};
    ship1.trianglePoints =  pointArr;

    /*
    int w, h;

    SDL_GetRendererOutputSize(game.renderer, &w, &h);
    SDL_GetWindowSize(game.window, w, h);
    */


    level.floor.x = 0;
    level.floor.y = 800;
    level.floor.w = 800;
    level.floor.h = 1;

    game.initWindow();

	game.myFont = game.initFont();

    // Pixels from our text
    
    game.textColor.r = 0;
    game.textColor.g = 255;
    game.textColor.b = 0;

    

    game.messageRect.x = 500;
    game.messageRect.y = 10;
    game.messageRect.w = 300;
    game.messageRect.h = 25;

	
	SDL_Color owncolor = {255,255,255};

	const Uint8 *kbmState = SDL_GetKeyboardState(NULL);
	
    int on_ground = 0;
	
    while (game.running) {
		while(SDL_PollEvent(&game.event)) {
	    	switch(game.event.type) {
			case SDL_QUIT:
				// shut down
				game.running = 0;
				break;
			}       
		}

        startTick = SDL_GetTicks();

        ship1.F_thrust = 0;
        ship1.resetParticlesSwitch = 1;

        if (kbmState[SDL_SCANCODE_W]) {
            ship1.F_thrust = 1400;
            ship1.numOfRenderParticles = 10;
            ship1.resetParticlesSwitch = 0;
            spawnParticle(&ship1 ,PARTICLE_RADIUS, PARTICLE_VELOCITY);
		}
		if (kbmState[SDL_SCANCODE_A]) {
            ship1.thrust_angle += 4;
            ship1.thrust_angle = normalizeThrustAngle((int)ship1.thrust_angle % 360);
		}
		else if (kbmState[SDL_SCANCODE_D]) {
            ship1.thrust_angle -= 4;
            ship1.thrust_angle = normalizeThrustAngle((int)ship1.thrust_angle % 360);
        }
        if (kbmState[SDL_SCANCODE_SPACE]) {
            if (SDL_GetTicks() - t_projectile > 200) {
                spawnProjectile(ship1.trianglePoints[0].x, ship1.trianglePoints[0].y, PROJECTILE_RADIUS, ship1.thrust_angle, PROJECTILE_VELOCITY);
                t_projectile = SDL_GetTicks();
            }
        }
		if (kbmState[SDL_SCANCODE_ESCAPE]) {
			game.running = 0;
		}
 
        ship1.on_ground = 0;
        for(int i = 0;i<4;i++) {
            if(ship1.trianglePoints[i].y >= level.floor.y) {
                ship1.on_ground = 1;
                break;
            }
        }
        
        if (ship1.resetParticlesSwitch) {
            resetParticles(&ship1);
        }
        checkProjectileCollision();
        applyForces();

        if ( (1000 / FPS) > SDL_GetTicks() - startTick) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - startTick));
        }

		game.render();
	}
	SDL_DestroyTexture(game.textureText);
	SDL_DestroyWindow(game.window);
	TTF_CloseFont(game.myFont);
	SDL_Quit();

	return 0;
}
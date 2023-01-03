
struct Game {
    SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Surface* surfaceText;
	SDL_Texture* textureText;
    SDL_Color textColor;
    char textStr[64];
	TTF_Font* myFont;
	SDL_Renderer *renderer;
	SDL_Event event;
    int running;

    void (*initWindow)();
    TTF_Font* (*initFont)();
    void (*initLevel)();
    void (*render)();
    SDL_Rect messageRect;
} game;

struct Circle {
    int x, y, radius;
    float v, angle;
};

struct Ship {
    SDL_Point* trianglePoints;
    float x;
    float y;
    int hp;
    float thrust_angle;
    int r;
    float F_y_total;
    float F_x_total;
    float F_thrust;
    float a;
    float v_x;
    float v_y;
    double s_x;
    double s_y;
    double t_1, t_2;
    int on_ground;
    int numOfParticles;
    int numOfRenderParticles;
    int resetParticlesSwitch;

    struct Circle particles[10];

    float (*getShipThrustX)();
    float (*getShipThrustY)();

    void (*spawnParticle)(int radius, float v);
} ship1;

struct Level {
    SDL_Rect floor;
    struct Circle projectiles[1024];
    int numOfProjectiles;
    
} level;

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

    float (*getShipThrustX)();
    float (*getShipThrustY)();

    void (*normalizeThrustAngle)();
} ship1;

struct Projectile {
    int x, y, radius;
    float v, angle;
};

struct Level {
    SDL_Rect floor;
    struct Projectile projectiles[1024];
    int numOfProjectiles;
} level;
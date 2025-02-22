#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define MAX_BALL 8

uint64_t ball_move;
uint64_t ball_add;

typedef enum ballhit
{
    WRONG,
    CORRECT,
    NOTHING
} Ballhit;

typedef enum bwall
{
    HIT,
    MESS,
    NO
} Bwall;

typedef struct shield
{
    int x;
    int y;
    int shield_ball_index;
    SDL_Texture *texture_shield;
} Shield;

typedef struct wall
{
    int x;
    int y;
    int wall_ball_index;
    SDL_Texture *texture_wall;
} Wall;

typedef struct ball
{
    bool available;
    int x;
    int y;
    int dir_x;
    int ball_index;
    SDL_Texture *texture;
} Ball;

void draw_shield(SDL_Renderer *renderer, Shield shield)
{
    SDL_SetRenderDrawColor(renderer, 255, 125, 100, 255);
    SDL_Rect object;
    object.x = shield.x;
    object.y = shield.y;
    object.w = 35;
    object.h = 150;
    SDL_RenderCopy(renderer, shield.texture_shield, NULL, &object);
}

void draw_wall(SDL_Renderer *renderer, Wall wall)
{
    SDL_SetRenderDrawColor(renderer, 255, 125, 100, 255);
    SDL_Rect object;
    object.x = wall.x;
    object.y = wall.y;
    object.w = 70;
    object.h = 580;
    SDL_RenderCopy(renderer, wall.texture_wall, NULL, &object);
}

void draw_ball(SDL_Renderer *renderer, Ball *ball_array)
{
    for (int i = 0; i < MAX_BALL; i++)
    {
        if (ball_array[i].available)
        {
            SDL_Rect object;
            object.x = ball_array[i].x;
            object.y = ball_array[i].y;
            object.w = 70;
            object.h = 70;
            SDL_RenderCopy(renderer, ball_array[i].texture, NULL, &object);
        }
    }
}

void draw_background(SDL_Renderer *renderer, SDL_Texture *background)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 850;
    rect.h = 530;
    SDL_RenderCopy(renderer, background, NULL, &rect);
}

// faces raining
void ball_a_m(Ball *ball_array, SDL_Texture **texture_ball, int *ball_index)
{

    // Add a face
    bool add_ball = false;
    if (SDL_GetTicks() - ball_add > 1000)
    {
        add_ball = true;
        ball_add = SDL_GetTicks();
    }

    if (add_ball)
    {

        Ball added_ball;
        added_ball.available = true;
        added_ball.ball_index = rand() % 2;
        added_ball.texture = texture_ball[added_ball.ball_index];
        added_ball.y = rand() % 490;
        added_ball.dir_x = 30 + rand() % 10;

        ball_array[*ball_index] = added_ball;

        if (*ball_index < MAX_BALL - 1)
        {
            (*ball_index) += 1;
        }
        else
        {
            (*ball_index) = 0;
        }
    }

    // Move ball right
    bool balls_move = false;
    if (SDL_GetTicks() - ball_move > 150)
    {
        balls_move = true;
        ball_move = SDL_GetTicks();     
    }

    if (balls_move)
    {
        for (int i = 0; i < 8; i++)
        {
            if (ball_array[i].available)
            {

                if (ball_array[i].x >= 0 && ball_array[i].x <= 720)
                {

                    ball_array[i].x += ball_array[i].dir_x;
                }
            }
        }
    }
}

Ballhit shield_ball(Shield shield, Ball *ball_array)
{
    for (int i = 0; i < MAX_BALL; i++)
    {
        if (ball_array[i].available)
        {
            int distance_shield_x = abs((ball_array[i].x + 50) - (shield.x + 20));
            int distance_shield_y = abs((ball_array[i].y + 10 ) - (shield.y + 30));

            if ((distance_shield_x < 20) && (distance_shield_y < 70))
            {
                ball_array[i].available = false;

                if (ball_array[i].ball_index == shield.shield_ball_index)
                {
                    return WRONG;
                }
                else
                {
                    return CORRECT;
                }
            }
        }
    }

    return NOTHING;
}

Bwall wall_ball(Wall wall, Ball *ball_array)
{
    for (int i = 0; i < MAX_BALL; i++)
    {
        if (ball_array[i].available)
        {
            int distance_wall_x = abs((ball_array[i].x + 50) - (wall.x + 17));
            int distance_wall_y = abs((ball_array[i].y + 35) - (wall.y+100));

            if ((distance_wall_x < 20))
            {
                ball_array[i].available = false;

                if (ball_array[i].ball_index == wall.wall_ball_index)
                {
                    return MESS;
                }
                else
                {
                    return HIT;
                }
            }
        }
    }

    return NO;
}

void draw_score(SDL_Renderer *renderer, TTF_Font *font, int score)
{
    SDL_Color tcolor = {255, 0, 10};

    char text[16];
    sprintf(text, "Score : %d", score);

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, tcolor);
    SDL_Texture *texturet = SDL_CreateTextureFromSurface(renderer, surface);
    int texW = 0, texH = 0;
    SDL_QueryTexture(texturet, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {0, 0, texW, texH};

    SDL_RenderCopy(renderer, texturet, NULL, &dstrect);
    SDL_DestroyTexture(texturet);
    SDL_FreeSurface(surface);
}

int main(int argc, char *args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    TTF_Font *font = TTF_OpenFont("arial.ttf", 25);

    SDL_Window *window = SDL_CreateWindow(
        "Castle game",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 850, 530, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *img_grass = IMG_Load("image/Castle-grass.png");
    SDL_Texture *texture_grass = SDL_CreateTextureFromSurface(renderer, img_grass);
    SDL_FreeSurface(img_grass);

    SDL_Surface *img_shield = IMG_Load("image/shield.png");
    SDL_Texture *texture_shield = SDL_CreateTextureFromSurface(renderer, img_shield);
    SDL_FreeSurface(img_shield);

    SDL_Surface *img_wall = IMG_Load("image/wall4.png");
    SDL_Texture *texture_wall = SDL_CreateTextureFromSurface(renderer, img_wall);
    SDL_FreeSurface(img_wall);

    SDL_Surface *img_ball = IMG_Load("image/ball.png");
    SDL_Surface *img_iceball = IMG_Load("image/iceball.png");
    SDL_Texture *texture_ball[2] = {
        SDL_CreateTextureFromSurface(renderer, img_ball),
        SDL_CreateTextureFromSurface(renderer, img_iceball)};
    SDL_FreeSurface(img_iceball);
    SDL_FreeSurface(img_ball);

    bool isRunning = true;
    SDL_Event event;

    int ball_renewal = 0;
    Ball ball_array[MAX_BALL] = {
        {.available = false},
        {.available = false},
        {.available = false},
        {.available = false},
        {.available = false},
        {.available = false},
        {.available = false},
    };

    Shield shield;
    shield.texture_shield = texture_shield;
    shield.x = 485;
    shield.y = 50;

    Wall wall;
    wall.texture_wall = texture_wall;
    wall.x = 650;
    wall.y = 0;

    int score = 0;

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            // Handles OS "Exit" event
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                /************/
                // TODO: Animation Logic here + keyboard logic here
                /************/
                // Add more functions here
                switch (event.key.keysym.scancode)
                {

                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if (shield.y > 9)
                        shield.y -= 50;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if (shield.y < 399)
                        shield.y += 50;
                    break;
                }
                break;
            }
        }

        // Draw black background

        ball_a_m(ball_array, texture_ball, &ball_renewal);

        switch (shield_ball(shield, ball_array))
        {
        case CORRECT:
            score += 1;
            break;
        }

        switch (wall_ball(wall, ball_array))
        {
        case HIT:
            score -= 5;
            break;
        }

        if (score < 0)
        {
            isRunning = false;
        }

        draw_background(renderer, texture_grass);

        draw_shield(renderer, shield);

        draw_wall(renderer, wall);

        draw_score(renderer, font, score);

        draw_ball(renderer, ball_array);

        // Present Render to screen
        SDL_RenderPresent(renderer);
    }

    bool end = true;

    while (end)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Surface *img_end = IMG_Load("image/end.png");
        SDL_Texture *texture_end = SDL_CreateTextureFromSurface(renderer, img_end);
        SDL_FreeSurface(img_end);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                end = false;
                break;
            }
        }

        draw_background(renderer, texture_end);

        SDL_RenderPresent(renderer);
        SDL_Delay(15);
    }

    SDL_DestroyTexture(texture_grass);
    SDL_DestroyTexture(texture_shield);
    SDL_DestroyTexture(texture_wall);

    for (int i = 0; i < 2; i++)
    {
        SDL_DestroyTexture(texture_ball[i]);
    }

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

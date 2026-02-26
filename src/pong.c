#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "colors.h"
#include "vec2.h"
#include "circle.h"

#define WIN_W 1280
#define WIN_H 720
#define FPS 60
#define PAD_W 20
#define PAD_H 100
#define PAD_OFFSET 30
#define PAD_SPEED 750
#define PAD_ACCL 300
#define PAD_RETD_FACTOR 2
#define BALL_R 20
#define BALL_SPEED 500

typedef enum
{
    NONE,
    LEFT_PAD,
    RIGHT_PAD,
} COLLIDER;

typedef struct
{
    SDL_Rect rect;
    double speed;
} PADDLE;

typedef struct
{
    CIRCLE circle;
    VEC2 vel;
} BALL;

PADDLE left_paddle;
PADDLE right_paddle;
BALL ball;

const double dt = 1.0 / FPS;

SDL_Renderer *renderer;
int left_score = 0, right_score = 0;
SDL_bool point_scored = SDL_FALSE;

void updateBallPos();
void updatePaddlePos();
void Frame_Update();
void Frame_Render();
COLLIDER detectPossiblePadCollision();
void handlePadCollision(COLLIDER collider);
void Level_Init();
void displayScore();

int main()
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Level_Init();
    SDL_bool running = SDL_TRUE;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_a:
                case SDLK_LEFT:
                    // move left pad down
                    if (left_paddle.speed < 0)
                        left_paddle.speed = 0;
                    // left_paddle.speed += PAD_ACCL;
                    left_paddle.speed = PAD_SPEED;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    // move left pad up
                    if (left_paddle.speed > 0)
                        left_paddle.speed = 0;
                    // left_paddle.speed -= PAD_ACCL;
                    left_paddle.speed = -PAD_SPEED;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    // move right pad down
                    if (right_paddle.speed < 0)
                        right_paddle.speed = 0;
                    // right_paddle.speed += PAD_ACCL;
                    right_paddle.speed = PAD_SPEED;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    // move right pad up
                    if (right_paddle.speed > 0)
                        right_paddle.speed = 0;
                    // right_paddle.speed -= PAD_ACCL;
                    right_paddle.speed = -PAD_SPEED;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_a:
                case SDLK_LEFT:
                    // left_paddle.speed /= PAD_RETD_FACTOR;
                    left_paddle.speed = 0;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    // left_paddle.speed /= PAD_RETD_FACTOR;
                    left_paddle.speed = 0;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    // right_paddle.speed /= PAD_RETD_FACTOR;
                    right_paddle.speed = 0;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    // right_paddle.speed /= PAD_RETD_FACTOR;
                    right_paddle.speed = 0;
                    break;
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, RGB24_BLACK.r, RGB24_BLACK.g, RGB24_BLACK.b, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        Frame_Update();
        Frame_Render();

        SDL_RenderPresent(renderer);
        SDL_Delay(dt * 1000);
    }

    displayScore();

    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return 0;
}

void updateBallPos()
{
    if (ball.vel.x)
        ball.circle.x += ball.vel.x * dt;
    if (ball.vel.y)
        ball.circle.y += ball.vel.y * dt;

    // apply vertical bounds
    if (ball.circle.y + ball.circle.r > WIN_H)
    {
        ball.circle.y = WIN_H - ball.circle.r;
        ball.vel.y *= -1;
    }
    else if (ball.circle.y - ball.circle.r < 0)
    {
        ball.circle.y = ball.circle.r;
        ball.vel.y *= -1;
    }

    // check horizontal bounds
    if (ball.circle.x + ball.circle.r > WIN_W)
    {
        left_score++;
        point_scored = SDL_TRUE;
    }
    else if (ball.circle.x - ball.circle.r < 0)
    {
        right_score++;
        point_scored = SDL_TRUE;
    }
}

void updatePaddlePos()
{
    left_paddle.speed = SDL_clamp(left_paddle.speed, -PAD_SPEED, PAD_SPEED);
    right_paddle.speed = SDL_clamp(right_paddle.speed, -PAD_SPEED, PAD_SPEED);
    if (left_paddle.speed)
        left_paddle.rect.y += left_paddle.speed * dt;
    if (right_paddle.speed)
        right_paddle.rect.y += right_paddle.speed * dt;

    // apply bounds
    if (left_paddle.rect.y < 0)
    {
        left_paddle.rect.y = 0;
        // left_paddle.speed *= -1;
    }
    else if (left_paddle.rect.y + left_paddle.rect.h > WIN_H)
    {
        left_paddle.rect.y = WIN_H - left_paddle.rect.h;
        // left_paddle.speed *= -1;
    }

    if (right_paddle.rect.y < 0)
    {
        right_paddle.rect.y = 0;
        // right_paddle.speed *= -1;
    }
    else if (right_paddle.rect.y + right_paddle.rect.h > WIN_H)
    {
        right_paddle.rect.y = WIN_H - right_paddle.rect.h;
        // right_paddle.speed *= -1;
    }
}

void Frame_Update()
{
    if (point_scored)
    {
        point_scored = SDL_FALSE;
        Level_Init();
    }
    else
    {
        updateBallPos();
        updatePaddlePos();
        handlePadCollision(detectPossiblePadCollision());
    }
}

void Frame_Render()
{
    SDL_SetRenderDrawColor(renderer, RGB24_WHITE.r, RGB24_WHITE.g, RGB24_WHITE.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &left_paddle.rect);
    SDL_RenderFillRect(renderer, &right_paddle.rect);
    CIRCLE_RenderFill(renderer, &ball.circle);
}

COLLIDER detectPossiblePadCollision()
{
    // detect left pad collision
    if (ball.circle.x - ball.circle.r < left_paddle.rect.x + left_paddle.rect.w)
    {
        return LEFT_PAD;
    }
    // detect right pad collision
    else if (ball.circle.x + ball.circle.r > right_paddle.rect.x)
    {
        return RIGHT_PAD;
    }
    else
        return NONE;
}

void handlePadCollision(COLLIDER collider)
{
    double y_offset;
    switch (collider)
    {
    case NONE:
        break;
    case LEFT_PAD:
        y_offset = left_paddle.rect.y + left_paddle.rect.h / 2 - ball.circle.y;
        if (abs(y_offset) > left_paddle.rect.h / 2)
            return;
        else
        {
            // push ball out of left pad
            ball.circle.x = left_paddle.rect.x + left_paddle.rect.w + ball.circle.r;
            double angle = -asin(y_offset / (left_paddle.rect.h / 2)) / 2;
            // apply ball vel based on angle
            VEC2 ball_dir = {
                .x = cos(angle),
                .y = sin(angle),
            };
            ball.vel = VEC2_Scale(ball_dir, BALL_SPEED);
        }
        break;
    case RIGHT_PAD:
        y_offset = right_paddle.rect.y + right_paddle.rect.h / 2 - ball.circle.y;
        if (abs(y_offset) > right_paddle.rect.h / 2)
            return;
        else
        {
            // push ball out of right pad
            ball.circle.x = right_paddle.rect.x - ball.circle.r;
            double angle = -asin(y_offset / (right_paddle.rect.h / 2)) / 2;
            // apply ball vel based on angle
            VEC2 ball_dir = {
                .x = -cos(angle),
                .y = sin(angle),
            };
            ball.vel = VEC2_Scale(ball_dir, BALL_SPEED);
        }
        break;
    default:
        printf("Invalid Collision\n");
    }
}

void Level_Init()
{
    left_paddle = (PADDLE){
        .rect = {
            .x = PAD_OFFSET,
            .y = (WIN_H - PAD_H) / 2,
            .w = PAD_W,
            .h = PAD_H,
        },
        .speed = 0,
    };

    right_paddle = (PADDLE){
        .rect = {
            .x = WIN_W - PAD_W - PAD_OFFSET,
            .y = (WIN_H - PAD_H) / 2,
            .w = PAD_W,
            .h = PAD_H,
        },
        .speed = 0,
    };

    VEC2 ball_dir = {
        .x = (double)rand() / RAND_MAX * 2 - 1,
        .y = (double)rand() / RAND_MAX - 0.5,
    };

    ball = (BALL){
        .circle = {
            .x = WIN_W / 2,
            .y = WIN_H / 2,
            .r = BALL_R,
        },
        .vel = VEC2_Scale(VEC2_Normalised(ball_dir), BALL_SPEED),
    };
}

void displayScore()
{
    printf("Left Score: %d\n", left_score);
    printf("Right Score: %d\n", right_score);
}

#include <raylib.h>
#include <raymath.h>
#include <cvecs.h>
#include <rocket.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static Vec rockets;
static size_t frame;
static Texture2D rocket_texture;
static Vec obstacles;

#define ROCKET_COUNT    30
#define FPS 60
#define ROCKET_LIFETIME (FPS * 5.f)
#define TARGET_POS (Vector2){GetScreenWidth() / 2.f - 10, 50 - 10}

void createRockets(void) {
    rocket_texture = LoadTexture("rocket.png");
    if (!IsTextureReady(rocket_texture)) {
        fprintf(stderr, "Texture rocket.png could not be loaded.\n");
        exit(EXIT_FAILURE);
    }
    rockets = createVecEx(ROCKET_COUNT);
    for (size_t i = 0; i < ROCKET_COUNT; i++) {
        Rocket * r = malloc(sizeof(Rocket));
        *r = createRocket(createRandomDNA(ROCKET_LIFETIME), (Vector2){(float)GetScreenWidth() / 2.f, GetScreenHeight() - 20}, Vector2Zero(), 20.f);
        appendVec(&rockets, r);
    }
}

void createObstacles(void) {
    obstacles = createVec();
    Rectangle * r = malloc(sizeof(Rectangle));
    *r = (Rectangle) {20, 300, (float)GetScreenWidth() / 2 - 10, 20};

    Rectangle * r2 = malloc(sizeof(Rectangle));
    *r2 = (Rectangle) {(float)GetScreenWidth() / 2 + 10, 500, (float)GetScreenWidth() / 2 - 10, 20};
    appendVec(&obstacles, r);
    appendVec(&obstacles, r2);
}

void freeRockets(void) {
    for (size_t i = 0; i < ROCKET_COUNT; i++) {
        Rocket * r = rockets.entries[i].val;
        freeRocket(*r);
        free(r);
    }
    freeVec(rockets);
}

double Vector2AngleCalc(Vector2 a, Vector2 b) {
    return acos(Vector2DotProduct(a, b) / (Vector2Length(a) * Vector2Length(b)));
}

void drawRockets(void) {
    for (size_t i = 0; i < ROCKET_COUNT; i++) {
        const Rocket * r = rockets.entries[i].val;
        const double w = 50.;
        const double h = 30.;
        const double rot_angle = atan2(r->vel.y, r->vel.x);
        DrawRectanglePro((Rectangle){r->pos.x - w / 2., r->pos.y - h / 2., w, h},
                         (Vector2){w / 2., h / 2.},
                         rot_angle * (180. / PI),
                         BLACK);
        // DrawLineV(r->pos, Vector2Add(r->pos, Vector2Scale(Vector2Subtract(r->pos, Vector2Add(r->pos, r->vel)), -5.f)), GREEN);
    }
}

void drawTarget(void) {
    DrawRectangleV(TARGET_POS, (Vector2){20, 20}, RED);
}

void updateRockets(void) {
    for (size_t i = 0; i < ROCKET_COUNT; i++) {
        Rocket * r = rockets.entries[i].val;
        if (r->alive) {
            updateRocket(r, frame);
            if ( checkCollision(r, (Rectangle){-10, 0, 10, GetScreenHeight()})
             || checkCollision(r, (Rectangle){0, -10, GetScreenWidth(), 10})
             || checkCollision(r, (Rectangle){0, GetScreenHeight(), GetScreenWidth(), 10})
             || checkCollision(r, (Rectangle){GetScreenWidth(), 0, 10, GetScreenHeight()}))
                r->alive = false;

            for (size_t j = 0; j < obstacles.count; j++) {
                if (checkCollision(r, *(Rectangle *)obstacles.entries[j].val)) r->alive = false;
            }
        }
    }
}

void resetRockets(void) {
    frame = 0;
    calcNewDNA(rockets, TARGET_POS);
    for (size_t i = 0; i < rockets.count; i++) {
        Rocket * r = rockets.entries[i].val;
        r->alive = true;
        r->pos = (Vector2){(float)GetScreenWidth() / 2, (float)GetScreenHeight() - 20};
        r->vel = (Vector2){0.f, 0.f};
    }
}

void drawObstacles(void) {
    for (size_t i = 0; i < obstacles.count; i++) {
        DrawRectangleRec(*(Rectangle *)obstacles.entries[i].val, RED);
    }
}

int main(void) {

    InitWindow(600, 800, "Smartrockets");
    SetTargetFPS(FPS);
    createRockets();
    createObstacles();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(GRAY);
            updateRockets();
            drawRockets();
            drawTarget();
            drawObstacles();
            if (frame++ > ROCKET_LIFETIME) {
                printf("Resetting Rockets...\n");
                resetRockets();
            }
        EndDrawing();
    }

    CloseWindow();
    freeRockets();

    return 0;
}

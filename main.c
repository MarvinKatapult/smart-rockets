
#include <raylib.h>
#include <raymath.h>
#include <cvecs.h>
#include <rocket.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static Vec rockets;
static size_t frame;
static Rectangle obstacle;

#define ROCKET_COUNT    30
#define FPS 240
#define ROCKET_LIFETIME (FPS * 0.5f)
#define TARGET_POS (Vector2){GetScreenWidth() / 2.f - 10, 50 - 10}

void createRockets(void) {
    rockets = createVecEx(ROCKET_COUNT);
    for (size_t i = 0; i < ROCKET_COUNT; i++) {
        Rocket * r = malloc(sizeof(Rocket));
        *r = createRocket(createRandomDNA(ROCKET_LIFETIME), (Vector2){GetScreenWidth() / 2, GetScreenHeight() - 10}, Vector2Zero(), 20.f);
        appendVec(&rockets, r);
    }
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
            if (checkCollision(r, obstacle)) r->alive = false;
        }
    }
}

void resetRockets(void) {
    frame = 0;
    calcNewDNA(rockets, TARGET_POS);
    for (size_t i = 0; i < rockets.count; i++) {
        Rocket * r = rockets.entries[i].val;
        r->alive = true;
        r->pos = (Vector2){(float)GetScreenWidth() / 2, (float)GetScreenHeight() - 10};
        r->vel = (Vector2){0.f, 0.f};
    }
}

void drawObstacle(void) {
    obstacle = (Rectangle) {
        .x = (float)GetScreenWidth() / 2.f - (float)GetScreenWidth() / 8.f,
        .y = (float)GetScreenHeight() / 2.f - 10.f,
        .width = (float)GetScreenWidth() / 4.f,
        .height = 20.f,
    };
    DrawRectangleRec(obstacle, RED);
}

int main(void) {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Smartrockets");
    SetTargetFPS(FPS);
    createRockets();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            updateRockets();
            drawRockets();
            drawTarget();
            drawObstacle();
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

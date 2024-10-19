#ifndef __SMARTROCKETS_H__
#define __SMARTROCKETS_H__

#include <raylib.h>
#include <cdefs.h>
#include <cvecs.h>

typedef struct {
    Vec dna;
    Vector2 pos;
    Vector2 vel;
    f64 m;
    f64 fitness;
    bool alive;
} Rocket;

Rocket createRocket(Vec dna, Vector2 pos, Vector2 vel, f64 m);
void freeRocket(Rocket r);

void updateRocket(Rocket * rocket, size_t frame);
bool checkCollision(Rocket * rocket, Rectangle obstacle);

void calcNewDNA(Vec rockets, Vector2 target);
Vec createRandomDNA(size_t lifetime);

#endif // __SMARTROCKETS_H__

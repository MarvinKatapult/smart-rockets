#include <rocket.h>
#include <raymath.h>

#include <stdio.h>
#include <stdlib.h>

static float highest_fit;

Rocket createRocket(Vec dna, Vector2 pos, Vector2 vel, f64 m) {
    return (Rocket){
        .dna = dna,
        .pos = pos,
        .vel = vel,
        .alive = true,
        .fitness = 0.f,
        .m = m,
    };
}

void freeRocket(Rocket r) {
    for (size_t i = 0; i < r.dna.count; i++) {
        free(r.dna.entries[i].val);
    }
    freeVec(r.dna);
}

void updateRocket(Rocket * rocket, size_t frame) {
    if (!rocket || frame >= rocket->dna.count) return;

    // TODO: Maybe Gravity
    const Vector2 f = *(Vector2 *)rocket->dna.entries[frame].val;
    const Vector2 a = (Vector2){ f.x / rocket->m, f.y / rocket->m};
    rocket->vel = Vector2Add(rocket->vel, a);
    rocket->pos = Vector2Add(rocket->pos, rocket->vel);
}

static Vector2 getRandomForce(int min, int max) {
    return (Vector2) {
        GetRandomValue(min, max) / 10.f,
        GetRandomValue(min, max) / 10.f,
    };
}

/** Fitness **/

static void calcRocketFitness(Rocket * r, Vector2 target) {
        r->fitness = 1000 / Vector2Distance(r->pos, target) * (r->alive ? 1.f : 0.5f);
        if (r->fitness > highest_fit) {
            highest_fit = r->fitness;
        }
    printf("Highest Fit:%f\n", highest_fit);
}

static void printFitness(Vec rockets) {
    for (size_t i = 0; i < rockets.count; i++) {
        Rocket * r = rockets.entries[i].val;
        printf("%lf\n", r->fitness);
    }
}

/** ---- **/

static void getRocketParents(Vec rockets, Rocket * parent1, Rocket * parent2) {
    float best_fits[2] = {0.f, 0.f};
    *parent1 = *(Rocket *)rockets.entries[0].val;
    *parent2 = *(Rocket *)rockets.entries[1].val;
    for (size_t i = 0; i < rockets.count; i++) {
        Rocket * r = rockets.entries[i].val;
        if (r->fitness > best_fits[0]) {
            best_fits[1] = best_fits[0];
            *parent2 = *parent1;
            best_fits[0] = r->fitness;
            *parent1 = *r;
        }
    }
    printf("Parent1Fitness:%f : Parent2Fitness:%f\n", parent1->fitness, parent2->fitness);
}

static void createCrossover(Rocket r1, Rocket r2, size_t i) {
    const Vector2 * f1 = (Vector2 *)r1.dna.entries[i].val;
    const Vector2 * f2 = (Vector2 *)r2.dna.entries[i].val;
    const Vector2 comb_f = Vector2Add(*f1, *f2);
    const Vector2 midway = Vector2Scale(comb_f, 0.5f);
    *(Vector2 *)r1.dna.entries[i].val = midway;
}

void calcNewDNA(Vec rockets, Vector2 target) {
    for (size_t i = 0; i < rockets.count; i++) {
        calcRocketFitness(rockets.entries[i].val, target);
    }

    Rocket parent1, parent2;
    getRocketParents(rockets, &parent1, &parent2);

    /** TODO **/
    // Rockets sortieren
    // Elitechilds direkt auswählen
    // Parents nach Fitness stochastisch auswählen
    /** ---- **/

    for (size_t i = 0; i < rockets.count; i++) {
        Rocket * r = rockets.entries[i].val;
        if (GetRandomValue(0, 100) < 2) {
            for (size_t i = 0; i < r->dna.count; i++) {
                *(Vector2 *)r->dna.entries[i].val = getRandomForce(-100, 100);
            }
        } else {
            if (r == &parent1 || r == &parent2) continue;
            for (size_t j = 0; j < r->dna.count; j++) {
                if (j > rockets.count / 2) {
                    createCrossover(*r, parent1, j);
                } else {
                    createCrossover(*r, parent2, j);
                }
            }

            for (size_t i = 0; i < r->dna.count; i++) {
                if (GetRandomValue(0, 100) < 2) *(Vector2 *)r->dna.entries[i].val = getRandomForce(-100, 100);
            }
        }
    }
    
    printFitness(rockets);
}

Vec createRandomDNA(size_t lifetime) {
    Vec dna = createVecEx(lifetime);
    for (size_t i = 0; i < lifetime; i++) {
        Vector2 * f = malloc(sizeof(Vector2));
        *f = getRandomForce(-100, 100);
        appendVec(&dna, f);
    }
    return dna;
}

bool checkCollision(Rocket * rocket, Rectangle obstacle) {
    return CheckCollisionPointRec(rocket->pos, obstacle);
}

#pragma once

#include "types.h"

Particle partInit(Vec2 p, Vec2 v, float m);
void initPlatforms(World& w, int nbPlat, int nbPlatBroken, int nbPlatMoving, int nbPlatJump);
void initMonster(World& w, int nbMonster);
void initBlackHole(World& w, int nbHole);
void initObject(World& w);
void createProjectile(World& w, Vec2 pos, Vec2 v);
void partAddForce(Particle& p, Vec2 force);
void updatePlatBroken(World& w);
void updatePlatMoving(World& w);
void updateParticle(Particle& p, World& w, int nbPlat, int &score);
void youLoose(World& w, Particle p);
void handlePart(Particle& p, World& w);
void updateAndDrawProjectiles(World& w, int &score);
void restartGame(World& w, Particle& p, int& score);



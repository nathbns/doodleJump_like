#pragma once

#include <cmath>
#include <algorithm>

// Forward decl pour éviter d'inclure SDL ici
struct SDL_Texture;
using Image = SDL_Texture*;

// Constantes du jeu
static const int DIMW = 600;
static const int DIMH = DIMW + 300;
static const int HEIGHT_GAME = 1000;
static const int PLATFORM_HEIGHT = 20;
static const int PLATFORM_WIDTH = 100;
static const int MAX_PLATFORM = 15;
static const int PARTICLE_RADIUS = 8;
static const int MAX_MONSTERS = 5;
static const int MAX_BLACK_HOLE = 2;
static const float dt = 0.05f;
// Interprétation: vitesseJeu = unités (pixels) par seconde
static const float vitesseJeu = 0.20f; // défilement par frame (plus rapide)
static const int MAX_PROJECTILES = 1;

// Intensité du rebond
static const float JUMP_VELOCITY = 50.0f; // saut plus haut
static const float JUMP_MULTIPLIER = 1.15f; // pour les plateformes jump

struct Vec2 { float x, y; };

inline Vec2 make_vec2(float r, float i) { return Vec2{r,i}; }
inline Vec2 make_vec2_exp(float r, float theta){ return make_vec2(r * std::cos(theta), r * std::sin(theta)); }

inline Vec2 operator+(Vec2 a, Vec2 b){ return make_vec2(a.x + b.x, a.y + b.y); }
inline Vec2 operator-(Vec2 a, Vec2 b){ return make_vec2(a.x - b.x, a.y - b.y); }
inline Vec2 operator*(Vec2 a, Vec2 b){ return make_vec2(a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y); }
inline Vec2 operator/(Vec2 a, float b){ return make_vec2(a.x/b, a.y/b); }
inline Vec2 operator/(float a, Vec2 b){ return make_vec2(a/b.x, a/b.y); }
inline Vec2 operator*(float a, Vec2 b){ return make_vec2(a * b.x, a * b.y); }
inline Vec2 operator*(Vec2 a, float b){ return make_vec2(a.x * b, a.y * b); }

inline float to_rad(float deg){ return float(M_PI) * deg / 180.0f; }
inline Vec2 rotate(Vec2 p, float cx, float cy, float theta_deg){
   Vec2 rot = make_vec2_exp(1.0f, to_rad(theta_deg));
   Vec2 center = make_vec2(cx, cy);
   return ((p - center) * rot) + center;
}

struct Color{ int r,g,b; };
inline Color make_color(int r, int g, int b){ return Color{r,g,b}; }
inline Color operator+(Color a, Color b){ return make_color(a.r+b.r,a.g+b.g,a.b+b.b); }
inline Color operator-(Color a, Color b){ return make_color(a.r-b.r,a.g-b.g,a.b-b.b); }
inline Color operator+(float a, Color b){ return make_color(int(a*b.r),int(a*b.g),int(a*b.b)); }
inline Color operator*(Color b, float a){ return make_color(int(a*b.r),int(a*b.g),int(a*b.b)); }
inline Color operator/(Color b, float a){ return make_color(int(a/b.r),int(a/b.g),int(a/b.b)); }

struct Particle{ Vec2 p,v,f; float m; };

struct Platform{
    Vec2 pos;
    Vec2 initialPos;
    bool broken;
    bool falling;
    bool moving;
    bool jump;
    Color col;
    Particle centerPlat;
};

struct Monster{ Vec2 pos; Image monsterImg; bool alive; };

struct BlackHole{
    Vec2 pos; float m; bool active = false; Image blackHoleImg;
};

struct Object{ Vec2 pos; Image objectImg; bool isTouch; };

struct Projectile { Vec2 pos; Vec2 v; bool isActive; };

struct World{
    Platform plat[MAX_PLATFORM]; int nbPlat;
    Monster monster[MAX_MONSTERS]; int nbMonster;
    Image player; bool playerProtection = false;
    BlackHole bh[MAX_BLACK_HOLE]; int nbBlackHole;
    Projectile projectiles[MAX_PROJECTILES]; int nbProjectiles;
    Object obj; bool isStarted = false; bool gameOver = false; bool showRestart = false;
};

inline bool checkCollision(Vec2 part1, float radius1, Vec2 part2, float radius2){
    float dx = part1.x - part2.x;
    float dy = part1.y - part2.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < (radius1 + radius2);
}

inline Color interpolationColor(Color a, Color b, float t){
    Color c; c.r = int(a.r*(1-t) + b.r*t); c.g = int(a.g*(1-t) + b.g*t); c.b = int(a.b*(1-t) + b.b*t); return c;
}



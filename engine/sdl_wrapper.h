// Wrapper SDL2 remplaçant l'API Grapic utilisée par le jeu
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

// Type d'image (équivalent Grapic::Image)
using Image = SDL_Texture*;

// Initialisation / fenêtre
bool winInit(const char* title, int w, int h);
void winClear();
bool winDisplay();
void winQuit();

// Dessin
void image_draw(Image img, float x, float y, int w, int h);
void color(int r,int g,int b);
void rectangle(float x1,float y1,float x2,float y2);
void circle(float cx, float cy, float radius);
void triangleFill(float x1,float y1,float x2,float y2,float x3,float y3);

// Ressources
Image image(const char* path);

// Entrées / utilitaires
bool isKeyPressed(SDL_Keycode key);
bool isMousePressed(Uint8 button);
void mousePos(int& x, int& y);
void setKeyRepeatMode(bool);
float elapsedTime();

// Texte
void fontSize(int size);
void print(int x, int y, int value);
void printText(int x, int y, const char* text);



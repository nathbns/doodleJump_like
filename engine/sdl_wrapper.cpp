#include "sdl_wrapper.h"
#include <iostream>
#include <string>

using namespace std;

namespace {
    struct SDL_Context {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool shouldQuit = false;
        int screenW = 0;
        int screenH = 0;
    } g_sdl;

    int g_draw_r = 0, g_draw_g = 0, g_draw_b = 0;
    TTF_Font* g_currentFont = nullptr;
    int g_currentFontSize = 24;
    std::string g_fontPath = "data/ttf/Roboto-Regular.ttf";
    Uint32 g_startTicks = 0;
    Uint32 g_mouseButtons = 0;
    int g_mouseX = 0, g_mouseY = 0;

    inline int toSDL_Y(float y, int h) { return (int)(g_sdl.screenH - y - h); }
    inline int toSDL_Y_pt(float y) { return (int)(g_sdl.screenH - y); }
}

bool winInit(const char* title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        cerr << "SDL_Init error: " << SDL_GetError() << endl;
        return false;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        cerr << "IMG_Init error: " << IMG_GetError() << endl;
        return false;
    }
    if (TTF_Init() != 0) {
        cerr << "TTF_Init error: " << TTF_GetError() << endl;
        return false;
    }
    g_sdl.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if (!g_sdl.window) {
        cerr << "SDL_CreateWindow error: " << SDL_GetError() << endl;
        return false;
    }
    g_sdl.renderer = SDL_CreateRenderer(g_sdl.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_sdl.renderer) {
        cerr << "SDL_CreateRenderer error: " << SDL_GetError() << endl;
        return false;
    }
    SDL_SetRenderDrawBlendMode(g_sdl.renderer, SDL_BLENDMODE_BLEND);
    g_startTicks = SDL_GetTicks();
    g_sdl.screenW = w;
    g_sdl.screenH = h;
    
    // Initialiser la police par défaut
    fontSize(24);
    
    return true;
}

void winClear() {
    SDL_SetRenderDrawColor(g_sdl.renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_sdl.renderer);
}

bool winDisplay() {
    SDL_RenderPresent(g_sdl.renderer);
    SDL_Event e;
    g_mouseButtons = SDL_GetMouseState(&g_mouseX, &g_mouseY);
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) g_sdl.shouldQuit = true;
    }
    return g_sdl.shouldQuit;
}

void winQuit() {
    if (g_currentFont) { TTF_CloseFont(g_currentFont); g_currentFont = nullptr; }
    if (g_sdl.renderer) { SDL_DestroyRenderer(g_sdl.renderer); g_sdl.renderer = nullptr; }
    if (g_sdl.window) { SDL_DestroyWindow(g_sdl.window); g_sdl.window = nullptr; }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

Image image(const char* path) {
    SDL_Texture* tex = IMG_LoadTexture(g_sdl.renderer, path);
    if (!tex) {
        cerr << "IMG_LoadTexture error ('" << path << "'): " << IMG_GetError() << endl;
    }
    return tex;
}

void image_draw(Image img, float x, float y, int w, int h) {
    if (!img) return;
    SDL_Rect dst{ (int)x, toSDL_Y(y, h), w, h };
    SDL_RenderCopy(g_sdl.renderer, img, nullptr, &dst);
}

void color(int r,int g,int b){ g_draw_r=r; g_draw_g=g; g_draw_b=b; }

void rectangle(float x1,float y1,float x2,float y2){
    float left = std::min(x1,x2);
    float right = std::max(x1,x2);
    float bottom = std::min(y1,y2);
    float top = std::max(y1,y2);
    SDL_Rect rect{ (int)left, (int)(g_sdl.screenH - top), (int)(right-left), (int)(top-bottom) };
    SDL_SetRenderDrawColor(g_sdl.renderer, g_draw_r, g_draw_g, g_draw_b, 255);
    SDL_RenderDrawRect(g_sdl.renderer, &rect);
}

void circle(float cx, float cy, float radius) {
    circleRGBA(g_sdl.renderer, (int)cx, toSDL_Y_pt(cy), (int)radius, g_draw_r, g_draw_g, g_draw_b, 255);
}

void triangleFill(float x1,float y1,float x2,float y2,float x3,float y3) {
    filledTrigonRGBA(
        g_sdl.renderer,
        (int)x1, toSDL_Y_pt(y1),
        (int)x2, toSDL_Y_pt(y2),
        (int)x3, toSDL_Y_pt(y3),
        g_draw_r, g_draw_g, g_draw_b, 255
    );
}

bool isKeyPressed(SDL_Keycode key) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    SDL_Scancode sc = SDL_GetScancodeFromKey(key);
    return state[sc] != 0;
}

bool isMousePressed(Uint8 button) { return (g_mouseButtons & SDL_BUTTON(button)) != 0; }

void mousePos(int& x, int& y) { x = g_mouseX; y = g_sdl.screenH - g_mouseY; }

void setKeyRepeatMode(bool) { /* SDL gère l'état clavier en continu */ }

float elapsedTime() { return (SDL_GetTicks() - g_startTicks) / 1000.0f; }

void fontSize(int size) {
    if (size == g_currentFontSize && g_currentFont) return;
    if (g_currentFont) { TTF_CloseFont(g_currentFont); g_currentFont = nullptr; }
    g_currentFontSize = size;
    g_currentFont = TTF_OpenFont(g_fontPath.c_str(), g_currentFontSize);
    if (!g_currentFont) {
        cerr << "TTF_OpenFont error for '" << g_fontPath << "': " << TTF_GetError() << endl;
        // Essayer avec une police alternative
        g_fontPath = "data/ttf/Raleway-Regular.ttf";
        g_currentFont = TTF_OpenFont(g_fontPath.c_str(), g_currentFontSize);
        if (!g_currentFont) {
            cerr << "TTF_OpenFont error for alternative font: " << TTF_GetError() << endl;
        }
    }
}

void print(int x, int y, int value) {
    if (!g_currentFont) return;
    SDL_Color col{ (Uint8)g_draw_r,(Uint8)g_draw_g,(Uint8)g_draw_b,255 };
    std::string text = std::to_string(value);
    SDL_Surface* surf = TTF_RenderText_Solid(g_currentFont, text.c_str(), col);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(g_sdl.renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }
    SDL_Rect dst{ x, toSDL_Y((float)y, surf->h), surf->w, surf->h };
    SDL_RenderCopy(g_sdl.renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

void printText(int x, int y, const char* text) {
    if (!text) return;
    
    if (!g_currentFont) {
        cerr << "Font not loaded, trying to initialize..." << endl;
        fontSize(g_currentFontSize);
        if (!g_currentFont) {
            cerr << "Failed to load font, text will not display" << endl;
            return;
        }
    }
    
    SDL_Color col{ (Uint8)g_draw_r,(Uint8)g_draw_g,(Uint8)g_draw_b,255 };
    SDL_Surface* surf = TTF_RenderText_Solid(g_currentFont, text, col);
    if (!surf) {
        cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << endl;
        return;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(g_sdl.renderer, surf);
    if (!tex) { 
        SDL_FreeSurface(surf); 
        cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
        return; 
    }
    
    // Utiliser des coordonnées SDL standard (origine en haut à gauche)
    SDL_Rect dst{ x, y, surf->w, surf->h };
    SDL_RenderCopy(g_sdl.renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}



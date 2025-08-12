#include "engine/sdl_wrapper.h"
#include "game/types.h"
#include "game/render.h"
#include "game/gameplay.h"
#include <cstdlib>
#include <ctime>

int main(int, char**){
    Particle p; // player
    World w;
    w.nbProjectiles = 0;

    bool stop = false;
    if (!winInit("Doodle Jump", DIMW, DIMH)) return 1;

    srand((unsigned)time(nullptr));

    p = partInit(make_vec2(DIMW/2.0f, 60.0f), make_vec2(0, 7), 1.0f);
    Vec2 g = make_vec2(0,-9.81f);

    initPlatforms(w, MAX_PLATFORM, 4, 3, 3);
    initMonster(w, MAX_MONSTERS);
    initBlackHole(w, MAX_BLACK_HOLE);
    initObject(w);

    Image background = image("data/background.png");
    Image gameOver = image("data/gameOver.png");
    Image pressStart = image("data/pressStart.png");

    int score = 0;

    while (!stop) {
        winClear();
        setKeyRepeatMode(true);
        image_draw(background, 0, 0, DIMW, DIMH);

        if (w.gameOver) {
            image_draw(gameOver, 150, 300, 300, 400);
            drawFinalScore(score);
            drawRestartButton(w);
            
            // Gestion du restart avec la touche R
            if(isKeyPressed(SDLK_r)){
                restartGame(w, p, score);
            }
            
            // Gestion du restart via bouton
            if(!w.showRestart){
                restartGame(w, p, score);
            }
        } else if (!w.isStarted) {
            image_draw(pressStart, 125, DIMH/2, 350, 100);
            
            // Instructions de contrôle - traduites en anglais
            fontSize(20);
            color(0, 0, 0); // NOIR pour être visible
            printText(DIMW/2 - 60, 200, "Controls:");
            fontSize(16);
            color(50, 50, 50); // GRIS FONCÉ pour être visible
            printText(DIMW/2 - 120, 250, "Left/Right arrows - Move");
            printText(DIMW/2 - 120, 280, "Left click - Shoot projectiles");
            printText(DIMW/2 - 120, 310, "S or right click - Start");
            
            handlePart(p, w);
        } else {
            drawPlayer(p, w.player);
            drawPlatforms(w);
            drawMonsters(w);
            drawBlackHole(w);
            drawObj(w);

            handlePart(p, w);
            partAddForce(p, g);
            updateParticle(p, w, MAX_PLATFORM, score);
            updatePlatBroken(w);
            updatePlatMoving(w);
            updateAndDrawProjectiles(w, score);
            youLoose(w, p);
        }
        stop = winDisplay();
    }
    winQuit();
    return 0;
}

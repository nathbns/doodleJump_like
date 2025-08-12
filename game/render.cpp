#include "render.h"
#include "../engine/sdl_wrapper.h"

void drawPlatforms(World w){
    for(int i = 0; i < w.nbPlat; i++){
        Color c = w.plat[i].col;
        color(c.r,c.g,c.b);
        rectangle(w.plat[i].pos.x, w.plat[i].pos.y, w.plat[i].pos.x + PLATFORM_WIDTH,w.plat[i].pos.y + PLATFORM_HEIGHT);
        rectangle(w.plat[i].pos.x+1, w.plat[i].pos.y+1, w.plat[i].pos.x + PLATFORM_WIDTH+1,w.plat[i].pos.y + PLATFORM_HEIGHT+1);
    }
}

void drawPlayer(Particle& p, Image player){ image_draw(player,p.p.x,p.p.y,30,40); }

void drawMonsters(World& w){
    for(int i = 0; i < w.nbMonster; i++){
        if(w.monster[i].alive == true){
            image_draw(w.monster[i].monsterImg, w.monster[i].pos.x, w.monster[i].pos.y, 30, 40);
        }
    }
}

void drawBlackHole(World& w){
    for(int i = 0; i < w.nbBlackHole; i++){
        image_draw(w.bh[i].blackHoleImg, w.bh[i].pos.x, w.bh[i].pos.y, 50, 60);
    }
}

void drawObj(World w){
    if(!w.obj.isTouch){ image_draw(w.obj.objectImg, w.obj.pos.x, w.obj.pos.y, 30, 40); }
}

void drawRestartButton(World& w){
    if(w.showRestart){
        
        fontSize(20);
        color(255, 0, 0);
        printText(DIMW/2 -50, 555, "RESTART");
        
        // Instructions en dessous
        fontSize(16);
        color(0, 0, 0);
        printText(DIMW/2 - 70, 580, "Press R to restart");
    }
}

void drawFinalScore(int score){
    fontSize(24);
    color(0, 0, 0);
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Final Score: %d", score);
    printText(DIMW/2 - 70, 660, buffer);
}



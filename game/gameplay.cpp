#include "gameplay.h"
#include "../engine/sdl_wrapper.h"

Particle partInit(Vec2 p, Vec2 v, float m){
    Particle pa; pa.p = p; pa.v = v; pa.f = make_vec2(0,0); pa.m = m; return pa;
}

void initPlatforms(World& w, int nbPlat, int nbPlatBroken, int nbPlatMoving, int nbPlatJump){
    int x,y, nbBroken = 0, nbMoving = 0, nbJump = 0;
    w.nbPlat = nbPlat;
    w.player = image("data/player.png");

    // Plateforme de départ garantie sous le joueur
    w.plat[0].centerPlat.v = make_vec2(0,0);
    w.plat[0].centerPlat.f = make_vec2(0,0);
    w.plat[0].centerPlat.m = 1.0f;
    w.plat[0].falling = false;
    w.plat[0].moving = false;
    w.plat[0].broken = false;
    w.plat[0].jump = false;
    w.plat[0].pos = make_vec2((DIMW - PLATFORM_WIDTH)/2, 20);
    w.plat[0].initialPos = w.plat[0].pos;
    w.plat[0].col = make_color(0,0,0);
    w.plat[0].centerPlat.p = make_vec2(w.plat[0].pos.x, w.plat[0].pos.y);

    for(int i = 1; i < w.nbPlat;i++){
        w.plat[i].centerPlat.v = make_vec2(0,0);
        w.plat[i].centerPlat.f = make_vec2(0,0);
        w.plat[i].centerPlat.m = 1.0f;
        w.plat[i].falling = false;
        x = rand() % (DIMW - PLATFORM_WIDTH);
        // Placement en colonnes montantes depuis le bas, écart régulier
        y = 40 + i * ((DIMH - 100) / w.nbPlat);
        w.plat[i].pos = make_vec2(x,y);
        w.plat[i].initialPos = w.plat[i].pos;
        Vec2 centerRect = make_vec2(w.plat[i].pos.x, w.plat[i].pos.y);
        w.plat[i].centerPlat.p = centerRect;

        if(nbMoving < nbPlatMoving && i > 5){ w.plat[i].moving = rand() % 2; } else { w.plat[i].moving = 0; }
        if(nbBroken < nbPlatBroken && i > 5){ w.plat[i].broken = rand() % 2; } else { w.plat[i].broken = 0; }
        if(!w.plat[i].broken || !w.plat[i].moving){
            if(nbJump < nbPlatJump && i > 5){ w.plat[i].jump = rand() % 2; } else { w.plat[i].jump = 0; }
        }

        if(w.plat[i].moving == 1 && w.plat[i].broken == 1){ w.plat[i].col = make_color(255,0,255); }
        else if(w.plat[i].moving == 1){ w.plat[i].col = make_color(0,0,255); nbMoving++; }
        else if(w.plat[i].broken == 1){ w.plat[i].col = make_color(255,0,0); nbBroken++; }
        else if(w.plat[i].jump == 1){ w.plat[i].col = make_color(0,255,0); nbJump++; }
        else { w.plat[i].col = make_color(0,0,0); }
    }
}

void initMonster(World& w, int nbMonster){
    w.nbMonster = nbMonster;
    int x,y;
    for(int i = 0; i < w.nbMonster; i++){
        w.monster[i].alive = true;
        w.monster[i].monsterImg = image("data/monster.png");
        x = rand() % DIMW; y = i * (DIMH / w.nbMonster) + 200;
        w.monster[i].pos = make_vec2(x, y);
    }
}

void initBlackHole(World& w, int nbHole){
    w.nbBlackHole = nbHole;
    int x,y;
    for(int i = 0; i < w.nbBlackHole; i++){
        w.bh[i].blackHoleImg = image("data/blackHole.png");
        x = rand() % DIMW; y = i * (DIMH / w.nbBlackHole) + 200;
        w.bh[i].pos = make_vec2(x, y);
    }
}

void initObject(World& w){
    int x,y; w.obj.objectImg = image("data/shield.png");
    x = rand() % DIMW; y = 200; w.obj.pos = make_vec2(x, y); w.obj.isTouch = false;
}

void createProjectile(World& w, Vec2 pos, Vec2 v){
    if (w.nbProjectiles < MAX_PROJECTILES){
        w.projectiles[w.nbProjectiles].pos = pos;
        w.projectiles[w.nbProjectiles].v = v;
        w.projectiles[w.nbProjectiles].isActive = true;
        w.nbProjectiles++;
    }
}

void partAddForce(Particle& p, Vec2 force){ p.f = p.f + force; }

void updatePlatBroken(World& w){
    for(int i = 0; i < w.nbPlat; i++){
        if(w.plat[i].falling){
            w.plat[i].centerPlat.f = make_vec2(0,-9.81f);
            w.plat[i].centerPlat.v.y += (w.plat[i].centerPlat.f.y / w.plat[i].centerPlat.m) * 0.01f;
            w.plat[i].centerPlat.p.y += w.plat[i].centerPlat.v.y * 0.01f;
            w.plat[i].pos.y = w.plat[i].centerPlat.p.y;
        }
    }
}

void updatePlatMoving(World& w){
    for(int i = 0; i < w.nbPlat; i++){
        if(w.plat[i].moving) {
            if (w.plat[i].centerPlat.v.x == 0){ w.plat[i].centerPlat.v.x = 0.1f; }
            w.plat[i].pos.x += w.plat[i].centerPlat.v.x;

            if (w.plat[i].pos.x + PLATFORM_WIDTH > DIMW || w.plat[i].pos.x < 0){
                w.plat[i].centerPlat.v.x = -w.plat[i].centerPlat.v.x;
                if (w.plat[i].pos.x + PLATFORM_WIDTH > DIMW){ w.plat[i].pos.x = DIMW - PLATFORM_WIDTH; }
                else if (w.plat[i].pos.x < 0){ w.plat[i].pos.x = 0; }
            }
        }
    }
}

void updateParticle(Particle& p, World& w, int /*nbPlat*/, int &score){
        float pPrevY = p.p.y;
        // Scroll par frame (comportement d'origine)
        float speed = vitesseJeu;
        p.p = p.v*dt+p.p;
        p.v = (p.f/p.m)*dt+p.v;
        p.f = make_vec2(0,0);

        if(p.p.x < 0){ p.p.x = -p.p.x; p.v.x = -p.v.x; }
        if(p.p.x > DIMW){ p.p.x = DIMW - (p.p.x - DIMW); p.v.x = -p.v.x; }

        for(int i=0;i<w.nbPlat;i++){
            Vec2 topLeft = make_vec2(w.plat[i].pos.x, w.plat[i].pos.y + PLATFORM_HEIGHT);
            Vec2 topRight = make_vec2(w.plat[i].pos.x + PLATFORM_WIDTH, w.plat[i].pos.y + PLATFORM_HEIGHT);

            if(p.p.y < DIMH/3){
                w.plat[i].pos = w.plat[i].pos - make_vec2(0,speed);
                w.plat[i].initialPos = w.plat[i].initialPos - make_vec2(0,speed);
                w.plat[i].centerPlat.p = w.plat[i].centerPlat.p - make_vec2(0,speed);
            }else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){
                w.plat[i].pos = w.plat[i].pos - make_vec2(0,speed*1.5f);
                w.plat[i].initialPos = w.plat[i].initialPos - make_vec2(0,speed*1.5f);
                w.plat[i].centerPlat.p = w.plat[i].centerPlat.p - make_vec2(0,speed*1.5f);
            }else{
                w.plat[i].pos = w.plat[i].pos - make_vec2(0,speed*1.7f);
                w.plat[i].initialPos = w.plat[i].initialPos - make_vec2(0,speed*1.7f);
                w.plat[i].centerPlat.p = w.plat[i].centerPlat.p - make_vec2(0,speed*1.7f);
            }

            if(p.p.y < topLeft.y && p.p.y > w.plat[i].pos.y && p.p.x >= topLeft.x && p.p.x <= topRight.x){
                // Rebond si on vient d'au-dessus (évite les rebonds depuis le dessous)
                if (pPrevY >= topLeft.y){
                    if(w.plat[i].jump){ p.p.y = topLeft.y; p.v.y = JUMP_VELOCITY * JUMP_MULTIPLIER; }
                    else { p.p.y = topLeft.y; p.v.y = JUMP_VELOCITY; }

                    if (!w.plat[i].broken){ score += 10; }
                    else { score += 5; w.plat[i].falling = true; }
                }
            }

            if(w.plat[i].pos.y < 0 && !w.plat[i].broken && !w.plat[i].falling){
                w.plat[i].pos.y = DIMH;
                w.plat[i].pos.x = rand() % (DIMW - PLATFORM_WIDTH) + PLATFORM_WIDTH / 2;
                w.plat[i].centerPlat.v = make_vec2(0,0);
                w.plat[i].centerPlat.f = make_vec2(0,0);
                w.plat[i].centerPlat.m = 1.0f;
                Vec2 centerRect = make_vec2(w.plat[i].pos.x + PLATFORM_WIDTH / 2, w.plat[i].pos.y + PLATFORM_HEIGHT / 2);
                w.plat[i].centerPlat.p = centerRect;
            }else if(w.plat[i].pos.y < 0 && w.plat[i].broken && w.plat[i].falling){
                w.plat[i].pos.y =  w.plat[i].initialPos.y + DIMH;
                w.plat[i].pos.x = rand() % (DIMW - PLATFORM_WIDTH) + PLATFORM_WIDTH / 2;
                w.plat[i].falling = false;
                w.plat[i].centerPlat.v = make_vec2(0,0);
                w.plat[i].centerPlat.f = make_vec2(0,0);
                w.plat[i].centerPlat.m = 1.0f;
                Vec2 centerRect = make_vec2(w.plat[i].pos.x + PLATFORM_WIDTH / 2, w.plat[i].pos.y + PLATFORM_HEIGHT / 2);
                w.plat[i].centerPlat.p = centerRect;
                w.plat[i].initialPos = make_vec2(w.plat[i].pos.x,w.plat[i].pos.y) - make_vec2(0,vitesseJeu);
            }
        }

        for(int i = 0; i< w.nbMonster;i++){
            if(p.p.y < DIMH/3){ w.monster[i].pos = w.monster[i].pos - make_vec2(0,speed); }
            else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){ w.monster[i].pos = w.monster[i].pos - make_vec2(0,speed*1.5f); }
            else { w.monster[i].pos = w.monster[i].pos - make_vec2(0,speed*1.7f); }
            if(w.monster[i].pos.y < 0){ w.monster[i].pos = make_vec2(rand() % DIMW, DIMH-5); if(!w.monster[i].alive){ w.monster[i].alive = rand()%2; } }
        }

        for(int i = 0; i< w.nbBlackHole;i++){
            if(p.p.y < DIMH/3){ w.bh[i].pos = w.bh[i].pos - make_vec2(0,speed); }
            else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){ w.bh[i].pos = w.bh[i].pos - make_vec2(0,speed*1.5f); }
            else { w.bh[i].pos = w.bh[i].pos - make_vec2(0,speed*1.7f); }

            Vec2 dir = w.bh[i].pos - p.p;
            float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if(distance < 50){
                Vec2 attractionDirection = dir / distance; float f = 1.5f; Vec2 attractionForce = f * attractionDirection;
                p.v = p.v + attractionForce * dt;
            }
            if(w.bh[i].pos.y < 0){ w.bh[i].pos = make_vec2(rand() % DIMW, DIMH-5); }
        }

        w.obj.pos = w.obj.pos - make_vec2(0,vitesseJeu);
        if(checkCollision(p.p, 6, w.obj.pos, 20)){ w.obj.isTouch = true; }
        if(w.obj.isTouch){
            w.playerProtection = true;
            color(0,0,0);
            circle(p.p.x+15, p.p.y+10, 50);
            circle(p.p.x+15, p.p.y+10, 51);
            circle(p.p.x+15, p.p.y+10, 52);
        }
        if(p.p.y < DIMH/3){ w.obj.pos = w.obj.pos - make_vec2(0,speed); }
        else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){ w.obj.pos = w.obj.pos - make_vec2(0,speed*1.5f); }
        else { w.obj.pos = w.obj.pos - make_vec2(0,speed*1.7f); }

        if(w.obj.pos.y < 0){ w.obj.pos = make_vec2(rand() % DIMW, DIMH-5); if(w.obj.isTouch){ w.obj.isTouch = false; w.playerProtection = false; } }

        // Affichage du score en cours de jeu avec meilleur contraste
        fontSize(32);
        // Ombre du texte pour meilleur contraste
        color(0,0,0);
        print(DIMW/2-1,DIMW+221,score);
        print(DIMW/2+1,DIMW+221,score);
        print(DIMW/2,DIMW+220,score);
        print(DIMW/2,DIMW+222,score);
        // Texte principal en jaune brillant
        color(255,255,0);
        print(DIMW/2,DIMW+221,score);
}

void youLoose(World& w, Particle p){
    if(p.p.y < 0){ w.gameOver = true; w.showRestart = true; }
    for(int i=0;i<w.nbMonster;i++){
        if(w.monster[i].alive && checkCollision(p.p, PARTICLE_RADIUS, w.monster[i].pos, 20) && !w.playerProtection){ 
            w.gameOver = true; w.showRestart = true; break; 
        }
    }
}

void handlePart(Particle& p, World& w){
    if(isKeyPressed(SDLK_s) || isMousePressed(SDL_BUTTON_RIGHT)){
        if (!w.isStarted){
            // Transition vers démarrage: position sûre du joueur au-dessus d'une plateforme centrale
            p.p = make_vec2(DIMW/2.0f, 60.0f);
            p.v = make_vec2(0.0f, 7.0f);
            w.gameOver = false;
            w.showRestart = false;
        }
        w.isStarted = true;
    }
    if(w.isStarted && !w.gameOver){
        if (isKeyPressed(SDLK_LEFT)){ p.p.x -= 2.0f; }
        if (isKeyPressed(SDLK_RIGHT)){ p.p.x += 2.0f; }
        if (isMousePressed(SDL_BUTTON_LEFT)){
            int mouse_x, mouse_y; mousePos(mouse_x, mouse_y);
            Vec2 m = make_vec2(mouse_x, mouse_y);
            Vec2 playerPos = p.p;
            Vec2 direction = m - playerPos;
            float norm = std::sqrt(direction.x*direction.x + direction.y*direction.y);
            if (norm > 0.0001f){
                Vec2 vitesseProject = direction / norm * 0.5f;
                createProjectile(w, playerPos, vitesseProject);
            }
        }
    }
}

void updateAndDrawProjectiles(World& w, int &score){
    float rotationSpeed = 360;
    for (int i = 0; i < w.nbProjectiles; i++){
        if (w.projectiles[i].isActive){
            w.projectiles[i].pos = w.projectiles[i].pos + w.projectiles[i].v;
            Color a = make_color(0, 0, 255);
            Color b = make_color(255, 0, 0);
            Color resCol = interpolationColor(a, b, std::cos(elapsedTime()) * 0.5f);
            color(resCol.r, resCol.g, resCol.b);
            float rotationAngle = elapsedTime() * rotationSpeed;
            float x = w.projectiles[i].pos.x; float y = w.projectiles[i].pos.y; float height = 6;
            Vec2 top = make_vec2(x, y - height);
            Vec2 left = make_vec2(x - height, y + height);
            Vec2 right = make_vec2(x + height, y + height);
            top = rotate(top, x, y, rotationAngle);
            left = rotate(left, x, y, rotationAngle);
            right = rotate(right, x, y, rotationAngle);
            triangleFill(top.x, top.y, left.x, left.y, right.x, right.y);
            for (int j = 0; j < w.nbMonster; j++){
                if (w.monster[j].alive && checkCollision(w.projectiles[i].pos, height, w.monster[j].pos, 20)){
                    score += 20; 
                    w.monster[j].alive = false; 
                    w.projectiles[i].isActive = false; 
                    // Réorganiser les projectiles actifs au lieu de tout remettre à zéro
                    for(int k = i; k < w.nbProjectiles - 1; k++){
                        w.projectiles[k] = w.projectiles[k + 1];
                    }
                    w.nbProjectiles--;
                    i--; // Réajuster l'index pour ne pas manquer le projectile suivant
                    break;
                }
            }
            if (w.projectiles[i].isActive && (w.projectiles[i].pos.x < 0 || w.projectiles[i].pos.x > DIMW || w.projectiles[i].pos.y < 0 || w.projectiles[i].pos.y > DIMH)){
                w.projectiles[i].isActive = false;
                // Réorganiser les projectiles actifs
                for(int k = i; k < w.nbProjectiles - 1; k++){
                    w.projectiles[k] = w.projectiles[k + 1];
                }
                w.nbProjectiles--;
                i--; // Réajuster l'index
            }
        }
    }
}

void restartGame(World& w, Particle& p, int& score){
    // Réinitialiser le score
    score = 0;
    
    // Réinitialiser l'état du monde
    w.isStarted = false;
    w.gameOver = false;
    w.showRestart = false;
    w.playerProtection = false;
    w.nbProjectiles = 0;
    
    // Réinitialiser le joueur
    p.p = make_vec2(DIMW/2.0f, 60.0f);
    p.v = make_vec2(0.0f, 7.0f);
    p.f = make_vec2(0.0f, 0.0f);
    p.m = 1.0f;
    
    // Réinitialiser les plateformes
    initPlatforms(w, MAX_PLATFORM, 4, 3, 3);
    
    // Réinitialiser les monstres
    initMonster(w, MAX_MONSTERS);
    
    // Réinitialiser les trous noirs
    initBlackHole(w, MAX_BLACK_HOLE);
    
    // Réinitialiser l'objet bouclier
    initObject(w);
    
    // Réinitialiser tous les projectiles
    for(int i = 0; i < MAX_PROJECTILES; i++){
        w.projectiles[i].isActive = false;
    }
}



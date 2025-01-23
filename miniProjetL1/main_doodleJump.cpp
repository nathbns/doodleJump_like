#include <Grapic.h>
#include <iostream>

using namespace grapic;
using namespace std;

const int DIMW = 600;
const int DIMH = DIMW + 300;
const int HEIGHT_GAME = 1000;
const int PLATFORM_HEIGHT = 20;
const int PLATFORM_WIDTH = 100;
const int MAX_PLATFORM = 15;
const int PARTICLE_RADIUS = 8;
const int MAX_MONSTERS = 5;
const int MAX_BLACK_HOLE = 2;
const float dt = 0.05;
const float vitesseJeu = 0.04;
const int MAX_PROJECTILES = 1;

struct Vec2 {
    float x, y;
};

Vec2 make_vec2(float r, float i) {
    Vec2 p;
    p.x = r;
    p.y = i;
    return p;
}

Vec2 make_vec2_exp(float r, float theta){
    return make_vec2(r * cos(theta), r * sin(theta));
}

Vec2 operator+(Vec2 a, Vec2 b){
    return make_vec2(a.x + b.x, a.y + b.y);
}

Vec2 operator-(Vec2 a, Vec2 b){
    return make_vec2(a.x - b.x, a.y - b.y);
}

Vec2 operator*(Vec2 a, Vec2 b){
    return make_vec2(a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y);
}

Vec2 operator/(Vec2 a, float b){
    return make_vec2(a.x/b, a.y/b);
}
Vec2 operator/(float a, Vec2 b){
    return make_vec2(a/b.x, a/b.y);
}

Vec2 operator*(float a, Vec2 b){
    return make_vec2(a * b.x, a * b.y);
}

Vec2 operator*(Vec2 a, float b){
    return make_vec2(a.x * b, a.y * b);
}

float to_rad(float deg){
    return M_PI * deg / 180.0;
}

Vec2 rotate(Vec2 p, float cx, float cy, float theta_deg){
   Vec2 rot = make_vec2_exp(1.0, to_rad(theta_deg));
   Vec2 center = make_vec2(cx, cy);
   return ((p - center) * rot) + center;
}

struct Color{
    int r,g,b;
};

Color make_color(int r, int g, int b){
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

Color operator+(Color a, Color b){
    return make_color(a.r+b.r,a.g+b.g,a.b+b.b);
}

Color operator-(Color a, Color b){
    return make_color(a.r-b.r,a.g-b.g,a.b-b.b);
}

Color operator+(float a, Color b){
    return make_color(a*b.r,a*b.g,a*b.b);
}

Color operator*(Color b, float a){
    return make_color(a*b.r,a*b.g,a*b.b);
}

Color operator/(Color b, float a){
    return make_color(a/b.r,a/b.g,a/b.b);
}

struct Particle{
    Vec2 p,v,f;
    float m;
};

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

struct Monster{
    Vec2 pos;
    Image monsterImg;
    bool alive;
};

struct BlackHole{
    Vec2 pos;
    float m;
    bool active = false;
    Image blackHoleImg;
};

struct Object{
    Vec2 pos;
    Image objectImg;
    bool isTouch;
};

struct Projectile {
    Vec2 pos;
    Vec2 v;
    bool isActive;
};

struct World{
    Platform plat[MAX_PLATFORM];
    int nbPlat;
    Monster monster[MAX_MONSTERS];
    int nbMonster;
    Image player;
    bool playerProtection = false;
    BlackHole bh[MAX_BLACK_HOLE];
    int nbBlackHole;
    Projectile projectiles[MAX_PROJECTILES];
    int nbProjectiles;
    Object obj;
    bool isStarted = false;
    bool gameOver = false;
};

Particle partInit(Vec2 p, Vec2 v, float m){
    Particle pa;
    pa.p = p;
    pa.v = v;
    pa.f = make_vec2(0,0);
    pa.m = m;
    return pa;
}

void initPlatforms(World& w, int nbPlat, int nbPlatBroken, int nbPlatMoving, int nbPlatJump){
    int x,y, nbBroken = 0, nbMoving = 0, nbJump = 0;
    w.nbPlat = nbPlat;
    w.player = image("data/player.png");

    for(int i = 0; i < w.nbPlat;i++){
        w.plat[i].centerPlat.v = make_vec2(0,0);
        w.plat[i].centerPlat.f = make_vec2(0,0);
        w.plat[i].centerPlat.m = 1.0;
        w.plat[i].falling = false;
        x = rand() % (DIMW - PLATFORM_WIDTH);
        y = i * (DIMH / w.nbPlat);
        w.plat[i].pos = make_vec2(x,y);
        w.plat[i].initialPos = w.plat[i].pos;
        Vec2 centerRect = make_vec2(w.plat[i].pos.x, w.plat[i].pos.y);
        w.plat[i].centerPlat.p = centerRect;

        if(nbMoving < nbPlatMoving && i > 5){
            w.plat[i].moving = rand() % 2;
        }else{
            w.plat[i].moving = 0;
        }

        if(nbBroken < nbPlatBroken && i > 5){
            w.plat[i].broken = rand() % 2;
        }else{
            w.plat[i].broken = 0;
        }

        if(!w.plat[i].broken || !w.plat[i].moving){
            if(nbJump < nbPlatJump && i > 5){ // pas de plat spé dès le debut
                w.plat[i].jump = rand() % 2;
            }else{
                w.plat[i].jump = 0;
            }
        }

        if(w.plat[i].moving == 1 && w.plat[i].broken == 1){
            w.plat[i].col = make_color(255,0,255);
        }else if(w.plat[i].moving == 1){
            w.plat[i].col = make_color(0,0,255);
            nbMoving++;
        }else if(w.plat[i].broken == 1){
            w.plat[i].col = make_color(255,0,0);
            nbBroken++;
        }else if(w.plat[i].jump == 1){
            w.plat[i].col = make_color(0,255,0);
            nbJump++;
        }else{
            w.plat[i].col = make_color(0,0,0);
        }
    }
}

void initMonster(World& w, int nbMonster){
    w.nbMonster = nbMonster;
    int x,y;
    for(int i = 0; i < w.nbMonster; i++){
        w.monster[i].alive = true;
        w.monster[i].monsterImg = image("/data/monster.png");
        x = rand() % DIMW;
        y = i * (DIMH / w.nbMonster) + 200;
        w.monster[i].pos = make_vec2(x, y);
    }
}

void initBlackHole(World& w, int nbHole){
    w.nbBlackHole = nbHole;
    int x,y;
    for(int i = 0; i < w.nbBlackHole; i++){
        w.bh[i].blackHoleImg = image("/data/blackHole.png");
        x = rand() % DIMW;
        y = i * (DIMH / w.nbBlackHole) + 200;
        w.bh[i].pos = make_vec2(x, y);
    }
}

void initObject(World& w){
    int x,y;
    w.obj.objectImg = image("/data/shield.png");
    x = rand() % DIMW;
    y = 200;
    w.obj.pos = make_vec2(x, y);
    w.obj.isTouch = false;
}

void createProjectile(World& w, Vec2 pos, Vec2 v){
    if (w.nbProjectiles < MAX_PROJECTILES){
        w.projectiles[w.nbProjectiles].pos = pos;
        w.projectiles[w.nbProjectiles].v = v;
        w.projectiles[w.nbProjectiles].isActive = true;
        w.nbProjectiles++;
    }
}

void partAddForce(Particle& p, Vec2 force){
    p.f = p.f + force;
}

void drawPlatforms(World w){
    for(int i = 0; i < w.nbPlat; i++){
        Color c = w.plat[i].col;
        color(c.r,c.g,c.b);
        rectangle(w.plat[i].pos.x, w.plat[i].pos.y, w.plat[i].pos.x + PLATFORM_WIDTH,w.plat[i].pos.y + PLATFORM_HEIGHT);
        rectangle(w.plat[i].pos.x+1, w.plat[i].pos.y+1, w.plat[i].pos.x + PLATFORM_WIDTH+1,w.plat[i].pos.y + PLATFORM_HEIGHT+1); // Pour doubler l'epaisseur du rectangle
    }
}

void drawPlayer(Particle& p, Image player){
    image_draw(player,p.p.x,p.p.y,30,40);
}

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
    if(!w.obj.isTouch){
        image_draw(w.obj.objectImg, w.obj.pos.x, w.obj.pos.y, 30, 40);
    }
}

void updatePlatBroken(World& w){ // Update plateformes cassées
    for(int i = 0; i < w.nbPlat; i++){
        if(w.plat[i].falling){
            w.plat[i].centerPlat.f = make_vec2(0,-9.81);
            w.plat[i].centerPlat.v.y += (w.plat[i].centerPlat.f.y / w.plat[i].centerPlat.m) * 0.01;
            w.plat[i].centerPlat.p.y += w.plat[i].centerPlat.v.y * 0.01;
            w.plat[i].pos.y = w.plat[i].centerPlat.p.y;
        }
    }
}

void updatePlatMoving(World& w){ // Update plateforme qui bouge le long de la largeur de la fenetre (les bleus)
    for(int i = 0; i < w.nbPlat; i++){
        if(w.plat[i].moving) {
            if (w.plat[i].centerPlat.v.x == 0){
                w.plat[i].centerPlat.v.x = 0.1;
            }
            w.plat[i].pos.x += w.plat[i].centerPlat.v.x;

            if (w.plat[i].pos.x + PLATFORM_WIDTH > DIMW || w.plat[i].pos.x < 0){
                w.plat[i].centerPlat.v.x = -w.plat[i].centerPlat.v.x;
                if (w.plat[i].pos.x + PLATFORM_WIDTH > DIMW){
                    w.plat[i].pos.x = DIMW - PLATFORM_WIDTH;
                } else if (w.plat[i].pos.x < 0){
                    w.plat[i].pos.x = 0;
                }
            }
        }
    }
}

bool checkCollision(Vec2 part1, float radius1, Vec2 part2, float radius2){
    float dx = part1.x - part2.x;
    float dy = part1.y - part2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (radius1 + radius2);
}

void updateParticle(Particle& p, World& w, int nbPlat, int &score){ // update de tout ce qui influence le joueur
        float pPrevY = p.p.y;

        float speed = vitesseJeu;

        p.p = p.v*dt+p.p;
        p.v = (p.f/p.m)*dt+p.v;
        p.f = make_vec2(0,0);

        if(p.p.x < 0){
            p.p.x = -p.p.x;
            p.v.x = -p.v.x;
        }

        if(p.p.x > DIMW){
            p.p.x = DIMW - (p.p.x - DIMW);
            p.v.x = -p.v.x;
        }

        // Condition collision entre platforme et particule
        for(int i=0;i<w.nbPlat;i++){
            Vec2 topLeft = make_vec2(w.plat[i].pos.x, w.plat[i].pos.y + PLATFORM_HEIGHT); // topLeft plateform
            Vec2 topRight = make_vec2(w.plat[i].pos.x + PLATFORM_WIDTH, w.plat[i].pos.y + PLATFORM_HEIGHT); //topRight plateform

            // Mise à jour vitesse plateforme en fonction de la position du joueur
            if(p.p.y < DIMH/3){
                w.plat[i].pos = w.plat[i].pos - make_vec2(0,speed);
                w.plat[i].initialPos = w.plat[i].initialPos - make_vec2(0,speed);
                w.plat[i].centerPlat.p = w.plat[i].centerPlat.p - make_vec2(0,speed);
            }else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){
                w.plat[i].pos = w.plat[i].pos - make_vec2(0,speed*1.5);
                w.plat[i].initialPos = w.plat[i].initialPos - make_vec2(0,speed*1.5);
                w.plat[i].centerPlat.p = w.plat[i].centerPlat.p - make_vec2(0,speed*1.5);
            }else{
                w.plat[i].pos = w.plat[i].pos - make_vec2(0,speed*1.7);
                w.plat[i].initialPos = w.plat[i].initialPos - make_vec2(0,speed*1.7);
                w.plat[i].centerPlat.p = w.plat[i].centerPlat.p - make_vec2(0,speed*1.7);
            }

            if(p.p.y < topLeft.y && p.p.y > w.plat[i].pos.y && p.p.x >= topLeft.x && p.p.x <= topRight.x){
                // Vérifie si le joueur vient de dessous de la plateforme et s'il ne devrait pas rebondir
                if (!(pPrevY < topLeft.y)){
                    if(w.plat[i].jump){
                        p.p.y = topLeft.y;
                        p.v.y = 7*1.15; //  Augmentation de 15% de la capacité de saut
                    }else{
                        p.p.y = topLeft.y;
                        p.v.y = 7;
                    }

                    if (!w.plat[i].broken){
                        score += 10;
                    }else{
                        score += 5;
                        w.plat[i].falling = true;
                    }
                }
            }

            if(w.plat[i].pos.y < 0 && !w.plat[i].broken && !w.plat[i].falling){
                w.plat[i].pos.y = DIMH; // Si la platform sort de la fenetre, plat deplacé vers le haut
                w.plat[i].pos.x = rand() % (DIMW - PLATFORM_WIDTH) + PLATFORM_WIDTH / 2; // nouvelle pos alea
                w.plat[i].centerPlat.v = make_vec2(0,0);
                w.plat[i].centerPlat.f = make_vec2(0,0);
                w.plat[i].centerPlat.m = 1.0;
                Vec2 centerRect = make_vec2(w.plat[i].pos.x + PLATFORM_WIDTH / 2, w.plat[i].pos.y + PLATFORM_HEIGHT / 2);
                w.plat[i].centerPlat.p = centerRect;
            }else if(w.plat[i].pos.y < 0 && w.plat[i].broken && w.plat[i].falling){
                w.plat[i].pos.y =  w.plat[i].initialPos.y + DIMH; // Si la platform cassé sort de la fenetre, platform deplacé vers le haut -> la taille de la fenetre + position init
                w.plat[i].pos.x = rand() % (DIMW - PLATFORM_WIDTH) + PLATFORM_WIDTH / 2;  // nouvelle pos alea
                w.plat[i].falling = false;
                w.plat[i].centerPlat.v = make_vec2(0,0);
                w.plat[i].centerPlat.f = make_vec2(0,0);
                w.plat[i].centerPlat.m = 1.0;
                Vec2 centerRect = make_vec2(w.plat[i].pos.x + PLATFORM_WIDTH / 2, w.plat[i].pos.y + PLATFORM_HEIGHT / 2);
                w.plat[i].centerPlat.p = centerRect;
                w.plat[i].initialPos = make_vec2(w.plat[i].pos.x,w.plat[i].pos.y) - make_vec2(0,vitesseJeu);
            }
        }

        //MONSTER
        for(int i = 0; i< w.nbMonster;i++){
            // Mise à jour vitesse monstres en fonction de la position du joueur
            if(p.p.y < DIMH/3){
                w.monster[i].pos = w.monster[i].pos - make_vec2(0,speed); // descend vers le bas
            }else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){
                w.monster[i].pos = w.monster[i].pos - make_vec2(0,speed*1.5);
            }else{
                w.monster[i].pos = w.monster[i].pos - make_vec2(0,speed*1.7);
            }

            if(w.monster[i].pos.y < 0){ // monstre sort de la fenetre, on le remet en haut
                w.monster[i].pos = make_vec2(rand() % DIMW, DIMH-5);
                if(!w.monster[i].alive){
                    w.monster[i].alive = rand()%2;
                }
            }
        }

       //BLACKHOLE
       for(int i = 0; i< w.nbBlackHole;i++){
            // pareil que pour les monstres et plateformes  -> mise a jour vitesse
            if(p.p.y < DIMH/3){
                 w.bh[i].pos = w.bh[i].pos - make_vec2(0,speed); // descend vers le bas
            }else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){
                 w.bh[i].pos = w.bh[i].pos - make_vec2(0,speed*1.5);
            }else{
                w.bh[i].pos = w.bh[i].pos - make_vec2(0,speed*1.7);
            }

            Vec2 dir = w.bh[i].pos - p.p;
            float distance = sqrt(dir.x * dir.x + dir.y * dir.y);
            if(distance < 50){
                // Calcule la force d'attraction en direction du trou noir
                Vec2 attractionDirection = dir / distance;
                float f = 1.5;
                Vec2 attractionForce = f * attractionDirection;

                // force d'attraction sur le  joueur
                p.v = p.v + attractionForce * dt;
            }else{
                p.v.x = 0;
            }

            if(w.bh[i].pos.y < 0){ // Si le trou noir sort de la fenêtre, on le remet en haut
                w.bh[i].pos = make_vec2(rand() % DIMW, DIMH-5);
            }
        }

        //OBJECT START
        w.obj.pos = w.obj.pos - make_vec2(0,vitesseJeu); // descend vers le bas
        if(checkCollision(p.p, 6, w.obj.pos, 20)){
            w.obj.isTouch = true;
        }
        if(w.obj.isTouch){
            w.playerProtection = true;
            color(0,0,0);
            circle(p.p.x+15, p.p.y+10, 50);
            circle(p.p.x+15, p.p.y+10, 51); // ->
            circle(p.p.x+15, p.p.y+10, 52); // -> Epaisseur du cercle
        }
        // mise à jour vitesse (pareil que pour les autres)
        if(p.p.y < DIMH/3){
            w.obj.pos = w.obj.pos - make_vec2(0,speed); // descend vers le bas
        }else if(p.p.y > DIMH/3 && p.p.y < (DIMH - DIMH/3)){
            w.obj.pos = w.obj.pos - make_vec2(0,speed*1.5);
        }else{
            w.obj.pos = w.obj.pos - make_vec2(0,speed*1.7);
        }

        if(w.obj.pos.y < 0){ // monstre sort de la fenetre, on le remet en haut
            w.obj.pos = make_vec2(rand() % DIMW, DIMH-5);
            if(w.obj.isTouch){
                w.obj.isTouch = false;
                w.playerProtection = false;
            }
        }

        fontSize(48);
        color(255,0,0);
        print(DIMW/2-1,DIMW+220,score); //->
        print(DIMW/2,DIMW+220,score); //->
        print(DIMW/2-1,DIMW+221,score); //->
        print(DIMW/2,DIMW+221,score); // Pour avoir un score en gras
}

void youLoose(World& w, Particle p){
    if(p.p.y < 0){
        w.gameOver = true; // Le joueur perd si il sort de l'écran
    }
    for(int i=0;i<w.nbMonster;i++){
        if(w.monster[i].alive && checkCollision(p.p, PARTICLE_RADIUS, w.monster[i].pos, 20) && !w.playerProtection){ // par hyp je met un radius de 20 pour le monstre
            w.gameOver = true;
            break; // On ne vérifie pas si on a touché d'autres monstre avec le break
        }
    }
}

void handlePart(Particle& p, World& w){ // Commande du jeu
    if(isKeyPressed(SDLK_s) || isMousePressed(SDL_BUTTON_RIGHT)){ // lance une partie avec la touche "s" ou un "click droit"
        w.isStarted = true;
    }
    if(w.isStarted){
        if (isKeyPressed(SDLK_LEFT)){
            p.p.x -= 0.5;
        }
        if (isKeyPressed(SDLK_RIGHT)){
            p.p.x += 0.5;
        }
        if (isMousePressed(SDL_BUTTON_LEFT)){
            int mouse_x, mouse_y;
            mousePos(mouse_x, mouse_y);
            Vec2 mousePos = make_vec2(mouse_x, mouse_y);
            Vec2 playerPos = p.p;
            Vec2 direction = mousePos - playerPos;
            float norm = sqrt(direction.x*direction.x + direction.y*direction.y);
            Vec2 vitesseProject = direction / norm * 0.5;
            createProjectile(w, playerPos, vitesseProject);
        }
    }
}

Color interpolationColor(Color a, Color b, float t){
    Color c;
    c.r = a.r*(1-t) + b.r*t;
    c.g = a.g*(1-t) + b.g*t;
    c.b = a.b*(1-t) + b.b*t;
    return c;
}

void updateAndDrawProjectiles(World& w, int &score){ // Update des projectiles
    float rotationSpeed = 360;

    for (int i = 0; i < w.nbProjectiles; i++){
        if (w.projectiles[i].isActive){
            w.projectiles[i].pos = w.projectiles[i].pos + w.projectiles[i].v;
            Color a = make_color(0, 0, 255);
            Color b = make_color(255, 0, 0);
            Color resCol = interpolationColor(a, b, cos(elapsedTime()) * 0.5);
            color(resCol.r, resCol.g, resCol.b);

            float rotationAngle = elapsedTime() * rotationSpeed;

            // Centre du triangle
            float x = w.projectiles[i].pos.x;
            float y = w.projectiles[i].pos.y;
            float height = 12;  // longueur coté triangle

            Vec2 top = make_vec2(x, y - height);
            Vec2 left = make_vec2(x - height, y + height);
            Vec2 right = make_vec2(x + height, y + height);

            // Rotation
            top = rotate(top, x, y, rotationAngle);
            left = rotate(left, x, y, rotationAngle);
            right = rotate(right, x, y, rotationAngle);

            triangleFill(top.x, top.y, left.x, left.y, right.x, right.y);

            // Collision monstre/projectile triangle
            for (int j = 0; j < w.nbMonster; j++){
                if (w.monster[j].alive && checkCollision(w.projectiles[i].pos, height, w.monster[j].pos, 20)){
                    score += 20;
                    w.monster[j].alive = false;
                    w.projectiles[i].isActive = false;
                    w.nbProjectiles = 0; // on remet à 0 le nombre de projectile,
                                        //  l'obj est de pouvoir tirer un projectile
                                       //   par projectile tant qu'on a pas touché un monstre ou sortie de la fenetre
                }
            }

            // Si sort de l'écran on désactive le projectile
            if (w.projectiles[i].pos.x < 0 || w.projectiles[i].pos.x > DIMW || w.projectiles[i].pos.y < 0 || w.projectiles[i].pos.y > DIMH){
                w.projectiles[i].isActive = false;
                w.nbProjectiles = 0;
            }
        }
    }
}

int main(int, char**){
    Particle p; // player
    World w;
    w.nbProjectiles = 0;

    bool stop = false;
    winInit("Doodle Jump", DIMW, DIMH);

    srand(time(NULL));

    p = partInit(make_vec2(250, 0), make_vec2(0, 7), 50);
    Vec2 g = make_vec2(0,-9.81);

    initPlatforms(w, MAX_PLATFORM, 4, 3,3);
    initMonster(w,MAX_MONSTERS);
    initBlackHole(w, MAX_BLACK_HOLE);
    initObject(w);

    Image background = image("/data/background.png");
    Image gameOver = image("/data/gameOver.png");
    Image pressStart = image("data/pressStart.png");

    int score = 0;

    while ( !stop ) {
        winClear();

        setKeyRepeatMode(true);

        image_draw(background,0,0,DIMW,DIMH);

        if(w.gameOver){
            image_draw(gameOver, 150, 300, 300, 400);
        }else if(!w.isStarted){
            image_draw(pressStart, 125, DIMH/2, 350, 100);
            handlePart(p, w);
        }else{
            drawPlayer(p,w.player);
            drawPlatforms(w);
            drawMonsters(w);
            drawBlackHole(w);
            drawObj(w);

            handlePart(p, w);

            partAddForce(p, g);

            updateParticle(p,w,MAX_PLATFORM,score);
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

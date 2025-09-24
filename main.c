#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 800
#define BASE_Y_LEVEL 350
#define BASE_X_POS 50
#define FALL_SPEED 4
#define GAME_SPEED 5

int nextSpawn = 0;

typedef struct{

    int posX;
    int posY;
    int height;
    int width;
    bool wasHit;

}Object;

typedef enum{
    PLAYING,
    GAME_OVER
}Screen_State;


void updatePlayer(int *playerY, Sound *s, Sound *j);
void drawPlayer(int *y,Texture2D *player,Texture2D *playerFlying);
void updateObject(Object *obj,Object *obj2,Object *obj3);
Object* newObject();
int randInt(int min, int max);
bool colCheck(int *playerY,Object* obj,Object* obj2,Object* obj3);
void drawObject(Object *obj, Texture2D *sprite);

int jetPackFuel = 100;
int global_score = 0;
bool stellaFlying = false;

int main(void)
{
    
    srand(time(NULL));
    int playerX = 0;
    int* pX = &playerX;
    int playerY = BASE_Y_LEVEL - 30;
    int* pY = &playerY;
    int objCount = 0;
    int hitCount = 0;
    bool wasColliding = false;
    char str[16];
    char scoreStr[16];
    char fuelStr[16];
    int counter = 0;
    int gameOverX = 0;
    int gameOverY = 0;

    Object *obj = newObject();
    Object *obj2 = newObject();
    Object *obj3 = newObject();
    obj2->posX += 100;
    obj3->posX += 300;
   
    

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stella Run");
    InitAudioDevice();

    Sound sound_jump = LoadSound("Assets/jump.mp3");
    Sound sound_jetpack = LoadSound("Assets/jetpack.mp3");

    SetTargetFPS(60);
    Texture2D stella = LoadTexture("Assets/stellapng.png");
    Texture2D stellaJetPack = LoadTexture("Assets/stellaJetpack.png");
    Texture2D cat1 = LoadTexture("Assets/cat1New.png");
    Texture2D cat2 = LoadTexture("Assets/cat2New.png");
    Texture2D cat3 = LoadTexture("Assets/cat3New.png");

    SetTextureFilter(stella,TEXTURE_FILTER_POINT);
    SetTextureFilter(stellaJetPack,TEXTURE_FILTER_POINT);  
    SetTextureFilter(cat1,TEXTURE_FILTER_POINT);
    SetTextureFilter(cat2,TEXTURE_FILTER_POINT);
    SetTextureFilter(cat3,TEXTURE_FILTER_POINT);

    Screen_State screenState = PLAYING;

    

    while (!WindowShouldClose())  
    {
        // varible update 
        switch (screenState)
        {
        case PLAYING:
            counter++;
            if(counter % 100 == 0){
                global_score++;
            }

            updatePlayer(pY,&sound_jump,&sound_jetpack);
            updateObject(obj,obj2,obj3);

            if(colCheck(&playerY, obj,obj2,obj3)){
                hitCount++;
            }

            if(global_score % 5 == 0){
                jetPackFuel++;
            }

            // convert int var to strings
            snprintf(str,16,"%d",hitCount); 
            snprintf(scoreStr,16,"%d",global_score);
            snprintf(fuelStr,16,"%d",jetPackFuel);

            if(hitCount != 0){
                screenState = GAME_OVER;
            }
            break;
        case GAME_OVER:
            if(gameOverY < SCREEN_HEIGHT - 32){
                gameOverY += 3;
            }else if(gameOverY <= 0){
                gameOverY -= 3;
            }

            break;
        
        default:
            break;
        }

        
        
        BeginDrawing();
            // draw functions
            switch (screenState)
            {
            case PLAYING:
                ClearBackground(BLACK);
                DrawRectangle(0,BASE_Y_LEVEL,SCREEN_WIDTH,2,WHITE);
                drawPlayer(pY, &stella,&stellaJetPack);
                drawObject(obj,&cat1);
                drawObject(obj2,&cat2);
                drawObject(obj3,&cat3);
                DrawText(str,SCREEN_WIDTH/2,20,16,WHITE);
                DrawText(scoreStr,SCREEN_WIDTH/2,50,16,WHITE);
                DrawText(fuelStr,20,20,16,WHITE);
                break;
            case GAME_OVER:
                ClearBackground(BLACK);
                DrawText("Game over :,(", SCREEN_WIDTH/2-(13),SCREEN_HEIGHT/2,32,WHITE);
                break;
            default:
                break;
            }
        EndDrawing();

    }

    // De-Initialization
    free(obj);
    free(obj2);
    free(obj3);
    
    UnloadSound(sound_jump);
    CloseAudioDevice();
    CloseWindow();       
    return 0;
}
void drawObject(Object *obj, Texture2D *sprite){
    float scale = 3.0f;
    DrawTexturePro(*sprite,
        (Rectangle){0,0,(float)obj->width,(float)obj->height},
        (Rectangle){(float)obj->posX,(float)obj->posY,obj->width*scale,(obj->height*scale)-17.0f},
        (Vector2){0,0},
        0.0f,
        WHITE
    );
}
void drawPlayer(int* y,Texture2D *player,Texture2D *playerFlying){
    float scale = 2.0f;
    if(stellaFlying){
        DrawTextureEx(*playerFlying,(Vector2){BASE_X_POS,(float)*y-10},0.0f,scale,WHITE);

    }else{
        DrawTextureEx(*player,(Vector2){BASE_X_POS,(float)*y-10},0.0f,scale,WHITE);
    }
}

void updatePlayer(int* playerY, Sound *s, Sound *j){
    static int vy = 0;                             
    const int GROUND_Y = BASE_Y_LEVEL - 30;

    if (IsKeyDown(KEY_SPACE) && *playerY >= GROUND_Y) {
        PlaySound(*s);
        vy = -18;
    }

    // gravity non-linear arc
    vy += (vy < 0) ? 1 : 1;                        
    if (vy > 24) vy = 24;                         

    *playerY += vy;

    if (*playerY > GROUND_Y) {// land and reset
        *playerY = GROUND_Y;
        vy = 0;
    }

    if (IsKeyPressed(KEY_J) && jetPackFuel > 0) {
        PlaySound(*j); 
                      
    }
    
    if(IsKeyDown(KEY_J) && jetPackFuel > 0){
        (*playerY) -= (vy);
        stellaFlying = true;
        jetPackFuel--;
    }

    if(IsKeyReleased(KEY_J) || jetPackFuel <= 0){
        StopSound(*j);
        stellaFlying = false;
    }

    //jetPackFuel++;
    
    
}

Object* newObject(){
    Object *newObj = malloc(sizeof(Object));
    newObj->height = 17;
    newObj->width = 30;
    newObj->posX = 810;
    newObj->posY = BASE_Y_LEVEL - newObj->height;

    return newObj;

}

void updateObject(Object *obj,Object *obj2,Object *obj3){

    if(obj->posX > 0){
        obj->posX-=GAME_SPEED;
    }else {
        obj->posX = 810 + randInt(100,300); 
        obj->wasHit = false;
    }

    if(obj2->posX > 0){
        obj2->posX-=GAME_SPEED;
    }else {
        obj2->posX = 900 + randInt(200,400);
        obj2->wasHit = false; 
    }
    if(obj3->posX > 0){
        obj3->posX-=GAME_SPEED;
    }else {
        obj3->posX = 1000 + randInt(300,500); 
        obj3->wasHit = false;
    }

}

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

bool colCheck(int *playerY,Object* obj,Object* obj2,Object* obj3){
    int bottomY = *playerY + 30;
    if(obj->posX >= BASE_X_POS && obj->posX <= BASE_X_POS + 30){
        if(bottomY >= obj->posY && bottomY <= obj->posY + obj->height && !obj->wasHit){
            obj->wasHit = true;
            return true;
        }
    }
    if(obj2->posX >= BASE_X_POS && obj2->posX <= BASE_X_POS + 30){
        if(bottomY >= obj2->posY && bottomY <= obj2->posY + obj2->height  && !obj2->wasHit){
            obj2->wasHit = true;
            return true;
        }
    }
    if(obj3->posX >= BASE_X_POS && obj3->posX <= BASE_X_POS + 30){
        if(bottomY >= obj3->posY && bottomY <= obj3->posY + obj3->height  && !obj3->wasHit){
            obj3->wasHit = true;
            return true;
        }
    }
    return false;
    

}


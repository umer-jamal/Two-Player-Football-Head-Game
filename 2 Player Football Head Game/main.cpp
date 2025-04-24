#include "raylib.h"
#include <iostream>
#include <string.h>
using namespace std;

float width = 1500;
float height = 750;
float scale = 0.50f;   // Scale for the sprites
Texture2D spriteR;
Texture2D spriteL;
Texture2D ball;
Texture2D bg;
Texture2D start;
float scale2 = 0.09f;  // Scale for the ball


class Player {
public:
    int score;
    float x;
    float y;
    Player(int a = 50, int b = 300) {
        x = a;
        score=0;
        y = b;
    }
    void increaseScore() { score++; }
    int getScore() { return score; }
    void movement(int z = 0) {
        if (z == 0) {  // Player 1 controls (Arrow keys)
            if (IsKeyDown(KEY_DOWN) && y + 10 <= height - 120) y += 10;
            if (IsKeyDown(KEY_UP) && y - 10 > 0) y -= 10;
            if (IsKeyDown(KEY_RIGHT) && x + 10 <= width - 50) x += 10;
            if (IsKeyDown(KEY_LEFT) && x - 10 > 0) x -= 10;
        } else {  // Player 2 controls (WASD keys)
            if (IsKeyDown(KEY_W) && y - 10 > 0) y -= 10;
            if (IsKeyDown(KEY_S) && y + 10 <= height - 120) y += 10;
            if (IsKeyDown(KEY_D) && x + 10 <= width - 50) x += 10;
            if (IsKeyDown(KEY_A) && x - 10 > 0) x -= 10;
        }
    }

    void update(int z = 0) {
        if (z == 1)
        {  
            DrawTextureEx(spriteL, Vector2{x, y}, 0, scale, WHITE);
        }
        else 
        {  
            DrawTextureEx(spriteR, Vector2{x, y}, 0, scale, WHITE);
        }
    }
};

void startingImg(){
    while (!WindowShouldClose())
    {
        ClearBackground(BLACK);
        BeginDrawing();
        DrawTexture(start,0,0,RAYWHITE);
        DrawText("Contols:\nWASD and Arrow Keys\nClick or press any key to start the game",100,height-100,30,RAYWHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            break;  
        }

        EndDrawing();
    }
    
}
int main(void) {
    float bx = width / 2, by = height / 2;
    float speedX = 8;
    Player p1;
    Player p2(1400);
    float speedY = 8;

    InitAudioDevice();
    Music song = LoadMusicStream("song.mp3");
    
    InitWindow(width, height, "umer");

    ball = LoadTexture("football.png");
    spriteL = LoadTexture("Left1.png");
    spriteR = LoadTexture("R1.png");
    bg = LoadTexture("background.jpg");
    start  = LoadTexture("starting.png");

    if (ball.id == 0 || spriteR.id == 0 || spriteL.id == 0 || bg.id == 0) {
        CloseWindow();
        return -1;
    }
    // float scaleX = width / (float)bg.width;
    // float scaleY = height / (float)bg.height;
    // float scale3 = (scaleX < scaleY) ? scaleX : scaleY;  

    // float bgX = (width - bg.width * scale3) / 2.0f;
    // float bgY = (height - bg.height * scale3) / 2.0f;

    float ballRadius = (ball.width * scale2) / 2.0f;

    PlayMusicStream(song);
    SetTargetFPS(70);

    startingImg();
    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawTexture(bg,0,0, WHITE);

        string score1 = "Score " + to_string(p1.getScore());
        string score2 = "Score " + to_string(p2.getScore());

        ClearBackground(GREEN);
        DrawLine(width / 2, 0, width / 2, height, WHITE);
        UpdateMusicStream(song);
        DrawRectangle(0, 0, 20, height, WHITE);
        DrawRectangle(width - 20, 0, 30, height, WHITE);
        DrawTextureEx(ball, Vector2{bx, by}, 0, scale2, WHITE);
        DrawText(score1.c_str(), 30, height - 30, 25, BLACK);
        DrawText(score2.c_str(), width - 120, height - 30, 25, BLACK);

        if (bx + 2 * ballRadius >= width || bx <= 0) speedX *= -1;
        if (by + 2 * ballRadius >= height || by <= 0) speedY *= -1;

        bx += speedX;
        by += speedY;

        if (bx <= 0) {
            p2.increaseScore();
        }

        if (bx + 2 * ballRadius >= width) {
            p1.increaseScore();
        }

        // Player 1 and Player 2 movement
        if (p1.x+70 < width / 2) p1.movement(1);
        else p1.x -= 10;

        if (p2.x > width / 2) p2.movement();
        else p2.x += 10;

        p2.update();  // use spriteL for Player 2
        p1.update(1);   // use spriteR for Player 1

        // Adjusted collision boxes with scaled player sprites
        Rectangle p1Rect = {p1.x, p1.y, spriteR.width * scale, spriteR.height * scale};
        Rectangle p2Rect = {p2.x, p2.y, spriteL.width * scale, spriteL.height * scale};

        // Ball and Player collision check
        if (CheckCollisionCircleRec(Vector2{bx + ballRadius, by + ballRadius}, ballRadius, p1Rect)) {
            speedX *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{bx + ballRadius, by + ballRadius}, ballRadius, p2Rect)) {
            speedX *= -1;
        }

        EndDrawing();
    }

    UnloadTexture(bg);
    UnloadTexture(ball);
    UnloadTexture(spriteR);
    UnloadTexture(spriteL);
    UnloadMusicStream(song);

    CloseAudioDevice();
    CloseWindow();

    InitWindow(300, 200 , "Winner");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (p1.getScore() > p2.getScore()) {
            DrawText("Player 1 Wins!\n", 100, 100, 20, WHITE);
        } else if (p1.getScore() < p2.getScore()) {
            DrawText("Player 2 Wins!\n", 100, 100, 20, WHITE);
        } else {
            DrawText("Match draw!\n", 100, 100, 20, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

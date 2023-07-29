#include <stdio.h>
#include "raylib.h"

struct Block
{
    Vector2 blockPos;
    bool state;
} Block;

void initialize_blocks(int numBlocksPerRow, int numRows, 
                       struct Block* blocks, 
                       int blockWidth, int blockHeight)
{
    // Initialize blocks
    for (size_t i=0; i<numBlocksPerRow * numRows; ++i)
    {
        blocks[i].blockPos.x = (i%numBlocksPerRow)*blockWidth;
        blocks[i].blockPos.y = (i/numRows)*blockHeight;
        blocks[i].state = 1;
    }
}

int main (int argc, char **argv)
{
    const int screenWidth = 800, screenHeight = 450;
    int paddleWidth = screenWidth / 10;
    int paddleHeight = paddleWidth / 10;
    int numBlocksPerRow = 10, numRows = 10;
    int blockWidth = screenWidth / numBlocksPerRow;
    int blockHeight = screenHeight / (3 * numRows);
    int clearance = 5;
    int ballDim = 8;

    Vector2 ballPos = {screenWidth/2.0, screenHeight/2.0};
    Vector2 ballSpeed = {4.0, 3.0};

    float paddlePos = screenWidth / 2.0;
    float paddleSpeed = 10.0;

    bool pause = 0, gameOver = 0;
    int frameCounter = 0;

    struct Block blocks[numBlocksPerRow * numRows];
    
    initialize_blocks(numBlocksPerRow, numRows, &blocks[0], blockWidth, blockHeight);

    InitWindow (screenWidth, screenHeight,
            "raylib [core] example - basic window");
    SetTargetFPS (60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;
        // Restart game
        if (IsKeyPressed(KEY_R))
        {
            gameOver = !gameOver;
            initialize_blocks(numBlocksPerRow, numRows, &blocks[0], blockWidth, blockHeight);
            paddlePos = screenWidth / 2.0;
            ballPos.x = screenWidth / 2.0;
            ballPos.y = screenHeight / 2.0;
        }
        
        // Ball movement
        if (!pause && !gameOver)
        {
            ballPos.x += ballSpeed.x;
            ballPos.y += ballSpeed.y;

            // Collision detection
            // Left and right walls
            if (((ballPos.x >= (screenWidth - ballDim / 2.0)))
                 || (ballPos.x <= ballDim / 2.0))
                ballSpeed.x *= -1.0;
            // Upper wall
            if (ballPos.y <= ballDim / 2.0)
                 ballSpeed.y *= -1.0;
            // Paddle
            if (ballPos.y>=screenHeight-paddleHeight-ballDim/2.0)
            {
                if ((ballPos.x>=paddlePos-paddleWidth/2.0)
                     && (ballPos.x<=paddlePos+paddleWidth/2.0))
                     ballSpeed.y *= -1.0;
                else
                    gameOver = !gameOver;
            }
            // Blocks
            if (ballPos.y-ballDim/2.0<=blockHeight*numRows)
            {
                int rowNum = ballPos.y/blockHeight-1;
                int colNum = ballPos.x/blockWidth;

                struct Block* curr_block = &blocks[rowNum*numBlocksPerRow+colNum];

                if (curr_block->state)
                {
                    curr_block->state = 0;
                    if ((ballPos.x+ballDim/2.0>=curr_block->blockPos.x)
                        || (ballPos.x-ballDim/2.0<=curr_block->blockPos.x+blockWidth))
                        ballSpeed.x *= -1.0;
                    else if ((ballPos.y+ballDim/2.0>=curr_block->blockPos.y)
                        || (ballPos.y-ballDim/2.0<=curr_block->blockPos.y+blockHeight))
                        ballSpeed.y *= -1.0;
                }
            }
        }
        else 
            frameCounter++;
    
        // Paddle movement
        if (IsKeyDown(KEY_LEFT) && paddlePos-paddleWidth/2.0 >= 0.0)
            paddlePos -= 1.0 * paddleSpeed;
        if (IsKeyDown(KEY_RIGHT) && paddlePos+paddleWidth/2.0 <= screenWidth)
            paddlePos += 1.0 * paddleSpeed;

        BeginDrawing ();
        ClearBackground (BLACK);

        // Draw paddle
        DrawRectangle (paddlePos - paddleWidth / 2,
      	     screenHeight - paddleHeight, paddleWidth, paddleHeight,
      	     WHITE);

        // Draw blocks
        for (size_t i = 0; i < numRows*numBlocksPerRow; ++i)
        {
            if (blocks[i].state==0)
                continue;
            DrawRectangle (blocks[i].blockPos.x + clearance,
                 blocks[i].blockPos.y + clearance,
                 blockWidth - clearance,
                 blockHeight - clearance, GREEN);
        }

        // Draw ball
        DrawCircleV(ballPos, (float)ballDim / 2.0, WHITE);
        
        // Draw "paused" or "game over" signs
        if (pause && ((frameCounter/30)%2))
            DrawText("PAUSED", 300, 200, 30, YELLOW);
        if (gameOver && ((frameCounter/30)%2))
            DrawText("GAME OVER. PRESS R TO RESTART.", 100, 200, 30, YELLOW);

        EndDrawing ();
    }
    CloseWindow ();

    return 0;
}

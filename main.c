#include <stdio.h>
#include "raylib.h"

struct Block
{
    Vector2 block_pos;
    bool state;
} Block;

// Sets block positions and state
void initialize_blocks(int num_blocks_per_row, int num_rows, 
                       struct Block* blocks, 
                       int block_width, int block_height)
{
    // Initialize blocks
    for (size_t i=0; i<num_blocks_per_row * num_rows; ++i)
    {
        blocks[i].block_pos.x = (i%num_blocks_per_row)*block_width;
        blocks[i].block_pos.y = (i/num_rows)*block_height;
        blocks[i].state = 1;
    }
}

int main (int argc, char **argv)
{
    const int screen_width = 800, screen_height = 450;
    int paddle_width = screen_width / 10;
    int paddle_height = paddle_width / 10;
    int num_blocks_per_row = 10, num_rows = 10;
    int block_width = screen_width / num_blocks_per_row;
    int block_height = screen_height / (3 * num_rows);
    int clearance = 5;
    int ball_dim = 8;
    int frame_counter = 0;
    unsigned int score = 0;

    Vector2 ball_pos = {screen_width/2.0, screen_height/2.0};
    Vector2 ball_speed = {4.0, 3.0};

    float paddle_pos = screen_width / 2.0;
    float paddle_speed = 10.0;

    bool game_over = 0;
    struct Block blocks[num_blocks_per_row * num_rows];
    
    initialize_blocks(num_blocks_per_row, num_rows, &blocks[0], block_width, block_height);

    InitWindow (screen_width, screen_height, "Breakout!");
    SetTargetFPS (60);

    while (!WindowShouldClose())
    {
        // Restart game
        if (IsKeyPressed(KEY_R))
        {
            // Check if the player won
            if (score!=num_rows*num_blocks_per_row)
                game_over = !game_over;
            score = 0;
            
            initialize_blocks(num_blocks_per_row, num_rows, 
                    &blocks[0], block_width, block_height);
            paddle_pos = screen_width / 2.0;
            ball_pos.x = screen_width / 2.0;
            ball_pos.y = screen_height / 2.0;
        }
        
        // Ball movement
        if (!game_over && score<num_rows*num_blocks_per_row)
        {
            ball_pos.x += ball_speed.x;
            ball_pos.y += ball_speed.y;

            // Collision detection
            // Left and right walls
            if ((ball_pos.x >= screen_width - ball_dim / 2.0)
                 || ball_pos.x <= ball_dim / 2.0)
                ball_speed.x *= -1.0;
            // Upper wall
            if (ball_pos.y <= ball_dim / 2.0)
                 ball_speed.y *= -1.0;
            // Paddle
            if (ball_pos.y>=screen_height-paddle_height-ball_dim/2.0)
            {
                if (ball_pos.x+ball_dim/2.0>=paddle_pos-paddle_width/2.0
                     && ball_pos.x-ball_dim/2.0<=paddle_pos+paddle_width/2.0)
                    ball_speed.y *= -1.0;
                else
                    game_over = !game_over;
            }
            // Blocks
            if (ball_pos.y-ball_dim/2.0<=block_height*num_rows)
            {
                int row_num = ball_pos.y/block_height-1;
                int col_num = ball_pos.x/block_width;

                struct Block* curr_block = &blocks[row_num*num_blocks_per_row+col_num];

                if (curr_block->state)
                {
                    curr_block->state = 0;
                    if (ball_pos.x+ball_dim/2.0>=curr_block->block_pos.x
                        || ball_pos.x-ball_dim/2.0<=curr_block->block_pos.x+block_width)
                        ball_speed.x *= -1.0;
                    else if (ball_pos.y+ball_dim/2.0>=curr_block->block_pos.y
                        || ball_pos.y-ball_dim/2.0<=curr_block->block_pos.y+block_height)
                        ball_speed.y *= -1.0;
                    score++;
                }
            }
        }
        else
            frame_counter++;
        
    
        // Paddle movement
        if (IsKeyDown(KEY_LEFT) && paddle_pos-paddle_width/2.0 >= 0.0)
            paddle_pos -= 1.0 * paddle_speed;
        if (IsKeyDown(KEY_RIGHT) && paddle_pos+paddle_width/2.0 <= screen_width)
            paddle_pos += 1.0 * paddle_speed;

        BeginDrawing ();
        ClearBackground(BLACK);

        // Draw paddle
        DrawRectangle (paddle_pos - paddle_width / 2,
      	     screen_height - paddle_height, paddle_width, paddle_height,
      	     WHITE);

        // Draw blocks
        for (size_t i = 0; i < num_rows*num_blocks_per_row; ++i)
        {
            if (blocks[i].state==0)
                continue;
            DrawRectangle (blocks[i].block_pos.x + clearance,
                 blocks[i].block_pos.y + clearance,
                 block_width - clearance,
                 block_height - clearance, GREEN);
        }

        // Draw ball
        DrawCircleV(ball_pos, (float)ball_dim / 2.0, WHITE);
        
        // Draw "you won" sign
        if (score==num_rows*num_blocks_per_row && ((frame_counter/30)%2))
        {
            DrawText("YOU WON !!!", 300, 200, 30, YELLOW);
            DrawText("PRESS R TO RESTART OR ESC TO EXIT.", 100, 240, 30, YELLOW);
        }
        
        // Draw "game over" sign
        if (game_over && ((frame_counter/30)%2))
        {
            DrawText("GAME OVER.", 300, 200, 30, YELLOW);
            DrawText("PRESS R TO RESTART OR ESC TO EXIT.", 100, 240, 30, YELLOW);
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

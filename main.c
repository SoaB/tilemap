#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define CUTE_TILED_IMPLEMENTATION
#include "cute_tiled.h"
#include <raylib.h>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800

typedef union {
    Color col;
    uint32_t icol;
} uColor;

typedef struct {
    Vector2 player;
    Vector2 pCell;
    Camera2D camera;
    cute_tiled_map_t* map;
    Texture2D tileImg;
    int mapPixW;
    int mapPixH;
} Game;

void ShowTiledInfo(cute_tiled_map_t* map)
{
    printf("Map Info:\n");
    printf("Layer Name : %s\n", map->layers->name.ptr);
    printf("Layer Width: %d\n", map->layers->width);
    printf("Layer Height: %d\n", map->layers->height);
    printf("Tiled Name: %s\n", map->tilesets->name.ptr);
    printf("Tiled image name: %s\n", map->tilesets->image.ptr);
    printf("Tiled Width: %d\n", map->tilesets->tilewidth);
    printf("Tiled Height: %d\n", map->tilesets->tileheight);
    printf("Total Tiles : %d\n", map->tilesets->tilecount);
}

float Clamp(float value, float min, float max)
{
    float result = (value < min) ? min : value;
    if (result > max)
        result = max;

    return result;
}

void UpdateInput(Game* g)
{
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        g->player.x += 4;
        if (g->player.x > g->mapPixW) {
            g->player.x = g->mapPixW;
        }
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        g->player.x -= 4; // Player movement
        if (g->player.x < 0) {
            g->player.x = 0;
        }
    } else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        g->player.y += 4; // Player movement
        if (g->player.y > g->mapPixH) {
            g->player.y = g->mapPixH;
        }
    } else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        g->player.y -= 4; // Player movement
        if (g->player.y < 0) {
            g->player.y = 0;
        }
    }
    // Camera target follows player
    float cx = Clamp(g->player.x, SCREEN_WIDTH / 2, g->mapPixW - SCREEN_WIDTH + SCREEN_WIDTH / 2);
    float cy = Clamp(g->player.y, SCREEN_HEIGHT / 2, g->mapPixH - SCREEN_HEIGHT + SCREEN_HEIGHT / 2);

    g->camera.target = (Vector2) { cx, cy };
}
void DrawTile(Game* g, int x, int y, int tileId)
{
    int columns = g->map->tilesets->columns;
    int tileWidth = g->map->tilesets->tilewidth;
    int tileHeight = g->map->tilesets->tileheight;
    int tileIdY = tileId / columns;
    int tileIdX = tileId % columns;
    Rectangle source = { tileWidth * tileIdX, tileHeight * tileIdY, tileWidth, tileHeight };
    Rectangle dest = { x * tileWidth, y * tileHeight, tileWidth, tileHeight };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(g->tileImg, source, dest, origin, 0.0f, WHITE);
}
void DrawMap(Game* g)
{
    int mapWidth = g->map->width;
    int mapHeight = g->map->height;
    for (int j = 0; j < mapHeight; j++) {
        for (int i = 0; i < mapWidth; i++) {
            DrawTile(g, i, j, g->map->layers->data[j * mapWidth + i]);
        }
    }
}
void DrawPlayer(Game* g)
{
    DrawRectangleV(g->player, (Vector2) { 8, 8 }, RED);
}
void Draw(Game* g)
{
    BeginMode2D(g->camera);
    DrawMap(g);
    DrawPlayer(g);
    EndMode2D();
}
bool InitGame(Game* g)
{
    g->player = (Vector2) { 400, 300 };
    g->camera.target = (Vector2) { g->player.x, g->player.y };
    g->camera.offset = (Vector2) { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
    g->camera.rotation = 0.0f;
    g->camera.zoom = 1.0f;
    g->map = cute_tiled_load_map_from_file("asset/rpg.tmj", NULL);
    if (!g->map) {
        printf("Cannot load map\n");
        return false;
    }
    g->mapPixW = g->map->width * g->map->tilesets->tilewidth;
    g->mapPixH = g->map->height * g->map->tilesets->tileheight;
    char fname[255];
    sprintf(fname, "asset/%s", g->map->tilesets->image.ptr);
    g->tileImg = LoadTexture(fname);
    ShowTiledInfo(g->map);

    return true;
}
int main(int argc, char** argv)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib example");
    SetTargetFPS(60);
    Game game;
    InitGame(&game);
    while (!WindowShouldClose()) {
        UpdateInput(&game);
        BeginDrawing();
        ClearBackground(BLACK);
        Draw(&game);
        EndDrawing();
    }
    cute_tiled_free_map(game.map);
    UnloadTexture(game.tileImg);
    CloseWindow();
    return 0;
}
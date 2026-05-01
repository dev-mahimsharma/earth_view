#include "raylib.h"
#include "raymath.h"  //usefulf for maths like vector, sin cos etc

#include <stdint.h>
#include <stdio.h>

static uint32_t win_width = 1280;
static uint32_t win_height = 720;

static const char *win_name = "HAZEL || Earth Simulator.";

int main() {

    InitWindow(win_width, win_height, win_name);
    SetTargetFPS(60);

    // camera setup

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 8.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float camDistance = 8.0f;
    float yaw = 0.0f;
    float pitch = 0.25f;

    // texture
    Texture2D earth_day_time_tex = LoadTexture("./images/earth_day.png");

    // these function are used for filtering the texture.

    GenTextureMipmaps(&earth_day_time_tex);
    SetTextureFilter(earth_day_time_tex, TEXTURE_FILTER_TRILINEAR);

    Model earth =
        LoadModelFromMesh(GenMeshSphere(2.0f, 128,
                                        128));  // it creates a model of sphere of radius 2.0f and
                                                // here 128,128 params means smoothness
    // of sphere

    earth.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
        earth_day_time_tex;  // adding texture in the model .

    // main loop logic

    while (!WindowShouldClose()) {

        // Mouse drag = rotate camera around sphere
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 delta = GetMouseDelta();

            yaw -= delta.x * 0.005f;
            pitch += delta.y * 0.005f;
        }

        // Mouse wheel = zoom
        camDistance -= GetMouseWheelMove() * 0.5f;

        if (camDistance < 3.0f)
            camDistance = 3.0f;
        if (camDistance > 20.0f)
            camDistance = 20.0f;

        // Keyboard rotation
        if (IsKeyDown(KEY_A))
            yaw -= 0.03f;
        if (IsKeyDown(KEY_D))
            yaw += 0.03f;
        if (IsKeyDown(KEY_W))
            pitch -= 0.03f;
        if (IsKeyDown(KEY_S))
            pitch += 0.03f;

        // Limit vertical rotation
        if (pitch > 1.5f)
            pitch = 1.5f;
        if (pitch < -1.5f)
            pitch = -1.5f;

        camera.position.x = camDistance * cosf(pitch) * sinf(yaw);
        camera.position.y = camDistance * sinf(pitch);
        camera.position.z = camDistance * cosf(pitch) * cosf(yaw);

        // main drawing area

        BeginDrawing();

        // clear the background to make it easier for front and back buffer
        // to load each frame easily by clearing the previous frame.
        ClearBackground((Color){5, 5, 15, 255});

        // this function draws fps in the window
        DrawFPS(20, 20);

        BeginMode3D(camera);  // this function is used to render 3d objects using created camera

        // using the created model

        DrawModel(earth, (Vector3){0, 0, 0}, 1.0f, WHITE);
        // @param earth is the model we created before
        // @ param (vector3){0,0,0} is the position where the model will be created in x,y,z scale
        // @param WHITE is the color .it means to display texture normally

        EndMode3D();

        EndDrawing();
    }

    UnloadModel(earth);                 // it removes the model before closing window
    UnloadTexture(earth_day_time_tex);  // it removes the texture
    CloseWindow();

    return 0;
}
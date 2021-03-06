#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;


float last_frame_ticks;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Chandrika's Pong Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
   
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - last_frame_ticks;
    last_frame_ticks = ticks;
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    ShaderProgram program (RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    
    Matrix projectionMatrix;
    Matrix viewMatrix;
    Matrix modelMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0, 2.0, -1.0, 1.0);
    glUseProgram(program.programID);
    
    Entity p1(0.7,0.1,3.45,0, 1,0);
    Entity p2(0.7,0.1,-3.45,0,1,0);
    Entity ball (0.2,0.2,0,0,1,1);
    Entity wTop(0.2,7.1,0,1.9,0,0);
    Entity wBottom(0.2,7.1,0,-1.9,0,0);
    
    SDL_Event event;
    bool done = false;
    
    while (!done) {
        
            if (!ball.detectCollision(&p1)){
                ball.setDirectionX(-1);
                ball.update(elapsed, 0.05);
            }
            else if (!ball.detectCollision(&p2)){
                ball.setDirectionX(1);
                ball.update(elapsed, 0.05);
            }
            else if (!ball.detectCollision(&wTop)){
                ball.setDirectionY(-1);
                ball.update(elapsed, 0.05);
            }
            else if (!ball.detectCollision(&wBottom)){
                ball.setDirectionY(1);
                ball.update(elapsed, 0.05);
            }
            else {
                ball.update(elapsed, 0.05);
            }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }

            const Uint8 *keys = SDL_GetKeyboardState(nullptr);
            if (keys[SDL_SCANCODE_UP]){
                    if (p1.getY() < 1.0f){
                        p1.update(elapsed, 0.5);
                    }
                }
            else if (keys[SDL_SCANCODE_DOWN]){
                if (p1.getY() > -1.0f){
                    p1.update(elapsed, -0.5);
                }
            }
            else if (keys[SDL_SCANCODE_W]){
                if (p2.getY() < 1.0f){
                    p2.update(elapsed, 0.5);
                }
            }
            else if (keys[SDL_SCANCODE_S]){
                if (p2.getY() > -1.0f){
                    p2.update(elapsed, -0.5);
                }
            }

            else if (ball.getX() > 3.55) {
                std::cout << "P2 wins" << std::endl;
                exit(2);
            }
            else if (ball.getX() < -3.55) {
                std::cout << "P1 wins" << std::endl;
                exit(1);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        program.setProjectionMatrix(projectionMatrix);
        program.setViewMatrix(viewMatrix);
        p1.draw(&program);
        p2.draw(&program);
        ball.draw (&program);
        wTop.draw (&program);
        wBottom.draw(&program);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}

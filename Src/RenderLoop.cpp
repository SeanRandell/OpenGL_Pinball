#include "RenderLoop.h"

RenderLoop::RenderLoop()
{

}

void RenderLoop::CheckInput(StateTracker* stateTracker, bool *quitApp) {
    //const Uint8* keys;
    //SDL_PumpEvents();
    //if (keys = SDL_GetKeyboardState(nullptr)) {
    //    if (keys[SDL_SCANCODE_ESCAPE]) {
    //        quitApp = true;
    //    }
    //    if (keys[SDL_SCANCODE_A]) {
    //        //fprintf(stderr, "left pressed in first event\n");
    //        stateTracker->camera->moveLeft = true;
    //    }
    //    //stateTracker->camera->moveLeft = false;
    //    if (keys[SDL_SCANCODE_D]) {
    //        //fprintf(stderr, "left pressed in first event\n");
    //        stateTracker->camera->moveRight = true;
    //    }
    //    //stateTracker->camera->moveRight = false;
    //    if (keys[SDL_SCANCODE_W]) {
    //        //fprintf(stderr, "left pressed in first event\n");
    //        stateTracker->camera->moveFoward = true;
    //    }
    //    //stateTracker->camera->moveFoward = false;
    //    if (keys[SDL_SCANCODE_S]) {
    //        //fprintf(stderr, "left pressed in first event\n");
    //        stateTracker->camera->moveBackward = true;
    //    }
    //    //stateTracker->camera->moveBackward = false;
    //}

    SDL_Event keyEvent;
    if (SDL_PollEvent(&keyEvent)) 
    {
        std::cout << "key pressed" << std::endl;
        switch (keyEvent.type) 
        {
            case SDL_QUIT:
                *quitApp = true;
                break;
            case SDL_KEYDOWN:
                switch (keyEvent.key.keysym.sym) 
                {
                    case SDLK_ESCAPE:
                        *quitApp = true;
                        break;
                    case SDLK_w:
                        stateTracker->camera->moveFoward = true;
                        std::cout << "move forward First" << std::boolalpha << stateTracker->camera->moveFoward << std::endl;
                        break;
                    case SDLK_s:
                        stateTracker->camera->moveBackward = true;
                        break;
                    case SDLK_a:
                        stateTracker->camera->moveLeft = true;
                        break;
                    case SDLK_d:
                        stateTracker->camera->moveRight = true;
                        break;
                    case SDLK_LEFT:
                        stateTracker->camera->turnLeft = true;
                        break;
                    case SDLK_RIGHT:
                        stateTracker->camera->turnRight = true;
                        break;
                    case SDLK_UP:
                        stateTracker->camera->tiltUp = true;
                        break;
                    case SDLK_DOWN:
                        stateTracker->camera->tiltDown = true;
                        break;
                }
                break;
                case SDL_KEYUP:
                    switch (keyEvent.key.keysym.sym) 
                    {
                        case SDLK_w:
                            stateTracker->camera->moveFoward = false;
                            break;
                        case SDLK_s:
                            stateTracker->camera->moveBackward = false;
                            break;
                        case SDLK_a:
                            stateTracker->camera->moveLeft = false;
                            break;
                        case SDLK_d:
                            stateTracker->camera->moveRight = false;
                            break;
                        case SDLK_LEFT:
                            stateTracker->camera->turnLeft = false;
                            break;
                        case SDLK_RIGHT:
                            stateTracker->camera->turnRight = false;
                            break;
                        case SDLK_UP:
                            stateTracker->camera->tiltUp = false;
                            break;
                        case SDLK_DOWN:
                            stateTracker->camera->tiltDown = false;
                            break;
                    }
                break;
        }
    }
}

void RenderLoop::UpdateState(StateTracker* stateTracker, float deltaTime)
{
    stateTracker->camera->ProcessCameraMoving(deltaTime);
    stateTracker->camera->ProcessCameraTurning(deltaTime);

    stateTracker->modelMatrix = glm::mat4(1.0f);
    stateTracker->viewMatrix = stateTracker->camera->GetViewMatrix();
}

void RenderLoop::RenderOnScreenDisplay()
{
}

void RenderLoop::RenderFrame(StateTracker* stateTracker)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    stateTracker->cubeShader->Use();
    //m_DefaultShader->SetMat4("u_ModelMatrix", m_ModelMatrix);
    stateTracker->cubeShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->cubeShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->cubeShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->cubeShader->SetLightingModel(*stateTracker->lightModel);

    stateTracker->cubeShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->cube->Render(stateTracker->cubeShader);
}


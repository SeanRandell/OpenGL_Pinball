#include "RenderLoop.h"
#include <utility>

RenderLoop::RenderLoop(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    windowWidth = screenWidth;
    windowHeight = screenHeight;

    physicsMethods = new Physics();
}

RenderLoop::~RenderLoop()
{
    delete physicsMethods;
}

void RenderLoop::Init(StateTracker* stateTracker, int screenWidth, int screenHeight)
{
    stateTracker->InitShadersAndTextures();
    stateTracker->BuildGameObjects();
    stateTracker->InitBuffers(screenWidth, screenHeight);
    stateTracker->particleGenerator->Init();

    stateTracker->blockShader->Use();
    stateTracker->blockShader->SetInt("objectMaterialUniform.diffuseMap", 0);
    stateTracker->blockShader->SetInt("objectMaterialUniform.specularMap", 1);
    stateTracker->blockShader->SetInt("objectMaterialUniform.reflectionMap", 2);
    stateTracker->blockShader->SetInt("skyBoxUniform", 3);
    stateTracker->blockShader->SetInt("objectDefaultMaterialUniform.diffuseMap", 4);

    //Subroutines
    stateTracker->OneTextureIndex = glGetSubroutineIndex(stateTracker->blockShader->ID, GL_FRAGMENT_SHADER, "OneTexture");
    stateTracker->ThreeTextureIndex = glGetSubroutineIndex(stateTracker->blockShader->ID, GL_FRAGMENT_SHADER, "ThreeTextures");

    //TODO fix for multtiShader?
    //stateTracker->blockShader->SetInt("shadowMap", 4);

    //particles
    stateTracker->particleShader->SetInt("sprite", 0);

    stateTracker->skyBoxShader->Use();
    stateTracker->skyBoxShader->SetInt("skyBoxUniform", 0);

    stateTracker->sphereShader->Use();
    stateTracker->sphereShader->SetInt("skyBoxUniform", 0);

    //bloom
    stateTracker->blurShader->Use();
    stateTracker->blurShader->SetInt("imageUniform", 0);

    stateTracker->bloomShader->Use();
    stateTracker->bloomShader->SetInt("sceneUniform", 0);
    stateTracker->bloomShader->SetInt("bloomBlurUniform", 1);

    //shadows
    //stateTracker->debugDepthQuadShader->Use();
    //stateTracker->debugDepthQuadShader->SetInt("depthMap", 0);

    // Add directional light to follow camera view direction
    stateTracker->lightModel->AddLight({
        .type = LightType::DirectionalLight,
        .ambient = glm::vec3(0.2, 0.2, 0.2),
        .diffuse = glm::vec3(0.5, 0.5, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .direction = glm::vec3(0.0, 0.0, -1.0),
        .position = glm::vec3(0.0, 8.0, 10.0),
        .isLightOn = true
        });
    // Add red spot lights 
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.2, 0.0, 0.0),
        .diffuse = glm::vec3(0.5, 0.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-4.5, 3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.2, 0.0, 0.0),
        .diffuse = glm::vec3(0.5, 0.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0, 0.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.2, 0.0, 0.0),
        .diffuse = glm::vec3(0.5, 0.0, 0.0),
        .specular = glm::vec3(1.0, 0.0, 0.0),
        .position = glm::vec3(4.5, -3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    // Add green spot lights
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 0.5, 0.0),
        .specular = glm::vec3(0.0, 1.0, 0.0),
        .position = glm::vec3(4.5, 3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 0.5, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-4.5, -3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 0.5, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(1.5, -3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    // blue point lights
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-1.5, 3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(3.0, 0.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-1.5, -3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    // purple point lights 
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.2, 0.0, 0.2),
        .diffuse = glm::vec3(0.5, 0.0, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(1.5, 3.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.2, 0.0, 0.2),
        .diffuse = glm::vec3(0.5, 0.0, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-3.0, 0.0, 1.5),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = false
        });
    //teal moving light
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.2),
        .diffuse = glm::vec3(0.0, 0.5, 0.5),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(10.5f, -8.0f, 1.5f),
        .constant = 1.0f,
        .linear = 0.14f,
        .quadratic = 0.07f,
        .isLightOn = true
        });

    // assign lights to pegs
    for (int i = 1; i < stateTracker->pegs.size(); i++)
    {
        stateTracker->pegs[i]->pegLight = stateTracker->lightModel->GetLight(i);
    }
}

void RenderLoop::CheckInput(StateTracker* stateTracker, bool* quitApp, float deltaTime) {

    SDL_Event keyEvent;
    if (SDL_PollEvent(&keyEvent))
    {
        switch (keyEvent.type)
        {
        case SDL_QUIT:
            *quitApp = true;
            break;
        case SDL_MOUSEMOTION:
        {
            //fprintf(stderr, "mouse x: %d, y: %d\n", keyEvent.button.x, keyEvent.button.y);
            //SDL_GetMouseState(&xpos, &ypos);

            static int xPosition = stateTracker->camera->lastX;
            static int yPosition = stateTracker->camera->lastY;
            xPosition += keyEvent.motion.xrel;
            yPosition += keyEvent.motion.yrel;

            if (stateTracker->camera->firstMouse && (stateTracker->camera->lastX = xPosition) && (stateTracker->camera->lastY = yPosition)) {
                stateTracker->camera->lastX = xPosition;
                stateTracker->camera->lastY = yPosition;
                stateTracker->camera->firstMouse = false;
            }

            float xOffSet = xPosition - stateTracker->camera->lastX;
            float yOffSet = stateTracker->camera->lastY - yPosition;

            stateTracker->camera->lastX = xPosition;
            stateTracker->camera->lastY = yPosition;

            if (stateTracker->isDebugOn)
            {
                stateTracker->camera->ProcessMouseMovement(xOffSet, yOffSet, deltaTime, true);
            }
        }
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
            case SDLK_b:
                stateTracker->isDebugOn = !stateTracker->isDebugOn;
                if (stateTracker->isDebugOn)
                {
                    stateTracker->localIsDebugHUDOnToggle = true;
                    stateTracker->isPhysicsOn = false;
                    stateTracker->isBoundingBoxesOn = true;
                    stateTracker->isLightingBoxesOn = true;
                }
                else
                {
                    stateTracker->camera->ResetCamera();
                    stateTracker->localIsDebugHUDOnToggle = true;
                    stateTracker->isPhysicsOn = true;
                    stateTracker->isBoundingBoxesOn = false;
                    stateTracker->isLightingBoxesOn = false;
                    stateTracker->isVertexNormalsDisplayOn = false;
                }
                break;
            case SDLK_m:
                stateTracker->camera->mouseControls = !stateTracker->camera->mouseControls;
                break;
            case SDLK_SPACE:
                // launch ball
                stateTracker->canLaunchBall = true;
                break;
            case SDLK_PERIOD:
                // '>' – right flipper
                stateTracker->moveRightFlipper = true;
                break;
            case SDLK_COMMA:
                //'<' –  left flipper
                stateTracker->moveLeftFlipper = true;
                break;
            case SDLK_q:
                // turn quadtree on and off
                stateTracker->isQuadTreeOn = !stateTracker->isQuadTreeOn;
                break;
            case SDLK_h:
                // turn debug hud on and off
                stateTracker->isDebugHUDOn = !stateTracker->isDebugHUDOn;
                stateTracker->localIsDebugHUDOnToggle = !stateTracker->localIsDebugHUDOnToggle;
                break;
            case SDLK_l:
                // turn lighting boxes on and off
                stateTracker->isLightingBoxesOn = !stateTracker->isLightingBoxesOn;
                break;
            case SDLK_n:
                // turn vertex normals on and off
                stateTracker->isVertexNormalsDisplayOn = !stateTracker->isVertexNormalsDisplayOn;
                break;
            case SDLK_p:
                // turn physics on and off
                stateTracker->isPhysicsOn = !stateTracker->isPhysicsOn;
                break;
            case SDLK_x:
                // turn physics bounding boxes on and off
                stateTracker->isBoundingBoxesOn = !stateTracker->isBoundingBoxesOn;
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

void RenderLoop::UpdateState(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree)
{
    if (stateTracker->isDebugOn)
    {
        stateTracker->camera->ProcessCameraMoving(deltaTime);
        stateTracker->camera->ProcessCameraTurning(deltaTime);
    }

    stateTracker->viewMatrix = stateTracker->camera->GetViewMatrix();
    stateTracker->modelMatrix = glm::mat4(1.0f);

    //PAUSED CALCULATIONS IN DEBUG MODE
    if (stateTracker->isDebugOn == false || stateTracker->isPhysicsOn)
    {
        if (stateTracker->isPhysicsOn)
        {
            if (stateTracker->launchCountdown >= stateTracker->launchCooldown)
            {
                if (stateTracker->canLaunchBall)
                {
                    //launch ball
                    stateTracker->LaunchBall();

                    stateTracker->launchCountdown = 0;
                    //std::cout << "launched" << std::endl;
                    stateTracker->canLaunchBall = false;
                }
            }
            else {
                stateTracker->launchCountdown += deltaTime;
                stateTracker->canLaunchBall = false;
                //std::cout << "Cooldown" << std::endl;
            }

            //PHYSICS
            physicsMethods->FipperPhysics(stateTracker, deltaTime);

            physicsMethods->CalculateBallPhysics(stateTracker, deltaTime, quadtree);

            // check if ball needs to be deleted
            if (stateTracker->spheres.size() > 0)
            {
                std::vector<Sphere*>::iterator it = stateTracker->spheres.begin();

                while (it != stateTracker->spheres.end()) {
                    Sphere* currentSphere = (Sphere*)*it;
                    if (currentSphere->position.y < -10.0) {
                        currentSphere->End();
                        it = stateTracker->spheres.erase(it);
                    }
                    else ++it;
                }
            }

            // Count physics objects
            stateTracker->physicsObjectCount = stateTracker->spheres.size() + stateTracker->blocks.size() + stateTracker->pegs.size();

            //int j = 1;
            //Update peg lights
            for (int i = 1; i < stateTracker->pegs.size(); i++)
            {
                stateTracker->pegs[i]->UpdatePegLights(deltaTime);
            }

            // update particles
            for (auto ball : stateTracker->spheres)
            {
                stateTracker->particleGenerator->Update(deltaTime, ball, 2, glm::vec2(ball->radius / 4.0f));
            }

            //if no spheres make sure all particles die
            if (stateTracker->spheres.size() == 0)
            {
                stateTracker->particleGenerator->UpdateAllParticles(deltaTime);
            }

            // move launch tube light up and down
            if (stateTracker->isLaunchLightMovingUp) 
            {
                stateTracker->lightModel->GetLight(12)->position.y += 2 * deltaTime;
                if (stateTracker->lightModel->GetLight(12)->position.y >= 4.0f)
                {
                    stateTracker->isLaunchLightMovingUp = false;
                }
            }
            else
            {
                stateTracker->lightModel->GetLight(12)->position.y -= 2 * deltaTime;
                if (stateTracker->lightModel->GetLight(12)->position.y <= -8.0f)
                {
                    stateTracker->isLaunchLightMovingUp = true;
                }
            }
        }
    }

    if (stateTracker->isPhysicsOn == false)
    {
        // keep calulating quadtree for display purposes
        physicsMethods->PopulateQuadtree(stateTracker, quadtree);
    }
}

void RenderLoop::RenderFrame(StateTracker* stateTracker, Quadtree* quadtree)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scene into floating point framebuffer
    // -----------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, stateTracker->hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //block
    stateTracker->blockShader->Use();
    stateTracker->blockShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->blockShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->blockShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->blockShader->SetLightingModel(*stateTracker->lightModel);

    for (auto block : stateTracker->blocks)
    {
        stateTracker->blockShader->SetBool("debugUniform", false);
        stateTracker->modelMatrix = glm::mat4(1.0f);
        stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, block->position);
        stateTracker->modelMatrix = glm::scale(stateTracker->modelMatrix, block->scale);
        stateTracker->blockShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
        if (block->isBumper || block->isScenery)
        {
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &stateTracker->OneTextureIndex);
            if (block->isScenery)
            {
                block->RenderNormalBlock(stateTracker->blockShader, stateTracker->skyBox, stateTracker->wallBlockTexture);
            }
            else
            {
                block->RenderNormalBlock(stateTracker->blockShader, stateTracker->skyBox, stateTracker->bumperBlockTexture);
            }
        }
        else
        {
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &stateTracker->ThreeTextureIndex);
            block->Render(stateTracker->blockShader, stateTracker->skyBox, stateTracker->diffuseMapTexture, stateTracker->specularMapTexture, stateTracker->reflectionMaptexture);
        }

        // render bounding box
        if (stateTracker->isDebugOn == false)
        {
            continue;
        }
        if (!stateTracker->isBoundingBoxesOn)
        {
            continue;
        }
        if (block->isScenery)
        {
            continue;
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonOffset(0, -1);
        glEnable(GL_POLYGON_OFFSET_FILL);
        stateTracker->blockShader->SetBool("debugUniform", true);
        block->Render(stateTracker->blockShader, stateTracker->skyBox, stateTracker->diffuseMapTexture, stateTracker->specularMapTexture, stateTracker->reflectionMaptexture);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonOffset(1, 0);
        glEnable(GL_POLYGON_OFFSET_FILL);
    }

    //block normal
    if (stateTracker->isVertexNormalsDisplayOn) {
        stateTracker->normalShader->Use();
        stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
        stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
        for (auto block : stateTracker->blocks)
        {
            stateTracker->modelMatrix = glm::mat4(1.0f);
            stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, block->position);
            stateTracker->modelMatrix = glm::scale(stateTracker->modelMatrix, block->scale);
            stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
            block->Render(stateTracker->normalShader, stateTracker->skyBox, stateTracker->diffuseMapTexture, stateTracker->specularMapTexture, stateTracker->reflectionMaptexture);
        }
    }

    //cylinder
    stateTracker->cubeShader->Use();
    stateTracker->cubeShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->cubeShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->cubeShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->cubeShader->SetLightingModel(*stateTracker->lightModel);
    for (auto peg : stateTracker->pegs)
    {
        stateTracker->cubeShader->SetBool("debugUniform", false);
        stateTracker->modelMatrix = glm::mat4(1.0f);
        stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, peg->position);
        stateTracker->cubeShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
        peg->Render(stateTracker->cubeShader);

        // render bounding box
        if (!stateTracker->isDebugOn)
        {
            continue;
        }
        if (!stateTracker->isBoundingBoxesOn)
        {
            continue;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonOffset(0, -1);
        glEnable(GL_POLYGON_OFFSET_FILL);
        stateTracker->cubeShader->SetBool("debugUniform", true);
        peg->Render(stateTracker->cubeShader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonOffset(1, 0);
        glEnable(GL_POLYGON_OFFSET_FILL);
    }

    //cylinder normal
    if (stateTracker->isVertexNormalsDisplayOn)
    {
        stateTracker->normalShader->Use();
        stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
        stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);

        for (auto peg : stateTracker->pegs)
        {
            stateTracker->modelMatrix = glm::mat4(1.0f);
            stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, peg->position);
            stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
            peg->Render(stateTracker->normalShader);
        }
    }

    //Particles
    stateTracker->particleShader->Use();
    stateTracker->particleShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->particleShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->particleShader->SetCamera("cameraUniform", *stateTracker->camera);
    glActiveTexture(GL_TEXTURE0);
    stateTracker->particleGenerator->Render(stateTracker->particleShader, stateTracker->particleTexture);

    // sphere
    stateTracker->sphereShader->Use();
    stateTracker->sphereShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->sphereShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->modelMatrix = glm::mat4(1.0f);
    stateTracker->sphereShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->sphereShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->sphereShader->SetLightingModel(*stateTracker->lightModel);
    for (unsigned int i = 0; i < stateTracker->spheres.size(); i++)
    {
        stateTracker->sphereShader->SetArrayVec3("offsets", i, stateTracker->spheres[i]->position);
    }
    stateTracker->sphereShader->SetBool("debugUniform", false);
    stateTracker->testSphere->Render(stateTracker->sphereShader, stateTracker->skyBox->cubemapTexture, stateTracker->spheres.size());

    // sphere bounding box
    if (stateTracker->isDebugOn)
    {
        if (stateTracker->isBoundingBoxesOn)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPolygonOffset(0, -1);
            glEnable(GL_POLYGON_OFFSET_FILL);
            stateTracker->sphereShader->SetBool("debugUniform", true);
            stateTracker->testSphere->Render(stateTracker->sphereShader, stateTracker->skyBox->cubemapTexture, stateTracker->spheres.size());
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glPolygonOffset(1, 0);
            glEnable(GL_POLYGON_OFFSET_FILL);
        }
    }

    // sphere normal
    if (stateTracker->isVertexNormalsDisplayOn)
    {
        stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
        stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
        for (unsigned int i = 0; i < stateTracker->spheres.size(); i++)
        {
            stateTracker->normalShader->Use();
            stateTracker->modelMatrix = glm::mat4(1.0f);
            stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, stateTracker->spheres[i]->position);
            stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
            stateTracker->spheres[i]->RenderNormals(stateTracker->normalShader, stateTracker->skyBox->cubemapTexture);
        }
    }

    //Lights
    if (stateTracker->isDebugOn && stateTracker->isLightingBoxesOn)
    {
        stateTracker->lightingShader->Use();
        stateTracker->lightingShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
        stateTracker->lightingShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
        stateTracker->lightingShader->SetBool("fixedColor", false);
        glBindVertexArray(stateTracker->cube->vertexArray);

        for (unsigned int i = 0; i < stateTracker->lightModel->GetNumberOfLights(); i++)
        {
            stateTracker->modelMatrix = glm::mat4(1.0f);
            stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, stateTracker->lightModel->GetLight(i)->position);

            stateTracker->modelMatrix = glm::scale(stateTracker->modelMatrix, glm::vec3(0.2f)); // a smaller cube
            stateTracker->lightingShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
            stateTracker->lightingShader->SetLight("lightUniform", *stateTracker->lightModel->GetLight(i));

            glDrawElements(GL_TRIANGLES, stateTracker->cube->numberOfFaces * 3, GL_UNSIGNED_INT, 0);
        }
        stateTracker->modelMatrix = glm::mat4(1.0f);
        stateTracker->lightingShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);

        //unbind cube vertex array
        glBindVertexArray(0);

        // directional light vector
        stateTracker->directionalLightShader->Use();
        stateTracker->modelMatrix = glm::mat4(1.0f);
        stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, stateTracker->lightModel->GetLight(0)->position);
        stateTracker->directionalLightShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
        stateTracker->directionalLightShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
        stateTracker->directionalLightShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);

        stateTracker->directionalLightShader->SetVec3("endVector", stateTracker->lightModel->GetLight(0)->direction);
        stateTracker->directionalLightShader->SetVec3("lineColor", glm::vec3(1, 0, 0));
        stateTracker->debugObject->Render(stateTracker->directionalLightShader);
    }

    // quad tree render
    if (stateTracker->isDebugOn && stateTracker->isQuadTreeOn)
    {
        glActiveTexture(0);
        //WHATEVER THE LAST SHADER IS
        stateTracker->lightingShader->Use();
        stateTracker->lightingShader->SetBool("fixedColor", true);
        quadtree->DrawQuadTree(stateTracker->directionalLightShader);
    }

    // draw skybox as last
    glDepthFunc(GL_LESS); // set depth function back to default
    stateTracker->skyBoxShader->Use();
    // remove translation from the view matrix
    stateTracker->viewMatrix = glm::mat4(glm::mat3(stateTracker->camera->GetViewMatrix()));
    stateTracker->skyBoxShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->skyBoxShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->skyBox->Render(stateTracker->skyBoxShader);
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

    // Bloom Shader
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. blur bright fragments with two-pass Gaussian Blur 
    // --------------------------------------------------

    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    stateTracker->blurShader->Use();
    glActiveTexture(GL_TEXTURE0);
    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, stateTracker->pingpongFBO[horizontal]);
        stateTracker->blurShader->SetInt("horizontal", horizontal);

        glBindTexture(GL_TEXTURE_2D, first_iteration ? stateTracker->colorBuffers[1] : stateTracker->pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
        stateTracker->quad->Render(stateTracker->blurShader);
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 3. now render floating point color buffer to 2D quad and
    //tonemap HDR colors to default framebuffer's (clamped) color range
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    stateTracker->bloomShader->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stateTracker->colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, stateTracker->pingpongColorbuffers[!horizontal]);
    stateTracker->bloomShader->SetBool("bloom", stateTracker->isBloomOn);
    stateTracker->bloomShader->SetFloat("exposure", stateTracker->bloomExposure);
    stateTracker->quad->Render(stateTracker->bloomShader);
}
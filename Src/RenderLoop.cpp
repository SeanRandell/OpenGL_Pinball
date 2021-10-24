#include "RenderLoop.h"
#include <utility>

RenderLoop::RenderLoop(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    //TODO fix this so that it does not break non fullscreen
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
    stateTracker->Init();
    stateTracker->BuildGameObjects();
    stateTracker->InitBuffers(screenWidth, screenHeight);
    stateTracker->particleGenerator->Init();

    stateTracker->blockShader->Use();
    stateTracker->blockShader->SetInt("objectMaterialUniform.diffuse", 0);
    stateTracker->blockShader->SetInt("objectMaterialUniform.specular", 1);
    stateTracker->blockShader->SetInt("objectMaterialUniform.reflectionMap", 2);
    stateTracker->blockShader->SetInt("skyBoxUniform", 3);

    //TODO fix for multtiShader?
    stateTracker->blockShader->SetInt("shadowMap", 4);

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
    stateTracker->debugDepthQuadShader->Use();
    stateTracker->debugDepthQuadShader->SetInt("depthMap", 0);


    // Add directional light to follow camera view direction
    stateTracker->lightModel->AddLight({
        .type = LightType::DirectionalLight,
        .ambient = glm::vec3(0.2, 0.2, 0.2),
        .diffuse = glm::vec3(1.0, 1.0, 1.0),
        .specular = glm::vec3(0.5, 0.5, 0.5),
        .direction = glm::vec3(1.0, 0.0, 0.0),
        .position = glm::vec3(0.0, 8.0, 0.0)
        });
    // Add x-axis red spot lights (palce all at x+3.0 so they are positioned around shaded cube)
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(1.0, 0.0, 0.0),
        .diffuse = glm::vec3(1.0, 0.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(2.0 + 3.0, 0.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(1.0, 0.0, 0.0),
        .diffuse = glm::vec3(1.0, 0.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-2.0 + 3.0, 0.0, 6.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    //// Add y-axis green spot lights
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 1.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, 2.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 1.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, -2.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    //// Add z-axis blue spot lights
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 1.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, 0.0, 2.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 1.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, 0.0, -2.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });

    // white lights 
    stateTracker->lightModel->AddLight({
    .type = LightType::PointLight,
    .ambient = glm::vec3(0.2, 0.2, 0.2),
    .diffuse = glm::vec3(1.0, 1.0, 1.0),
    .specular = glm::vec3(1.0, 1.0, 1.0),
    .position = glm::vec3(1.0 , .0, 2.0),
    .constant = 1.0f,
    .linear = 0.35f,
    .quadratic = 0.44f
        });

    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(1.0, 1.0, 1.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(8.0 + 3.0, 3.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
}

void RenderLoop::CheckInput(StateTracker* stateTracker, bool* quitApp, float deltaTime) {
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

           //if (stateTracker->camera->moveFoward) {
       //    std::cout << "move forward Second" << std::boolalpha << stateTracker->camera->moveFoward << std::endl;
       //}


    SDL_Event keyEvent;

    //while (SDL_PollEvent(&keyEvent)) {
    //    if (keyEvent.type == SDL_MOUSEMOTION) {
    //        //fprintf(stderr, "mouse x: %d, y: %d\n", keyEvent.button.x, keyEvent.button.y);
    //        //SDL_GetMouseState(&xpos, &ypos);

    //        static int xPosition = stateTracker->camera->lastX;
    //        static int yPosition = stateTracker->camera->lastY;
    //        xPosition += keyEvent.motion.xrel;
    //        yPosition += keyEvent.motion.yrel;

    //        if (stateTracker->camera->firstMouse && (stateTracker->camera->lastX = xPosition) && (stateTracker->camera->lastY = yPosition)) {
    //            stateTracker->camera->lastX = xPosition;
    //            stateTracker->camera->lastY = yPosition;
    //            stateTracker->camera->firstMouse = false;
    //        }

    //        float xOffSet = xPosition - stateTracker->camera->lastX;
    //        float yOffSet = stateTracker->camera->lastY - yPosition;

    //        stateTracker->camera->lastX = xPosition;
    //        stateTracker->camera->lastY = yPosition;

    //        stateTracker->camera->ProcessMouseMovement(xOffSet, yOffSet, deltaTime, true);
    //    }
    //}
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

            stateTracker->camera->ProcessMouseMovement(xOffSet, yOffSet, deltaTime, true);
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
                //TODO - reset camera turn debug stuff off
                break;
            case SDLK_m:
                stateTracker->camera->mouseControls = !stateTracker->camera->mouseControls;
                break;
            case SDLK_SPACE:
                //TODO - launch ball after time
                stateTracker->canLaunchBall = true;
                std::cout << "space pressed" << std::endl;
                break;
            case SDLK_PERIOD:
                // '>' � right flipper
                stateTracker->moveRightFlipper = true;
                break;
            case SDLK_COMMA:
                //'<' �  left flipper
                stateTracker->moveLeftFlipper = true;
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
    stateTracker->camera->ProcessCameraMoving(deltaTime);
    stateTracker->camera->ProcessCameraTurning(deltaTime);

    // Update directional camera to align with camera forward direction
    stateTracker->lightModel->GetLight(0)->direction = stateTracker->camera->front;

    stateTracker->modelMatrix = glm::mat4(1.0f);
    stateTracker->viewMatrix = stateTracker->camera->GetViewMatrix();


    if (stateTracker->launchCountdown >= stateTracker->launchCooldown) {
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
        //std::cout << "Cooldown" << std::endl;
    }

    //PHYSICS
    physicsMethods->CalculateBallPhysics(stateTracker, deltaTime, quadtree);

    // check if ball needs to be deleted
    if (stateTracker->spheres.size() > 0)
    {
        //auto i = std::remove_if(stateTracker->spheres.begin(), stateTracker->spheres.end(), [&](Sphere* o) {
        //    return (o->position.x <= -200 || o.position->z <= -200 || o.position->z >= 200 || o.position->x >= 200);
        //    });
        //if (i != stateTracker->spheres.end()) {
        //    stateTracker->spheres[i]->end;
        //    stateTracker->spheres.erase(i);
        //}
        //stateTracker->spheres.erase(std::remove(
        //    stateTracker->spheres.begin(), stateTracker->spheres.end(), 8), vec.end());
        //std::vector<Sphere*>::std::iterator it;
        //for (it2 = stateTracker->spheres.begin(); it2 != stateTracker->spheres.end();)
        //{
        //        if (...)
        //        {
        //            it2 = uc.erase(it2);
        //        }
        //        else
        //        {
        //            ++it2;
        //        }
        //    ...
        //}
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

    // update particles
    for (int i = 0; i < stateTracker->spheres.size(); i++)
    {

    }
}

void RenderLoop::RenderFrame(StateTracker* stateTracker, Quadtree* quadtree)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
            // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
    //glm::mat4 lightProjection, lightView;
    //glm::mat4 lightSpaceMatrix;
    //float near_plane = 1.0f, far_plane = 7.5f;
    ////lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
    //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    //lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    //lightSpaceMatrix = lightProjection * lightView;
    //// render scene from light's point of view
    //stateTracker->simpleDepthShader->Use();
    //stateTracker->simpleDepthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //glBindFramebuffer(GL_FRAMEBUFFER, stateTracker->depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    //RenderScene(stateTracker->simpleDepthShader);
    //RenderScene(stateTracker);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// reset viewport
    //glViewport(0, 0, screenWidth, screenHeight);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scene into floating point framebuffer
    // -----------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, stateTracker->hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //block
    stateTracker->blockShader->Use();
    //stateTracker->blockShader->SetVec3("objectMaterialUniform.ambient", stateTracker->block->material.ambient);
    //stateTracker->blockShader->SetFloat("objectMaterialUniform.shininess", stateTracker->block->material.shininess);
    stateTracker->blockShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->blockShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->blockShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->blockShader->SetLightingModel(*stateTracker->lightModel);

    for (unsigned int i = 0; i < stateTracker->blocks.size(); i++)
    {
        stateTracker->modelMatrix = glm::mat4(1.0f);
        glm::vec3 testVector = glm::vec3(stateTracker->blocks[i]->position.x, stateTracker->blocks[i]->position.y, stateTracker->blocks[i]->position.z);
        stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, testVector);
        stateTracker->modelMatrix = glm::scale(stateTracker->modelMatrix, stateTracker->blocks[i]->scale);
        stateTracker->blockShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
        stateTracker->blocks[i]->Render(stateTracker->blockShader, stateTracker->skyBox, stateTracker->modelMatrix);

        //stateTracker->blockShader->SetVec3(("offsets[" + std::to_string(i) + "]")), stateTracker->[i]);
        //stateTracker->blocks[i]->Render();
    }

    //block normal
    //stateTracker->normalShader->Use();
    //stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->block->Render(stateTracker->normalShader, stateTracker->skyBox);

    // cube
    //stateTracker->cubeShader->Use();
    //stateTracker->cubeShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->cubeShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->cubeShader->SetCamera("cameraUniform", *stateTracker->camera);
    //stateTracker->cubeShader->SetLightingModel(*stateTracker->lightModel);

    //stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, glm::vec3(3.0, 0.0, 0.0));
    //stateTracker->cubeShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->cube->Render(stateTracker->cubeShader);

    //stateTracker->normalShader->Use();
    //stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->cube->Render(stateTracker->normalShader);

    // sphere
    stateTracker->sphereShader->Use();
    stateTracker->sphereShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->sphereShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->sphereShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->sphereShader->SetLightingModel(*stateTracker->lightModel);

    //--------------------
    //stateTracker->sphereMatrices.clear();
    //for (int i = 0; i < stateTracker->spherePositions.size(); i++)
    //{
    //    glm::mat4 newMatrix = glm::mat4(1.0f);
    //    glm::translate(newMatrix, stateTracker->spherePositions[i]);
    //    stateTracker->sphereMatrices.push_back(newMatrix);
    //}
    //if (stateTracker->spherePositions.size() > 0) {
    //    glBindBuffer(GL_ARRAY_BUFFER, stateTracker->sphereVertexBuffer);
    //    glBufferSubData(GL_ARRAY_BUFFER,
    //        stateTracker->testSphere->GetInterleavedVertexSize(),
    //        stateTracker->sphereMatrices.size() * sizeof(glm::mat4),
    //        stateTracker->sphereMatrices.data());
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glBindVertexArray(stateTracker->sphereVertexArray);



    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, stateTracker->skyBox->cubemapTexture);
    //    //glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);


    //    // draw center sphere
    //    glDrawElementsInstanced(GL_TRIANGLES,            // primitive type
    //        stateTracker->testSphere->GetIndexCount(), // # of indices
    //        GL_UNSIGNED_INT,         // data type
    //        (void*)0,                // ptr to indices
    //        stateTracker->spherePositions.size());

    //    glBindVertexArray(0);
    //}
    //-------------------------

    for (unsigned int i = 0; i < stateTracker->spheres.size(); i++)
    {
        stateTracker->modelMatrix = glm::mat4(1.0f);
        stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, stateTracker->spheres[i]->position);
        stateTracker->modelMatrix = glm::scale(stateTracker->modelMatrix, stateTracker->spheres[i]->scale);
        stateTracker->sphereShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
        stateTracker->spheres[i]->Render(stateTracker->sphereShader, stateTracker->skyBox->cubemapTexture, stateTracker->sphereMatrices);

        //sphere normal
    /*    stateTracker->normalShader->Use();
        stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
        stateTracker->spheres[i]->Render(stateTracker->normalShader, stateTracker->skyBox->cubemapTexture);*/
    }
    //cylinder
    //stateTracker->sphereShader->Use();
    //stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, glm::vec3(9.0, 0.0, 0.0));
    //stateTracker->sphereShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->cylinder->Render(stateTracker->sphereShader);

    //stateTracker->normalShader->Use();
    //stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->cylinder->Render(stateTracker->normalShader);


    //Lights
    stateTracker->lightingShader->Use();
    stateTracker->lightingShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->lightingShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);

    glBindVertexArray(stateTracker->cube->vertexArray);


    //float mat_ambient[] = { 0.2, 0.0, 0.0, 1.0 };
    //float mat_colour[] = { 0.2, 0.0, 0.0, 1.0 };
    //float mat_specular[] = { 1.0, 0.0, 0.0, 0.0 };
    //float mat_shininess[] = { 100.0 };
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glm::vec3 testVector = glm::normalize(stateTracker->lightModel->GetLight(0)->direction);
    //glm::vec3 testVector = stateTracker->lightModel->GetLight(0)->direction;
    //glm::vec3 posVector = stateTracker->lightModel->GetLight(0)->position;

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

    //QUAD TEST
    //stateTracker->blurShader->Use();
    //stateTracker->blurShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->blurShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->modelMatrix = glm::mat4(1.0f);
    //stateTracker->lightingShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->quad->Render(stateTracker->lightingShader);


    //// set directional light
    //stateTracker->directionalLightShader->Use();
    //stateTracker->modelMatrix = glm::mat4(1.0f);
    //stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, stateTracker->lightModel->GetLight(0)->position);
    //stateTracker->directionalLightShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->directionalLightShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->directionalLightShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);

    //stateTracker->directionalLightShader->SetVec3("endVector", stateTracker->lightModel->GetLight(0)->direction);
    //stateTracker->directionalLightShader->SetVec3("lineColor", glm::vec3(1, 0, 0));
    //stateTracker->debugObject->Render(stateTracker->directionalLightShader);


    //directional light direction shader
    //stateTracker->directionalLightShader->Use();
    //stateTracker->directionalLightShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->directionalLightShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->directionalLightShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //// set directional light
    //stateTracker->directionalLightShader->SetLight("lightUniform", *stateTracker->lightModel->GetLight(0));
    //stateTracker->cube->Render(stateTracker->directionalLightShader);
    //glUseProgram(0);
    glActiveTexture(0);
    //glMatrixMode(GL_MODELVIEW);
    //WHATEVER THE LAST SHADER IS
    //glLoadIdentity();
    //glPushMatrix();
    quadtree->DrawQuadTree(stateTracker->lightingShader);
    //glPopMatrix();
    //quadtree->DrawQuadTree();

    // draw skybox as last
    glDepthFunc(GL_LESS); // set depth function back to default
    stateTracker->skyBoxShader->Use();
    // remove translation from the view matrix
    stateTracker->viewMatrix = glm::mat4(glm::mat3(stateTracker->camera->GetViewMatrix()));
    stateTracker->skyBoxShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->skyBoxShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->skyBox->Render(stateTracker->skyBoxShader);
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

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

    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    //

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    stateTracker->bloomShader->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stateTracker->colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, stateTracker->pingpongColorbuffers[!horizontal]);
    stateTracker->bloomShader->SetBool("bloom", stateTracker->isBloomOn);
    stateTracker->bloomShader->SetFloat("exposure", stateTracker->bloomExposure);
    stateTracker->quad->Render(stateTracker->bloomShader);



    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glActiveTexture(0);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //debug quadtree
    //float mat_ambient[] = { 0.2, 0.0, 0.0, 1.0 };
    //float mat_colour[] = { 0.2, 0.0, 0.0, 1.0 };
    //float mat_specular[] = { 1.0, 0.0, 0.0, 0.0 };
    //float mat_shininess[] = { 100.0 };
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glPushMatrix();
    //glColor3d(1, 0, 0);
    //stateTracker->quadtree->DrawQuadTree();
    //glPopMatrix();
}

void RenderLoop::RenderScene(StateTracker* stateTracker)
{

}


#include "RenderLoop.h"
#include <utility>

RenderLoop::RenderLoop(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    //TODO fix this so that it does not break non fullscreen
    windowWidth = screenWidth;
    windowHeight = screenHeight;
}

void RenderLoop::Init(StateTracker* stateTracker, int screenWidth, int screenHeight)
{
    stateTracker->Init();
    stateTracker->BuildGameObjects();
    stateTracker->InitBuffers(screenWidth, screenHeight);

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

void RenderLoop::CheckInput(StateTracker* stateTracker, bool* quitApp) {
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
    if (SDL_PollEvent(&keyEvent))
    {
        std::cout << "key pressed" << std::endl;
        switch (keyEvent.type)
        {
        case SDLK_b:
            stateTracker->isDebugOn = !stateTracker->isDebugOn;
            //TODO - reset camera turn debug stuff off
            break;
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

    // Update directional camera to align with camera forward direction
    stateTracker->lightModel->GetLight(0)->direction = stateTracker->camera->front;

    stateTracker->modelMatrix = glm::mat4(1.0f);
    stateTracker->viewMatrix = stateTracker->camera->GetViewMatrix();

    //PHYSICS
    auto DoCirclesOverLap = [](float x1, float y1, float radius1, float x2, float y2, float radius2) {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= (radius1 + radius2));
    };

    // vector for the balls that have collided
    std::vector<std::pair<Sphere*, Sphere*>> vectorCollidingPairs;

    //Update ball positions
    for (auto& ball : stateTracker->spheres) {

        //friction
        ball->acceleration.x = -ball->velocity.x * 0.08f;
        ball->acceleration.y = -ball->velocity.y * 0.08f;

        // velocities
        ball->velocity.x += ball->acceleration.x * deltaTime;
        ball->velocity.y += ball->acceleration.y * deltaTime;

        //positions
        ball->position.x += ball->velocity.x * deltaTime;
        ball->position.y += ball->velocity.y * deltaTime;

        // wrap around the screen
        if (ball->position.x < 0) ball->position.x += float(screenWidth);
        if (ball->position.x >= screenWidth) ball->position.x -= float(screenWidth); 
        if (ball->position.y < 0) ball->position.y += float(screenHeight);
        if (ball->position.y >= 0) ball->position.y -= float(screenHeight);

        //if ball is approaching zero then stop it
        if (fabs(ball->velocity.x * ball->velocity.x + ball->velocity.y * ball->velocity.y) < 0.01f) {
            ball->velocity.x = 0;
            ball->velocity.y = 0;
        }
    }

    // test every ball against every other ball
    // current ball
    for (auto& ball : stateTracker->spheres)
    {
        // target ball
        for (auto& target : stateTracker->spheres)
        {
            // filter out self collisions
            if (ball->id != target->id)
            {
                if (DoCirclesOverLap(ball->position.x, ball->position.y, ball->radius, target->position.x, target->position.y, target->radius))
                {
                    // Distance between ball centers
                    float distance = sqrtf((ball->position.x - target->position.x) * (ball->position.x - target->position.x) + (ball->position.y - target->position.y) * (ball->position.y - target->position.y));

                    float overlap = 0.5f * (distance - ball->radius - target->radius);

                    //collision has occured
                    vectorCollidingPairs.push_back({ ball, target });

                    //Displace current ball
                    ball->position.x -= overlap * (ball->position.x - target->position.x) / distance;
                    ball->position.y -= overlap * (ball->position.y - target->position.y) / distance;

                    //Displace target ball
                    target->position.x += overlap * (ball->position.x - target->position.x) / distance;
                    target->position.y += overlap * (ball->position.y - target->position.y) / distance;
                }
            }
        }
    }

    // work out dynamic collisions
    for (auto collidedSpheres : vectorCollidingPairs)
    {
        Sphere* ball1 = collidedSpheres.first;
        Sphere* ball2 = collidedSpheres.second;

        // Distance between ball centers
        float distance = sqrtf((ball1->position.x - ball2->position.x) * (ball1->position.x - ball2->position.x) + (ball1->position.y - ball2->position.y) * (ball1->position.y - ball2->position.y));

        //Normal 
        float nx = (ball2->position.x - ball1->position.x) / distance;
        float ny = (ball2->position.y - ball1->position.y) / distance;

        //Tangent
        float tx = -ny;
        float ty = nx;

        // Dot Product Tangent
        float dpTan1 = ball1->velocity.x * tx + ball1->velocity.y * ty;
        float dpTan2 = ball2->velocity.x * tx + ball2->velocity.y * ty;

        //TODO if a do not do this if it is a peg that the ball is colliding with.
        float dpNormal1 = ball1->velocity.x * nx + ball1->velocity.y * ny;
        float dpNormal2 = ball2->velocity.x * nx + ball2->velocity.y * ny;

        //conservation of momentum in 1D
        float momentum1 = (dpNormal1 * (ball1->mass - ball2->mass) + 2.0f * ball2->mass * dpNormal2) / (ball1->mass + ball2->mass);
        float momentum2 = (dpNormal2 * (ball2->mass - ball1->mass) + 2.0f * ball1->mass * dpNormal1) / (ball1->mass + ball2->mass);


        // update velocities
        ball1->velocity.x = tx * dpTan1 * momentum1;
        ball1->velocity.y = ty * dpTan1 * momentum1;

        //TODO if a ball and not a peg
        ball2->velocity.x = tx * dpTan2 * momentum2;
        ball2->velocity.y = ty * dpTan2 * momentum2;


        //TODO wiki shortened version of these calcs 
        //float kx = (ball1->velocity.x - ball2->velocity.x);
        //float ky = (ball1->velocity.y - ball2->velocity.y);
        //float p = 2.0 * (nx * kx + ny * ky) / (ball1->mass + ball2->mass);
        //ball1->velocity.x = ball1->velocity.x - p * ball2->mass * nx;
        //ball1->velocity.y = ball1->velocity.y - p * ball2->mass * ny;
        //ball2->velocity.x = ball2->velocity.x + p * ball1->mass * nx;
        //ball2->velocity.y = ball2->velocity.y + p * ball1->mass * ny;

    }
}

void RenderLoop::RenderFrame(StateTracker* stateTracker)
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

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, stateTracker->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    //RenderScene(stateTracker->simpleDepthShader);
    RenderScene(stateTracker);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, screenWidth, screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, stateTracker->blocks[i]->position);
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
    //stateTracker->sphereShader->Use();
    //stateTracker->sphereShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    //stateTracker->sphereShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    //stateTracker->sphereShader->SetCamera("cameraUniform", *stateTracker->camera);
    //stateTracker->sphereShader->SetLightingModel(*stateTracker->lightModel);

    //stateTracker->modelMatrix = glm::mat4(1.0f);
    //stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, glm::vec3(6.0, 0.0, 0.0));
    //stateTracker->sphereShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->sphere->Render(stateTracker->sphereShader, stateTracker->skyBox->cubemapTexture);

    // sphere normal
    //stateTracker->normalShader->Use();
    //stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    //stateTracker->sphere->Render(stateTracker->normalShader, stateTracker->skyBox->cubemapTexture);

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
}

void RenderLoop::RenderScene(StateTracker* stateTracker)
{

}


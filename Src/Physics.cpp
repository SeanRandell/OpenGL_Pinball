#include "Physics.h"

void Physics::CalculateBallPhysics(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree)
{
    if (stateTracker->isQuadTreeOn)
    {
        BuildQuadtree(stateTracker, deltaTime, quadtree);
    }
    else
    {
        for (auto& ball : stateTracker->spheres) {
            PhyiscsPerSphere(ball, stateTracker->spheres, stateTracker->blocks, stateTracker->pegs, deltaTime);
        }
    }
}

void Physics::BuildQuadtree(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree)
{
    PopulateQuadtree(stateTracker, quadtree);

    for (auto& ball : stateTracker->spheres) {
        std::vector<Object*>objectList = quadtree->Query(ball);

        std::vector<Sphere*> sphereList;
        std::vector<Block*> blockList;
        std::vector<Cylinder*> pegList;

        for (Object* object : objectList)
        {
            if (object->GetName() == "Sphere") {

                sphereList.push_back((Sphere*)object);
            }
            if (object->GetName() == "Block") {

                blockList.push_back((Block*)object);
            }
            if (object->GetName() == "Peg") {

                pegList.push_back((Cylinder*)object);
            }
        }
        PhyiscsPerSphere(ball, sphereList, blockList, pegList, deltaTime);
    }
}

//void Physics::PhyiscsPerSphere(Sphere* ball, std::vector<Object*> objectList, float deltaTime)
void Physics::PhyiscsPerSphere(Sphere* ball, std::vector<Sphere*> sphereList, std::vector<Block*> blockList, std::vector<Cylinder*> pegList, float deltaTime)
{
    // vector for the balls and Pegs that have collided
    std::vector<std::pair<Sphere*, Sphere*>> vectorSphereCollidingPairs;
    std::vector<std::pair<Sphere*, Cylinder*>> vectorPegCollidingPairs;

    //Update ball 

    //gravity and friction
    ball->acceleration.x = -ball->velocity.x * 0.4f;
    ball->acceleration.y = -ball->velocity.y * 0.8f - 25.0f;

    // velocities
    ball->velocity.x += ball->acceleration.x * deltaTime;
    ball->velocity.y += ball->acceleration.y * deltaTime;

    // position
    ball->position.x += ball->velocity.x * deltaTime;
    ball->position.y += ball->velocity.y * deltaTime;

    for (auto& target : sphereList)
    {
        // filter out self collisions
        if (ball->id == target->id)
        {
            continue;
        }
        bool overlapTest = DoCirclesOverLap(ball->position.x, ball->position.y, ball->radius, target->position.x, target->position.y, target->radius);
        if (!overlapTest)
        {
            continue;
        }
        //collission has occured
        vectorSphereCollidingPairs.push_back({ ball, target });

        // Distance between ball centers
        float distance = sqrtf((ball->position.x - target->position.x) * (ball->position.x - target->position.x) + (ball->position.y - target->position.y) * (ball->position.y - target->position.y));
        float overlap = 0.5f * (distance - ball->radius - target->radius);

        //Displace current ball
        ball->position.x -= overlap * (ball->position.x - target->position.x) / distance;
        ball->position.y -= overlap * (ball->position.y - target->position.y) / distance;

        //Displace target ball
        target->position.x += overlap * (ball->position.x - target->position.x) / distance;
        target->position.y += overlap * (ball->position.y - target->position.y) / distance;
    }

    // work out dynamic collisions
    for (auto collidedSpheres : vectorSphereCollidingPairs)
    {
        CollideTwoSpheres(collidedSpheres);
    }

    // target peg
    for (auto& target : pegList)
    {
        bool overlapTest = DoCirclesOverLap(ball->position.x, ball->position.y, ball->radius, target->position.x, target->position.y, target->topRadius);
        if (!overlapTest)
        {
            continue;
        }
        //collission has occured
        vectorPegCollidingPairs.push_back({ ball, target });

        // Distance between ball and peg centers
        float distance = sqrtf((ball->position.x - target->position.x) * (ball->position.x - target->position.x) + (ball->position.y - target->position.y) * (ball->position.y - target->position.y));
        float overlap = 0;

        overlap = 1.0f * (distance - ball->radius - target->topRadius);

        //Displace current ball
        ball->position.x -= overlap * (ball->position.x - target->position.x) / distance;
        ball->position.y -= overlap * (ball->position.y - target->position.y) / distance;
    }


    // work out dynamic collisions with Pegs
    for (auto collidedObjects : vectorPegCollidingPairs)
    {
        CollideSphereAndPeg(collidedObjects);
    }

    //test every ball against every other block
        // target ball
    for (auto& currentBlock : blockList)
    {
        SphereAndBlockCollision(currentBlock, ball);
    }
}

void Physics::SphereAndBlockCollision(Block*& currentBlock, Sphere* ball)
{
    if (currentBlock->isScenery == false)
    {
        Collision collision = CheckCollision(*ball, *currentBlock);
        if (std::get<0>(collision)) // if collision is true
        {
            // collision resolution
            Direction dir = std::get<1>(collision);
            glm::vec2 diff_vector = std::get<2>(collision);
            if (dir == LEFT || dir == RIGHT) // horizontal collision
            {
                if (currentBlock->isBumper)
                {
                    ball->velocity.x = -ball->velocity.x; // reverse horizontal velocity
                                                          //ball->acceleration.x += 20.0f;
                    if (currentBlock->isActiveFlipper)
                    {
                        ball->velocity.x += 12.0f;
                    }
                }
                else
                {
                    ball->velocity.x = -ball->velocity.x; // reverse horizontal velocity
                }

                // relocate
                float penetration = ball->radius - std::abs(diff_vector.x);
                if (dir == LEFT)
                {
                    ball->position.x += penetration; // move ball to right
                }
                else
                {
                    ball->position.x -= penetration; // move ball to left;
                }
            }
            else // vertical collision
            {
                if (currentBlock->isBumper)
                {
                    ball->velocity.y = -ball->velocity.y; // reverse vertical velocity
                                                          //ball->acceleration.y += 5.0f;
                    if (currentBlock->isActiveFlipper)
                    {
                        ball->velocity.y += 12.0f;
                    }
                }
                else
                {
                    ball->velocity.y = -ball->velocity.y; // reverse vertical velocity
                }
                // relocate
                float penetration = ball->radius - std::abs(diff_vector.y);
                if (dir == UP)
                {
                    ball->position.y -= penetration; // move ball bback up
                }
                else
                {
                    ball->position.y += penetration; // move ball back down
                }
            }
        }
    }
}

void Physics::CollideSphereAndPeg(std::pair<Sphere*, Cylinder*>& collidedObjects)
{
    Sphere* ball = collidedObjects.first;
    Cylinder* peg = collidedObjects.second;

    // Distance between ball centers
    float distance = sqrtf((ball->position.x - peg->position.x) * (ball->position.x - peg->position.x) + (ball->position.y - peg->position.y) * (ball->position.y - peg->position.y));

    //Normal 
    float nx = (peg->position.x - ball->position.x) / distance;
    float ny = (peg->position.y - ball->position.y) / distance;

    //Tangent
    float tx = -ny;
    float ty = nx;

    // Dot Product Tangent
    float dpTan1 = ball->velocity.x * tx + ball->velocity.y * ty;
    //float dpTan2 = peg->velocity.x * tx + peg->velocity.y * ty;

    //TODO if a do not do this if it is a peg that the ball is colliding with.
    //float dpNormal1 = ball->velocity.x * nx + ball->velocity.y * ny;
    //float dpNormal2 = peg->velocity.x * nx + peg->velocity.y * ny;

    ////conservation of momentum in 1D
    //float momentum1 = (dpNormal1 * (ball->mass - peg->mass) + 2.0f * peg->mass * dpNormal2) / (ball->mass + peg->mass);
    //float momentum2 = (dpNormal2 * (peg->mass - ball->mass) + 2.0f * ball->mass * dpNormal1) / (ball->mass + peg->mass);

    // update velocities
    ball->velocity.x = tx * dpTan1; //+ nx * momentum1;
    ball->velocity.y = ty * dpTan1;// +ny * momentum1;

    //reset peg light
    peg->ResetCooldownTimer();
    peg->pegHasBeenHit = true;
}

void Physics::CollideTwoSpheres(std::pair<Sphere*, Sphere*>& collidedSpheres)
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
    ball1->velocity.x = tx * dpTan1 + nx * momentum1;
    ball1->velocity.y = ty * dpTan1 + ny * momentum1;

    ball2->velocity.x = tx * dpTan2 + nx * momentum2;
    ball2->velocity.y = ty * dpTan2 + ny * momentum2;
}

bool Physics::DoCirclesOverLap(float x1, float y1, float radius1, float x2, float y2, float radius2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (radius1 + radius2);
}

Collision Physics::CheckCollision(Sphere& one, Block& two) // AABB - Circle collision
{
    // get center point circle first 
    //glm::vec2 center(one.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents((two.scale.x / 2.0f), (two.scale.y / 2.0f));
    //glm::vec2 aabb_center(
    //    two.position.x + aabb_half_extents.x,
    //    two.position.y + aabb_half_extents.y
    //);

    glm::vec2 blockPosition = glm::vec2(two.position.x, two.position.y);
    glm::vec2 ballPosition = glm::vec2(one.position.x, one.position.y);
    // get difference vector between both centers
    glm::vec2 difference = ballPosition - blockPosition;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = blockPosition + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - ballPosition;
    //std::cout << "Block: " << "(" << blockPosition.x << ", " << blockPosition.y << ") "
    //    << "Ball: " << "(" << ballPosition.x << ", " << ballPosition.y << ") "
    //    << "Diff: " << "(" << difference.x << ", " << difference.y << ") "
    //    << "length: " << "(" << glm::length(difference) << ") " << std::endl;
    /*  if (DoCirclesOverLap(one.position.x, one.position.y, one.radius, two.position.x, two.position.y, 0.5)) */

    if (glm::length(difference) < one.radius)
    {
        //std::cout << "COLLISION" << std::endl;
        // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Direction Physics::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            bestMatch = i;
        }
    }
    return (Direction)bestMatch;
}

void Physics::FipperPhysics(StateTracker* stateTracker, float deltaTime)
{
    // update left flipper
    if (stateTracker->moveLeftFlipper && !stateTracker->leftFlipperMoving)
    {
        // if less then launch peak
        if (stateTracker->leftFlipper->position.y <= -6.5f)
        {
            stateTracker->leftFlipper->position.y += 8 * deltaTime;
            stateTracker->leftFlipper->isActiveFlipper = true;
        }
        // if at peak
        if (stateTracker->leftFlipper->position.y >= -6.5f)
        {
            stateTracker->leftFlipperMoving = true;
            stateTracker->leftFlipper->isActiveFlipper = false;
        }
    }
    // if still moving
    if (stateTracker->leftFlipperMoving)
    {
        // if higher than default
        if (stateTracker->leftFlipper->position.y > -8.0f)
        {
            stateTracker->leftFlipper->position.y -= 10 * deltaTime;
        }
        else
        {
            // reset and ready for next trigger
            stateTracker->leftFlipperMoving = false;
            stateTracker->moveLeftFlipper = false;
            stateTracker->leftFlipper->position.y = -8.0f;
        }
    }

    // update right flipper
    if (stateTracker->moveRightFlipper && !stateTracker->rightFlipperMoving)
    {
        // if less then launch peak
        if (stateTracker->rightFlipper->position.y <= -6.5f)
        {
            stateTracker->rightFlipper->position.y += 8 * deltaTime;
            stateTracker->rightFlipper->isActiveFlipper = true;
        }
        // if at peak
        if (stateTracker->rightFlipper->position.y >= -6.5f)
        {
            stateTracker->rightFlipperMoving = true;
            stateTracker->rightFlipper->isActiveFlipper = false;
        }
    }
    // if still moving
    if (stateTracker->rightFlipperMoving)
    {
        // if higher than default
        if (stateTracker->rightFlipper->position.y > -8.0f)
        {
            stateTracker->rightFlipper->position.y -= 10 * deltaTime;
        }
        else
        {
            // reset and ready for next trigger
            stateTracker->rightFlipperMoving = false;
            stateTracker->moveRightFlipper = false;
            stateTracker->rightFlipper->position.y = -8.0f;
        }
    }
}

void Physics::PopulateQuadtree(StateTracker* stateTracker, Quadtree* quadtree)
{
    for (auto& ball : stateTracker->spheres) {
        quadtree->Insert(ball);
    }

    for (auto& block : stateTracker->blocks) {
        quadtree->Insert(block);
    }

    for (auto& peg : stateTracker->pegs) {
        quadtree->Insert(peg);
    }
}
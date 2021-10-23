#include "Physics.h"

void Physics::CalculateBallPhysics(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree)
{
    //build quadtree
    for (auto& ball : stateTracker->spheres) {
        quadtree->Insert(ball);
    }

    for (auto& block : stateTracker->blocks) {
        quadtree->Insert(block);
    }

    for (auto& ball : stateTracker->spheres) {
        std::vector<Object*> quadtreeResult = quadtree->Query(ball);
    }


    auto DoCirclesOverLap = [](float x1, float y1, float radius1, float x2, float y2, float radius2) {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (radius1 + radius2);
    };

    // vector for the balls that have collided
    std::vector<std::pair<Sphere*, Sphere*>> vectorCollidingPairs;

    //Update ball positions
    for (auto& ball : stateTracker->spheres) {

        //friction
        ball->acceleration.x = -ball->velocity.x * 0.01f;
        ball->acceleration.y = -ball->velocity.y * 0.01f;

        // velocities
        ball->velocity.x += ball->acceleration.x * deltaTime;
        ball->velocity.y += ball->acceleration.y * deltaTime;

        //positions
        ball->position.x += ball->velocity.x * deltaTime;
        ball->position.y += ball->velocity.y * deltaTime;

        // wrap around the screen
        //if (ball->position.x < 0) ball->position.x += float(stateTracker->screenWidth);
        //if (ball->position.x >= stateTracker->screenWidth) ball->position.x -= float(stateTracker->screenWidth);
        //if (ball->position.y < 0) ball->position.y += float(stateTracker->screenHeight);
        //if (ball->position.y >= 0) ball->position.y -= float(stateTracker->screenHeight);

        //if ball is approaching zero then stop it
        //if (fabs(ball->velocity.x * ball->velocity.x + ball->velocity.y * ball->velocity.y) < 0.01f) {
        //    ball->velocity.x = 0;
        //    ball->velocity.y = 0;
        //}
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
                    //collission has occured
                    vectorCollidingPairs.push_back({ ball, target });

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

    //test every ball against every other block
    for (auto& ball : stateTracker->spheres)
    {
        // target ball
        for (auto& currentBlock : stateTracker->blocks)
        {
            Collision collision = CheckCollision(*ball, *currentBlock);
            if (std::get<0>(collision)) // if collision is true
            {
                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // horizontal collision
                {
                    ball->velocity.x = -ball->velocity.x; // reverse horizontal velocity
                    // relocate
                    float penetration = ball->radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        ball->position.x += penetration; // move ball to right
                    else
                        ball->position.x -= penetration; // move ball to left;
                }
                else // vertical collision
                {
                    ball->velocity.y = -ball->velocity.y; // reverse vertical velocity
                    // relocate
                    float penetration = ball->radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        ball->position.y -= penetration; // move ball bback up
                    else
                        ball->position.y += penetration; // move ball back down
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
        ball1->velocity.x = tx * dpTan1 + nx * momentum1;
        ball1->velocity.y = ty * dpTan1 + ny * momentum1;

        //TODO if a ball and not a peg
        ball2->velocity.x = tx * dpTan2 + nx * momentum2;
        ball2->velocity.y = ty * dpTan2 + ny * momentum2;


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


Collision Physics::CheckCollision(Sphere& one, Block& two) // AABB - Circle collision
{
    // get center point circle first 
    //glm::vec2 center(one.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents((1 / 2.0f), (1 / 2.0f));
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
    std::cout << "Block: " << "(" << blockPosition.x << ", " << blockPosition.y << ") "
        << "Ball: " << "(" << ballPosition.x << ", " << ballPosition.y << ") "
        << "Diff: " << "(" << difference.x << ", " << difference.y << ") "
        << "length: " << "(" << glm::length(difference) << ") " << std::endl;
    /*  if (DoCirclesOverLap(one.position.x, one.position.y, one.radius, two.position.x, two.position.y, 0.5)) */

    if (glm::length(difference) < one.radius)
    {
        std::cout << "COLLISION" << std::endl;
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
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
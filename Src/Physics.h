#pragma once
#include "StateTracker.h"
#include <tuple>
#include <functional>

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Physics
{
public:
    Collision CheckCollision(Sphere& one, Block& two);
    void CalculateBallPhysics(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree);
    void BuildQuadtree(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree);
    void PopulateQuadtree(StateTracker* stateTracker, Quadtree* quadtree);
    void PhyiscsPerSphere(Sphere* ball, std::vector<Sphere*> sphereList, std::vector<Block*> blockList, std::vector<Cylinder*> pegList, float deltaTime);
    void SphereAndBlockCollision(Block*& currentBlock, Sphere* ball);
    void CollideSphereAndPeg(std::pair<Sphere*, Cylinder*>& collidedObjects);
    void CollideTwoSpheres(std::pair<Sphere*, Sphere*>& collidedSpheres);
    bool DoCirclesOverLap(float x1, float y1, float radius1, float x2, float y2, float radius2);
    Direction VectorDirection(glm::vec2 target);
    void FipperPhysics(StateTracker* stateTracker, float deltaTime);
};
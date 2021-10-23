#pragma once
#include "StateTracker.h"
#include <tuple>
#include <functional>

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Physics {
public:
	Collision CheckCollision(Sphere& one, Block& two);
    void CalculateBallPhysics(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree);
	Direction VectorDirection(glm::vec2 target);
};
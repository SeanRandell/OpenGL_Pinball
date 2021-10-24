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
	void PhyiscsPerSphere(Sphere* ball, std::vector<Object*> objectList, float deltaTime);
	bool DoCirclesOverLap(float x1, float y1, float radius1, float x2, float y2, float radius2);
	Direction VectorDirection(glm::vec2 target);
};
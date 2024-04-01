#pragma once
#include <vector>
#include <glad/glad.h>
#include "Sphere.h"
#include "Block.h"
#include "Rectangle.h"

struct Circle
{
    float x, y, radius;
    Circle(float x, float y, float radius)
    {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }
};

class Quadtree
{
public:
    int MAX_LEVELS = 100;
    int capacity;
    bool divided;
    int level;
    Rectangle* boundary;
    std::vector<Object*> objects;
    Quadtree* northeast;
    Quadtree* northwest;
    Quadtree* southeast;
    Quadtree* southwest;

    Quadtree(Rectangle* boundary, int capacity);
    ~Quadtree();

    void SubDivide();
    bool Insert(Object* block);
    std::vector<Object*> Query(Sphere* circle);
    std::vector<Quadtree*> FindQuadrant(Sphere* circle);
    std::vector<Object*> GetObjectList(Quadtree* currentQuadtree);
    void DrawQuadTree(ShaderObject* shader);
    void DrawQuadTree();
};
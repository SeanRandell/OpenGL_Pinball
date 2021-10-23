#pragma once
#include <vector>
#include <glad/glad.h>
#include "Sphere.h"
#include "Block.h"
#include "Rectangle.h"

struct Circle {
    float x, y, radius;
    Circle(float x, float y, float radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }
};

class Quadtree {
public:
    //int MAX_OBJECTS = 10;
    int MAX_LEVELS = 5;
    int capacity;
    bool divided;
    int level;
    //std::vector<Rectangle> objects;
    Rectangle* boundary;
    std::vector<Object*> objects;
    Quadtree* northeast;
    Quadtree* northwest;
    Quadtree* southeast;
    Quadtree* southwest;
    //std::vector<Quadtree*> nodes;

    Quadtree(Rectangle* boundary, int capacity);
    ~Quadtree();
    void Clear();
    void Split();
    void GetIndex();
    void SubDivide();
    bool Insert(Object* block);
    std::vector<Rectangle> Retrieve();
    std::vector<Object*> Query(Sphere* circle);
    Quadtree* FindQuadrant(Sphere* circle);
    std::vector<Object*> GetObjectList(Quadtree* currentQuadtree);
    void DrawQuadTree(RTRShader* shader);    
    void DrawQuadTree();
};
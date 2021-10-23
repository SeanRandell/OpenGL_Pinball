#pragma once
#include <vector>
#include <glad/glad.h>
#include "Sphere.h"

struct Circle {
    float x, y, radius;
    Circle(float x, float y, float radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }
};

struct Rectangle {
    //float left, bottom, width, height;
    float x, y, width, height;
    float left, right, top, bottom;
    Rectangle(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->left = x - width / 2;
        this->right = x + width / 2;
        this->top = y + height / 2;
        this->bottom = y - height / 2;
    }

    bool Contains(Sphere* circle) {
        return (
            circle->position.x + circle->radius >= this->x - this->width &&
            circle->position.x + circle->radius <= this->x + this->width &&
            circle->position.y + circle->radius >= this->y - this->width &&
            circle->position.y + circle->radius <= this->y + this->width
            );
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
    std::vector<Sphere*> objects;
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
    bool Insert(Sphere* circle);
    std::vector<Rectangle> Retrieve();
    std::vector<Sphere*> Query(Sphere* circle);
    Quadtree* FindQuadrant(Sphere* circle);
    std::vector<Sphere*> GetObjectList(Quadtree* currentQuadtree);
    void DrawQuadTree(RTRShader* shader);    
    void DrawQuadTree();
};
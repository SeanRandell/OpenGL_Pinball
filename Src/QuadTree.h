#pragma once
#include <vector>

struct Rectangle {
    float left, bottom, width, height;
};

class Quadtree {
public:
    int MAX_OBJECTS = 10;
    int MAX_LEVELS = 5;

    int level;
    std::vector<Rectangle> objects;
    Rectangle bounds;
    std::vector<Quadtree*> nodes;

    Quadtree(int pLevel, Rectangle pBounds);
    void Clear();
    void Split();
    void GetIndex();
    void Insert();
    std::vector<Rectangle> Retrieve();
};
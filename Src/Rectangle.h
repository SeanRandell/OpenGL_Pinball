#pragma once

class Rectangle {
public:
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
};
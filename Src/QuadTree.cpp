#include "QuadTree.h"

Quadtree::Quadtree(Rectangle* boundary, int capacity)
{
    this->level = 0;
    this->boundary = boundary;
    this->capacity = capacity;
    this->divided = false;
    northeast = nullptr;
    northwest = nullptr;
    southwest = nullptr;
    southeast = nullptr;
}

Quadtree::~Quadtree()
{
    objects.clear();
    if (divided) {
        delete northeast->boundary;
        delete northeast;
        delete northwest->boundary;
        delete northwest;
        delete southeast->boundary;
        delete southeast;
        delete southwest->boundary;
        delete southwest;
    }
}

void Quadtree::SubDivide()
{
    float x = boundary->x;
    float y = boundary->y;
    float width = boundary->width;
    float height = boundary->height;

    Rectangle* ne = new Rectangle(x + width / 4, y + height / 4, width / 2, height / 2);
    Rectangle* nw = new Rectangle(x - width / 4, y + height / 4, width / 2, height / 2);
    Rectangle* se = new Rectangle(x + width / 4, y - height / 4, width / 2, height / 2);
    Rectangle* sw = new Rectangle(x - width / 4, y - height / 4, width / 2, height / 2);
    northeast = new Quadtree(ne, capacity);
    northeast->level = level + 1;
    northwest = new Quadtree(nw, capacity);
    northwest->level = level + 1;
    southeast = new Quadtree(se, capacity);
    southeast->level = level + 1;
    southwest = new Quadtree(sw, capacity);
    southwest->level = level + 1;

    divided = true;
}

bool Quadtree::Insert(Object* object) {
    // if the circle is not in this rectangle than return
    if (!object->IsContainedWithin(boundary)) {
        return false;
    }
    // if the rectangle has less than capacity than 
    if (objects.size() < capacity && !divided) {
        objects.push_back(object);
        return true;
    }
    // divide quad tree
    if (!divided) {
        SubDivide();

        for (int i = 0; i < objects.size(); i++)
        {
            // then add to child rectangles
            northeast->Insert(objects[i]);
            northwest->Insert(objects[i]);
            southeast->Insert(objects[i]);
            southwest->Insert(objects[i]);
        }
        objects = std::vector<Object*>();
    }

    // then add to child rectangles
    bool insertIntoNortheast = northeast->Insert(object);
    bool insertIntoNorthwest = northwest->Insert(object);
    bool insertIntoSoutheast = southeast->Insert(object);
    bool insertIntoSouthwest = southwest->Insert(object);
    return(insertIntoNortheast || insertIntoNorthwest || insertIntoSoutheast || insertIntoSouthwest);
}

std::vector<Object*> Quadtree::Query(Sphere* circle)
{
    if (level > MAX_LEVELS) {
        return objects;
    }

    if (!divided)
    {
        return objects;
    }
    std::vector <Quadtree*> quadrantList = FindQuadrant(circle);
    std::vector<Object*> objectList;
    for (Quadtree* quadrant : quadrantList)
    {
        std::vector<Object*> innerObjectList = quadrant->Query(circle);
        for (Object* innerObject : innerObjectList)
        {
            objectList.push_back(innerObject);
        }
    }
    return objectList;
}

std::vector<Quadtree*> Quadtree::FindQuadrant(Sphere* circle)
{
    std::vector<Quadtree*> resultList;
    if (circle->IsContainedWithin(northeast->boundary)) {
        resultList.push_back(northeast);
    }
    if (circle->IsContainedWithin(northwest->boundary)) {
        resultList.push_back(northwest);
    }
    if (circle->IsContainedWithin(southeast->boundary)) {
        resultList.push_back(southeast);
    }
    if (circle->IsContainedWithin(southwest->boundary)) {
        resultList.push_back(southwest);
    }
    return resultList;
}

void Quadtree::DrawQuadTree(RTRShader* shader)
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    glVertex3f(boundary->left, boundary->bottom, 0.0f); // bottom left
    glVertex3f(boundary->right, boundary->bottom, 0.0f); // bottom right
    glVertex3f(boundary->right, boundary->top, 0.0f); // top right 
    glVertex3f(boundary->left, boundary->top, 0.0f); // top left

    glEnd();
    if (divided) {
        northeast->DrawQuadTree(shader);
        northwest->DrawQuadTree(shader);
        southeast->DrawQuadTree(shader);
        southwest->DrawQuadTree(shader);
    }
    glPopMatrix();
}
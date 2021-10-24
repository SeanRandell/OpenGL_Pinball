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
    if (objects.size() < capacity) {
        objects.push_back(object);
        return true;
    }
    // divide quad tree
    else {
        if (!divided) {
            SubDivide();
        }
        // then add to child rectangles
        bool insertIntoNortheast = northeast->Insert(object);
        bool insertIntoNorthwest = northwest->Insert(object);
        bool insertIntoSoutheast = southeast->Insert(object);
        bool insertIntoSouthwest = southwest->Insert(object);

        return(insertIntoNortheast || insertIntoNorthwest || insertIntoSoutheast || insertIntoSouthwest);
    }
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
    Quadtree* currentQuadrant = FindQuadrant(circle);
    if (currentQuadrant == NULL)
    {
        return std::vector<Object*>();
    }

    return currentQuadrant->Query(circle);
}

Quadtree* Quadtree::FindQuadrant(Sphere* circle)
{
    if (circle->IsContainedWithin(northeast->boundary)) {
        return northeast;
    }
    if (circle->IsContainedWithin(northwest->boundary)) {
        return northwest;
    }
    if (circle->IsContainedWithin(southeast->boundary)) {
        return southeast;
    }
    if (circle->IsContainedWithin(southwest->boundary)) {
        return southwest;
    }
}

std::vector<Object*> Quadtree::GetObjectList(Quadtree* currentQuadtree)
{
    std::vector<Object*> resultVector;

    //for each of the objects in this quadtree
    // is it in the bounds of the quadtree passsed in
    // if it is. Add to result vector
    for (Object* currentObject : objects)
    {
        if (currentObject->IsContainedWithin(currentQuadtree->boundary))
        {
            resultVector.push_back(currentObject);
        }
    }

    return resultVector;
}

void Quadtree::DrawQuadTree(RTRShader* shader)
//void Quadtree::DrawQuadTree()
{
    //glm::mat4 modelMatrix = glm::mat4(1.0f);
    //shader->SetMat4("modelMatrixUniform", modelMatrix);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glPushMatrix();
    //float mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    //float mat_colour[] = { 0.5, 0.5, 0.5, 1.0 };
    //float mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //float mat_shininess[] = { 100.0 };
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glColor3d(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    //glNormal3f(0, 0, 1);
    glVertex3f(boundary->left, boundary->bottom, 0.0f); // bottom left
    glVertex3f(boundary->right, boundary->bottom, 0.0f); // bottom right
    glVertex3f(boundary->right, boundary->top, 0.0f); // top right 
    glVertex3f(boundary->left, boundary->top, 0.0f); // top left

    //glVertex3f(-1.0, 1.0f, 0.0f); // top left
    //glVertex3f(1.0f, 1.0f, 0.0f); // top right 
    //glVertex3f(1.0f, -1.0f, 0.0f); // bottom right
    //glVertex3f(-1.0f, -1.0f, 0.0f); // bottom left
    glEnd();
    if (divided) {
        northeast->DrawQuadTree(shader);
        northwest->DrawQuadTree(shader);
        southeast->DrawQuadTree(shader);
        southwest->DrawQuadTree(shader);
        /*       northeast->DrawQuadTree();
               northwest->DrawQuadTree();
               southeast->DrawQuadTree();
               southwest->DrawQuadTree();*/
    }
    glPopMatrix();
}
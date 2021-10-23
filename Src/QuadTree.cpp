#include "QuadTree.h"


Quadtree::Quadtree(Rectangle* boundary, int capacity)
{
    //this->level = 
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
    Rectangle* nw = new Rectangle(x - width / 4, y - height / 4, width / 2, height / 2);
    Rectangle* se = new Rectangle(x + width / 4, y + height / 4, width / 2, height / 2);
    Rectangle* sw = new Rectangle(x - width / 4, y + height / 4, width / 2, height / 2);
    northeast = new Quadtree(ne, capacity);
    northwest = new Quadtree(nw, capacity);
    southeast = new Quadtree(se, capacity);
    southwest = new Quadtree(sw, capacity);

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
        return(
            northeast->Insert(object) ||
            northwest->Insert(object) ||
            southeast->Insert(object) ||
            southwest->Insert(object)
            );
    }
}

std::vector<Object*> Quadtree::Query(Sphere* circle)
{
    if (level > MAX_LEVELS) {
        return objects;
    }
    // for every object in current 

    //split the gameboard into 4

    //find out which quadrant the ball is in
    if (!divided)
    {
        return objects;
    }
    Quadtree* currentQuadrant = FindQuadrant(circle);
    if (currentQuadrant == NULL)
    {
        return std::vector<Object*>();
    }
    // get a list of everything in that quadrant
    //std::vector<Sphere*> objectList = currentQuadrant->objects;
    //currentQuadrant->objects = objectList;
    currentQuadrant->level += 1;

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



//shower 10 - 20 min
// look at code until 11
// go to bread
// wake up in the morning
// have breakfst










//Quadtree::Quadtree(int pLevel, Rectangle pBounds) {
//    level = pLevel;
//    bounds = pBounds;
//}
//
///*
// * Clears the quadtree
// */
//void Quadtree::Clear() {
//    objects.clear();
//
//    for (int i = 0; i < nodes.size(); i++) {
//        if (nodes[i] != nullptr) {
//            nodes[i]->Clear();
//            nodes[i] = nullptr;
//        }
//    }
//}
//
///*
// * Splits the node into 4 subnodes
// */
// void Quadtree::Split() {
//    int subWidth = (int)(bounds.getWidth() / 2);
//    int subHeight = (int)(bounds.getHeight() / 2);
//    int x = (int)bounds.getX();
//    int y = (int)bounds.getY();
//
//    nodes[0] = new Quadtree(level + 1, new Rectangle(x + subWidth, y, subWidth, subHeight));
//    nodes[1] = new Quadtree(level + 1, new Rectangle(x, y, subWidth, subHeight));
//    nodes[2] = new Quadtree(level + 1, new Rectangle(x, y + subHeight, subWidth, subHeight));
//    nodes[3] = new Quadtree(level + 1, new Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
//}
//
///*
// * Determine which node the object belongs to. -1 means
// * object cannot completely fit within a child node and is part
// * of the parent node
// */
//private int getIndex(Rectangle pRect) {
//    int index = -1;
//    double verticalMidpoint = bounds.getX() + (bounds.getWidth() / 2);
//    double horizontalMidpoint = bounds.getY() + (bounds.getHeight() / 2);
//
//    // Object can completely fit within the top quadrants
//    boolean topQuadrant = (pRect.getY() < horizontalMidpoint && pRect.getY() + pRect.getHeight() < horizontalMidpoint);
//    // Object can completely fit within the bottom quadrants
//    boolean bottomQuadrant = (pRect.getY() > horizontalMidpoint);
//
//    // Object can completely fit within the left quadrants
//    if (pRect.getX() < verticalMidpoint && pRect.getX() + pRect.getWidth() < verticalMidpoint) {
//        if (topQuadrant) {
//            index = 1;
//        }
//        else if (bottomQuadrant) {
//            index = 2;
//        }
//    }
//    // Object can completely fit within the right quadrants
//    else if (pRect.getX() > verticalMidpoint) {
//        if (topQuadrant) {
//            index = 0;
//        }
//        else if (bottomQuadrant) {
//            index = 3;
//        }
//    }
//
//    return index;
//}
//
///*
// * Insert the object into the quadtree. If the node
// * exceeds the capacity, it will split and add all
// * objects to their corresponding nodes.
// */
//public void insert(Rectangle pRect) {
//    if (nodes[0] != null) {
//        int index = getIndex(pRect);
//
//        if (index != -1) {
//            nodes[index].insert(pRect);
//
//            return;
//        }
//    }
//
//    objects.add(pRect);
//
//    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
//        if (nodes[0] == null) {
//            split();
//        }
//
//        int i = 0;
//        while (i < objects.size()) {
//            int index = getIndex(objects.get(i));
//            if (index != -1) {
//                nodes[index].insert(objects.remove(i));
//            }
//            else {
//                i++;
//            }
//        }
//    }
//}
//
///*
// * Return all objects that could collide with the given object
// */
//public List retrieve(List returnObjects, Rectangle pRect) {
//    int index = getIndex(pRect);
//    if (index != -1 && nodes[0] != null) {
//        nodes[index].retrieve(returnObjects, pRect);
//    }
//
//    returnObjects.addAll(objects);
//
//    return returnObjects;
//}
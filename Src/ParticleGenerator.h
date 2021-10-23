#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "RTRShader.h"
#include "object.h"
#include "Quad.h"
#include "Sphere.h"
//#include "texture.h"
//#include "game_object.h"


// Represents a single particle and its state
struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    float life;

    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    unsigned int particleTexture;
    // constructor
    ParticleGenerator(unsigned int amount);
    ~ParticleGenerator();
    // update all particles
    void Update(float dt, Sphere& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // render all particles
    void Render(RTRShader* shader);
    void Init();
private:

    // state
    std::vector<Particle> particles;
    unsigned int amount;

    std::string particlePath;

    // render state
    Quad* particleQuad;
    //unsigned int VAO;
    // initializes buffer and vertex attributes

    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int FirstUnusedParticle();
    // respawns particle
    void RespawnParticle(Particle& particle, Sphere& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    unsigned int LoadTexture(std::string* path);

};

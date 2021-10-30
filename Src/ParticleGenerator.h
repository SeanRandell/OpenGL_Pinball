#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "RTRShader.h"
#include "object.h"
#include "Quad.h"
#include "Sphere.h"

// Represents a single particle and its state
struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    float life;

    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
    Particle(glm::vec2 position, float life) : velocity(0.0f), color(1.0f), life(0.0f)
    {
        this->position = position;
        this->life = life;
    }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    unsigned int particleTexture;
    std::vector<Particle*> particles;
    // constructor
    ParticleGenerator(unsigned int amount);
    ~ParticleGenerator();
    // update all particles
    void Update(float dt, Sphere* object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void UpdateAllParticles(float deltaTime);
    // render all particles
    void Render(RTRShader* shader,TextureObject* particleTexture);
    void Init();
private:

    // state
    //std::vector<std::pair<Sphere*,std::vector<Particle>>> particles;

    unsigned int amount;

    std::string particlePath;

    // render state
    Quad* particleQuad;
    //unsigned int VAO;
    // initializes buffer and vertex attributes

    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int FirstUnusedParticle();
    // respawns particle
    void RespawnParticle(Particle* particle, Sphere* object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    //unsigned int LoadTexture(std::string* path);

};

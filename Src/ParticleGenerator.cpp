#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(unsigned int amount)
{
    this->amount = amount;
    particlePath = "../OpenGL_Pinball/Src/particle.png";
    particleTexture = 0;
    this->particleQuad = new Quad();
}

ParticleGenerator::~ParticleGenerator()
{
    particleQuad->End();
    delete particleQuad;

    for (auto particle : particles)
    {
        delete particle;
    }
}

void ParticleGenerator::Update(float deltaTime, Sphere* object, unsigned int newParticles, glm::vec2 offset)
{
    // add new particles if sphere has enough velocity
    if (sqrt((object->velocity.x * object->velocity.x) + (object->velocity.y * object->velocity.y)) >= 15.0f)
    {
        for (unsigned int i = 0; i < newParticles; ++i)
        {
            int unusedParticle = this->FirstUnusedParticle();
            this->RespawnParticle(this->particles[unusedParticle], object, offset);
        }
    }
    UpdateAllParticles(deltaTime);
}

void ParticleGenerator::UpdateAllParticles(float deltaTime)
{
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle* currentParticle = this->particles[i];
        currentParticle->life -= deltaTime; // reduce life
        if (currentParticle->life > 0.0f)
        {	// particle is alive, thus update
            currentParticle->position -= currentParticle->velocity * deltaTime;
            if (currentParticle->color.a > 0)
            {
                currentParticle->color.a -= deltaTime * 2.5f;
                if (currentParticle->color.a < 0)
                {
                    currentParticle->color.a = 0;
                }
            }
        }
    }
}

void ParticleGenerator::Init()
{
    particleQuad->Init();

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        particles.push_back(new Particle());
    }
}

// render all particles
void ParticleGenerator::Render(ShaderObject* shader, TextureObject* particleTexture)
{
    // use additive blending to give it a 'glow' effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (Particle* particle : this->particles)
    {
        if (particle->life > 0.0f)
        {
            shader->SetVec2("offset", particle->position);
            shader->SetVec4("color", particle->color);
            particleTexture->Bind();
            particleQuad->Render(shader);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
}

unsigned int ParticleGenerator::FirstUnusedParticle()
{
    // stores the index of the last particle used (for quick access to next dead particle)
    unsigned int lastUsedParticle = 0;
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i]->life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i]->life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(Particle* particle, Sphere* object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 200.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle->position = glm::vec2(object->position.x, object->position.y) + random;
    particle->color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle->life = 1.0f;
    particle->velocity = object->velocity * 0.1f;
}
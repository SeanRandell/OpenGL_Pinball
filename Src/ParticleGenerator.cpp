#include "ParticleGenerator.h"
//amount = 500

ParticleGenerator::ParticleGenerator(unsigned int amount)
{
    this->amount = amount;
    particlePath = "../Assignment2/Src/particle.png";
    particleTexture = 0;
    this->particleQuad = new Quad();
    //this->init();
}

ParticleGenerator::~ParticleGenerator()
{
    particleQuad->End();
    delete particleQuad;
}

void ParticleGenerator::Update(float deltaTime, Sphere& object, unsigned int newParticles, glm::vec2 offset)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->FirstUnusedParticle();
        this->RespawnParticle(this->particles[unusedParticle], object, offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& currentParticle = this->particles[i];
        currentParticle.life -= deltaTime; // reduce life
        if (currentParticle.life > 0.0f)
        {	// particle is alive, thus update
            currentParticle.position -= currentParticle.velocity * deltaTime;
            currentParticle.color.a -= deltaTime * 2.5f;
        }
    }
}

void ParticleGenerator::Init()
{
    particleTexture = LoadTexture(&particlePath);

    // set up mesh and attribute properties

    particleQuad->Init();

    //unsigned int VBO;
    //float particle_quad[] = {
    //    0.0f, 1.0f, 0.0f, 1.0f,
    //    1.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 0.0f, 0.0f,

    //    0.0f, 1.0f, 0.0f, 1.0f,
    //    1.0f, 1.0f, 1.0f, 1.0f,
    //    1.0f, 0.0f, 1.0f, 0.0f
    //};
    //glGenVertexArrays(1, &this->VAO);
    //glGenBuffers(1, &VBO);
    //glBindVertexArray(this->VAO);
    //// fill mesh buffer
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    //// set mesh attributes
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// render all particles
void ParticleGenerator::Render(RTRShader* shader)
{
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader->Use();
    for (Particle particle : this->particles)
    {
        if (particle.life > 0.0f)
        {
            shader->SetVec2("offset", particle.position);
            shader->SetVec4("color", particle.color);
            glBindTexture(GL_TEXTURE_2D, particleTexture);
            //glBindVertexArray(this->VAO);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            //glBindVertexArray(0);
            particleQuad->Render(shader);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



unsigned int ParticleGenerator::FirstUnusedParticle()
{
    // stores the index of the last particle used (for quick access to next dead particle)
    unsigned int lastUsedParticle = 0;
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, Sphere& object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.position = glm::vec2(object.position.x, object.position.y) + random + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.life = 1.0f;
    particle.velocity = object.velocity * 0.1f;
}

unsigned int ParticleGenerator::LoadTexture(std::string* path) {

    //std::cout << testString << std::endl;
    unsigned int textureID2 = 0;
    glGenTextures(1, &textureID2);

    // load image, create texture and generate mipmaps
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path->c_str(), &width, &height, &nrChannels, 0);
    //unsigned char* data = stbi_load(skyBoxFaces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID2);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
    }

    return textureID2;
}
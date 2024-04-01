#include "RTRShader.h"

RTRShader::RTRShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode) {
    unsigned int vertex = CreateShader(GL_VERTEX_SHADER, vertexCode, "VERTEX");

    unsigned int fragment = CreateShader(GL_FRAGMENT_SHADER, fragmentCode, "FRAGMENT");

    unsigned int geometry;
    if (geometryCode != nullptr)
    {
        geometry = CreateShader(GL_GEOMETRY_SHADER, geometryCode, "GEOMETRY");
    }

    // shader Program
    if (geometryCode != nullptr) {
        CreateProgram(vertex, fragment, geometry);
    }
    else {
        CreateProgram(vertex, fragment);
    }
}

unsigned int RTRShader::CreateShader(int ShaderType, const char* shaderCode, const char* ShaderName) {
    unsigned int returnShader = glCreateShader(ShaderType);
    glShaderSource(returnShader, 1, &shaderCode, NULL);
    glCompileShader(returnShader);
    CheckCompileErrors(returnShader, ShaderName);

    return returnShader;
}

void RTRShader::CreateProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int geometryShader)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    if (geometryShader != 0) {
        glAttachShader(ID, geometryShader);
    }
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != 0) {
        {
            glDeleteShader(geometryShader);
        }
    }
}

RTRShader::~RTRShader() {
    if (ID != 0) glDeleteProgram(ID);
}

// activate the shader
// ------------------------------------------------------------------------
void RTRShader::Use()
{
    glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void RTRShader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void RTRShader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void RTRShader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// ------------------------------------------------------------------------
void RTRShader::SetVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void RTRShader::SetVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void RTRShader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void RTRShader::SetVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void RTRShader::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void RTRShader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void RTRShader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void RTRShader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void RTRShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void RTRShader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
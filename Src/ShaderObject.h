#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Camera.h"
#include "Lighting.h"

#define MAX_UNIFORM_NAME 128

class ShaderObject
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    ShaderObject(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);
    ~ShaderObject();

    unsigned int CreateShader(int ShaderType, const char* shaderCode, const char* ShaderName);
    // TODO - create define for zero
    void CreateProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int geometryShader = 0);
    // use/activate the shader
    void Use();
    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;

    void SetVec2(const std::string& name, const glm::vec2& value) const;

    void SetVec2(const std::string& name, float x, float y) const;

    void SetVec3(const std::string& name, const glm::vec3& value) const;

    void SetVec3(const std::string& name, float x, float y, float z) const;

    void SetVec4(const std::string& name, const glm::vec4& value) const;

    void SetVec4(const std::string& name, float x, float y, float z, float w) const;

    void SetMat2(const std::string& name, const glm::mat2& matrix) const;

    void SetMat3(const std::string& name, const glm::mat3& matrix) const;

    void SetMat4(const std::string& name, const glm::mat4& matrix) const;

    void SetArrayBool(const char* arrayName, int arrayIndex, bool value) { SetBool(MakeArrayName(arrayName, arrayIndex), value); }
    void SetArrayInt(const char* arrayName, int arrayIndex, int value) { SetInt(MakeArrayName(arrayName, arrayIndex), value); }
    void SetArrayFloat(const char* arrayName, int arrayIndex, float value) { SetFloat(MakeArrayName(arrayName, arrayIndex), value); }
    void SetArrayVec2(const char* arrayName, int arrayIndex, const glm::vec2& value) { SetVec2(MakeArrayName(arrayName, arrayIndex), value); }
    void SetArrayVec2(const char* arrayName, int arrayIndex, float x, float y) { SetVec2(MakeArrayName(arrayName, arrayIndex), x, y); }
    void SetArrayVec3(const char* arrayName, int arrayIndex, const glm::vec3& value) { SetVec3(MakeArrayName(arrayName, arrayIndex), value); }
    void SetArrayVec3(const char* arrayName, int arrayIndex, float x, float y, float z) { SetVec3(MakeArrayName(arrayName, arrayIndex), x, y, z); }
    void SetArrayVec4(const char* arrayName, int arrayIndex, const glm::vec4& value) { SetVec4(MakeArrayName(arrayName, arrayIndex), value); }
    void SetArrayVec4(const char* arrayName, int arrayIndex, float x, float y, float z, float w) { SetVec4(MakeArrayName(arrayName, arrayIndex), x, y, z, w); }
    void SetArrayMat2(const char* arrayName, int arrayIndex, const glm::mat2& matrix) { SetMat2(MakeArrayName(arrayName, arrayIndex), matrix); }
    void SetArrayMat3(const char* arrayName, int arrayIndex, const glm::mat3& matrix) { SetMat3(MakeArrayName(arrayName, arrayIndex), matrix); }
    void SetArrayMat4(const char* arrayName, int arrayIndex, const glm::mat4& matrix) { SetMat4(MakeArrayName(arrayName, arrayIndex), matrix); }

    void SetArrayStructBool(const char* arrayName, int arrayIndex, const char* ElementName, bool value) { SetBool(MakeArrayStructName(arrayName, arrayIndex, ElementName), value); }
    void SetArrayStructInt(const char* arrayName, int arrayIndex, const char* ElementName, int value) { SetInt(MakeArrayStructName(arrayName, arrayIndex, ElementName), value); }
    void SetArrayStructFloat(const char* arrayName, int arrayIndex, const char* ElementName, float value) { SetFloat(MakeArrayStructName(arrayName, arrayIndex, ElementName), value); }
    void SetArrayStructVec2(const char* arrayName, int arrayIndex, const char* ElementName, const glm::vec2& value) { SetVec2(MakeArrayStructName(arrayName, arrayIndex, ElementName), value); }
    void SetArrayStructVec2(const char* arrayName, int arrayIndex, const char* ElementName, float x, float y) { SetVec2(MakeArrayStructName(arrayName, arrayIndex, ElementName), x, y); }
    void SetArrayStructVec3(const char* arrayName, int arrayIndex, const char* ElementName, const glm::vec3& value) { SetVec3(MakeArrayStructName(arrayName, arrayIndex, ElementName), value); }
    void SetArrayStructVec3(const char* arrayName, int arrayIndex, const char* ElementName, float x, float y, float z) { SetVec3(MakeArrayStructName(arrayName, arrayIndex, ElementName), x, y, z); }
    void SetArrayStructVec4(const char* arrayName, int arrayIndex, const char* ElementName, const glm::vec4& value) { SetVec4(MakeArrayStructName(arrayName, arrayIndex, ElementName), value); }
    void SetArrayStructVec4(const char* arrayName, int arrayIndex, const char* ElementName, float x, float y, float z, float w) { SetVec4(MakeArrayStructName(arrayName, arrayIndex, ElementName), x, y, z, w); }
    void SetArrayStructMat2(const char* arrayName, int arrayIndex, const char* ElementName, const glm::mat2& matrix) { SetMat2(MakeArrayStructName(arrayName, arrayIndex, ElementName), matrix); }
    void SetArrayStructMat3(const char* arrayName, int arrayIndex, const char* ElementName, const glm::mat3& matrix) { SetMat3(MakeArrayStructName(arrayName, arrayIndex, ElementName), matrix); }
    void SetArrayStructMat4(const char* arrayName, int arrayIndex, const char* ElementName, const glm::mat4& matrix) { SetMat4(MakeArrayStructName(arrayName, arrayIndex, ElementName), matrix); }

    void SetMaterial(const char* name, ObjectMaterial& material) {
        SetVec3(MakeStructName(name, "ambient"), material.ambient);
        SetVec3(MakeStructName(name, "diffuse"), material.diffuse);
        SetVec3(MakeStructName(name, "specular"), material.specular);
        SetFloat(MakeStructName(name, "shininess"), material.shininess);
    }

    void SetSpecularMap(const char* name, SpecularMaterial& material) {
        SetInt(MakeStructName(name, "diffuse"), material.diffuse);
        SetInt(MakeStructName(name, "specular"), material.specular);
        SetFloat(MakeStructName(name, "shininess"), material.shininess);
    }

    void SetArrayMaterial(const char* name, int arrayIndex, ObjectMaterial& material) {
        char full_name[MAX_UNIFORM_NAME + 1];
        sprintf_s(full_name, MAX_UNIFORM_NAME, "%s[%d].", name, arrayIndex);
        SetVec3(MakeStructName(full_name, "ambient"), material.ambient);
        SetVec3(MakeStructName(full_name, "diffuse"), material.diffuse);
        SetVec3(MakeStructName(full_name, "specular"), material.specular);
        SetFloat(MakeStructName(full_name, "shininess"), material.shininess);
    }

    void SetLight(const char* name, Light& light) {
        SetInt(MakeStructName(name, "type"), (int)light.type);
        SetVec3(MakeStructName(name, "ambient"), light.ambient);
        SetVec3(MakeStructName(name, "diffuse"), light.diffuse);
        SetVec3(MakeStructName(name, "specular"), light.specular);
        SetVec3(MakeStructName(name, "direction"), light.direction);
        SetVec3(MakeStructName(name, "position"), light.position);
        SetFloat(MakeStructName(name, "constant"), light.constant);
        SetFloat(MakeStructName(name, "linear"), light.linear);
        SetFloat(MakeStructName(name, "quadratic"), light.quadratic);
        SetBool(MakeStructName(name, "isLightOn"), light.isLightOn);
        //SetFloat(MakeStructName(name, "CutOff"), light.cutoff);
        //SetFloat(MakeStructName(name, "OuterCutoff"), light.outerCutoff);
    }

    void SetArrayLight(const char* name, int arrayIndex, Light& light) {
        char full_name[MAX_UNIFORM_NAME + 1];
        sprintf_s(full_name, MAX_UNIFORM_NAME, "%s[%d]", name, arrayIndex);
        SetInt(MakeStructName(full_name, "type"), (int)light.type);
        SetVec3(MakeStructName(full_name, "ambient"), light.ambient);
        SetVec3(MakeStructName(full_name, "diffuse"), light.diffuse);
        SetVec3(MakeStructName(full_name, "specular"), light.specular);
        SetVec3(MakeStructName(full_name, "direction"), light.direction);
        SetVec3(MakeStructName(full_name, "position"), light.position);
        SetFloat(MakeStructName(full_name, "constant"), light.constant);
        SetFloat(MakeStructName(full_name, "linear"), light.linear);
        SetFloat(MakeStructName(full_name, "quadratic"), light.quadratic);
        SetBool(MakeStructName(full_name, "isLightOn"), light.isLightOn);
        //SetFloat(MakeStructName(full_name, "CutOff"), light.cutoff);
        //SetFloat(MakeStructName(full_name, "OuterCutoff"), light.OuterCutoff);
    }

    void SetLightingModel(LightingModel& lightModel) {
        SetInt("numberOfLightsUniform", lightModel.GetNumberOfLights());
        for (int i = 0; i < lightModel.GetNumberOfLights(); i++) {
            Light* light = lightModel.GetLight(i);
            SetArrayLight("lightsUniform", i, *light);
        }
    }

    void SetCamera(const char* name, Camera& camera) {
        SetVec3(MakeStructName(name, "Position"), camera.position);
        SetVec3(MakeStructName(name, "Front"), camera.front);
        SetVec3(MakeStructName(name, "Up"), camera.worldUp);
        SetVec3(MakeStructName(name, "Right"), camera.right);
    }

private:
    void CheckCompileErrors(unsigned int shader, std::string type);
    char uniformName[MAX_UNIFORM_NAME + 1]{ 0 };
    char* MakeArrayName(const char* arrayName, int arrayIndex) {
        sprintf_s(uniformName, MAX_UNIFORM_NAME, "%s[%d]", arrayName, arrayIndex);
        return uniformName;
    }
    char* MakeArrayStructName(const char* arrayName, int arrayIndex, const char* ElementName) {
        sprintf_s(uniformName, MAX_UNIFORM_NAME, "%s[%d].%s", arrayName, arrayIndex, ElementName);
        return uniformName;
    }
    char* MakeStructName(const char* structName, const char* ElementName) {
        sprintf_s(uniformName, MAX_UNIFORM_NAME, "%s.%s", structName, ElementName);
        return uniformName;
    }
};
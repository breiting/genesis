#include <genesis/view/Shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "assets/agent_frag.h"
#include "assets/agent_vert.h"
#include "assets/canvas_frag.h"
#include "assets/canvas_vert.h"
#include "assets/target_frag.h"
#include "assets/target_vert.h"

namespace gen {

Shader::~Shader() {
    if (m_Id) glDeleteProgram(m_Id);
}

void Shader::CompileShader(const std::string& vertexCode, const std::string& fragmentCode) {
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "Error compiling vertex shader: " << infoLog << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "Error compiling fragment shader: " << infoLog << std::endl;
    }

    // Shader-Programm creation
    m_Id = glCreateProgram();
    glAttachShader(m_Id, vertex);
    glAttachShader(m_Id, fragment);
    glLinkProgram(m_Id);
    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_Id, 512, nullptr, infoLog);
        std::cerr << "Error binding shader: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Bind() const {
    glUseProgram(m_Id);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& matrix) const {
    glUniformMatrix3fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
}

void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
}

unsigned int Shader::GetInt(const std::string& name) const {
    return glGetUniformLocation(m_Id, name.c_str());
}

void Shader::BuildAgent() {
    CompileShader(agent_vert_glsl, agent_frag_glsl);
}

void Shader::BuildTarget() {
    CompileShader(target_vert_glsl, target_frag_glsl);
}

void Shader::BuildCanvas() {
    CompileShader(canvas_vert_glsl, canvas_frag_glsl);
}

}  // namespace gen

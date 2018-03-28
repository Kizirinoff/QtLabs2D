#pragma once

#include "OpenGL_raii.h"
#include <string>
#include <vector>

namespace glcore
{

// ����� ������� glCompileShader
// ����������� ������ ��������� ���� �� ������ � �������� �����
// @param type - ��� GL_VERTEX_SHADER, GL_FRAGMENT_SHADER ��� GL_GEOMETRY_SHADER.
ShaderObject compileShader(gl::GLenum type, const std::string &source);

// ����� ������� glLinkProgram
// ��������� ���������� �������� � ������ ���������
ProgramObject linkProgram(const std::vector<ShaderObject> &shaders);

// ����� ������� glValidateProgram
// ��������� - �������������� ����, ������� ����� ��������
// � ��������� ������������������ ��� ��������������� ����������� GLSL
// ���� �������� ����, ��� ��������� � ����� out.
void validateProgram(const ProgramObject &program, std::ostream &out);

} // namespace glcore

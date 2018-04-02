#pragma once

#include "OpenGL_raii.h"
#include <string>
#include <vector>

namespace glcore
{

// ����� ������� glGenVertexArrays
// ������ VAO, �������� ����� ������� ������ � ��������� ������.
VAO createVAO();

// ����� ������� glGenBuffers
// @param target - ��� GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER ��� GL_UNIFORM_BUFFER
VBO createVBO();

// ����� ������� glGenBuffers � glBufferData
// ������� �� ���������� �������� �����
// @param target - ��� GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER ��� GL_UNIFORM_BUFFER
VBO createStaticVBO(gl::GLenum target, const void *bytes, const size_t byteCount);

// ����� ������� glBufferData
// ������� �� ���������� �������� �����
// @param target - ��� GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER ��� GL_UNIFORM_BUFFER
void setStreamBufferData(gl::GLuint buffer, gl::GLenum target, const void *bytes, const size_t byteCount);

// ��� ������ ������� ��������� ������ � ���� �������. ��������� ��������� ����:
// - T[], ������ � ����� C
// - std::vector<T>
// - std::array<T>
// - std::initializer_list<T>, �.�. ������ �������������
template<class ArrayT>
VBO createStaticVBO(gl::GLenum target, const ArrayT &verticies)
{
	// VertexT - ��� �������� �������
	using VertexT = std::decay_t<decltype(verticies[0])>;

	// ���������� ��������� �� ������ ������� � ��������� �� ����� �������.
	const auto bytes = reinterpret_cast<const void *>(std::data(verticies));
	// ��������� ������ ������� � ������.
	const size_t byteCount = sizeof(VertexT) * std::size(verticies);

	return createStaticVBO(target, bytes, byteCount);
}

// ��� ������ ������� ��������� ������ � ���� �������. ��������� ��������� ����:
// - T[], ������ � ����� C
// - std::vector<T>
// - std::array<T>
// - std::initializer_list<T>, �.�. ������ �������������
template<class ArrayT>
void setStreamBufferData(gl::GLuint buffer, gl::GLenum target, const ArrayT &verticies)
{
	// VertexT - ��� �������� �������
	using VertexT = std::decay_t<decltype(verticies[0])>;

	// ���������� ��������� �� ������ ������� � ��������� �� ����� �������.
	const auto bytes = reinterpret_cast<const void *>(std::data(verticies));
	// ��������� ������ ������� � ������.
	const size_t byteCount = sizeof(VertexT) * std::size(verticies);

	setStreamBufferData(buffer, target, bytes, byteCount);
}

} // namespace glcore

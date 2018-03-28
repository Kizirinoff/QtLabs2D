#pragma once

#include "OpenGL_raii.h"
#include <string>
#include <vector>

namespace glcore
{

// ����� ������� glGenVertexArrays
// ������ VAO, �������� ����� ������� ������ � ��������� ������.
VAO createVAO();

// ����� ������� glGenBuffers � glBufferData
// ������� �� ���������� �������� �����
// @param target - ��� GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER ��� GL_UNIFORM_BUFFER
VBO createStaticVBO(gl::GLenum target, const void *bytes, const size_t byteCount);

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
	const size_t bytesCount = sizeof(VertexT) * std::size(verticies);

	return createStaticVBO(target, bytes, bytesCount);
}

} // namespace glcore

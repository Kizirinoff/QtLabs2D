#include "stdafx.h"

#include "SimpleScene.h"
#include <algorithm>
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>

// ���������� ������� �� gl32core, ���������������� ����������� glbinding.
using namespace gl32core;

namespace
{

// ���� ��������� ������ ��������� ��� ������:
//  1) ������� ���� ������������ �������
//  2) ��������� ��������������� ������������� �������,
//     �� ���� ��������� ������� �� ��������� ������.
const char kVertexShaderCode[] = R"**(#version 110
in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
void main()
{
    v_color = i_color;
    gl_Position = u_projection_matrix * vec4(i_position, 0.0, 1.0);
}
)**";

// ���� ����������� ������ ������������� ��������� ���������� ��
//  ���������� ������� ����.
static const char kFragmentShaderCode[] = R"**(#version 110
in vec4 v_color;
void main()
{
    gl_FragColor = v_color;
}
)**";

} // anonymous namespace

SimpleScene::SimpleScene() = default;

SimpleScene::~SimpleScene() = default;

void SimpleScene::initialize()
{
	glcore::InitGLBinding();
	initializeShaders();

	m_vao = glcore::CreateVAO();
	glBindVertexArray(m_vao);

	// ���������� ������ ������ �������������, �������������� ����,
	//  ������ ����������� ����� ��������� � ����������� ����.
	std::vector<VertexP2C4> verticies = tesselateCircle(50, { 350, 280 }, glm::vec4{ 1, 1, 0, 0 });

	// ���������� ������ ������ �������������, �������������� ������������,
	//  ��������� ��� � ������ ������ �����.
	const std::vector<glm::vec2> convexPoints = {
		{ 100, 200 },
		{ 250, 210 },
		{ 220, 290 },
		{ 130, 300 },
		{ 100, 250 },
	};
	const std::vector<VertexP2C4> convexVerticies = tesselateConvex(convexPoints, glm::vec4{ 0, 1, 0, 0 });
	std::copy(convexVerticies.begin(), convexVerticies.end(), std::back_inserter(verticies));

	// ��������� ������ � ��������� �����.
	m_vbo = glcore::CreateStaticVBO(GL_ARRAY_BUFFER, verticies);

	// ��������� �������� ��������� ������ � ��������� �������� VAO � VBO.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	bindVertexData(verticies);

	m_trianglesCount = verticies.size();
}

void SimpleScene::update(float deltaSeconds)
{
	(void)deltaSeconds;
}

void SimpleScene::redraw(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	glClear(GL_COLOR_BUFFER_BIT);

	// ������������� ������� ���������������� �������������.
	setProjectionMatrix(width, height);

	glDrawArrays(GL_TRIANGLES, 0, m_trianglesCount);
}

void SimpleScene::initializeShaders()
{
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::CompileShader(GL_VERTEX_SHADER, kVertexShaderCode));
	shaders.emplace_back(glcore::CompileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode));
	m_program = glcore::LinkProgram(shaders);
}

void SimpleScene::bindVertexData(const std::vector<VertexP2C4> &verticies)
{
	// OpenGL ������ �������� �������� �������� ����� ������������ ��������� VertexP2C4.
	const void *colorOffset = reinterpret_cast<void *>(offsetof(VertexP2C4, rgba));
	const void *posOffset = reinterpret_cast<void *>(offsetof(VertexP2C4, xy));
	const size_t stride = sizeof(VertexP2C4);

	// ����������� ������� i_color � ������ � ��������� ������.
	const int colorLocation = glGetAttribLocation(m_program, "i_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, glm::vec4().length(), GL_FLOAT, GL_FALSE, stride, colorOffset);

	// ����������� ������� i_position � ������ � ��������� ������.
	const int posLocation = glGetAttribLocation(m_program, "i_position");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, glm::vec2().length(), GL_FLOAT, GL_FALSE, stride, posOffset);
}

void SimpleScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// ��������� ������� ���������������� �������������
	const glm::mat4 mat = glm::ortho(0.f, float(width), float(height), 0.f);

	// ������� ������� ��� ��������� � ����������� ���������
	glUniformMatrix4fv(glGetUniformLocation(m_program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

#include "stdafx.h"
#include "AnimatedScene.h"
#include <algorithm>
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <libplatform/ResourceLoader.h>

namespace
{
glm::vec2 animateMoveAlongY(const glm::vec2 &point, float phase)
{
	constexpr float kAmplitudePx = 50;
	const float deviation = std::abs(0.5f - phase) / 0.5f;
	return {
		point.x,
		point.y + kAmplitudePx * deviation
	};
}
} // namespace

// ���������� ������� �� gl32core, ���������������� ����������� glbinding.
using namespace gl32core;

AnimatedScene::AnimatedScene() = default;

AnimatedScene::~AnimatedScene() = default;

void AnimatedScene::initialize()
{
	glcore::initGLBinding();
	initializeShaders();

	m_vao = glcore::createVAO();
	glBindVertexArray(m_vao);

	// ��������� ������ � ��������� �����.
	m_vbo = glcore::createVBO();
}

void AnimatedScene::update(float deltaSeconds)
{
	m_totalTime += deltaSeconds;
}

void AnimatedScene::redraw(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	// ���������� ������ ������ �������������, �������������� �������� ����.
	std::vector<VertexP2C4> verticies = tesselatePolarRose(100.0f, 7, glm::vec2{ 0, 0 }, glm::vec4{ 0.72, 0.2, 1, 0 });
	animateShape(verticies);

	// ��������� ������� �� ����������
	glcore::setStreamBufferData(m_vbo, GL_ARRAY_BUFFER, verticies);

	// ��������� �������� ��������� ������ � ��������� �������� VAO � VBO.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	bindVertexData(verticies);

	// ���������� ����� ����������.
	m_trianglesCount = verticies.size();

	glClear(GL_COLOR_BUFFER_BIT);

	// ������������� ������� ���������������� �������������.
	setProjectionMatrix(width, height);

	glDrawArrays(GL_TRIANGLES, 0, m_trianglesCount);
}

bool AnimatedScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	using platform::Key;

	switch (event.getKey())
	{
	case Key::Equal:
		// TODO: zoom camera in.
		break;
	case Key::Minus:
		// TODO: zoom camera out.
		break;
	case Key::Left:
		// TODO: move camera left.
		break;
	case Key::Right:
		// TODO: move camera right.
		break;
	case Key::Up:
		// TODO: move camera up.
		break;
	case Key::Down:
		// TODO: move camera down.
		break;
	}
	return false;
}

void AnimatedScene::initializeShaders()
{
	platform::ResourceLoader loader;

	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("draw2d.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("draw2d.frag")));
	m_program = glcore::linkProgram(shaders);
}

void AnimatedScene::bindVertexData(const std::vector<VertexP2C4> &verticies)
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

void AnimatedScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// ��������� ������� ���������������� �������������
	const glm::mat4 mat = glm::ortho(-0.5f * float(width), 0.5f * float(width), -0.5f * float(height), 0.5f * float(height));

	// ������� ������� ��� ��������� � ����������� ���������
	glUniformMatrix4fv(glGetUniformLocation(m_program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

void AnimatedScene::animateShape(std::vector<VertexP2C4> &verticies)
{
	constexpr float kAnimationPeriodSec = 2.0;
	for (auto &v : verticies)
	{
		const float phase = std::fmod(m_totalTime, kAnimationPeriodSec) / kAnimationPeriodSec;
		assert(phase >= 0.f && phase <= 1.f);
		v.xy = animate(v.xy, phase);
	}
}

glm::vec2 AnimatedScene::animate(const glm::vec2 &point, float phase)
{
	return animateMoveAlongY(point, phase);
}

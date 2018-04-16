#include "SimpleScene.h"
#include "RandomColorGenerator.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>

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

constexpr float PI = 3.1415926f;

glm::vec2 euclidean(float radius, float angleRadians)
{
	return {
		radius * sin(angleRadians),
		radius * cos(angleRadians)
	};
}

// ���������� ������ ������ ������������� ��� ��������� ��������������, ��������� ��������� � �������.
//  @param center - �������������� ����� ��������������
//  @param hullPoints - ������� ��������������
//  @param colorGen - ��������� ����� ���������� �������������
std::vector<VertexP2C4> tesselateConvexByCenter(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints, RandomColorGenerator &colorGen)
{
	const size_t size = hullPoints.size();
	std::vector<VertexP2C4> verticies;
	verticies.reserve(3u * size);
	for (size_t pointIndex = 0; pointIndex < size; ++pointIndex)
	{
		// ���������� ���� ������������ � ��������� ��� ��� ������� � ������.
		const glm::vec4 triangleColor = colorGen.GenerateColor();
		const size_t nextPointIndex = (pointIndex + 1) % size;
		verticies.push_back({ hullPoints.at(pointIndex), triangleColor });
		verticies.push_back({ hullPoints.at(nextPointIndex), triangleColor });
		verticies.push_back({ center, triangleColor });
	}

	return verticies;
}

// ���������� ������ ������ ������������� ��� ��������� ��������������, ��������� ���������.
std::vector<VertexP2C4> tesselateConvex(const std::vector<glm::vec2> &verticies, RandomColorGenerator &colorGen)
{
	// ����� ��������� �������������� - ��� ������� �������������� ��� ������
	const glm::vec2 center = std::accumulate(verticies.begin(), verticies.end(), glm::vec2()) / float(verticies.size());
	return tesselateConvexByCenter(center, verticies, colorGen);
}

// ������� ����� ���� �� ������������,
//  ���������� ������ � ��������� �������������.
std::vector<VertexP2C4> tesselateCircle(float radius, const glm::vec2 &center, RandomColorGenerator &colorGen)
{
	assert(radius > 0);

	// ���� ���������������� � ������� �������������.
	// ������� ������� ������� ������������ ����� ����� 2.
	constexpr float step = 2;
	// ����� ������������� ����� ����� ����������, ������� �� ��� �� ����������.
	const auto pointCount = static_cast<unsigned>(radius * 2 * PI / step);

	// ��������� �����-����������� �� ����������.
	std::vector<glm::vec2> points(pointCount);
	for (unsigned pi = 0; pi < pointCount; ++pi)
	{
		const auto angleRadians = static_cast<float>(2.f * PI * pi / pointCount);
		points[pi] = center + euclidean(radius, angleRadians);
	}

	return tesselateConvexByCenter(center, points, colorGen);
}

} // anonymous namespace

SimpleScene::SimpleScene()
{
}

SimpleScene::~SimpleScene()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vbo);
}

void SimpleScene::initialize()
{
	QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();

	initializeShaders();

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindVertexArray(m_vao);

	// ���������� ������ ������ �������������, �������������� ����,
	//  ������ ����������� ����� ��������� � ����������� ����.
	RandomColorGenerator colorGen;
	std::vector<VertexP2C4> verticies = tesselateCircle(50, { 350, 280 }, colorGen);

	// ���������� ������ ������ �������������, �������������� ������������,
	//  ��������� ��� � ������ ������ �����.
	const std::vector<glm::vec2> convexPoints = {
		{ 100, 200 },
	{ 250, 210 },
	{ 220, 290 },
	{ 130, 300 },
	{ 100, 250 },
	};
	const std::vector<VertexP2C4> convexVerticies = tesselateConvex(convexPoints, colorGen);
	std::copy(convexVerticies.begin(), convexVerticies.end(), std::back_inserter(verticies));

	// ��������� �������� ��������� ������ � ��������� �������� VAO.
	bindVertexData(verticies);

	m_vertexCount = verticies.size();
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

	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

void SimpleScene::initializeShaders()
{
	m_vertexShader = compileShader(GL_VERTEX_SHADER, kVertexShaderCode);
	m_fragmentShader = compileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode);
	m_program = linkProgram({ m_vertexShader, m_fragmentShader });
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

	// ��������� ������ � ��������� �����.
	glBufferData(GL_ARRAY_BUFFER, stride * verticies.size(), verticies.data(), GL_STATIC_DRAW);
}

void SimpleScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// ��������� ������� ���������������� �������������
	const glm::mat4 mat = glm::ortho(0.f, float(width), float(height), 0.f);

	// ������� ������� ��� ��������� � ����������� ���������
	glUniformMatrix4fv(glGetUniformLocation(m_program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint SimpleScene::compileShader(GLenum type, const std::string & source)
{
	// �������� ������ �������
	GLuint shader = glCreateShader(type);

	// ������� �������� ��� ������� �������������
	const auto length = static_cast<int>(source.length());
	const char* sourceLine = source.data();
	glShaderSource(shader, 1, (const GLchar**)&sourceLine, &length);

	// ������ ������������ �������������� ������ � ��������� ������
	glCompileShader(shader);

	GLint ok = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (ok == GL_FALSE)
	{
		// ��� ������� ���� ��� ������, ������� �� ������
		// � � ������ ������� ���� ������ ����� ����.
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		// ���� �����, �������� ������ ������� ������� � �������� � �� ���
		std::string log(logLength, '\0');
		GLsizei ignored = 0;
		glGetShaderInfoLog(shader, log.size(), &ignored, (GLchar*)log.data());

		// ������� ����������, ��������� � ���� ���
		throw std::runtime_error("shader compilation failed: " + log);
	}

	return shader;
}

GLuint SimpleScene::linkProgram(const std::vector<GLuint>& shaders)
{
	// ����������� � ������������ ����� ������.
	GLuint obj = glCreateProgram();

	// ����������� ����� ���������������� �������.
	for (GLuint shader : shaders)
	{
		glAttachShader(obj, shader);
	}

	// ������ ������������ ��������� ���������� � ��������� ������.
	glLinkProgram(obj);

	GLint status = 0;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// ��� ������� ���� ��� ������, ������� �� ������
		// � � ������ ������� ���� ������ ����� ����.
		GLint logLength = 0;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &logLength);

		// ���� �����, �������� ������ ������� ������� � �������� � �� ���
		std::string log(logLength, '\0');
		GLsizei ignored = 0;
		glGetProgramInfoLog(obj, log.size(), &ignored, (GLchar*)log.data());

		// ������� ����������, ��������� � ���� ���
		throw std::runtime_error("program linking failed " + log);
	}

	return obj;
}

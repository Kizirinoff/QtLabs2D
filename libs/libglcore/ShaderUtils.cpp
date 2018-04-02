#include "ShaderUtils.h"
#include <glbinding/gl32core/gl.h>

using namespace gl32core;

namespace glcore
{
namespace
{
std::string readShaderInfoLog(GLuint id)
{
	// ��� ������� ���� ��� ������, ������� �� ������
	// � � ������ ������� ���� ������ ����� ����.
	GLint logLength = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

	// ���� �����, �������� ������ ������� ������� � �������� � �� ���
	std::string log(logLength, '\0');
	if (logLength > 0)
	{
		GLsizei ignored = 0;
		glGetShaderInfoLog(id, static_cast<gl::GLsizei>(log.size()), &ignored, &log[0]);
	}

	return log;
}

std::string readProgramInfoLog(GLuint id)
{
	// ��� ������� ���� ��� ������, ������� �� ������
	// � � ������ ������� ���� ������ ����� ����.
	GLint logLength = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

	// ���� �����, �������� ������ ������� ������� � �������� � �� ���
	std::string log(logLength, '\0');
	if (logLength > 0)
	{
		GLsizei ignored = 0;
		glGetProgramInfoLog(id, static_cast<gl::GLsizei>(log.size()), &ignored, &log[0]);
	}

	return log;
}

void checkShaderCompilationStatus(GLuint id)
{
	GLboolean ok = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
	if (ok == GL_FALSE)
	{
		// ������� ����������, ��������� � ���� ���
		const std::string log = readShaderInfoLog(id);
		throw std::runtime_error("shader compilation failed: " + log);
	}
}

void checkProgramLinkingStatus(GLuint id)
{
	GLboolean status = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// ������� ����������, ��������� � ���� ���
		const std::string log = readProgramInfoLog(id);
		throw std::runtime_error("program linking failed " + log);
	}
}
} // anonymous namespace

ShaderObject compileShader(gl::GLenum type, const std::string &source)
{
	// �������� ������ �������
	ShaderObject obj = ShaderObject(glCreateShader(type));

	// ������� �������� ��� ������� �������������
	const auto length = static_cast<int>(source.length());
	const char *sourceLine = source.data();
	glShaderSource(obj.get(), 1, (const GLchar **)&sourceLine, &length);

	// ������ ������������ �������������� ������ � ��������� ������
	glCompileShader(obj.get());
	checkShaderCompilationStatus(obj.get());

	return obj;
}

ProgramObject linkProgram(const std::vector<ShaderObject> &shaders)
{
	// ����������� � ������������ ����� ������.
	ProgramObject obj = ProgramObject(glCreateProgram());

	// ����������� ����� ���������������� �������.
	for (const ShaderObject &shader : shaders)
	{
		glAttachShader(obj.get(), shader.get());
	}

	// ������ ������������ ��������� ���������� � ��������� ������.
	glLinkProgram(obj.get());
	checkProgramLinkingStatus(obj.get());

	// ����������� �������, ��������� ��������� ��� �������.
	for (const ShaderObject &shader : shaders)
	{
		glDetachShader(obj.get(), shader.get());
	}

	return obj;
}

void validateProgram(const ProgramObject &program, std::ostream &out)
{
	// ������ ������������ ��������� ��������� ���������.
	// ������������ ����� ������� �� ������������� ������ ��� ������������� ���.
	glValidateProgram(program.get());

	GLboolean status = 0;
	glGetProgramiv(program.get(), GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// ���������� ��� ��������� � �����
		const std::string log = readProgramInfoLog(program.get());
		out << log;
	}
}

} // namespace glcore

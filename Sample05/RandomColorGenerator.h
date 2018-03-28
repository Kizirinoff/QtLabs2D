#pragma once
#include <glm/vec4.hpp>
#include <vector>
#include <random>

class RandomColorGenerator
{
public:
	RandomColorGenerator();

	glm::vec4 GenerateColor();

private:
	// ������� ������ ��� ������.
	std::vector<glm::vec4> m_palette;

	// random_device - ��� �������� ��������������� ��������������� �����.
	std::random_device m_rd;

	// mt19937 - ��� "����� ��������", �� ���������� ���� ��������������� �����,
	//  ��������� � ��������� �� ���������� ���������.
	std::mt19937 m_generator;

	// � �������� ��������� ������������� ����������� ���������
	//  ������� �� ����� ���������.
	std::uniform_int_distribution<size_t> m_indexDist;
};
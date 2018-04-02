#pragma once

namespace platform
{

// ���� ������� ������.
enum class Key
{
	Unknown,
	Equal,
	Plus,
	Minus,
	Left,
	Right,
	Up,
	Down,
};

// ��������� ��� ��������� ������� � ������� ����������
class IKeyEvent
{
public:
	virtual ~IKeyEvent() = default;
	virtual Key getKey() const = 0;
};

// ��������� ��������� ��������� �������. ������������� ���������� ������ ������� true, ���� ������� ����������.
class IInputListener
{
public:
	virtual ~IInputListener() = default;

	virtual bool keyPressEvent(IKeyEvent &event) { return false; };
	virtual bool keyReleaseEvent(IKeyEvent &event) { return false; };
};

} // namespace platform

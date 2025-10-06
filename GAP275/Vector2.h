#pragma once

#include <iostream>

template<typename Type>
struct Vector2
{
	Type m_x;
	Type m_y;

	constexpr Vector2 operator*(const Type& right) { return Vector2{ m_x * right, m_y * right }; }
	constexpr Vector2 operator+(const Type& right) const { return Vector2{ m_x + right, m_y + right }; }
	constexpr Vector2 operator+(const Vector2& right) const { return Vector2{ m_x + right.m_x, m_y + right.m_y }; }
	constexpr Vector2 operator-(const Vector2& right) { return Vector2{ m_x - right.m_x, m_y - right.m_y }; }
	constexpr Type GetLength() { return m_x * m_x + m_y * m_y; }
	constexpr Vector2 GetNormalized()
	{
		float length = sqrtf(GetLength());

		if (length == 0)
			return Vector2{ 0, 0 };
		else
			return Vector2{ m_x / length, m_y / length };
	}

	constexpr Type Magnitude() const
	{
		return static_cast<Type>(std::sqrt(m_x * m_x + m_y * m_y));
	}
};
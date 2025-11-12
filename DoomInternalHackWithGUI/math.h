#pragma once


#include <string>
#include <unordered_map>
#include <iostream>
#include "dependencies/imgui/imgui.h"
#include "globals.h"

#define M_PI 3.14159265358979323846

struct Vector2
{
	float x, y;

	Vector2(float x = 0, float y = 0) : x(x), y(y) {}
	Vector2(ImVec2 vec) : x(vec.x), y(vec.y) {}

	static Vector2 lerp(const Vector2& from, const Vector2& to, float t) {
		return {
			from.x + (to.x - from.x) * t,
			from.y + (to.y - from.y) * t
		};
	}

	float Magnitude() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vector2 Normalize() const
	{
		float mag = Magnitude();
		return Vector2(x / mag, y / mag);
	}

	static float Distance(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 operator*(const Vector2& other) const {
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 operator/(const Vector2& other) const {
		return Vector2(x / other.x, y / other.y);
	}

	Vector2 operator/(float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}


	operator ImVec2() const {
		return ImVec2(x, y);
	}
};

struct Vector3
{
	float x, y, z;
	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	bool operator!=(const Vector3& rhs) const
	{
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
	}

	bool operator==(const Vector3& rhs) const
	{
		return !((x != rhs.x) || (y != rhs.y) || (z != rhs.z));
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator/(float scalar) const
	{
		return { x / scalar, y / scalar, z / scalar };
	}

	bool isNearZero() {
		return
			(x < 0.001 && x > 0) || (x < 0 && x > -0.001) ||
			(y < 0.001 && y > 0) || (y < 0 && y > -0.001) ||
			(z < 0.001 && z > 0) || (z < 0 && z > -0.001);
	}

	static float Distance(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	operator Vector2() const
	{
		return Vector2(x, y);
	}

	operator ImVec2() const {
		return ImVec2(x, y);
	}
};

struct Vector4 {
	float x, y, z, w;
};

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];

	void transpose() {
		for (int i = 0; i < 4; ++i) {
			for (int j = i + 1; j < 4; ++j) {
				std::swap(matrix[i][j], matrix[j][i]);
			}
		}
	}
};


inline bool worldToScreenPoint(Vector3 worldPoint, view_matrix_t viewProjMtx, Vector3& screenPoint) {
	Vector4 clipPoint;
	clipPoint.x = viewProjMtx.matrix[0][0] * worldPoint.x + viewProjMtx.matrix[0][1] * worldPoint.y + viewProjMtx.matrix[0][2] * worldPoint.z + viewProjMtx.matrix[0][3];
	clipPoint.y = viewProjMtx.matrix[1][0] * worldPoint.x + viewProjMtx.matrix[1][1] * worldPoint.y + viewProjMtx.matrix[1][2] * worldPoint.z + viewProjMtx.matrix[1][3];
	clipPoint.z = viewProjMtx.matrix[2][0] * worldPoint.x + viewProjMtx.matrix[2][1] * worldPoint.y + viewProjMtx.matrix[2][2] * worldPoint.z + viewProjMtx.matrix[2][3];
	clipPoint.w = viewProjMtx.matrix[3][0] * worldPoint.x + viewProjMtx.matrix[3][1] * worldPoint.y + viewProjMtx.matrix[3][2] * worldPoint.z + viewProjMtx.matrix[3][3];
	if (clipPoint.w < 3) {
		return false;
	}
	else {
		float invW = 1.0f / clipPoint.w;
		screenPoint.x = (clipPoint.x * invW + 1.0f) * 0.5f * clientWidth;
		screenPoint.y = clientHeight - (clipPoint.y * invW + 1.0f) * 0.5f * clientHeight;
		screenPoint.z = clipPoint.w;
		return true;
	}
}
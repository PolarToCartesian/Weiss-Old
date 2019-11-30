#pragma once

#include "Camera.h"
#include "../misc/includes.h"

class OrthographicCamera : public Camera
{
private:
	float m_InvAspectRatio = 0.0f;

public:
	OrthographicCamera(const Vec2f& position, Window& window, const float ratation);

	void rotate(const float angle);

	void setPosition(const Vec2f& v);

	void setRotation(const Vec2f& v);

	void translate(const Vec2f& v);

	virtual void calculateTransform() override;

	void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char left, const char right, const char up, const char down);
};
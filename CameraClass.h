#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class CameraClass
{
public: 
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void MoveForward(float amount);
	void MoveBackward(float amount);
	void MoveRight(float amount);
	void MoveLeft(float amount);


	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMFLOAT3 m_forward;
	XMFLOAT3 m_right;
	XMMATRIX m_viewMatrix;
};
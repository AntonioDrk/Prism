#pragma once
class PointLight
{
public:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 attenuation;
	float range;

	PointLight();
	~PointLight();

private:


};
#pragma once

#include <directxmath.h>
#include <string>

class Light
{
private:
	DirectX::XMFLOAT4X4 mTransform;
	DirectX::XMFLOAT4 mTranslation;
	DirectX::XMFLOAT4 mWorldPos;
	DirectX::XMFLOAT4 mColour;
	std::string mName;

public:
	Light();
	Light(const DirectX::XMFLOAT4X4& entityTransform, const DirectX::XMFLOAT4& lightPos, const DirectX::XMFLOAT4& lightColour, const std::string& name);
	~Light();

	const DirectX::XMFLOAT4& Position() const;
	const DirectX::XMFLOAT4& Colour() const;
	const std::string& Name() const;

	void SetTransform(const DirectX::XMFLOAT4X4& entityTransform);
	void SetColour(const DirectX::XMFLOAT4& colour);
};
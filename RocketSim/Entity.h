#pragma once

#include "Shape.h"
#include "Light.h"
#include <string>
#include <vector>
#include <directxmath.h>

class Entity
{
private:
	std::vector<Shape> mShapes;
	std::vector<Light> mLights;

	DirectX::XMFLOAT4X4 mTransform;
	DirectX::XMFLOAT3 mTranslation;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mScale;
	DirectX::XMFLOAT3 mForward;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mRight;

	std::string mName;
	float mSpeed;
	float mRotationSpeed;

	void UpdateChildren();
	void SetTransform();
	void SetDirection();

public:
	Entity();
	Entity(const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& scale, const std::string& name, const float& speed, const float& rotationSpeed);
	~Entity();

	const std::vector<Shape>& Shapes() const;
	//std::vector<Shape>& Shapes();
	const std::vector<Light>& Lights() const;
	//std::vector<Light>& Lights();
	const DirectX::XMFLOAT3& Up() const;
	const DirectX::XMFLOAT3& Right() const;
	const DirectX::XMFLOAT3& Forward() const;
	const DirectX::XMFLOAT3& Position() const;
	const DirectX::XMFLOAT4X4& Transform() const;
	const std::string& Name() const;
	const float& Speed() const;
	const float& RotationSpeed() const;

	void AddShape(const GeometryType& geometryType, const bool& skyBox, const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& scale, 
		const std::wstring& diffuseMap, const std::wstring& normalMap, const std::wstring& displacementMap, const std::wstring& shader, const std::string &name, 
		const std::vector<InstanceData>* const instanceData, const bool& particleSystem);
	void AddLight(const DirectX::XMFLOAT4& translation, const DirectX::XMFLOAT4& colour, const std::string& name);
	void MoveEntity(const DirectX::XMFLOAT3& translation, const float& timeScale, const float& dt);
	void RotateEntity(const DirectX::XMFLOAT3& rotation, const float& timeScale, const float& dt);
	void RotateAroundPoint(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& rotation, const float& timeScale, const float& dt);
	void SetPosition(const DirectX::XMFLOAT3& position);
	void ResetRotation();
	void SetSpeed(const float& speed);
	void SetRotationSpeed(const float& speed);
	void RemoveInstancesFromShape(const std::string& name, const std::vector<InstanceData>& instances);
	void ChangeLightColour(const std::string& name, const DirectX::XMFLOAT4& colour);
};

bool operator==(const Entity& a, const Entity& b);

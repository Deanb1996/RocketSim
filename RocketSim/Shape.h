#pragma once

#include <string>
#include <vector>
#include <directxmath.h>
#include <Windows.h>
#include <algorithm>

/// <summary>
/// Types of geometry for the shape object
/// </summary>
enum class GeometryType
{
	CUBE,
	SPHERE,
	CYLINDER,
	CONE,
	QUAD
};

/// <summary>
/// Structure of vertex data to be passed to the shader
/// </summary>
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 BiTangent;
};

/// <summary>
/// Instance data
/// </summary>
struct InstanceData
{
	DirectX::XMFLOAT3 Pos;

	/// <summary>
	/// Comparison operator for InstanceData objects using position vector as comparison value
	/// </summary>
	/// <param name="a">Instance to compare with this instance</param>
	/// <returns>Bool representing if instances are equal or not</returns>
	bool operator==(const InstanceData& a) const
	{
		return (fabs(a.Pos.x - Pos.x) < FLT_EPSILON && fabs(a.Pos.y - Pos.y) < FLT_EPSILON && fabs(a.Pos.z - Pos.z) < FLT_EPSILON);
	}
};

class Shape
{
private:
	std::vector<SimpleVertex> mVertices;
	std::vector<WORD> mIndices;
	std::vector<InstanceData> mInstanceData;
	DirectX::XMFLOAT4X4 mTransform;
	DirectX::XMFLOAT3 mTranslation;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mScale;
	DirectX::XMFLOAT3 mWorldPos;
	std::wstring mDiffuseTexture;
	std::wstring mNormalMap;
	std::wstring mDisplacementMap;
	std::wstring mShader;
	std::string mName;
	GeometryType mGeometryType;
	bool mSkyBox;
	bool mParticleSystem;

	void Cube();
	void Sphere() const;
	void Cylinder();
	void Cone();
	void Quad();

public:
	Shape();
	Shape(const GeometryType& geometryType, const bool& skyBox, const DirectX::XMFLOAT4X4& entityTransform, const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation, 
		const DirectX::XMFLOAT3& scale, const std::wstring& diffuseTexture, const std::wstring& normalMap, const std::wstring& displacementMap, const std::wstring& shader,
		const std::string& name, const std::vector<InstanceData>* const instanceData, const bool& particleSystem);
	~Shape();

	const std::vector<SimpleVertex>& Vertices() const;
	const std::vector<WORD>& Indices() const;
	const std::vector<InstanceData>& Instances() const;
	const std::wstring& DiffuseTexture() const;
	const std::wstring& NormalMap() const;
	const std::wstring& DisplacementMap() const;
	const std::wstring& Shader() const;
	const DirectX::XMFLOAT3& WorldPos() const;
	const DirectX::XMFLOAT4X4& Transform() const;
	const GeometryType& Geometry() const;
	const std::string& Name() const;
	const bool& SkyBox() const;
	const bool& ParticleSystem() const;

	void SetTransform(const DirectX::XMFLOAT4X4& entityTransform);
	void RemoveInstances(const std::vector<InstanceData>& instancesToRemove);
};
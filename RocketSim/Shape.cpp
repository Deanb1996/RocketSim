#include "Shape.h"

using namespace std;
using namespace DirectX;

/// <summary>
/// Sets the vertices and indices of the shape to that of a cube
/// </summary>
void Shape::Cube()
{
	//24 vertices
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });

	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });

	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) });

	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) });

	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) });

	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });

	//36 indices
	mIndices.push_back(3);	mIndices.push_back(1);	mIndices.push_back(0);
	mIndices.push_back(2);	mIndices.push_back(1);	mIndices.push_back(3);
	mIndices.push_back(6);	mIndices.push_back(4);	mIndices.push_back(5);
	mIndices.push_back(7);	mIndices.push_back(4);	mIndices.push_back(6);
	mIndices.push_back(11);	mIndices.push_back(9);	mIndices.push_back(8);
	mIndices.push_back(10);	mIndices.push_back(9);	mIndices.push_back(11);
	mIndices.push_back(14);	mIndices.push_back(12);	mIndices.push_back(13);
	mIndices.push_back(15);	mIndices.push_back(12);	mIndices.push_back(14);
	mIndices.push_back(19);	mIndices.push_back(17);	mIndices.push_back(16);
	mIndices.push_back(18);	mIndices.push_back(17);	mIndices.push_back(19);
	mIndices.push_back(22);	mIndices.push_back(20);	mIndices.push_back(21);
	mIndices.push_back(23);	mIndices.push_back(20);	mIndices.push_back(22);
}

/// <summary>
/// Sets the vertices and indices of the shape to that of a sphere NOT IMPLEMENTED
/// </summary>
void Shape::Sphere() const
{
}

/// <summary>
/// Sets the vertices and indices of the shape to that of a cylinder
/// </summary>
void Shape::Cylinder()
{
	const int pointsOnCircle = 30;

	//Sides
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;

		//Tube top
		XMFLOAT3 normal(sin(theta), 0.0f, cos(theta));
		XMFLOAT3 tangent(0, 1.0f, 0);
		XMFLOAT3 biTangent;
		XMStoreFloat3(&biTangent, XMVector3Cross(XMLoadFloat3(&normal), XMLoadFloat3(&tangent)));
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), 1.0f, cos(theta)), normal, XMFLOAT2(fraction, 1.0f), tangent, biTangent });	

		//Tube bottom
		tangent.y = -1.0f;
		XMStoreFloat3(&biTangent, XMVector3Cross(XMLoadFloat3(&normal), XMLoadFloat3(&tangent)));
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), normal, XMFLOAT2(fraction, 0.0f), tangent, biTangent });
	}

	//Cap top centre
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT3(0, 0, 1.0f) });

	//Cap bottom centre
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });

	//Caps
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;

		//Cap top
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), 1.0f, cos(theta)), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(1.0f, 0, 0), XMFLOAT3(0, 0, 1.0f) });

		//Cap bottom
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });
	}

	//Two triangles for each rectangle
	for (int i = 0; i < (pointsOnCircle * 2); i += 2)
	{
		mIndices.push_back(i);
		mIndices.push_back(i + 1);
		mIndices.push_back(i + 2);
		mIndices.push_back(i + 1);
		mIndices.push_back(i + 3);
		mIndices.push_back(i + 2);
	}

	//Triangles for caps
	for (int i = 2; (i < pointsOnCircle * 2); i += 2)
	{
		//Top cap
		mIndices.push_back(pointsOnCircle * 2);
		mIndices.push_back((pointsOnCircle * 2) + i);
		mIndices.push_back((pointsOnCircle * 2) + (i + 2));

		//Bottom cap
		mIndices.push_back(pointsOnCircle * 2 + 1);
		mIndices.push_back((pointsOnCircle * 2 + 1) + (i + 2));
		mIndices.push_back((pointsOnCircle * 2 + 1) + i);
	}
}

/// <summary>
/// Sets the vertices and indices of the shape to that of a cone
/// </summary>
void Shape::Cone()
{
	const int pointsOnCircle = 600;

	//Calculate vector of side of cone
	const float radius = 1;
	const float height = 2;
	const float sideLength = sqrtf(radius * radius + height * height);
	const float coneX = radius / sideLength;
	const float coneY = -height / sideLength;

	//Cone sides
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;

		//Top point
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(-coneY * sin(theta), coneX, -coneY * cos(theta)), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT3(0, 0, 1.0f) });

		//Bottom circle
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), XMFLOAT3(-coneY * sin(theta), coneX, -coneY * cos(theta)), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });
	}

	//Bottom centre
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });

	//Cone Bottom
	for (int i = 0; i < pointsOnCircle; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircle - 1);
		const float theta = 2 * XM_PI * fraction;

		//Bottom circle
		mVertices.emplace_back(SimpleVertex{ XMFLOAT3(sin(theta), -1.0f, cos(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(sin(theta) + 1 / 2, cos(theta) + 1 / 2), XMFLOAT3(-1.0f, 0, 0), XMFLOAT3(0, 0, -1.0f) });
	}

	//Triangles of cone sides
	for (int i = 0; i < (pointsOnCircle * 2) - 2; i += 2)
	{
		mIndices.push_back(i);
		mIndices.push_back(i + 1);
		mIndices.push_back(i + 3);
	}
	mIndices.push_back(58);
	mIndices.push_back(59);
	mIndices.push_back(1);

	//Triangles of bottom circle
	for (int i = 1; (i < pointsOnCircle); i++)
	{
		//Bottom cap
		mIndices.push_back(pointsOnCircle * 2);
		mIndices.push_back((pointsOnCircle * 2) + (i + 1));
		mIndices.push_back((pointsOnCircle * 2) + i);
	}
}

/// <summary>
/// Sets the vertices and indices of the shape to that of a quad
/// </summary>
void Shape::Quad()
{
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });
	mVertices.emplace_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) });

	mIndices.push_back(3);	mIndices.push_back(1);	mIndices.push_back(0);
	mIndices.push_back(2);	mIndices.push_back(1);	mIndices.push_back(3);
}

/// <summary>
/// Default constructor
/// </summary>
Shape::Shape() :
	mGeometryType(GeometryType::CUBE),
	mSkyBox(0),
	mParticleSystem(0)
{
}

/// <summary>
/// Constructs a shape object with given values
/// </summary>
/// <param name="geometryType">The shapes geometry type eg. cube</param>
/// <param name="skyBox">Bool determining if the shape is a skybox</param>
/// <param name="entityTransform">Transformation matrix of shapes parent entity</param>
/// <param name="translation">Translation vector of the shape</param>
/// <param name="rotation">Rotation vector of the shape</param>
/// <param name="scale">Scale vector of the shape</param>
/// <param name="diffuseTexture">Diffuse texture of the shape</param>
/// <param name="normalMap">Normal map of the shape</param>
/// <param name="displacementMap">Displacement map of the shape</param>
/// <param name="shader">Shader of the shape</param>
/// <param name="name">Name of the shape</param>
/// <param name="instanceData">Instance data for all the instances of the shape</param>
/// <param name="particleSystem">Bool determining if the shape is a particle system</param>
Shape::Shape(const GeometryType& geometryType, const bool& skyBox, const XMFLOAT4X4& entityTransform, const XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation,
	const XMFLOAT3& scale, const std::wstring& diffuseTexture, const std::wstring& normalMap, const std::wstring& displacementMap, const wstring& shader, 
	const string& name, const std::vector<InstanceData>* const instanceData, const bool& particleSystem) :
	mTranslation(translation),
	mRotation(rotation),
	mScale(scale),
	mDiffuseTexture(diffuseTexture),
	mNormalMap(normalMap),
	mDisplacementMap(displacementMap),
	mShader(shader),
	mName(name),
	mGeometryType(geometryType),
	mSkyBox(skyBox),
	mParticleSystem(particleSystem)
{
	//Sets transform of shape relative to parent entities transform
	SetTransform(entityTransform);

	//Checks instance data pointer isn't null then sets instance data of shape
	if (instanceData)
	{
		mInstanceData = *instanceData;
	}

	//Sets vertices and indices of shape based on geometry type eg. cube
	switch (geometryType)
	{
	//Sets vertices and indices of the shape to those of a cube
	case GeometryType::CUBE: Cube();
		break;
	//Sets vertices and indices of the shape to those of a sphere
	case GeometryType::SPHERE: Sphere();
		break;
	//Sets vertices and indices of the shape to those of a cylinder
	case GeometryType::CYLINDER: Cylinder();
		break;
	//Sets vertices and indices of the shape to those of a cone
	case GeometryType::CONE: Cone();
		break;
	//Sets vertices and indices of the shape to those of a quad
	case GeometryType::QUAD: Quad();
		break;
	}
}

/// <summary>
/// Default destructor
/// </summary>
Shape::~Shape()
{
}

/// <summary>
/// Get method for shapes vertices
/// </summary>
/// <returns>Vector containing shapes vertices</returns>
const vector<SimpleVertex>& Shape::Vertices() const
{
	return mVertices;
}

/// <summary>
/// Get method for shapes indices
/// </summary>
/// <returns>Vector containing shapes indices</returns>
const vector<WORD>& Shape::Indices() const
{
	return mIndices;
}

/// <summary>
/// Get method for shapes instance data
/// </summary>
/// <returns>Shapes instance data</returns>
const std::vector<InstanceData>& Shape::Instances() const
{
	return mInstanceData;
}

/// <summary>
/// Get method for shapes diffuse texture filename
/// </summary>
/// <returns>Filename of shapes diffuse texture</returns>
const std::wstring& Shape::DiffuseTexture() const
{
	return mDiffuseTexture;
}

/// <summary>
/// Get method for shapes normal map filename
/// </summary>
/// <returns>Filename of shapes normal map</returns>
const std::wstring& Shape::NormalMap() const
{
	return mNormalMap;
}

/// <summary>
/// Get method for shapes displacement map filename
/// </summary>
/// <returns>Filename of shapes displacementMap</returns>
const std::wstring& Shape::DisplacementMap() const
{
	return mDisplacementMap;
}

/// <summary>
/// Get method for the shapes shader filename
/// </summary>
/// <returns>Filename of shapes shader</returns>
const wstring& Shape::Shader() const
{
	return mShader;
}

/// <summary>
/// Get method for the shapes world pos
/// </summary>
/// <returns>Vector containing world pos of shape</returns>
const DirectX::XMFLOAT3& Shape::WorldPos() const
{
	return mWorldPos;
}

/// <summary>
/// Get method for shapes transform matrix
/// </summary>
/// <returns>Shapes transform matrix</returns>
const XMFLOAT4X4& Shape::Transform() const
{
	return mTransform;
}

/// <summary>
/// Get method for shapes geometry type
/// </summary>
/// <returns>Shapes geometry type</returns>
const GeometryType& Shape::Geometry() const
{
	return mGeometryType;
}

/// <summary>
/// Get method for shapes name
/// </summary>
/// <returns>Name of shape</returns>
const std::string& Shape::Name() const
{
	return mName;
}

/// <summary>
/// Get method for shapes sky box bool
/// </summary>
/// <returns>Bool that determines whether the shape is a skybox</returns>
const bool& Shape::SkyBox() const
{
	return mSkyBox;
}

/// <summary>
/// Get method for shapes particle system bool
/// </summary>
/// <returns>Bool that determines whether the shape is a particle system</returns>
const bool& Shape::ParticleSystem() const
{
	return mParticleSystem;
}

/// <summary>
/// Sets the shapes transform based on parent entities transform
/// </summary>
/// <param name="entityTransform">Transform matrix of parent entity</param>
void Shape::SetTransform(const XMFLOAT4X4& entityTransform)
{
	//Loads transform matrix into XMMATRIX object for matrix multiplication
	const XMMATRIX entityTransformMatrix = XMLoadFloat4x4(&entityTransform);
	XMMATRIX transformMatrix = XMMatrixIdentity();

	//Sets transform matrix as parent entities transform matrix then mulitplies by the shapes translation, rotation and scale
	transformMatrix *= XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	transformMatrix *= XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z);
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mTranslation));
	transformMatrix *= entityTransformMatrix;

	//Stores multiplied transform matrix into XMFLOAT4X4
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Sets world pos of shape
	mWorldPos = XMFLOAT3(mTransform._41, mTransform._42, mTransform._43);
}

/// <summary>
/// Removes instances from instance data of the shape
/// </summary>
/// <param name="instancesToRemove">List of instances to be removed</param>
void Shape::RemoveInstances(const std::vector<InstanceData>& instancesToRemove)
{
	for (auto& instance : instancesToRemove)
	{
		mInstanceData.erase(remove(mInstanceData.begin(), mInstanceData.end(), instance), mInstanceData.end());
	}
}

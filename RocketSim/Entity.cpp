#include "Entity.h"

using namespace std;
using namespace DirectX;

/// <summary>
/// Default constructor
/// </summary>
Entity::Entity() :
	mSpeed(0),
	mRotationSpeed(0)
{
}

/// <summary>
/// Constructs an entity object with given values
/// </summary>
/// <param name="translation">Translation vector of entity</param>
/// <param name="rotation">Rotation vector of entity</param>
/// <param name="scale">Scale vector of entity</param>
/// <param name="name">Name of the entity</param>
/// <param name="speed">Movement speed of the entity</param>
/// <param name="rotationSpeed">Rotation speed of the entity</param>
Entity::Entity(const XMFLOAT3& translation, const XMFLOAT3& rotation, const XMFLOAT3& scale, const string& name, const float& speed, const float& rotationSpeed) :
	mTranslation(translation),
	mRotation(rotation),
	mScale(scale),
	mName(name),
	mSpeed(speed),
	mRotationSpeed(rotationSpeed)
{
	SetTransform();
	SetDirection();
}

/// <summary>
/// Default destructor
/// </summary>
Entity::~Entity()
{
}

/// <summary>
/// Creates a shape with given values and adds it to the entities list of shapes
/// </summary>
/// <param name="geometryType">Geometry type of the shape</param>
/// <param name="skyBox">Bool determining if the shape is a skybox</param>
/// <param name="translation">Translation vector of the shape</param>
/// <param name="rotation">Rotation vector of the shape</param>
/// <param name="scale">Scale vector of the shape</param>
/// <param name="diffuseTexture">Diffuse texture of the shape</param>
/// <param name="normalMap">Normal map of the shape</param>
/// <param name="displacementMap">Displacement map of the shape</param>
/// <param name="shader">Shader of the shape</param>
/// <param name="name">Name of the shape</param>
/// <param name="instanceData">Pointer to instance data for shape</param>
/// <param name="particleSystem">Bool determining if the shape is a particle system</param>
void Entity::AddShape(const GeometryType& geometryType, const bool& skyBox, const XMFLOAT3& translation, const XMFLOAT3& rotation, const XMFLOAT3& scale, 
	const std::wstring& diffuseTexture, const std::wstring& normalMap, const std::wstring& displacementMap, const wstring& shader, const string& name, 
	const std::vector<InstanceData>* const instanceData, const bool& particleSystem)
{
	//Calls shape constructor with given geometry type
	switch (geometryType)
	{
	//Creates shape with cube geometry
	case GeometryType::CUBE:
	{
		const Shape Shape(GeometryType::CUBE, skyBox, mTransform, translation, rotation, scale, diffuseTexture, normalMap, displacementMap, shader, name, instanceData, particleSystem);
		mShapes.push_back(Shape);
		break;
	}
	//Creates shape with sphere geometry
	case GeometryType::SPHERE:
	{
		const Shape Shape(GeometryType::SPHERE, skyBox, mTransform, translation, rotation, scale, diffuseTexture, normalMap, displacementMap, shader, name, instanceData, particleSystem);
		mShapes.push_back(Shape);
		break;
	}
	//Creates shape with cylinder geometry
	case GeometryType::CYLINDER:
	{
		const Shape Shape(GeometryType::CYLINDER, skyBox, mTransform, translation, rotation, scale, diffuseTexture, normalMap, displacementMap, shader, name, instanceData, particleSystem);
		mShapes.push_back(Shape);
		break;
	}
	//Creates shape with cone geometry
	case GeometryType::CONE:
	{
		const Shape Shape(GeometryType::CONE, skyBox, mTransform, translation, rotation, scale, diffuseTexture, normalMap, displacementMap, shader, name, instanceData, particleSystem);
		mShapes.push_back(Shape);
		break;
	}
	//Creates shape with quad geometry
	case GeometryType::QUAD:
	{
		const Shape Shape(GeometryType::QUAD, skyBox, mTransform, translation, rotation, scale, diffuseTexture, normalMap, displacementMap, shader, name, instanceData, particleSystem);
		mShapes.push_back(Shape);
		break;
	}
	}
}

/// <summary>
/// Creates a light with given values and adds it to the entities list of lights
/// </summary>
/// <param name="translation">Translation vector of the light</param>
/// <param name="colour">Colour of the light (RGB)</param>
/// <param name="name">Name of the light</param>
void Entity::AddLight(const DirectX::XMFLOAT4& translation, const DirectX::XMFLOAT4& colour, const std::string& name)
{
	const Light Light(mTransform, translation, colour, name);
	mLights.push_back(Light);
}

/// <summary>
/// Get method for entities list of shapes
/// </summary>
/// <returns>List of shapes in the entity</returns>
const vector<Shape>& Entity::Shapes() const
{
	return mShapes;
}

///// <summary>
///// Non-const get method for entities list of shapes
///// </summary>
///// <returns>List of shapes in the entity</returns>
//std::vector<Shape>& Entity::Shapes()
//{
//	return mShapes;
//}

/// <summary>
/// Get method for entities list of lights
/// </summary>
/// <returns>List of lights in the entity</returns>
const std::vector<Light>& Entity::Lights() const
{
	return mLights;
}

///// <summary>
///// Non-const get method entities list of lights
///// </summary>
///// <returns>List of lights in the entity</returns>
//std::vector<Light>& Entity::Lights()
//{
//	return mLights;
//}

/// <summary>
/// Get method for entities up direction
/// </summary>
/// <returns>Up vector of entity</returns>
const DirectX::XMFLOAT3& Entity::Up() const
{
	return mUp;
}

/// <summary>
/// Get method for entities right direction
/// </summary>
/// <returns>Right vector of entity</returns>
const DirectX::XMFLOAT3& Entity::Right() const
{
	return mRight;
}

/// <summary>
/// Get method for entities forward direction
/// </summary>
/// <returns>Forward vector of entity</returns>
const DirectX::XMFLOAT3& Entity::Forward() const
{
	return mForward;
}

/// <summary>
/// Get method for entities position
/// </summary>
/// <returns>Vector containing entities position</returns>
const DirectX::XMFLOAT3& Entity::Position() const
{
	return mTranslation;
}

/// <summary>
/// Get method for entities transform
/// </summary>
/// <returns>Entities transform matrix</returns>
const DirectX::XMFLOAT4X4& Entity::Transform() const
{
	return mTransform;
}

/// <summary>
/// Get method for entities name
/// </summary>
/// <returns>Enities name</returns>
const std::string & Entity::Name() const
{
	return mName;
}

/// <summary>
/// Get method for entities speed
/// </summary>
/// <returns>Movement speed of the entity</returns>
const float & Entity::Speed() const
{
	return mSpeed;
}

/// <summary>
/// Get method for entities rotation speed
/// </summary>
/// <returns>Rotation speed of the entity</returns>
const float & Entity::RotationSpeed() const
{
	return mRotationSpeed;
}

/// <summary>
/// Calls the set transform method of all the shapes/lights in the entities lists of shapes/lights to update the transform based on entities transform
/// </summary>
void Entity::UpdateChildren()
{
	for (auto& shape : mShapes)
	{
		shape.SetTransform(mTransform);
	}

	for (auto& light : mLights)
	{
		light.SetTransform(mTransform);
	}
}

/// <summary>
/// Sets the transform of the entity object
/// </summary>
void Entity::SetTransform()
{
	//Sets transform matrix to an identity matrix then applies translation, rotation and scale
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	transformMatrix *= XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z);
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mTranslation));

	//Stores multiplied transform matrix in XMFLOAT4X4
	XMStoreFloat4x4(&mTransform, transformMatrix);
}

/// <summary>
/// Sets direction vectors for entity
/// </summary>
void Entity::SetDirection()
{
	//Extracts new right, up and forward direction vectors from transform matrix
	const XMVECTOR rightVector = XMLoadFloat3(&XMFLOAT3(mTransform._11, mTransform._12, mTransform._13));
	const XMVECTOR upVector = XMLoadFloat3(&XMFLOAT3(mTransform._21, mTransform._22, mTransform._23));
	const XMVECTOR forwardVector = XMLoadFloat3(&XMFLOAT3(mTransform._31, mTransform._32, mTransform._33));

	//Normalises and stores the direction vectors
	XMStoreFloat3(&mRight, XMVector3Normalize(rightVector));
	XMStoreFloat3(&mUp, XMVector3Normalize(upVector));
	XMStoreFloat3(&mForward, XMVector3Normalize(forwardVector));
}

/// <summary>
/// Translates the entities position based on a given translation vector
/// </summary>
/// <param name="translation">Translation vector to translate the entity by</param>
/// <param name="timeScale">Timescale to modify the translation by</param>
/// <param name="dt">Delta time</param>
void Entity::MoveEntity(const DirectX::XMFLOAT3& translation, const float& timeScale, const float& dt)
{
	//Loads position and translation into XMVECTOR objects for vector maths
	XMVECTOR positionVector = XMLoadFloat3(&mTranslation);
	const XMVECTOR translationVector = XMLoadFloat3(&translation);

	//Modifies position vector and stores it in XMFLOAT3
	positionVector = positionVector + translationVector * mSpeed * dt;
	XMStoreFloat3(&mTranslation, positionVector);

	//Updates transform
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);
	transformMatrix = transformMatrix * XMMatrixTranslationFromVector(XMLoadFloat3(&translation) * timeScale * mSpeed * dt);
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Updates child shapes
	UpdateChildren();
}

/// <summary>
/// Rotates the entity based on a given rotation vector
/// </summary>
/// <param name="rotation">Rotation vector to rotate the entity by</param>
/// <param name="timeScale">Timescale to modify the rotation by</param>
/// <param name="dt">Delta time</param>
void Entity::RotateEntity(const DirectX::XMFLOAT3 & rotation, const float& timeScale, const float & dt)
{
	//Creates rotation matrix based on local axis
	XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&mRight), rotation.x * timeScale * mRotationSpeed * dt) * XMMatrixRotationAxis(XMLoadFloat3(&mUp), rotation.y * timeScale * mRotationSpeed * dt) * XMMatrixRotationAxis(XMLoadFloat3(&mForward), rotation.z * timeScale * mRotationSpeed * dt);

	//Applies rotation to transform matrix
	const XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);
	rotationMatrix = rotationMatrix * transformMatrix;
	XMStoreFloat4x4(&mTransform, rotationMatrix);

	//Updates direction vectors and transform for child shapes
	SetDirection();
	UpdateChildren();
}

/// <summary>
/// Rotate the entity around a given point based on a given rotation vector
/// </summary>
/// <param name="point">Point to rotate around</param>
/// <param name="rotation">Rotation vector to rotate the entity by</param>
/// <param name="timeScale">Timescale to modify the rotation by</param>
/// <param name="dt">Delta time</param>
void Entity::RotateAroundPoint(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& rotation, const float& timeScale, const float& dt)
{
	//Creates translation and rotation matricies
	const XMMATRIX translation1 = XMMatrixTranslationFromVector(-XMLoadFloat3(&point));
	const XMMATRIX rotation1 = XMMatrixRotationX(rotation.x * timeScale * mRotationSpeed * dt) * XMMatrixRotationY(rotation.y * timeScale * mRotationSpeed * dt) * XMMatrixRotationZ(rotation.z * timeScale * mRotationSpeed * dt);
	const XMMATRIX translation2 = XMMatrixTranslationFromVector(XMLoadFloat3(&point));

	//Applies rotation to transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);
	transformMatrix = transformMatrix * translation1 * rotation1 * translation2;
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Updates translation vector to hold new world pos
	mTranslation = XMFLOAT3(mTransform._41, mTransform._42, mTransform._43);

	//Updates direction vectors and transform for child shapes
	SetDirection();
	UpdateChildren();
}

/// <summary>
/// Sets entities position
/// </summary>
/// <param name="position">New position of entity</param>
void Entity::SetPosition(const DirectX::XMFLOAT3 & position)
{
	//Sets entities translation to new position
	mTranslation = position;

	//Updates transform and child shapes
	SetTransform();
	UpdateChildren();
}

/// <summary>
/// Resets rotation of entity to starting rotation
/// </summary>
void Entity::ResetRotation()
{
	//Sets transform matrix to an identity matrix then applies translation, rotation and scale
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	transformMatrix *= XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z);
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mTranslation));

	//Stores multiplied transform matrix in XMFLOAT4X4 and updates children
	XMStoreFloat4x4(&mTransform, transformMatrix);
	UpdateChildren();
}

/// <summary>
/// Sets movement speed of entity
/// </summary>
/// <param name="speed">New movement speed of entity</param>
void Entity::SetSpeed(const float & speed)
{
	mSpeed = speed;
}

/// <summary>
/// Sets rotation speed of entity
/// </summary>
/// <param name="speed">New rotation speed of entity</param>
void Entity::SetRotationSpeed(const float & speed)
{
	mRotationSpeed = speed;
}

/// <summary>
/// Calls remove instances method on child shape
/// </summary>
/// <param name="name">Name of child shape</param>
/// <param name="instances">Instances to remove from the shape</param>
void Entity::RemoveInstancesFromShape(const string& name, const vector<InstanceData>& instances)
{
	for (auto& shape : mShapes)
	{
		if (shape.Name() == name)
		{
			shape.RemoveInstances(instances);
		}
	}
}

/// <summary>
/// Calls set colour method on child light
/// </summary>
/// <param name="name">Name of child light</param>
/// <param name="colour">New colour of light</param>
void Entity::ChangeLightColour(const string& name, const XMFLOAT4& colour)
{
	for (auto& light : mLights)
	{
		if (light.Name() == name)
		{
			light.SetColour(colour);
		}
	}
}


/// <summary>
/// Comparison operator for entity objects using name data member as comparison value
/// </summary>
/// <param name="a">Entity a</param>
/// <param name="a">Entity b</param>
/// <returns>Bool representing if entities are equal or not</returns>
bool operator==(const Entity& a, const Entity& b)
{
	return (a.Name() == b.Name());
}

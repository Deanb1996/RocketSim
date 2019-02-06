#include "Light.h"

using namespace DirectX;
using namespace std;

/// <summary>
/// Default constructor for light object
/// </summary>
Light::Light() :
	mTranslation(XMFLOAT4(0, 0, 0, 0)),
	mColour(XMFLOAT4(0, 0, 0, 0))
{
}

/// <summary>
/// Constructs a light object with given values
/// </summary>
/// <param name="entityTransform">Transform of parent entity</param>
/// <param name="lightPos">Position of the light object</param>
/// <param name="lightColour">Colour of the light object</param>
/// <param name="name">Name of light</param>
Light::Light(const DirectX::XMFLOAT4X4& entityTransform, const DirectX::XMFLOAT4& lightPos, const DirectX::XMFLOAT4& lightColour, const string& name):
	mTranslation(lightPos),
	mColour(lightColour),
	mName(name)
{
	SetTransform(entityTransform);
}

/// <summary>
/// Default destructor
/// </summary>
Light::~Light()
{
}

/// <summary>
/// Get method for lights position
/// </summary>
/// <returns>Position of the light</returns>
const DirectX::XMFLOAT4& Light::Position() const
{
	return mWorldPos;
}

/// <summary>
/// Get method for lights colour
/// </summary>
/// <returns>Colour of the light</returns>
const DirectX::XMFLOAT4& Light::Colour() const
{
	return mColour;
}

/// <summary>
/// Get method for lights name
/// </summary>
/// <returns>Name of light</returns>
const std::string & Light::Name() const
{
	return mName;
}

/// <summary>
/// Sets transform of light based on parent entity
/// </summary>
/// <param name="entityTransform">Transform of parent entity</param>
void Light::SetTransform(const DirectX::XMFLOAT4X4 & entityTransform)
{
	//Loads transform matrix into XMMATRIX object for matrix multiplication
	const XMMATRIX entityTransformMatrix = XMLoadFloat4x4(&entityTransform);
	XMMATRIX transformMatrix = XMMatrixIdentity();

	//Sets transform matrix as parent entities transform matrix then mulitplies by the lights translation
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat4(&mTranslation));
	transformMatrix *= entityTransformMatrix;

	//Stores multiplied transform matrix into XMFLOAT4X4
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Sets lights world pos
	mWorldPos = XMFLOAT4(mTransform._41, mTransform._42, mTransform._43, 1.0f);
}

/// <summary>
/// Set method for colour of light
/// </summary>
/// <param name="colour">New colour of light</param>
void Light::SetColour(const DirectX::XMFLOAT4 & colour)
{
	mColour = colour;
}

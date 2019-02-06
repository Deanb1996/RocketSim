#include "Camera.h"

using namespace DirectX;

/// <summary>
/// Sets view matrix of camera object
/// </summary>
void Camera::SetView()
{
	//Loads the position and directions into XMVECTOR objects for vector maths
	const XMVECTOR positionVector = XMLoadFloat3(&mPos);
	const XMVECTOR upDirVector = XMLoadFloat3(&mUp);

	//Checks if the camera is a tracking camera or a free moving camera
	//Creates view matrix then stores it in XMFLOAT4X4
	if (mLookAt)
	{
		const XMVECTOR mLookAtVector = XMLoadFloat3(&mLookAtV);
		const XMMATRIX viewMatrix = XMMatrixLookAtLH(positionVector, mLookAtVector, upDirVector);
		XMStoreFloat4x4(&mView, viewMatrix);
	}
	else
	{
		const XMVECTOR forwardVector = XMLoadFloat3(&mForward);
		const XMMATRIX viewMatrix = XMMatrixLookAtLH(positionVector, positionVector + forwardVector, upDirVector);
		XMStoreFloat4x4(&mView, viewMatrix);
	}
}

/// <summary>
/// Sets projection matrix of camera object
/// </summary>
/// <param name="fov">Field of view of the camera</param>
/// <param name="aspectRatio">Aspect ratio of the window</param>
/// <param name="near">Near plane for rendering</param>
/// <param name="">Far plane for rendering</param>
void Camera::SetProjection(const float& fov, const float& aspectRatio, const float& near, const float& far)
{
	//Creates projection matrix
	const XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, near, far);
	
	//Stores projection matrix in XMFLOAT4X4
	XMStoreFloat4x4(&mProj, projectionMatrix);
}

/// <summary>
/// Sets transform matrix of camera
/// </summary>
void Camera::SetTransform()
{
	//Sets transform matrix to an identity matrix then applies translation and rotation then stores new transform matrix
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mPos));
	transformMatrix *= XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z);
	XMStoreFloat4x4(&mTransform, transformMatrix);
}

/// <summary>
/// Sets direction vectors of camera
/// </summary>
void Camera::SetDirection()
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
/// Default constructor
/// </summary>
Camera::Camera() :
	mSpeed(0),
	mRotationSpeed(0),
	mAllowMovement(false),
	mLookAt(false)
{
}

/// <summary>
/// Constructs a camera with given values
/// </summary>
/// <param name="allowMovement">Bool that determines if camera allows free movement controlled by user input</param>
/// <param name="position">Position of the camera</param>
/// <param name="rotation">Rotation of the camera</param>
/// <param name="fov">FOV of the camera</param>
/// <param name="aspectRatio">Aspect ratio of the camera</param>
/// <param name="near">Near plane for drawing</param>
/// <param name="far">Far plane for drawing</param>
/// <param name="speed">Movement speed of the camera</param>
/// <param name="rotationSpeed">Rotation speed of the camera</param>
Camera::Camera(const bool& allowMovement, const XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, const float& fov, const float& aspectRatio,
	const float& near, const float& far, const float& speed, const float& rotationSpeed) :
	mPos(position),
	mRotation(rotation),
	mSpeed(speed),
	mRotationSpeed(rotationSpeed),
	mAllowMovement(allowMovement),
	mLookAt(false)
{
	//Sets projection, view and transform matricies
	SetProjection(fov, aspectRatio, near, far);
	SetTransform();
	SetDirection();
	SetView();
}

/// <summary>
/// Constructs a camera with given values
/// </summary>
/// <param name="allowMovement">Bool that determines if camera allows free movement controlled by user input</param>
/// <param name="position">Position of the camera</param>
/// <param name="rotation">Rotation of the camera</param>
/// <param name="lookAt">Position the camera looks at</param>
/// <param name="fov">FOV of the camera</param>
/// <param name="aspectRatio">Aspect ratio of the camera</param>
/// <param name="near">Near plane for drawing</param>
/// <param name="far">Far plane for drawing</param>
/// <param name="speed">Movement speed of the camera</param>
/// <param name="rotationSpeed">Rotation speed of the camera</param>
Camera::Camera(const bool& allowMovement, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& lookAt, const float& fov,
	const float& aspectRatio, const float& near, const float& far, const float& speed, const float& rotationSpeed) :
	mPos(position),
	mRotation(rotation),
	mLookAtV(lookAt),
	mSpeed(speed),
	mRotationSpeed(rotationSpeed),
	mAllowMovement(allowMovement),
	mLookAt(true)
{
	//Sets projection, view and transform matricies
	SetProjection(fov, aspectRatio, near, far);
	SetTransform();
	SetDirection();
	SetView();
}

/// <summary>
/// Default destructor
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// Get method for cameras view matrix
/// </summary>
/// <returns>View matrix of camera</returns>
const XMFLOAT4X4& Camera::View() const
{
	return mView;
}

/// <summary>
/// Get method for cameras project matrix
/// </summary>
/// <returns>Projection matrix of camera</returns>
const XMFLOAT4X4& Camera::Projection() const
{
	return mProj;
}

/// <summary>
/// Get method for the cameras position
/// </summary>
/// <returns>Position vector of the camera</returns>
const XMFLOAT3& Camera::Position() const
{
	return mPos;
}

/// <summary>
/// Get method for cameras up direction
/// </summary>
/// <returns>Up direction of the camera</returns>
const XMFLOAT3& Camera::Up() const
{
	return mUp;
}

/// <summary>
/// Get method for cameras right direction
/// </summary>
/// <returns>Right direction of camera</returns>
const DirectX::XMFLOAT3 & Camera::Right() const
{
	return mRight;
}

/// <summary>
/// Get method for cameras forward direction
/// </summary>
/// <returns>Forward direction of the camera</returns>
const DirectX::XMFLOAT3 & Camera::Forward() const
{
	return mForward;
}

/// <summary>
/// Get method for cameras allow movement bool
/// </summary>
/// <returns>Bool determining whether movement is allowed or not</returns>
const bool & Camera::AllowMovement() const
{
	return mAllowMovement;
}

/// <summary>
/// Get method for cameras movement speed
/// </summary>
/// <returns>Movement speed of the camera</returns>
const float & Camera::Speed() const
{
	return mSpeed;
}

/// <summary>
/// Get method for cameras rotation speed
/// </summary>
/// <returns>Rotation speed of the camera</returns>
const float & Camera::RotationSpeed() const
{
	return mRotationSpeed;
}

/// <summary>
/// Translates the cameras position and transform matrix by given translation
/// Updates view matrix
/// </summary>
/// <param name="translation">Translation vector to translate the camera by</param>
/// <param name="dt">Delta time</param>
void Camera::MoveCamera(const XMFLOAT3& translation, const float& dt)
{
	//Loads position and translation vectors into XMVECTOR objects for vector maths
	const XMVECTOR translationVector = XMLoadFloat3(&translation);
	XMVECTOR positionVector = XMLoadFloat3(&mPos);

	//Modifies position vector by given translation
	positionVector = positionVector + translationVector * mSpeed * dt;
	XMStoreFloat3(&mPos, positionVector);

	//Applies translation to transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);
	transformMatrix = transformMatrix * XMMatrixTranslationFromVector(translationVector);
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Updates view matrix
	SetView();
}

/// <summary>
/// Translates the cameras position and transform matrix by given translation based on a given timescale
/// Updates view matrix
/// </summary>
/// <param name="translation">Translation vector to translate the camera by</param>
/// <param name="timeScale">Timescale to modify the translation by</param>
/// <param name="dt">Delta time</param>
void Camera::MoveCamera(const DirectX::XMFLOAT3 & translation, const float & timeScale, const float & dt)
{
	//Loads position and translation vectors into XMVECTOR objects for vector maths
	const XMVECTOR translationVector = XMLoadFloat3(&translation);
	XMVECTOR positionVector = XMLoadFloat3(&mPos);

	//Modifies position vector by given translation
	positionVector = positionVector + translationVector * timeScale * mSpeed * dt;
	XMStoreFloat3(&mPos, positionVector);

	//Applies translation to transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);
	transformMatrix = transformMatrix * XMMatrixTranslationFromVector(translationVector);
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Updates view matrix
	SetView();
}

/// <summary>
/// Rotates the cameras transform matrix by given rotation
/// Sets new direction vectors
/// Updates view matrix
/// </summary>
/// <param name="rotation">Rotation vector to rotate the camera by</param>
/// <param name="dt">Delta time from update loop</param>
void Camera::RotateCamera(const XMFLOAT3& rotation, const float& dt)
{
	//Creates rotation matrix based on local x axis and world y axis
	const XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&mRight), rotation.x * mRotationSpeed * dt) * XMMatrixRotationY(rotation.y * mRotationSpeed * dt);

	//Modifies rotation vector and stores it in XMFLOAT3
	const XMVECTOR rotationVector = XMLoadFloat3(&mRotation);
	XMVector3Transform(rotationVector, rotationMatrix);
	XMStoreFloat3(&mRotation, rotationVector);

	//Applies rotation to transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);
	transformMatrix = transformMatrix * rotationMatrix;
	XMStoreFloat4x4(&mTransform, transformMatrix);

	//Updates view matrix and direction vectors of camera
	SetDirection();
	SetView();
}

/// <summary>
/// Set method for cameras look at position
/// </summary>
/// <param name="lookAt">Look at position</param>
void Camera::SetLookAt(const DirectX::XMFLOAT3 & lookAt)
{
	mLookAtV = lookAt;
	SetView();
}

/// <summary>
/// Set method for cameras position
/// </summary>
/// <param name="position">New position for camera</param>
void Camera::SetPosition(const DirectX::XMFLOAT3 & position)
{
	//Sets position to given position
	mPos = position;

	//Updates view matrix and transform matrix
	SetTransform();
	SetView();
}

/// <summary>
/// Set method for cameras speed
/// </summary>
/// <param name="speed">New speed for camera</param>
void Camera::SetSpeed(const float & speed)
{
	mSpeed = speed;
}

/// <summary>
/// Set method for cameras rotation speed
/// </summary>
/// <param name="speed">New rotation speed for camera</param>
void Camera::SetRotationSpeed(const float & speed)
{
	mRotationSpeed = speed;
}

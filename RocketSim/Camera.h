#pragma once

#include <directxmath.h>

class Camera
{
private:
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;
	DirectX::XMFLOAT4X4 mTransform;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mForward;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mRight;
	DirectX::XMFLOAT3 mLookAtV;
	float mSpeed;
	float mRotationSpeed;
	bool mAllowMovement;
	bool mLookAt;

	void SetView();
	void SetProjection(const float& fov, const float& aspectRatio, const float& near, const float& far);
	void SetTransform();
	void SetDirection();

public:
	Camera();
	Camera(const bool& allowMovement, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, const float& fov, const float& aspectRatio,
		const float& near, const float& far, const float& speed, const float& rotationSpeed);
	Camera(const bool& allowMovement, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& lookAt, const float& fov,
		const float& aspectRatio, const float& near, const float& far, const float& speed, const float& rotationSpeed);
	~Camera();

	const DirectX::XMFLOAT4X4& View() const;
	const DirectX::XMFLOAT4X4& Projection() const;
	const DirectX::XMFLOAT3& Position() const;
	const DirectX::XMFLOAT3& Up() const;
	const DirectX::XMFLOAT3& Right() const;
	const DirectX::XMFLOAT3& Forward() const;
	const bool& AllowMovement() const;
	const float& Speed() const;
	const float& RotationSpeed() const;

	void MoveCamera(const DirectX::XMFLOAT3& translation, const float& dt);
	void MoveCamera(const DirectX::XMFLOAT3& translation, const float& timeScale, const float& dt);
	void RotateCamera(const DirectX::XMFLOAT3& rotation, const float& dt);
	void SetLookAt(const DirectX::XMFLOAT3& lookAt);
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetSpeed(const float& speed);
	void SetRotationSpeed(const float& speed);
};
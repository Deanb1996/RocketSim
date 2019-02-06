#pragma once

#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "Keyboard.h"
#include <algorithm>
#include "AntTweakManager.h"

class Scene
{
private:
	std::vector<Camera> mCameras{};
	std::vector<Entity> mEntities{};
	float mTimeScale{};
	float mTimeSinceExplosion{};
	int mWidth{};
	int mHeight{};
	bool mExitApplication{};
	bool mRocketLaunched{};
	bool mEngineOn{};
	bool mReset{};
	std::unique_ptr<DirectX::Keyboard> mKeyboard{};

	//Camera pointers
	Camera* mActiveCamera{};
	Camera* mRocketCam{};
	Camera* mRocketNoseCam{};
	Camera* mRocketBodyCam{};

	//Entity pointers
	Entity* mSkyBox{};
	Entity* mRocket{};
	Entity* mTerrain{};
	Entity* mSun{};
	Entity* mMoon{};
	Entity* mExplosion{};
	Entity* mLauncher{};
	Entity* mSmoke{};

	//AntTweak variables
	AntTweakManager* mAntTweak{};
	DirectX::XMFLOAT4 mSunColour;
	DirectX::XMFLOAT4 mMoonColour;
	DirectX::XMFLOAT3 mSunPos;
	DirectX::XMFLOAT3 mMoonPos;
	DirectX::XMFLOAT3 mRocketDir;
	DirectX::XMFLOAT3 mRocketPos;
	DirectX::XMFLOAT3 mCameraPos;
	DirectX::XMFLOAT3 mCameraDir;
	DirectX::XMFLOAT3 mCameraUpDir;
	DirectX::XMFLOAT3 mCameraRightDir;
	float mRocketSpeed{};
	float mRocketRotationSpeed{};
	float mSunRotationSpeed{};
	float mMoonRotationSpeed{};
	float mCameraSpeed{};
	float mCameraRotationSpeed{};
	float mCubeCount{};

	void Input(const float& dt);
	void CreateCameras();
	void CreateTerrain();
	void SpawnRocket();
	void RocketCollision();
	void SpawnExplosion(const DirectX::XMFLOAT3& position);
	void SetEntityPointers();
	void InitAntTweak();
	void UpdateAntTweak();
	void AnimateRocket(const float& dt);

public:
	Scene();
	Scene(const int& width, const int& height, AntTweakManager& antTweakManager);
	~Scene();
	void InitScene();
	void Update(const float& dt);

	const Camera* const ActiveCamera() const;
	const std::vector<Entity>& Entities() const;
	const bool& ExitApplication() const;

	Scene& operator=(const Scene&) = delete;
	Scene(const Scene&) = delete;
};
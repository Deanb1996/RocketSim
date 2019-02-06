#include "Scene.h"

using namespace std;
using namespace DirectX;

/// <summary>
/// Handles all input from the user every update frame
/// </summary>
/// <param name="dt">Delta time</param>
void Scene::Input(const float& dt)
{
	//Gets state of keyboard
	const auto keyBoard = mKeyboard->GetState();

	//Close application key
	if (keyBoard.Escape)
	{
		mExitApplication = true;
	}

	//Reset scene to inital state
	if (keyBoard.R)
	{
		mReset = true;
		mRocketLaunched = false;
		mEngineOn = false;
		InitScene();
	}

	//Increase/decrease time scale
	if (keyBoard.T && !keyBoard.LeftShift)
	{
		mTimeScale -= 0.1f;
	}
	if (keyBoard.T && keyBoard.LeftShift)
	{
		mTimeScale -= 0.1f;
	}

	//Camera selection
	if (keyBoard.F1)
	{
		mActiveCamera = &mCameras[0];
		mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
	}
	if (keyBoard.F2)
	{
		mActiveCamera = &mCameras[1];
		mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
	}
	if (keyBoard.F3)
	{
		mActiveCamera = &mCameras[2];
		mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
	}
	if (keyBoard.F4)
	{
		mActiveCamera = &mCameras[3];
		mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
	}
	if (keyBoard.F5)
	{
		mActiveCamera = &mCameras[4];
		mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
	}

	//Camera movement controls
	if (keyBoard.W && mActiveCamera->AllowMovement() && keyBoard.LeftControl)
	{
		mActiveCamera->MoveCamera(mActiveCamera->Forward(), dt);
		mSkyBox->MoveEntity(mActiveCamera->Forward(), 1, dt);
	}
	if (keyBoard.A && mActiveCamera->AllowMovement() && keyBoard.LeftControl)
	{
		mActiveCamera->MoveCamera(XMFLOAT3(-mActiveCamera->Right().x, -mActiveCamera->Right().y, -mActiveCamera->Right().z), dt);
		mSkyBox->MoveEntity(XMFLOAT3(-mActiveCamera->Right().x, -mActiveCamera->Right().y, -mActiveCamera->Right().z), 1, dt);
	}
	if (keyBoard.S && mActiveCamera->AllowMovement() && keyBoard.LeftControl)
	{
		mActiveCamera->MoveCamera(XMFLOAT3(-mActiveCamera->Forward().x, -mActiveCamera->Forward().y, -mActiveCamera->Forward().z), dt);
		mSkyBox->MoveEntity(XMFLOAT3(-mActiveCamera->Forward().x, -mActiveCamera->Forward().y, -mActiveCamera->Forward().z), 1, dt);
	}
	if (keyBoard.D && mActiveCamera->AllowMovement() && keyBoard.LeftControl)
	{
		mActiveCamera->MoveCamera(mActiveCamera->Right(), dt);
		mSkyBox->MoveEntity(mActiveCamera->Right(), 1, dt);
	}
	if ((keyBoard.PageUp || keyBoard.Space) && mActiveCamera->AllowMovement() && keyBoard.LeftControl)
	{
		mActiveCamera->MoveCamera(mActiveCamera->Up(), dt);
		mSkyBox->MoveEntity(mActiveCamera->Up(), 1, dt);
	}
	if ((keyBoard.PageDown || keyBoard.Q) && mActiveCamera->AllowMovement() && keyBoard.LeftControl)
	{
		mActiveCamera->MoveCamera(XMFLOAT3(-mActiveCamera->Up().x, -mActiveCamera->Up().y, -mActiveCamera->Up().z), dt);
		mSkyBox->MoveEntity(XMFLOAT3(-mActiveCamera->Up().x, -mActiveCamera->Up().y, -mActiveCamera->Up().z), 1, dt);
	}

	//Camera rotation controls
	if (keyBoard.W && mActiveCamera->AllowMovement() && !keyBoard.LeftControl)
	{
		mActiveCamera->RotateCamera(XMFLOAT3(-XMConvertToRadians(5), 0, 0), dt);
	}
	if (keyBoard.A && mActiveCamera->AllowMovement() && !keyBoard.LeftControl)
	{
		mActiveCamera->RotateCamera(XMFLOAT3(0, -XMConvertToRadians(5), 0), dt);
	}
	if (keyBoard.S && mActiveCamera->AllowMovement() && !keyBoard.LeftControl)
	{
		mActiveCamera->RotateCamera(XMFLOAT3(XMConvertToRadians(5), 0, 0), dt);
	}
	if (keyBoard.D && mActiveCamera->AllowMovement() && !keyBoard.LeftControl)
	{
		mActiveCamera->RotateCamera(XMFLOAT3(0, XMConvertToRadians(5), 0), dt);
	}

	//Switch between render modes : wire frame, diffuse(non - textured), diffuse(textured), diffuse(textured) with displacement mapping, and toon / cel shading 
	if (keyBoard.F6)
	{

	}

	//Increase/decease pitch of launcher
	if (keyBoard.OemPeriod && keyBoard.LeftShift && !mRocketLaunched)
	{
		mRocket->RotateEntity(XMFLOAT3(0, 0, XMConvertToRadians(-5)), mTimeScale, dt);
		mLauncher->RotateEntity(XMFLOAT3(0, 0, XMConvertToRadians(-5)), mTimeScale, dt);
		mRocketNoseCam->SetLookAt(mRocket->Shapes()[1].WorldPos());
	}
	if (keyBoard.OemComma && keyBoard.LeftShift && !mRocketLaunched)
	{
		mRocket->RotateEntity(XMFLOAT3(0, 0, XMConvertToRadians(5)), mTimeScale, dt);
		mLauncher->RotateEntity(XMFLOAT3(0, 0, XMConvertToRadians(5)), mTimeScale, dt);
		mRocketNoseCam->SetLookAt(mRocket->Shapes()[1].WorldPos());
	}

	//Launch rocket
	if (keyBoard.F11)
	{
		mRocketLaunched = true;
	}
}

/// <summary>
/// Creates all cameras in the scene
/// </summary>
void Scene::CreateCameras()
{
	//Sets fov and aspect ratio for cameras
	const float fov = XMConvertToRadians(60);
	const float aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);

	//Creates adjustable camera looking at launcher (camera 1)
	Camera newCamera(true, XMFLOAT3(0.5f, 8.5f, -1.0f), XMFLOAT3(0, 0, 0), fov, aspectRatio, 0.1f, 100.0f, 1.0f, 10.0f);
	mCameras.push_back(newCamera);

	//Creates adjustable camera looking at terrain surface (camera 2)
	newCamera = Camera(true, XMFLOAT3(10.0f, 22.0f, 2.0f), XMFLOAT3(XMConvertToRadians(90), 0, 0), fov, aspectRatio, 0.1f, 100.0f, 1.0f, 10.0f);
	mCameras.push_back(newCamera);

	//Creates tracking camera looking at rocket (camera 3)
	newCamera = Camera(false, XMFLOAT3(0.5f, 8.2f, 1.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5f, 2.5f, 2.4f), fov, aspectRatio, 0.1f, 100.0f, 1.0f, 10.0f);
	mCameras.push_back(newCamera);

	//Creates tracking camera looking at rocket nose (camera 4)
	newCamera = Camera(false, XMFLOAT3(0.5f, 9.2f, 2.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5f, 3.0f, 2.4f), fov, aspectRatio, 0.1f, 100.0f, 1.0f, 10.0f);
	mCameras.push_back(newCamera);

	//Creates tracking camera looking at rocket body (camera 5)
	newCamera = Camera(false, XMFLOAT3(0.5f, 8.2f, 2.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5f, 2.4f, 2.4f), fov, aspectRatio, 0.1f, 100.0f, 1.0f, 10.0f);
	mCameras.push_back(newCamera);

	//Sets pointers to cameras;
	mActiveCamera = &mCameras[0];
	mRocketCam = &mCameras[2];
	mRocketNoseCam = &mCameras[3];
	mRocketBodyCam = &mCameras[4];
}

/// <summary>
/// Creates terrain made of instanced cubes
/// </summary>
void Scene::CreateTerrain()
{
	//Sets instance data for terrain cubes and creates pointer to it
	vector<InstanceData> instances;
	for (int y = 0; y < 40; y++)
	{
		for (int z = 0; z < 30; z++)
		{
			for (int x = 0; x < 100; x++)
			{
				instances.emplace_back(InstanceData{ XMFLOAT3(x*2, y*2, z*2) });
			}
		}
	}
	vector<InstanceData>* const instancesPointer = &instances;

	//Creates Terrain entity made with an instanced cube shape
	Entity newEntity(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.1f, 0.1f, 0.1f), "Terrain", 0.0f, 0.0f);
	newEntity.AddShape(GeometryType::CUBE, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 1.0f), L"ground_diffuse.dds", L"ground_normal.dds", L"ground_displacement.dds", L"InstancedShader.fx", "TerrainCube", &instances, false);
	mEntities.push_back(newEntity);
}

/// <summary>
/// Spawns new rocket entity
/// </summary>
void Scene::SpawnRocket()
{
	//Rocket
	Entity newEntity (XMFLOAT3(0.5f, 8.2f, 3.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0.1f, 0.1f, 0.1f), "Rocket", 1.5f, 2.0f);
	newEntity.AddShape(GeometryType::CYLINDER, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 4.0f, 1), L"rocket_diffuse.dds", L"rocket_normal.dds", L"rocket_displacement.dds", L"BumpShader.fx", "Cylinder", nullptr, false);
	newEntity.AddShape(GeometryType::CONE, false, XMFLOAT3(0, 6.0f, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 2.0f, 1), L"skymap2.dds", L"", L"", L"ReflectiveShiny.fx", "Cone", nullptr, false);
	mEntities.push_back(newEntity);

	SetEntityPointers();

	//Reset cameras to rockets starting location
	mRocketCam->SetPosition(XMFLOAT3(0.5f, 8.2f, 1.0f));
	mRocketCam->SetLookAt(mRocket->Position());
	mRocketNoseCam->SetPosition(XMFLOAT3(mRocket->Shapes()[1].WorldPos().x + 0.3f, mRocket->Shapes()[1].WorldPos().y + 1.0f, mRocket->Shapes()[1].WorldPos().z - 0.2f));
	mRocketNoseCam->SetLookAt(mRocket->Shapes()[1].WorldPos());
	mRocketBodyCam->SetPosition(XMFLOAT3(0.5f, 8.2f, 2.5f));
	mRocketBodyCam->SetLookAt(mRocket->Shapes()[0].WorldPos());

	//Update skybox
	mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
}

/// <summary>
/// Handles collision logic for the rocket and produces an explosion
/// </summary>
void Scene::RocketCollision()
{
	const XMFLOAT3 conePosF3 = mRocket->Shapes()[1].WorldPos();
	const XMVECTOR conePos = XMLoadFloat3(&conePosF3);
	const XMFLOAT4X4 terrainTransformF4 = mTerrain->Transform();
	const XMMATRIX terrainTransform = XMLoadFloat4x4(&terrainTransformF4);
	vector<InstanceData> terrainCubes = mTerrain->Shapes()[0].Instances();
	vector<InstanceData> cubesToRemove;
	const float collisionRadius = 0.3f;
	const float explosionRadius = 2.0f;
	bool collided = false;

	//Checks for collision
	for (const auto& cube : terrainCubes)
	{
		const XMVECTOR cubePos = XMVector3Transform(XMLoadFloat3(&cube.Pos), terrainTransform);
		XMFLOAT3 distance;
		XMStoreFloat3(&distance, XMVector3Length(cubePos - conePos));
		if (distance.x < collisionRadius)
		{
			collided = true;
			break;
		}
	}

	//Checks to see if cube is within explosion radius
	if (collided == true)
	{
		for (const auto& cube : terrainCubes)
		{
			const XMVECTOR cubePos = XMVector3Transform(XMLoadFloat3(&cube.Pos), terrainTransform);
			XMFLOAT3 distance;
			XMStoreFloat3(&distance, XMVector3Length(cubePos - conePos));
			if (distance.x < explosionRadius)
			{
				cubesToRemove.push_back(cube);
			}
		}
	}

	//If collided, delete cubes in explosion radius and delete rocket
	if (cubesToRemove.size() > 0)
	{
		mEntities.erase(remove(mEntities.begin(), mEntities.end(), *mRocket), mEntities.end());
		mRocket = nullptr;
		mTerrain->RemoveInstancesFromShape("TerrainCube", cubesToRemove);
		mRocketLaunched = false;
		mEngineOn = false;
		SpawnExplosion(conePosF3);
	}
}

/// <summary>
/// Spawns explosion entity
/// </summary>
/// <param name="position">Spawn position of the explosion</param>
void Scene::SpawnExplosion(const XMFLOAT3& position)
{
	//Creates quads for smoke particle system
	vector<InstanceData> smokeInstances;
	for (int i = 0; i < 600; i++)
	{
		smokeInstances.emplace_back(InstanceData{ XMFLOAT3(0, 0, i) });
	}
	const vector<InstanceData>* const smokeInstancesPointer = &smokeInstances;

	//Creates quads for explosion particle system
	vector<InstanceData> explosionInstances;
	for (int i = 0; i < 600; i++)
	{
		explosionInstances.emplace_back(InstanceData{ XMFLOAT3(0, 0, i) });
	}
	vector<InstanceData>* const explosionInstancesPointer = &explosionInstances;

	Entity newEntity(XMFLOAT3(position.x, position.y - 1, position.z), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), "Smoke", 0.0f, 0.0f);
	newEntity.AddShape(GeometryType::QUAD, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), L"smoke.dds", L"", L"", L"SmokeParticle.fx", "SmokeParticle", smokeInstancesPointer, true);
	mEntities.push_back(newEntity);

	newEntity = Entity(XMFLOAT3(position.x, position.y - 1, position.z), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), "Explosion", 0.0f, 0.0f);
	newEntity.AddShape(GeometryType::QUAD, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5f, 0.5f, 0.5f), L"fire.dds", L"", L"", L"ExplosionParticle.fx", "ExplosionParticle", explosionInstancesPointer, true);
	newEntity.AddLight(XMFLOAT4(0.0f, 3.0f, 0.0f, 1.0f), XMFLOAT4(0.9f, 0.3f, 0.2f, 1.0f), "ExplosionLight");
	mEntities.push_back(newEntity);

	mTimeSinceExplosion = 0.0f;

	SetEntityPointers();
}

/// <summary>
/// Sets pointers to entities
/// </summary>
void Scene::SetEntityPointers()
{
	//Set pointers to entities
	for (auto& entity : mEntities)
	{
		if (entity.Name() == "Terrain")
		{
			mTerrain = &entity;
		}
		if (entity.Name() == "Skybox")
		{
			mSkyBox = &entity;
		}
		if (entity.Name() == "Rocket")
		{
			mRocket = &entity;
		}
		if (entity.Name() == "Sun")
		{
			mSun = &entity;
		}
		if (entity.Name() == "Moon")
		{
			mMoon = &entity;
		}
		if (entity.Name() == "Explosion")
		{
			mExplosion = &entity;
		}
		if (entity.Name() == "Launcher")
		{
			mLauncher = &entity;
		}
		if (entity.Name() == "Smoke")
		{
			mSmoke = &entity;
		}
	}
}

/// <summary>
/// Initialises anttweak bars and variables
/// </summary>
void Scene::InitAntTweak()
{
	//Creates anttweak bars
	mAntTweak->AddBar("Stats");
	mAntTweak->AddBar("ActiveCamera");

	//World stats
	mAntTweak->AddVariable("Stats", "TimeScale", TW_TYPE_FLOAT, &mTimeScale, "step=0.1 group=WorldStats");
	mAntTweak->AddVariable("Stats", "CubeCount", TW_TYPE_FLOAT, &mCubeCount, "group=WorldStats");

	//Rocket
	mAntTweak->AddVariable("Stats", "Rocket", TW_TYPE_DIR3F, &mRocketDir, "");
	mAntTweak->AddVariable("Stats", "RocketX", TW_TYPE_FLOAT, &mRocketPos.x, "group=Rocket");
	mAntTweak->AddVariable("Stats", "RocketY", TW_TYPE_FLOAT, &mRocketPos.y, "group=Rocket");
	mAntTweak->AddVariable("Stats", "RocketZ", TW_TYPE_FLOAT, &mRocketPos.z, "group=Rocket");
	mAntTweak->AddVariable("Stats", "RocketSpeed", TW_TYPE_FLOAT, &mRocketSpeed, "step=0.1 group=Rocket min = 0 max = 5");
	mAntTweak->AddVariable("Stats", "RocketRotationSpeed", TW_TYPE_FLOAT, &mRocketRotationSpeed, "step=0.1 group=Rocket min=0 max=10");

	//Sun
	mAntTweak->AddVariable("Stats", "Sun", TW_TYPE_COLOR4F, &mSunColour, "");
	mAntTweak->AddVariable("Stats", "SunX", TW_TYPE_FLOAT, &mSunPos.x, "group=Sun");
	mAntTweak->AddVariable("Stats", "SunY", TW_TYPE_FLOAT, &mSunPos.y, "group=Sun");
	mAntTweak->AddVariable("Stats", "SunZ", TW_TYPE_FLOAT, &mSunPos.z, "group=Sun");
	mAntTweak->AddVariable("Stats", "SunRotationSpeed", TW_TYPE_FLOAT, &mSunRotationSpeed, "step=0.1 group=Sun min=0 max=10");

	//Moon
	mAntTweak->AddVariable("Stats", "Moon", TW_TYPE_COLOR4F, &mMoonColour, "");
	mAntTweak->AddVariable("Stats", "MoonX", TW_TYPE_FLOAT, &mMoonPos.x, "group=Moon");
	mAntTweak->AddVariable("Stats", "MoonY", TW_TYPE_FLOAT, &mMoonPos.y, "group=Moon");
	mAntTweak->AddVariable("Stats", "MoonZ", TW_TYPE_FLOAT, &mMoonPos.z, "group=Moon");
	mAntTweak->AddVariable("Stats", "MoonRotationSpeed", TW_TYPE_FLOAT, &mMoonRotationSpeed, "step=0.1 group=Moon min=0 max=10");

	//Camera
	mAntTweak->AddVariable("ActiveCamera", "CameraDir", TW_TYPE_DIR3F, &mCameraDir, "");
	mAntTweak->AddVariable("ActiveCamera", "CameraUpDir", TW_TYPE_DIR3F, &mCameraUpDir, "");
	mAntTweak->AddVariable("ActiveCamera", "CameraRightDir", TW_TYPE_DIR3F, &mCameraRightDir, "");
	mAntTweak->AddVariable("ActiveCamera", "CameraX", TW_TYPE_FLOAT, &mCameraPos.x, "group=CamStats");
	mAntTweak->AddVariable("ActiveCamera", "CameraY", TW_TYPE_FLOAT, &mCameraPos.y, "group=CamStats");
	mAntTweak->AddVariable("ActiveCamera", "CameraZ", TW_TYPE_FLOAT, &mCameraPos.z, "group=CamStats");
	mAntTweak->AddVariable("ActiveCamera", "CameraSpeed", TW_TYPE_FLOAT, &mCameraSpeed, "step=0.1 group=CamStats min=0 max=5");
	mAntTweak->AddVariable("ActiveCamera", "CameraRotationSpeed", TW_TYPE_FLOAT, &mCameraRotationSpeed, "step=0.1 group=CamStats min=0 max=20");

	//Initialises initial values of modifiable anttweak variables
	mRocketSpeed = mRocket->Speed();
	mRocketRotationSpeed = mRocket->RotationSpeed();
	mSunColour = mSun->Lights()[0].Colour();
	mSunRotationSpeed = mSun->RotationSpeed();
	mMoonColour = mMoon->Lights()[0].Colour();
	mMoonRotationSpeed = mSun->RotationSpeed();
	mCameraSpeed = mActiveCamera->Speed();
	mCameraRotationSpeed = mActiveCamera->RotationSpeed();
}

/// <summary>
/// Updates anttweak variables
/// </summary>
void Scene::UpdateAntTweak()
{
	//Cube count
	mCubeCount = mTerrain->Shapes()[0].Instances().size();

	//Checks if rocket is not null
	if (mRocket)
	{
		//Rocket
		mRocketDir = mRocket->Up();
		mRocketPos = mRocket->Position();
		mRocket->SetSpeed(mRocketSpeed);
		mRocket->SetRotationSpeed(mRocketRotationSpeed);
	}
	//Sun
	mSunPos = mSun->Position();
	mSun->ChangeLightColour("SunLight", mSunColour);
	mSun->SetRotationSpeed(mSunRotationSpeed);

	//Moon
	mMoonPos = mMoon->Position();
	mMoon->ChangeLightColour("MoonLight", mMoonColour);
	mMoon->SetRotationSpeed(mMoonRotationSpeed);

	//Camera
	mCameraDir = mActiveCamera->Forward();
	mCameraUpDir = mActiveCamera->Up();
	mCameraRightDir = mActiveCamera->Right();
	mCameraPos = mActiveCamera->Position();
	mActiveCamera->SetSpeed(mCameraSpeed);
	mActiveCamera->SetRotationSpeed(mCameraRotationSpeed);
}

/// <summary>
/// Animates rocket and adjusts tracking cameras following the rocket
/// </summary>
void Scene::AnimateRocket(const float& dt)
{
	if (!mEngineOn)
	{
		//Creates 50 quads for particle system
		vector<InstanceData> instances;
		for (int i = 0; i < 200; i++)
		{
			instances.emplace_back(InstanceData{ XMFLOAT3(0, 0, i) });
		}
		vector<InstanceData>* const instancesPointer = &instances;

		//Adds particle system and light to back of rocket
		mRocket->AddShape(GeometryType::QUAD, false, XMFLOAT3(0, -4.0f, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1.0f, 1), L"fire.dds", L"", L"", L"EngineParticle.fx", "RocketEngineFlame", instancesPointer, true);
		mRocket->AddLight(XMFLOAT4(0, -4.5f, 0, 1.0f), XMFLOAT4(0.5f, 0.3f, 0.2f, 1.0f), "RocketEngineLight");
		mEngineOn = true;
	}

	//Move rocket
	mRocket->MoveEntity(mRocket->Up(), mTimeScale, dt);

	//Clamps rotation from going more than 180 degrees
	if (mRocket->Up().y >= -0.999f)
	{
		//Rotate rocket
		mRocket->RotateEntity(XMFLOAT3(0, 0, XMConvertToRadians(-5)), mTimeScale, dt);
	}

	//Move cameras and cameras look at
	mRocketCam->MoveCamera(mRocket->Up(), mTimeScale * mRocket->Speed(), dt);
	mRocketCam->SetLookAt(mRocket->Position());
	mRocketNoseCam->SetPosition(XMFLOAT3(mRocket->Shapes()[1].WorldPos().x + 0.3f, mRocket->Shapes()[1].WorldPos().y + 1.0f, mRocket->Shapes()[1].WorldPos().z - 0.2f));
	mRocketNoseCam->SetLookAt(mRocket->Shapes()[1].WorldPos());
	mRocketBodyCam->MoveCamera(mRocket->Up(), mTimeScale * mRocket->Speed(), dt);
	mRocketBodyCam->SetLookAt(mRocket->Shapes()[0].WorldPos());

	//Update skybox
	mSkyBox->SetPosition(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z));
}

/// <summary>
/// Default constructor
/// </summary>
Scene::Scene()
{
}

/// <summary>
/// Constructs a Scene object with the given values
/// </summary>
/// <param name="width">Width of the windows window</param>
/// <param name="height">Height of the windows window</param>
/// <param name="antTweakManager">Reference to the anttweak manager</param>
Scene::Scene(const int& width, const int& height, AntTweakManager& antTweakManager) :
	mTimeScale(1.0f),
	mWidth(width),
	mHeight(height),
	mExitApplication(false),
	mRocketLaunched(false),
	mEngineOn(false),
	mReset(false),
	mAntTweak(&antTweakManager)
{
}

/// <summary>
/// Default destructor
/// </summary>
Scene::~Scene()
{
}

/// <summary>
/// Initialises all objects required at the beginning of the scene eg. entities, shapes, lights and cameras
/// </summary>
void Scene::InitScene()
{
	//Creates instance of keyboard object for input handling
	if (!mKeyboard)
	{
		mKeyboard = std::make_unique<Keyboard>();
	}

	//Clears all contents of lists for when resetting state of scene
	if (mCameras.size() > 0)
	{
		mCameras.clear();
	}
	if (mEntities.size() > 0)
	{
		mEntities.clear();
	}

	//Reserves memory for entities vector so adding/removing entities doesn't invalidate pointers
	mEntities.reserve(10);

	//Create Cameras
	CreateCameras();

	//Creates terrain
	CreateTerrain();

	//Creates sun entity
	Entity newEntity(XMFLOAT3(-20, 3.2f, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), "Sun", 1.0f, 2.0f);
	newEntity.AddShape(GeometryType::CUBE, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.05f, 0.05f, 0.05f), L"fire.dds", L"", L"", L"EffectShader.fx", "SunCube", nullptr, false);
	newEntity.AddLight(XMFLOAT4(0.0f, 0, 0.0f, 1.0f), XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f), "SunLight");
	mEntities.push_back(newEntity);

	//Creates moon entity
	newEntity = Entity(XMFLOAT3(40, 3.2f, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), "Moon", 1.0f, 2.0f);
	newEntity.AddShape(GeometryType::CUBE, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.05f, 0.05f, 0.05f), L"smoke.dds", L"", L"", L"EffectShader.fx", "MoonCube", nullptr, false);
	newEntity.AddLight(XMFLOAT4(0.0f, 0, 0.0f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.4f, 1.0f), "MoonLight");
	mEntities.push_back(newEntity);

	//Skybox
	newEntity = Entity(XMFLOAT3(mActiveCamera->Position().x, mActiveCamera->Position().y, mActiveCamera->Position().z), XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 1.0f), "Skybox", 1.0f, 0.0f);
	newEntity.AddShape(GeometryType::CUBE, true, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), L"skymap2.dds", L"", L"", L"SkyBoxShader.fx", "Skybox", nullptr, false);
	mEntities.push_back(newEntity);

	//Creates launcher
	newEntity = Entity(XMFLOAT3(0.5f, 8.2f, 3.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), "Launcher", 1.0f, 2.0f);
	newEntity.AddShape(GeometryType::CUBE, false, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.2f, 0.5f, 0.2f), L"launcher_diffuse.dds", L"launcher_normal.dds", L"launcher_displacement.dds", L"BumpShader.fx", "LauncherCube", nullptr, false);
	mEntities.push_back(newEntity);

	//Spawns rocket
	SpawnRocket();

	//Sets entity pointers
	SetEntityPointers();

	if (mReset != true)
	{
#ifndef _DEBUG
		//Initialise AntTweak values
		InitAntTweak();
#endif
	}
}

/// <summary>
/// Update loop, manages all movement of objects and spawning of new objects during runtime
/// </summary>
/// <param name="dt">Delta time</param>
void Scene::Update(const float& dt)
{
	//Calls input method that handles all user input
	Input(dt);

#ifndef _DEBUG
	//Update AntTweak values
	UpdateAntTweak();
#endif

	//Animates sun
	if (mSun)
	{
		mSun->RotateAroundPoint(XMFLOAT3(10.0f, 3.2f, 0.0f), XMFLOAT3(0, 0, XMConvertToRadians(-5)), mTimeScale, dt);
	}

	//Animates moon
	if (mMoon)
	{
		mMoon->RotateAroundPoint(XMFLOAT3(10.0f, 3.2f, 0.0f), XMFLOAT3(0, 0, XMConvertToRadians(-5)), mTimeScale, dt);
	}

	//Animates rocket if rocket has been launced
	if (mRocketLaunched && mRocket)
	{
		AnimateRocket(dt);
	}

	//Checks if rocket exists
	if (mRocket)
	{
		RocketCollision();
	}
	else
	{
		mLauncher->ResetRotation();
		SpawnRocket();
	}

	//Despawns explosion after time
	if (mSmoke)
	{
		mTimeSinceExplosion += (dt * mTimeScale);
		if (mTimeSinceExplosion >= 3.0f && mExplosion)
		{
			mEntities.erase(remove(mEntities.begin(), mEntities.end(), *mExplosion), mEntities.end());
			mExplosion = nullptr;
			SetEntityPointers();
		}
		if (mTimeSinceExplosion >= 6.0f)
		{
			mEntities.erase(remove(mEntities.begin(), mEntities.end(), *mSmoke), mEntities.end());
			mSmoke = nullptr;
			SetEntityPointers();
		}
	}
}

/// <summary>
/// Get method for the active camera
/// </summary>
/// <returns>Pointer to the active camera of the scene</returns>
const Camera* const Scene::ActiveCamera() const
{
	return mActiveCamera;
}

/// <summary>
/// Get method for scenes entity list
/// </summary>
/// <returns>List of entities in the scene</returns>
const std::vector<Entity>& Scene::Entities() const
{
	return mEntities;
}

/// <summary>
/// Get method for scenes exit application bool
/// </summary>
/// <returns>Bool that determines whether or not to exit the application</returns>
const bool & Scene::ExitApplication() const
{
	return mExitApplication;
}


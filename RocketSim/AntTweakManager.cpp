#include "AntTweakManager.h"

using namespace std;

/// <summary>
/// Default constructor
/// </summary>
AntTweakManager::AntTweakManager()
{
}

/// <summary>
/// Initialises anttweak GUI
/// </summary>
/// <param name="graphicsAPI">Graphics API used in application</param>
/// <param name="dXDevice">Pointer to directX device</param>
/// <param name="width">Width of window</param>
/// <param name="height">Height of window</param>
void AntTweakManager::Init(const TwGraphAPI& graphicsAPI, ID3D11Device* const dXDevice, const int& width, const int& height) const
{
	TwInit(graphicsAPI, dXDevice);
	TwWindowSize(width, height);
}

/// <summary>
/// Adds a bar to the anttweak GUI
/// </summary>
/// <param name="barName">Name of bar to be added</param>
void AntTweakManager::AddBar(const string& barName)
{
	TwBar* newBar = TwNewBar(barName.c_str());
	mBars.insert(pair<string, TwBar*>(barName, newBar));
}

/// <summary>
/// Adds a variable to the anttweak GUI
/// </summary>
/// <param name="barName">Name of the bar to hold the variable</param>
/// <param name="variableName">Name of the variable to be added</param>
/// <param name="variableType">Type of the variable to be added</param>
/// <param name="variable">Pointer to the variable to be added</param>
/// <param name="behaviourDefinition">Anttweak behaviour definition string</param>
void AntTweakManager::AddVariable(const string& barName, const string& variableName, const TwType& variableType, const void* const variable, const string& behaviourDefinition)
{
	auto it = mBars.find(barName);
	TwAddVarRW(it->second, variableName.c_str(), variableType, const_cast<void*>(variable), behaviourDefinition.c_str());
}

/// <summary>
/// Deletes a bar from the anttweak GUI
/// </summary>
/// <param name="barName">Name of bar to be deleted</param>
void AntTweakManager::DeleteBar(const string& barName)
{
	auto it = mBars.find(barName);
	TwDeleteBar(it->second);
	mBars.erase(it);
}

/// <summary>
/// Refreshes and draws the anttweak GUI
/// </summary>
void AntTweakManager::Draw()
{
	for (auto& bar : mBars)
	{
		TwRefreshBar(bar.second);
	}
	TwDraw();
}

/// <summary>
/// Deletes all bars and de-allocates all memory assigned to anttweak
/// </summary>
void AntTweakManager::Cleanup() const
{
	TwDeleteAllBars();
	TwTerminate();
}

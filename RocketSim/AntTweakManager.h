#pragma once

#include <AntTweakBar.h>
#include <d3d11_1.h>
#include <map>
#include <string>

class AntTweakManager
{
private:
	std::map<std::string, TwBar*> mBars;
public:
	AntTweakManager();

	void Init(const TwGraphAPI& graphicsAPI, ID3D11Device* const dXDevice, const int& width, const int& height) const;
	void AddBar(const std::string& barName);
	void AddVariable(const std::string& barName, const std::string& variableName, const TwType& variableType, const void* const variable, const std::string& behaviourDefinition);
	void DeleteBar(const std::string& barName);
	void Draw();
	void Cleanup() const;
};
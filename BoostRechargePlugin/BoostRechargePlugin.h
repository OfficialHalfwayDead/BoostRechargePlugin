#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
#pragma comment( lib, "bakkesmod.lib" )

class BoostRechargePlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::shared_ptr<bool> isOn;
	std::shared_ptr<float> interval;
	std::shared_ptr<float> gameSpeed;
	std::shared_ptr<float> boostAmount;


public:
	BoostRechargePlugin() = default;
	~BoostRechargePlugin() = default;
	void onLoad();
	void onUnload();
	void Enable();
	void Tick();
};
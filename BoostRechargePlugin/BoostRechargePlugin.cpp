#include "BoostRechargePlugin.h"

BAKKESMOD_PLUGIN(BoostRechargePlugin, "Dribble in a circle plugin.", "1.0", PLUGINTYPE_FREEPLAY)

void BoostRechargePlugin::onLoad()
{
	isOn = std::make_shared<bool>(false);
	interval = std::make_shared<float>(0.5);
	gameSpeed = std::make_shared<float>(1);
	boostAmount = std::make_shared<float>(3.f);

	cvarManager->registerCvar("boost_recharge", "0", "Turns the boost recharge mod to recharge the set amount of boost at set interval.", true, true, 0.0f, true, 1.0f, false).bindTo(isOn);
	cvarManager->registerCvar("boost_recharge_interval", "0.5", "The interval between recharge triggers in seconds.", true, true, 0.01f, false, 0.0f, true).bindTo(interval);
	cvarManager->registerCvar("boost_recharge_amount", "3", "The % of boost that is to be restored every interval tick.", true, true, 0.0f, true, 100.0f, true).bindTo(boostAmount);


	cvarManager->getCvar("boost_recharge").addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) { if (cvar.getBoolValue()) this->Enable(); });
	cvarManager->getCvar("sv_soccar_gamespeed").bindTo(gameSpeed);

}

void BoostRechargePlugin::onUnload()
{
}

void BoostRechargePlugin::Enable()
{
	if (*isOn) 
	{

		gameWrapper->SetTimeout(std::bind(&BoostRechargePlugin::Tick, this), (*interval) / (*gameSpeed) );
	}
}

void BoostRechargePlugin::Tick()
{
	if (!gameWrapper->IsInFreeplay() || !(*isOn))
	{
		cvarManager->getCvar("boost_recharge").setValue(false);
		return;
	}


	auto server = gameWrapper->GetGameEventAsServer();
	if (server.IsNull())
	{
		cvarManager->getCvar("boost_recharge").setValue(false);
		return;
	}
	
	auto cars = server.GetCars();
	for (int i = 0; i < cars.Count(); i++)
	{
		auto car = cars.Get(i);
		if (car.IsNull()) // maybe somehow still possible
		{
			cvarManager->getCvar("boost_recharge").setValue(false);
			return;
		}

		auto boost = car.GetBoostComponent();
		if (boost.IsNull())
		{
			cvarManager->getCvar("boost_recharge").setValue(false);
			return;
		}
		if (boost.GetUnlimitedBoostRefCount())
		{
			boost.SetUnlimitedBoost2(false);
		}
		boost.GiveBoost2(*boostAmount / 100.f);
	}

	gameWrapper->SetTimeout(std::bind(&BoostRechargePlugin::Tick, this), (*interval) / (*gameSpeed));
}

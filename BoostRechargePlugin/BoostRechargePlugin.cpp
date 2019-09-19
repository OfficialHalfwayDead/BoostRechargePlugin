#include "BoostRechargePlugin.h"

BAKKESMOD_PLUGIN(BoostRechargePlugin, "Boost Recharge Plugin", "1.2", PLUGINTYPE_FREEPLAY)

void BoostRechargePlugin::onLoad()
{
	isOn = std::make_shared<bool>(false);
	onlyGround = std::make_shared<bool>(false);
	interval = std::make_shared<float>(0.5);
	gameSpeed = std::make_shared<float>(1);
	boostAmount = std::make_shared<float>(3.f);
	boostLimit = std::make_shared<float>(100.f);

	cvarManager->registerCvar("boost_recharge", "0", "Turns the boost recharge mod to recharge the set amount of boost at set interval.", true, true, 0.0f, true, 1.0f, true).bindTo(isOn);
	cvarManager->registerCvar("boost_recharge_max", "100", "The maximum value to which to recharge boost.", true, true, 0.0f, false, 100.0f, true).bindTo(boostLimit);
	cvarManager->registerCvar("boost_recharge_interval", "0.5", "The interval between recharge triggers in seconds.", true, true, 0.01f, false, 0.0f, true).bindTo(interval);
	cvarManager->registerCvar("boost_recharge_amount", "3", "The % of boost that is to be restored every interval tick.", true, true, -100.0f, true, 100.0f, true).bindTo(boostAmount);
	cvarManager->registerCvar("boost_recharge_groundonly", "0", "Only recharge boost while on ground.", true, true, 0.0f, true, 1.0f, true).bindTo(onlyGround);


	cvarManager->getCvar("boost_recharge").addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) { if (cvar.getBoolValue()) this->Enable(); });
	cvarManager->getCvar("sv_soccar_gamespeed").bindTo(gameSpeed);

	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.OnInit", bind(&BoostRechargePlugin::Enable, this));

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
		//cvarManager->getCvar("boost_recharge").setValue(false);
		return;
	}


	auto server = gameWrapper->GetGameEventAsServer();
	if (server.IsNull())
	{
		//cvarManager->getCvar("boost_recharge").setValue(false);
		return;
	}
	
	auto cars = server.GetCars();
	for (int i = 0; i < cars.Count(); i++)
	{
		auto car = cars.Get(i);
		if (car.IsNull()) // maybe somehow still possible
		{
			//cvarManager->getCvar("boost_recharge").setValue(false);
			return;
		}

		auto boost = car.GetBoostComponent();
		if (boost.IsNull())
		{
			//cvarManager->getCvar("boost_recharge").setValue(false);
			return;
		}
		if (boost.GetUnlimitedBoostRefCount())
		{
			boost.SetUnlimitedBoost2(false);
		}
		if (!(*onlyGround) || car.IsOnGround() || car.IsOnWall())
		{
			float max_recharge = fmax(0.0f, (*boostLimit / 100.f) - boost.GetCurrentBoostAmount());
			boost.GiveBoost2(fmin(*boostAmount / 100.f, max_recharge));
		}
			
	}

	gameWrapper->SetTimeout(std::bind(&BoostRechargePlugin::Tick, this), (*interval) / (*gameSpeed));
}

#pragma once

#include "ForceEnums.hpp"
#include "ErrorValues.h"




//Inspired by Dwarf Fortress

//enum WoundTypes::EnWoundTypes; //Forward declaration of woundtypes

/*
Tells us how a material and bodypart is morphed by the force
This stores the force we start off with, the force we end with, and the kind of effect it has on a material
The AppliedForceEffect class does not do any calculations. It's main purpose is to store data. 

WoundTypes::WoundtypeMapping possibleWoundTypes will be used by the WoundHandler to determine what kind of wound to apply to the body part

In summary: StartForce gets absorbed, leaving an endforce. Based on the material its applied to, it can cause a deformation, fracture, or no effect
The possibleWoundTypes are the ones that will be applied to a bodypart
*/
class AppliedForceEffect
{

private:
	float forceAbsorption; //How much of the total force is absorbed by the material
	float startForce;
	float endForce;
	ForceEffecectTypes::MaterialEffectType effect;
	AppliedForceTypes::AppliedForceType appliedForceType;
	float damageAmount;
	


public:

	AppliedForceEffect();
	AppliedForceEffect(float initialForce);
	void setStartForce(float force);
	void calcEndForce(float force);
	void setMaterialEffect(ForceEffecectTypes::MaterialEffectType _effect);
	void setAppliedForceType(AppliedForceTypes::AppliedForceType _appliedForceType);
	void setDamageAmount(int _damageAmount);



	AppliedForceTypes::AppliedForceType getAppliedForceType();
	ForceEffecectTypes::MaterialEffectType getMaterialEffectType();
	float getStartForce();
	float getEndForce();
	float getDamageAmount();

};



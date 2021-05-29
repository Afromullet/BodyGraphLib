#include "AppliedForceEffect.hpp"



AppliedForceEffect::AppliedForceEffect()
{
	forceAbsorption =  ERROR_NUMBER; //How much of the total force is absorbed by the material
	startForce = ERROR_NUMBER;
	endForce = ERROR_NUMBER;
	appliedForceType = AppliedForceTypes::enNoForceApplied;
	damageAmount = 1;
}

AppliedForceEffect::AppliedForceEffect(float initialForce)
{
	startForce = initialForce;
	endForce = ERROR_NUMBER;
	forceAbsorption = ERROR_NUMBER;
	appliedForceType = AppliedForceTypes::enNoForceApplied;
	damageAmount = 1;
}

//When we know the endforce, we also know how much of the force is being absorbed, so we calculate the forceAbsorption whenever the endForce is set. 
void AppliedForceEffect::calcEndForce(float force)
{ 
	endForce = force; 

	//So we don't have division by zero. 
	if (startForce == 0)
	{
		startForce = 1;
	}
	forceAbsorption = endForce / startForce;
}

void AppliedForceEffect::setStartForce(float force)
{ 
	startForce = force; 
	endForce = ERROR_NUMBER; //With a new startForce, there's no reason to keep an existing endForce since it should be recalculated. Indicates there is an error if it's used before recalculation
}

void AppliedForceEffect::setMaterialEffect(ForceEffecectTypes::MaterialEffectType _effect) 
{ 
	effect = _effect; 
}

void AppliedForceEffect::setAppliedForceType(AppliedForceTypes::AppliedForceType _appliedForceType)
{
	appliedForceType = _appliedForceType;
}

void AppliedForceEffect::setDamageAmount(int _damageAmount)
{
	damageAmount = _damageAmount;
}

AppliedForceTypes::AppliedForceType AppliedForceEffect::getAppliedForceType()
{
	return appliedForceType;
}

ForceEffecectTypes::MaterialEffectType AppliedForceEffect::getMaterialEffectType()
{
	return effect;
}

float AppliedForceEffect::getStartForce()
{ 
	return startForce; 
}

float AppliedForceEffect::getEndForce() 
{ 
	return endForce; 
}

float AppliedForceEffect::getDamageAmount()
{
	return damageAmount;
}


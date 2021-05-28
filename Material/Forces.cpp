#include "Forces.hpp"

ApplyingForce::ApplyingForce() : initialForce(1) 
{
	
}

ApplyingForce::ApplyingForce(float _contactArea, float _initialForce, AppliedForceTypes::AppliedForceType _appliedForceType)
	: contactArea(_contactArea), initialForce(_initialForce),appliedForceType(_appliedForceType)
{
}

float ApplyingForce::getInitialForce() 
{ 
	return initialForce; 
}

void ApplyingForce::setInitialForce(float _initialForce) 
{ 
	initialForce = _initialForce;
}

void ApplyingForce::setAppliedForceType(AppliedForceTypes::AppliedForceType _appliedForceType)
{
	appliedForceType = _appliedForceType;
}

AppliedForceTypes::AppliedForceType ApplyingForce::getAppliedForceType () const
{ 
	return appliedForceType; 
}

float ApplyingForce::getForceApplied()
{
	float finalForce;
	finalForce = initialForce * size;
	return finalForce;
}

ResistingForce::ResistingForce()
{
	density = 1;
}

ResistingForce::ResistingForce(float _initialForce, float _density) : initialResistingForce(_initialForce),density(_density)
{

}

float ResistingForce::getForceAbsorbed(float forceApplied)
{ 
	return 	forceApplied / (initialResistingForce * density);

}

void ResistingForce::setInitialResistingForce(float _initialResistingForce)
{
	initialResistingForce = _initialResistingForce;
}

void ResistingForce::setDensity(float _density)
{
	density = _density;
}

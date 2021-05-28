#pragma once

#include "ForceEnums.hpp"


/*
This is the thing that applies force. This is the abstract base class..The Applying Force might have different parameters we want to use, so leave it to the user
The idea behind subclassing is to implement more datafields that are used in the calculation. A BasicAppliedForce for example uses size and contact area.
The getForceApplied virtual method will use these data fields to calculate the force applied. 

*/
class ApplyingForce
{

private:
	AppliedForceTypes::AppliedForceType appliedForceType;
	float size;
	float contactArea;
	

protected:
	float initialForce;
	

public:
	ApplyingForce();
	ApplyingForce(float _contactArea, float _initialForce, AppliedForceTypes::AppliedForceType _appliedForceType);
	
	float getForceApplied();
	float getInitialForce();
	AppliedForceTypes::AppliedForceType getAppliedForceType() const;

	void setInitialForce(float _initialForce);
	void setAppliedForceType(AppliedForceTypes::AppliedForceType _appliedForceType);
};


/*
This is to handle how much force is resisted. I.E, material strength will be copied directly into initialResisting Force. A basicResisting force currently uses density in the calculation
*/
class ResistingForce
{

protected:
	float initialResistingForce;
	float density;

public:


	ResistingForce();
	ResistingForce(float initialForce, float _density);

	float getForceAbsorbed(float forceApplied) ;
	
	void setInitialResistingForce(float _initialResistingForce);
	void setDensity(float _density);
};




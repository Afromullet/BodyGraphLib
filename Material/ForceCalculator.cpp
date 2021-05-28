#pragma once

#include "ForceCalculator.hpp"
#include "ErrorValues.h"

/*
Takes as input an applying and resisting force and determines whether the force is enough to deform or fracture a material. 
AppliedForceEffect handles the calculations, using the applyingForce and resistingForce. The remaining force (endForce) is compared to the  material strength to determine what effect applies
The end force has to be greater than the deformation strength or fracture strength for the associated effect to occur
*/
AppliedForceEffect ApplyForceToMaterial(float materialDeformationStrength, float materialFractureStrength, ApplyingForce applyingForce, ResistingForce resistingForce)
{
	float forceApplied, forceAbsorbed;
	AppliedForceEffect forceEffect;
	
	forceEffect.setStartForce(applyingForce.getInitialForce());
	forceEffect.calcEndForce(resistingForce.getForceAbsorbed(forceEffect.getStartForce())); //Tells us how much of the force remains after whatever absorption happens

	if (forceEffect.getEndForce() > materialDeformationStrength)
	{
		
		//Calling the getter twice for setMaterialEffect and setAppliedForceType rather than creating local variables inside the conditional. Shouldn't really be a problem.   
		if (forceEffect.getEndForce()  > materialFractureStrength)
		{
			forceEffect.setMaterialEffect(ForceEffecectTypes::getFractureEffectType(applyingForce.getAppliedForceType()));
			forceEffect.setAppliedForceType(applyingForce.getAppliedForceType());
			
		}
		else
		{
			forceEffect.setMaterialEffect(ForceEffecectTypes::getDeformationEffectType(applyingForce.getAppliedForceType()));
			forceEffect.setAppliedForceType(applyingForce.getAppliedForceType());
		}
	}
	else
	{
		forceEffect.setMaterialEffect(ForceEffecectTypes::enNoEffect);
	}
	return forceEffect;
}

/*
This is used to handle force propagation. The ApplyForceToMaterial with an ApplyingForce calculates the initial force absorption, leaving us with an AppliedForceEffect
The intention with this ApplyForceToMaterial is to take that force and then apply it to the next resisting force.
Because we're dealing with applying the force to multiple layers, the AppliedForceEffect in this function uses the endForce of the input applyingForce, since that's what is remaining after
Absorption by previous layers. 
*/
AppliedForceEffect ApplyForceToMaterial(float materialDeformationStrength, float materialFractureStrength, AppliedForceEffect appliedForceEffect, ResistingForce resistingForce)
{


	if (appliedForceEffect.getAppliedForceType() == AppliedForceTypes::enNoForceApplied)
	{
		appliedForceEffect.setMaterialEffect(ForceEffecectTypes::enNoEffect);
		appliedForceEffect.setAppliedForceType(AppliedForceTypes::enNoForceApplied);
		appliedForceEffect.setStartForce(ERROR_NUMBER);
		

	}

	AppliedForceEffect forceEffect(appliedForceEffect.getEndForce());
	forceEffect.setAppliedForceType(appliedForceEffect.getAppliedForceType());
	forceEffect.calcEndForce(resistingForce.getForceAbsorbed(forceEffect.getStartForce()));

	if (forceEffect.getEndForce() > materialDeformationStrength)
	{
		if (forceEffect.getEndForce() > materialFractureStrength)
		{
			forceEffect.setMaterialEffect(ForceEffecectTypes::getFractureEffectType(appliedForceEffect.getAppliedForceType()));

		}
		else
		{
			forceEffect.setMaterialEffect(ForceEffecectTypes::getDeformationEffectType(appliedForceEffect.getAppliedForceType()));
		}
	}
	else
	{
		forceEffect.setMaterialEffect(ForceEffecectTypes::enNoEffect);
	}

	return forceEffect;


}

/*
The force calculator for when there is no resisting force
*/
AppliedForceEffect ApplyForceWithNoMaterial(ApplyingForce applyingForce)
{
	AppliedForceEffect forceEffect;

	//Since there's nothing to be absorbed, the start and end force are the same. 
	forceEffect.setAppliedForceType(applyingForce.getAppliedForceType());
	forceEffect.setStartForce(applyingForce.getInitialForce());
	forceEffect.calcEndForce(applyingForce.getInitialForce()); 
	forceEffect.setMaterialEffect(ForceEffecectTypes::enNoEffect);
	return forceEffect;
}

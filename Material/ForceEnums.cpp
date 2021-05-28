#include "ForceEnums.hpp"	


/*
The enumerated return type let's us identify how a material is deformed when a given force is applied
Doesn't do any calculations for determining how the deformation occurs. Just gives an indication of what kind of deformation occurs
*/
ForceEffecectTypes::MaterialEffectType ForceEffecectTypes::getDeformationEffectType(AppliedForceTypes::AppliedForceType appliedForceType)
{

	if (appliedForceType == AppliedForceTypes::enCompression)
	{
		return ForceEffecectTypes::enCompDefEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enImpact)
	{
		return ForceEffecectTypes::enImpactDefEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enTensile)
	{
		return ForceEffecectTypes::enTensileDefEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enTorsion)
	{
		return ForceEffecectTypes::enTorsionDefEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enShear)
	{
		return ForceEffecectTypes::enShearDefEffect;
	}
	else
	{
		return ForceEffecectTypes::enNoEffect;
	}
}

/*
The enumerated return type let's us identify how a material is fractured when a given force is applied
Doesn't do any calculations for determining how the fracturing occurs. Just gives an indication of what kind of fracturing occurs

*/
ForceEffecectTypes::MaterialEffectType ForceEffecectTypes::getFractureEffectType(AppliedForceTypes::AppliedForceType appliedForceType)
{
	if (appliedForceType == AppliedForceTypes::enCompression)
	{
		return ForceEffecectTypes::enCompFracEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enImpact)
	{
		return ForceEffecectTypes::enImpactFracEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enTensile)
	{
		return ForceEffecectTypes::enTensileFracEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enTorsion)
	{
		return ForceEffecectTypes::enTorsionFracEffect;
	}
	else if (appliedForceType == AppliedForceTypes::enShear)
	{
		return ForceEffecectTypes::enShearFracEffect;
	}
	else
	{
		return ForceEffecectTypes::enNoEffect;
	}

}


 
/*
Determines whether AppliedForceType is a fracture type
*/
bool ForceEffecectTypes::isFractureEffectType(ForceEffecectTypes::MaterialEffectType materialEffectType)
{
	
	bool retval =    materialEffectType == ForceEffecectTypes::enCompFracEffect
		|| materialEffectType ==  ForceEffecectTypes::enImpactFracEffect || 
		materialEffectType ==  ForceEffecectTypes::enTensileFracEffect 
		|| materialEffectType ==  ForceEffecectTypes::enTorsionFracEffect || 
		materialEffectType ==  ForceEffecectTypes::enShearFracEffect 
		&& (materialEffectType != ForceEffecectTypes::enNoEffect);

	return retval;
}


/*
Determines whether AppliedForceType is a deformation type
*/
bool ForceEffecectTypes::isDeformationEffectType(ForceEffecectTypes::MaterialEffectType materialEffectType)
{

	return materialEffectType ==  ForceEffecectTypes::enCompDefEffect | materialEffectType ==  ForceEffecectTypes::enImpactDefEffect |
		materialEffectType == ForceEffecectTypes::enTensileDefEffect | materialEffectType ==  ForceEffecectTypes::enTorsionDefEffect | 
		materialEffectType ==  ForceEffecectTypes::enShearDefEffect;


}


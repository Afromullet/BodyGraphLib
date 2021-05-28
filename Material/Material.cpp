//
//  Material.cpp
//  Colony
//
//  Created by Sean on 3/31/18.
//  Copyright © 2018 Afromullet. All rights reserved.
//

#include "Material.hpp"

/*
Used to assign the material deformation strength
*/
void Material::setDeformationStrenghts(float compression, float impact, float tensile, float torsion, float shear)
{

	compDefStrength = compression;
	impactDefStrength = impact;
	tensileDefStrength = tensile;
	torsionDefStrenght = torsion;
	shearDefStrength = shear;
}

/*
Used to assign the material fracture strength
*/
void Material::setFractureStrength(float compression, float impact, float tensile, float torsion, float shear)
{
	compFractStrength = compression;
	impactFractStrength = impact;
	tensileFractStrength = tensile;
	torsionFractStrenght = torsion;
	shearFractStrength = shear;
}

void Material::setMaterialName(std::string name)
{
	materialName = name;
}

void Material::setDensity(float _density)
{
	density = _density;
}

void Material::setIsDeformed(bool _isDeformed)
{
	isDeformed = _isDeformed;
}

void Material::setIsFractured(bool _isFractured)
{
	isFractured = _isFractured;
}

void Material::setMaxDeformationFormationAllowed(int _maxDeformationAllowed)
{
	maxDeformationAllowed = _maxDeformationAllowed;
}

void Material::setMaxFractureAllowed(int _maxFractureAllowed)
{
	maxFractureAllowed = _maxFractureAllowed;
}



void Material::setDeformationPercentage(float _deformationPercentage)
{
	deformationPercentage = _deformationPercentage;
}

void Material::setFracturePercentage(float _fracturePercentage)
{
	fracturePercentage = _fracturePercentage;
}

float Material::getDeformationStrength(AppliedForceTypes::AppliedForceType appliedForceType)
{

	float materialStrength = 0.0f;
	if (appliedForceType == AppliedForceTypes::enImpact)
	{
		materialStrength = impactDefStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enShear)
	{
		materialStrength = shearDefStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enTensile)
	{
		materialStrength = tensileDefStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enCompression)
	{
		materialStrength = compDefStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enTorsion)
	{
		materialStrength = torsionDefStrenght;
	}

	return materialStrength;
}

float Material::getFractureStrength(AppliedForceTypes::AppliedForceType appliedForceType)
{
	float materialStrength = 0.0f;
	if (appliedForceType == AppliedForceTypes::enImpact)
	{
		materialStrength = impactFractStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enShear)
	{
		materialStrength = shearFractStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enTensile)
	{
		materialStrength = tensileFractStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enCompression)
	{
		materialStrength = compFractStrength;
	}
	else if (appliedForceType == AppliedForceTypes::enTorsion)
	{
		materialStrength = torsionFractStrenght;
	}

	return materialStrength;
}

bool Material::getIsDeformed()
{
	return isDeformed;
}

bool Material::getIsFractured()
{
	return isFractured;
}

float Material::getDensity()
{
	return density;
}

float Material::getMaxDeformationAllowed()
{
	return maxDeformationAllowed;
}

float Material::getMaxFractureAllowed()
{
	return maxFractureAllowed;
}
float Material::getDeformationPercentage()
{
	return deformationPercentage;
}

float Material::getFracturePercentage()
{
	return fracturePercentage;

}
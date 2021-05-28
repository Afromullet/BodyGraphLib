#pragma once

#include <string>
#include "ForceEnums.hpp"

class Material
{
private:
    
    //Past this point, the object starts plastic deformation (elastic limit)
    float compDefStrength;
    float impactDefStrength;
    float tensileDefStrength;
    float torsionDefStrenght;
    float shearDefStrength;
    
    
    //Past this point the object starts to break (fracture strength)
    float compFractStrength;
    float impactFractStrength;
    float tensileFractStrength;
    float torsionFractStrenght;
    float shearFractStrength;
    
    float plasticity; //The higher this, the harder the material to deform
    float density;
    
	std::string materialName;

	bool isDeformed;
	bool isFractured;


	//If the numberOfTimesDeformed hits the maxDeformationsAllowed, the material starts to become fractured more easily
	//If the numberOfTimesFractured hits the maxFracturesAllowed, the material becomes broken. 
	float maxDeformationAllowed;
	float maxFractureAllowed;
	float deformationPercentage;
	float fracturePercentage;
 
public:

	void setDeformationStrenghts(float compression, float impact, float tensile, float torsion, float shear);
	void setFractureStrength(float compression, float impact, float tensile, float torsion, float shear);	
	void setMaterialName(std::string name);
	void setDensity(float _density);
	void setIsDeformed(bool _isDeformed);
	void setIsFractured(bool _isFractured);
	void setMaxDeformationFormationAllowed(int _maxDeformationAllowed);
	void setMaxFractureAllowed(int _maxFractureAllowed);
	void setDeformationPercentage(float _deformationPercentage);
	void setFracturePercentage(float _fracturePercentage);

	float getDeformationStrength(AppliedForceTypes::AppliedForceType appliedForceType);
	float getFractureStrength(AppliedForceTypes::AppliedForceType appliedForceType);
	bool getIsDeformed();
	bool getIsFractured();
	float getMaxDeformationAllowed();
	float getMaxFractureAllowed();
	float getDeformationPercentage();
	float getFracturePercentage();

	float getDensity();
};

//
//  BodyPart.cpp
//  Colony
//
//  Created by Sean on 1/3/18.
//  Copyright © 2018 Afromullet. All rights reserved.
//

#include "BodyPart.hpp"


//Todo implement. 
bool BodyPart::operator==(BodyPart &other) const
{
	return true; 
}

bool BodyPart::operator!=(BodyPart &other) const
{
    return !(*this == other);
}

void BodyPart::operator=(const BodyPart &other)
{
    id = other.id;
    health = other.health;
    bodyPartToken = other.bodyPartToken;
    section = other.section;
	hasFineMotorSkills = other.hasFineMotorSkills;

    canMoveCreature = other.canMoveCreature;
    canSee = other.canSee;
    canSmell = other.canSmell;
    canBreathe = other.canBreathe;
    isInternal = other.isInternal;
	bodyPartName = other.bodyPartName;
    isOrgan = other.isOrgan;
	//woundPropeties = other.woundPropeties;
	//woundHandler = woundHandler;
}
 
BodyPart::BodyPart() 
{
	id = ERROR_NUMBER;
	health = ERROR_NUMBER;
	bodyPartToken = ERROR_STRING;
	section = ERROR_STRING;
	hasFineMotorSkills = false;
	canMoveCreature = false;
	canSee = false;
	canSmell = false;
	canBreathe = false;
	isInternal = false;
	bodyPartName = ERROR_STRING;
	isOrgan = false;
	//woundHandler.setBodypart(this);
	//woundHandler.setDamageAmount(0);

    
}

BodyPart::BodyPart(std::string bptoken,std::string bpname) : bodyPartToken(bptoken)
{
	bodyPartName = bpname;
    health = 10;
    isOrgan = false;
//	woundHandler.setBodypart(this);

}

void BodyPart::setBodyPartToken(std::string val)
{
    bodyPartToken = val;
}

void BodyPart::setSection(std::string val)
{
    section = val;
}

void BodyPart::setCanMoveCreature(bool val)
{
    canMoveCreature = val;
}

void BodyPart::setCanSee(bool val)
{
    canSee = val;
}

void BodyPart::setCanSmell(bool val)
{
    canSmell = val;
}

void BodyPart::setCanBreathe(bool val)
{
    canBreathe = val;
}

void BodyPart::setIsInternal(bool val)
{
    isInternal = val;
}

void BodyPart::setRelativeSize(float val)
{
    if(relativeSize < 0)
        return;
    relativeSize = val;
}

void BodyPart::setIsOrgan(bool val)
{
    isOrgan = val;
}


void BodyPart::setBodyPartName(std::string bpname)
{
	bodyPartName = bpname;
}

void BodyPart::setHealth(int _health)
{
	health = _health;
}

void BodyPart::setLightWoundThreshold(float amount)
{
	lightWoundTreshold = amount;
}

void BodyPart::setMediumWoundThreshold(float amount)
{
	mediumWoundThreshold = amount;
}

void BodyPart::setHeavyWoundThreshold(float amount)
{
	heavyWoundThreshold = amount;
}

void BodyPart::setDamageAmount(float amount)
{
	//woundHandler.setDamageAmount(amount);
}

void BodyPart::addToDamageAmount(float amount)
{

	//woundHandler.addToDamageAmount(amount);
}

void BodyPart::addArmorToLayer(Armor armor)
{
	armorlayer.addArmor(armor);
}

/*
void BodyPart::setWoundProperties(WoundTypes::WoundProperties _woundPropeties)
{
	woundPropeties = _woundPropeties;
}
*/
ArmorLayers & BodyPart::getArmorLayerRef()
{
	return armorlayer;
}

std::list<Armor>::iterator BodyPart::getLayerIterator()
{
	return armorlayer.getLayerIterator();
}

std::list<Armor>::iterator BodyPart::getLayerIteratorEnd()
{
	return armorlayer.getLayerIteratorEnd();
}

std::string BodyPart::getBodyPartToken() const
{
    return bodyPartToken;
}

std::string BodyPart::getSection() const
{
    return section;
}


std::string BodyPart::getBodyPartName() const 
{ 
	return bodyPartName; 
}

int BodyPart::getNumberOfArmorLayers() const
{
	return armorlayer.getNumberOfLayers();
}


float BodyPart::getDamageAmount()
{
	//return woundHandler.getDamageAmount();
	return 0.0;
}

float BodyPart::getLightwoundThreshold() const
{

	return lightWoundTreshold;

}

float BodyPart::getMediumoundThreshold() const
{
	
	return mediumWoundThreshold;

}

float BodyPart::getHeavywoundThreshold() const
{
	;
	return heavyWoundThreshold;

}

/*
Use the last element of the std::list<AppliedForceEffect> to calculate the wound severity.
The last element is the force remaining after it has been absorbed by armor etc.

WoundTypes::EnWoundSeverity BodyPart::calculateWoundSeverity(AppliedForceBodypartPair & appliedForcePair)
{

	WoundTypes::EnWoundSeverity woundSeverity = WoundTypes::EnWoundSeverity::enErrorSeverity;

	float finalForce = GetFinalForce(appliedForcePair).getEndForce();

	if (finalForce >= heavyWoundThreshold)
	{
		woundSeverity = WoundTypes::EnWoundSeverity::heavyWound;
		
	}
	else if (finalForce >= mediumWoundThreshold)
	{
		woundSeverity = WoundTypes::EnWoundSeverity::mediumWound;

	}
	else if (finalForce >= lightWoundTreshold)
	{
		woundSeverity = WoundTypes::EnWoundSeverity::lightWound;
	}

	return woundSeverity;
}
*/
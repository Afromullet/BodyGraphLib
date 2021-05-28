//
//  BodyPart.cpp
//  Colony
//
//  Created by Sean on 1/3/18.
//  Copyright © 2018 Afromullet. All rights reserved.
//

#include "BodyPart.hpp"
#include "ErrorValues.h"

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
	//return woundHandler.getLightwoundThreshold();
	return 0.0;

}

float BodyPart::getMediumoundThreshold() const
{
	//return woundHandler.getMediumoundThreshold();
	return 0.0;

}

float BodyPart::getHeavywoundThreshold() const
{
	//return woundHandler.getHeavywoundThreshold();
	return 0.0;

}
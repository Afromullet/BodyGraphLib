#include "Item.hpp"
#include <boost/uuid/uuid_generators.hpp>



//Overloaded operators

void Item::operator=(const Item &other)
{

	equipmentName = other.equipmentName;
	material = other.material;
	size = other.size;
	mass = other.size;
	sections = other.sections;
	tag = boost::uuids::random_generator()();

}

Item::Item()
{
	tag = boost::uuids::random_generator()();
}


Item::Item(const Item &other)
{

	equipmentName = other.equipmentName;
	material = other.material;
	size = other.size;
	mass = other.size;
	sections = other.sections;
	tag = boost::uuids::random_generator()();

}

/*
Calculates the contributions of the material to its properties
I.E, mass, density, etc
*/
void Item::CalculateMaterialBonuses()
{
	mass = material.getDensity() * volume; //Density = mass * volume
}

std::string Item::getItemName() const
{
	return equipmentName;
}


Material Item::getMaterial() const
{
	return material;
}



ResistingForce Item::getResistingForce() 
{

	//Something might have changed so we want to recalculate the resistingforce before we get it. Maybe change that later
	CalculateMaterialBonuses();
	calcResistingForce(); 
	return resistingForce;


}

//Gets the materials deformation strength, based on what kind of force is being applied
float Item::getMaterialDeformationStrength(AppliedForceTypes::AppliedForceType appliedforceType)
{
	return material.getDeformationStrength(appliedforceType);
}
//Gets the materials fracture strength, based on what kind of force is being applied
float Item::getMaterialFractureStrength(AppliedForceTypes::AppliedForceType appliedforceType)
{
	return material.getFractureStrength(appliedforceType);
}



//TODO add check to not allow duplicate sections
void Item::addSection(std::string value)
{
	if (std::find(sections.begin(), sections.end(), value) == sections.end())
	{
		sections.push_back(value);
	}

}

void Item::setItemName(std::string value)
{
	equipmentName = value;
}


void Item::setMaterial(Material value)
{
	material = value;
	CalculateMaterialBonuses();

}

/*
Returns True if the section passed as a parameter is one of the valid sections of this item. 
*/
bool Item::isValidSection(std::string _section)
{
	for (auto it = sections.begin(); it != sections.end(); ++it)
	{
		if (*it == _section)
		{
			return true;
		}
	}

	return false;
}

void Item::setupResistingForce()
{
	resistingForce.setDensity(material.getDensity());
}

void Item::calcResistingForce()
{
	resistingForce.setDensity(material.getDensity());
	resistingForce.setInitialResistingForce(1);
}

void Item::deformMaterial(float deformationAmount)
{
	material.setDeformationPercentage(material.getDeformationPercentage() + deformationAmount);
}

void Item::fractureMaterial(float fractureAmount)
{
	material.setFracturePercentage(material.getFracturePercentage() + fractureAmount);
}

float Item::getDeformationPercentage()
{
	return material.getDeformationPercentage();
}

float Item::getFracturePercentage()
{
	return material.getFracturePercentage();
}

float Item::getMaxDeformationAllowed()
{
	return material.getMaxDeformationAllowed();
}

float Item::getMaxFractureAllowed()
{
	return material.getMaxFractureAllowed();
}

#pragma once

#include <string>
#include <list>
#include "../Material/Material.hpp"
#include "../Material/ForceEnums.hpp"
#include "../Material/Forces.hpp"
#define BOOST_UUID_FORCE_AUTO_LINK //Todo figure out a better way to handle this than using the define here
#include <boost/uuid/uuid.hpp>

class Item
{

private:

	float size; 
	float mass;
	int volume; 
	boost::uuids::uuid tag;

	Material material; //Only allows a single material. This will prevent a headache. 
	std::string equipmentName;
	ResistingForce resistingForce;


	std::list<std::string> sections; //The body part sections this item can be attached to (if any). 

public:

	//Overloaded operators
	void operator=(const Item &other);

	Item();
	Item(const Item &other);

	void CalculateMaterialBonuses();

	std::string getItemName() const;
	Material getMaterial() const;

	ResistingForce getResistingForce() ;

	void calcResistingForce();

	void addSection(std::string value);
	void setItemName(std::string value);
	void setMaterial(Material value);
	bool isValidSection(std::string _section);
	void setupResistingForce();


	//Gets the materials deformation strength, based on what kind of force is being applied
	float getMaterialDeformationStrength(AppliedForceTypes::AppliedForceType appliedforceType);

	//Gets the materials fracture strength, based on what kind of force is being applied
	float getMaterialFractureStrength(AppliedForceTypes::AppliedForceType appliedforceType);

	void deformMaterial(float deformationAmount);
	void fractureMaterial(float fractureAmount);
	float getDeformationPercentage();
	float getFracturePercentage();
	float getMaxDeformationAllowed();
	float getMaxFractureAllowed();
























};
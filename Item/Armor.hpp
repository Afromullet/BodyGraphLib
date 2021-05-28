#pragma once
#include "Item.hpp"


class Armor : public Item
{
private:

	float movementModifier; //The higher this is, the harder it is to move with the armor. 
	float coverPercentage; //A piece of armor might not cover the entire body part it's attached to. This is a value from 0-100%
	bool canBeCovered; //This bool indicates that another piece of armor can cover this. Another piece of armor can be equipped over when canBeCovered is true. 
	
public:

	Armor();

	void setMovementModifier(float _movementModifier);
	void setCoverPercentage(float _coverPercentage);
	void setCanBeCovered(bool _canBeCovered);
};
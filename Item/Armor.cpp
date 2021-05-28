#include "Armor.hpp"

Armor::Armor()
{

}

void Armor::setMovementModifier(float _movementModifier)
{
	movementModifier = _movementModifier;
}

void Armor::setCoverPercentage(float _coverPercentage)
{
	coverPercentage = _coverPercentage;
}

void Armor::setCanBeCovered(bool _canBeCovered)
{
	canBeCovered = _canBeCovered;
}


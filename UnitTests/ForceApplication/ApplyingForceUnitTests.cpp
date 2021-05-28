#include "../../Material/ForceCalculator.hpp"
#include "../../Material/Forces.hpp"
#include "..//../Item/Armor.hpp"
#include "..//..//Item/ArmorLayer.h"
#include "..//UnitTestFileHandler.h"
#include <iostream>
#include <string>
#include "ForceUnitTestHeaders.h"

//Not going to use boost unit test at the moment.

namespace ApplyingForceUnitTests
{
	extern bool ForceEffecectTypes::isDeformationEffectType(ForceEffecectTypes::MaterialEffectType materialEffectType);
	extern bool ForceEffecectTypes::isFractureEffectType(ForceEffecectTypes::MaterialEffectType materialEffectType);

	//We iterate over this to be able to test each type of force.
	AppliedForceTypes::AppliedForceType forcetypeArray[] = { AppliedForceTypes::enCompression,AppliedForceTypes::enImpact,AppliedForceTypes::enShear,AppliedForceTypes::enTensile,AppliedForceTypes::enTorsion };
	int forcetypeArray_size = sizeof(forcetypeArray) / sizeof(forcetypeArray[0]);

	Material easilydeformedMaterial, easilyfracturedMaterial, unbreakableMaterial;

	UnitTestFileHandler ufhAppliedForce("AppliedForceUnitTestResults.txt");

	void initializeApplidForceUnitTestValues()
	{
		easilydeformedMaterial.setDeformationStrenghts(1, 1, 1, 1, 1);
		easilydeformedMaterial.setFractureStrength(1000000, 1000000, 1000000, 1000000, 1000000);
		easilydeformedMaterial.setDensity(1);

		easilyfracturedMaterial.setDeformationStrenghts(1, 1, 1, 1, 1);
		easilyfracturedMaterial.setFractureStrength(1, 1, 1, 1, 1);
		easilyfracturedMaterial.setDensity(1);

		unbreakableMaterial.setDeformationStrenghts(1000000, 1000000, 1000000, 1000000, 1000000);
		unbreakableMaterial.setFractureStrength(1000000, 1000000, 1000000, 1000000, 1000000);
		unbreakableMaterial.setDensity(1);


	}

	/*This unit test:
	1) A ResistingForce with little strength
	2) an ApplyingForce with a very high initial force
	3) Low materialDeformation and materialFracture strength

	Expected Result: Should propagate through all layers with a material fracture effect

	*/
	void Test_Fracture_effect()
	{

		ResistingForce resForce;
		resForce.setDensity(1);
		resForce.setInitialResistingForce(1);

		ApplyingForce appForce;
		appForce.setInitialForce(10000);

		AppliedForceEffect effectType;
		ForceEffecectTypes::MaterialEffectType matEffect;
		for (int i = 0; i < forcetypeArray_size; i++)
		{
			appForce.setAppliedForceType(forcetypeArray[i]);
			effectType = ApplyForceToMaterial(1, 1, appForce, resForce);
			matEffect = effectType.getMaterialEffectType();

			if (!ForceEffecectTypes::isFractureEffectType(matEffect))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "Did not succeed in fracture");
				break;
			}
			if (ForceEffecectTypes::isDeformationEffectType(matEffect))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "Caused Deformation instead of fracture");
				break;

				//Should never be a deformation efffect. 
			}
		}
	}

	/*This unit test:
	1) A ResistingForce with little low deformation strength but high
	2) an ApplyingForce with a very high initial force
	3) Low materialDeformation and materialFracture strength

	Expected Result: Should propagate through all layers with a material fracture effect

	*/
	void Test_Deformation_Effect()
	{

		ResistingForce resForce;
		resForce.setDensity(1);
		resForce.setInitialResistingForce(1);

		ApplyingForce appForce;
		AppliedForceEffect effectType;
		ForceEffecectTypes::MaterialEffectType matEffect;
		for (int i = 0; i < forcetypeArray_size; i++)
		{
			appForce.setAppliedForceType(forcetypeArray[i]);
			appForce.setInitialForce(50);
			effectType = ApplyForceToMaterial(1, 10000000, appForce, resForce);
			matEffect = effectType.getMaterialEffectType();
			if (!ForceEffecectTypes::isDeformationEffectType(matEffect))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "Did not succeed in deformation");
				break;
			}
			if (ForceEffecectTypes::isFractureEffectType(matEffect))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "Caused fracture instead of deformation");
				break;
			}
		}
	}



	/*This unit test:
	1) An ApplyingForce that is too weak to cause an effect

	Expected Result: No effect

	*/
	void Test_No_Material_Effect()
	{
		ResistingForce resForce;
		resForce.setDensity(1000000);
		resForce.setInitialResistingForce(1);

		ApplyingForce appForce;
		AppliedForceEffect effectType;
		ForceEffecectTypes::MaterialEffectType matEffect;
		for (int i = 0; i < forcetypeArray_size; i++)
		{
			appForce.setAppliedForceType(forcetypeArray[i]);
			appForce.setInitialForce(100);

			effectType = ApplyForceToMaterial(10000000000, 100000000, appForce, resForce);
			matEffect = effectType.getMaterialEffectType();
			if (matEffect != ForceEffecectTypes::enNoEffect)
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The material effect is not set to NoEffect");
				break;
			}
		}


	}


	/*
	Objective: Apply an ApplyingForce to an ArmorLayer. We should end up with a list of AppliedForceEffects, where each has a fracture effect

	1) Create a set of armor with materials that can easily be fractured
	2) Go through the list of armor, starting with the ApplyingForce.
	3) Continue to apply the remaining force

	Using a list here because that's also the way we're going to traverse it, since we're dealing with force absorption through layers.
	Uses the list returned by the ArmorLayer to calculate the applied force outside of the class operation(which I will add layer)
	For now I'm focused on just calculating the applyingForce to a list of Armor without ArmorLayers potentially getting in the way

	Expected Result: Every element of the list should be a fracture effect. The number of fracture effects should be equal too the length of the armor alyer

	Using many local variables here to make things as clear as possible.
	*/
	void Test_Fracture_Effect_On_Each_Layer() {


		int numberOfArmorLayers = 10; //Arbitrary choice. Any number greater than a few should work
		ArmorLayers armorLayers;

		//Outside the loop because having everthing being a copy works for this test case
		Armor armor;
		armor.setMaterial(easilyfracturedMaterial);
		for (int i = 0; i < numberOfArmorLayers; i++)
		{
			//Want their strengths to be low enough to where they can easily be fractured. 
			armor.setItemName("Armor " + std::to_string(i)); //Only difference between them is the name
			armorLayers.addArmor(armor);
		}

		ApplyingForce appForce;
		for (int i = 0; i < forcetypeArray_size; i++)
		{
			std::list<AppliedForceEffect> forceAbsorptionList(0);
			appForce.setAppliedForceType(forcetypeArray[i]);
			appForce.setInitialForce(50);

			forceAbsorptionList = armorLayers.propagateForce(appForce);

			//Next we're making sure that we have a list of the right size (should be equal to numberOfArmorLayers) and that each element has a Fracture material effect
			if (forceAbsorptionList.size() != numberOfArmorLayers)
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The forceAbsorptionList is not equal to the numberOfArmorLayers");

			}

			for (auto forceIt = forceAbsorptionList.begin(); forceIt != forceAbsorptionList.end(); ++forceIt)
			{
				if (!ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()))
				{
					ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We do not have a fracture effect on one of the list elements");

					break;
				}

				if (ForceEffecectTypes::isDeformationEffectType(forceIt->getMaterialEffectType()))
				{
					ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We have a deformation effect on one of the elemtns of the list");
					break;
				}

			}

		}
	}


	/*
	Objective: Apply an ApplyingForce to an ArmorLayer. We should end up with a list of AppliedForceEffects, where each has a deformation effect

	1) Create a set of armor with materials that can easily be deformed but not fractued
	2) Go through the list of armor, starting with the ApplyingForce.
	3) Continue to apply the remaining force

	Using a list here because that's also the way we're going to traverse it, since we're dealing with force absorption through layers.
	Uses the list returned by the ArmorLayer to calculate the applied force outside of the class operation(which I will add layer)
	For now I'm focused on just calculating the applyingForce to a list of Armor without ArmorLayers potentially getting in the way

	Expected Result: Every element of the list should be a deformation effect. The number of deformed effects should be equal too the length of the armor alyer

	Using many local variables here to make things as clear as possible.
	*/
	void Test_Deformed_Effect_On_Each_Layer()
	{
		int numberOfArmorLayers = 10; //Arbitrary choice. Any number greater than a few should work

		ArmorLayers armorLayers;

		//Outside the loop because having everthing being a copy works for this test case

		Armor armor;
		armor.setMaterial(easilydeformedMaterial);
		for (int i = 0; i < numberOfArmorLayers; i++)
		{
			//Want their strengths to be low enough to where they can easily be deformed but not fractured. 
			armor.setItemName("Armor " + std::to_string(i)); //Only difference between them is the name
			armorLayers.addArmor(armor);
		}


		ApplyingForce appForce;

		for (int i = 0; i < forcetypeArray_size; i++)
		{
			std::list<AppliedForceEffect> forceAbsorptionList(0);
			appForce.setAppliedForceType(forcetypeArray[i]);
			appForce.setInitialForce(50);
			forceAbsorptionList = armorLayers.propagateForce(appForce);

			//Next we're making sure that we have a list of the right size (should be equal to numberOfArmorLayers) and that each element has a deformed material effect

			if (forceAbsorptionList.size() != numberOfArmorLayers)
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The forceAbsorptionList is not equal to the numberOfArmorLayers");

			}

			for (auto forceIt = forceAbsorptionList.begin(); forceIt != forceAbsorptionList.end(); ++forceIt)
			{
				if (!ForceEffecectTypes::isDeformationEffectType(forceIt->getMaterialEffectType()))
				{
					ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We do not have a deformation effect on one of the list elements");

					break;
				}

				if (ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()))
				{
					ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We have a fracture effect on one of the elemtns of the list");
					break;
				}

			}

		}
	}

	/*
	Objective: Apply an ApplyingForce to an ArmorLayer. We should end up with a list of AppliedForceEffects, where each has no effect

	1) Create a set of armor with materials that can neither be deformed or fractured
	2) Go through the list of armor, starting with the ApplyingForce.
	3) Continue to apply the remaining force

	Using a list here because that's also the way we're going to traverse it, since we're dealing with force absorption through layers.
	Uses the list returned by the ArmorLayer to calculate the applied force outside of the class operation(which I will add layer)
	For now I'm focused on just calculating the applyingForce to a list of Armor without ArmorLayers potentially getting in the way

	Expected Result: Every element of the list should be have no effect. The number of no effects should be equal too the length of the armor alyer

	Using many local variables here to make things as clear as possible.
	*/
	void Test_No_Effect_On_Each_Layer()
	{
		int numberOfArmorLayers = 10; //Arbitrary choice. Any number greater than a few should work

		ArmorLayers armorLayers;

		//Outside the loop because having everthing being a copy works for this test case

		Armor armor;
		armor.setMaterial(unbreakableMaterial);
		for (int i = 0; i < numberOfArmorLayers; i++)
		{
			//Want their strengths to be low enough to where they can easily be deformed but not fractured. 
			armor.setItemName("Armor " + std::to_string(i)); //Only difference between them is the name
			armorLayers.addArmor(armor);
		}

		ApplyingForce appForce;
		for (int i = 0; i < forcetypeArray_size; i++)
		{
			std::list<AppliedForceEffect> forceAbsorptionList(0);
			appForce.setAppliedForceType(forcetypeArray[i]);
			appForce.setInitialForce(50);

			forceAbsorptionList = armorLayers.propagateForce(appForce);

			//Next we're making sure that we have a list of the right size (should be equal to numberOfArmorLayers) and that each element has a deformed material effect

			if (forceAbsorptionList.size() != numberOfArmorLayers)
			{

				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The forceAbsorptionList is nto equal to the numberOfArmorLayers");
			}

			for (auto forceIt = forceAbsorptionList.begin(); forceIt != forceAbsorptionList.end(); ++forceIt)
			{
				if (ForceEffecectTypes::isDeformationEffectType(forceIt->getMaterialEffectType()))
				{

					ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We do not have a no effect on one of the list elements");
					break;
				}

				if (ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()))
				{
					ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We do not have a no effect on one of the elemtns of the list");
					break;
				}

			}

		}
	}


	/*
	Objective: Apply an ApplyingForce to an ArmorLayer of length 3.

	The first layer is easily fractured, the second layer is easily deformed, the third layer is unbreakable

	Expected Result

	Layer 0 = Fracture Effect
	Layer 1 = Deformation Effect
	Layer 3 = No Effect

	Using many local variables here to make things as clear as possible.
	*/
	void Test_Decremental_Armor_Layers()
	{
		ArmorLayers armorLayers;
		Armor armor;

		//Create the three layers
		armor.setMaterial(easilyfracturedMaterial);
		armorLayers.addArmor(armor);
		armor.setMaterial(easilydeformedMaterial);
		armorLayers.addArmor(armor);
		armor.setMaterial(unbreakableMaterial);
		armorLayers.addArmor(armor);


		ApplyingForce appForce;
		AppliedForceEffect forceEffect;
		for (int i = 0; i < forcetypeArray_size; i++)
		{
			std::list<AppliedForceEffect> forceAbsorptionList(0);
			appForce.setAppliedForceType(forcetypeArray[i]);
			appForce.setInitialForce(50);

			forceAbsorptionList = armorLayers.propagateForce(appForce);

			//Next we're making sure that we have a list of the right size (should be equal to numberOfArmorLayers) and that each element has a deformed material effect

			if (forceAbsorptionList.size() != 3)
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The forceAbsorptionList is nto equal to the numberOfArmorLayers");

			}

			auto forceIt = forceAbsorptionList.begin(); //Get the easily deformed layer


			//First one should be a fracture
			if (!ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The first layer should be a fracture");

			}


			std::advance(forceIt, 1); //Get the easily deformed layer
			//First one should be a fracture
			if (!ForceEffecectTypes::isDeformationEffectType(forceIt->getMaterialEffectType()))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "The second layer should be a deformation");

			}


			std::advance(forceIt, 1); //Get the unbreakable layer
			//Should neither be a fracture nor a deformation
			if (ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()) || ForceEffecectTypes::isDeformationEffectType(forceIt->getMaterialEffectType()))
			{
				ufhAppliedForce.write_unit_test_fail_information(__FUNCTION__, "We do not have a no effect on the third layer.");

			}
		}
	}
}

void RunAppliedForceUnitTests()
{
	ApplyingForceUnitTests::initializeApplidForceUnitTestValues();
	ApplyingForceUnitTests::Test_Fracture_effect();
	ApplyingForceUnitTests::Test_Deformation_Effect();
	ApplyingForceUnitTests::Test_No_Material_Effect();
	ApplyingForceUnitTests::Test_Fracture_Effect_On_Each_Layer();
	ApplyingForceUnitTests::Test_Deformed_Effect_On_Each_Layer();
	ApplyingForceUnitTests::Test_No_Effect_On_Each_Layer();
	ApplyingForceUnitTests::Test_Decremental_Armor_Layers();
	ApplyingForceUnitTests::ufhAppliedForce.closefile();
}
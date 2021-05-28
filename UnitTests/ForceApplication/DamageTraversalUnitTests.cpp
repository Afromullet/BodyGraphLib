#include "ForceUnitTestHeaders.h"


#include "../../Bodygraph/BodyGraph.hpp"
#include "../../Bodygraph/BodyGraphReader.hpp"
#include "../../DamageTraverser.hpp"
#include "..//UnitTestFileHandler.h"
#include "..//../Item/Armor.hpp"
#include <string>

namespace TraveralUnitTestValues
{
	CreatureBody basicHumanoidTestBody;
	Material easilydeformedMaterial, easilyfracturedMaterial, unbreakableMaterial;

	//We iterate over this to be able to test each type of force.
	AppliedForceTypes::AppliedForceType forcetypeArray[] = { AppliedForceTypes::enCompression,AppliedForceTypes::enImpact,AppliedForceTypes::enShear,AppliedForceTypes::enTensile,AppliedForceTypes::enTorsion };
	int forcetypeArray_size = sizeof(forcetypeArray) / sizeof(forcetypeArray[0]);


	void prepareBody()
	{
		//BodyGraphReader reader = BodyGraphReader("BasicHumanoidUnitTestBody.xml");
		BodyGraphReader reader = BodyGraphReader("BasicHumanoidBody.xml"); //todo replace the direct path
		basicHumanoidTestBody.setBodyGraph(reader.anatomyGraph);
		basicHumanoidTestBody.prepareEdgeMap();
	}

	

	void initializeDamageTraversalUnitTestValues()
	{



		easilydeformedMaterial.setDeformationStrenghts(1, 1, 1, 1, 1);
		easilydeformedMaterial.setFractureStrength(1000000, 1000000, 1000000, 1000000, 1000000);
		easilydeformedMaterial.setDensity(1);
		easilydeformedMaterial.setMaxDeformationFormationAllowed(10);

		easilyfracturedMaterial.setDeformationStrenghts(1, 1, 1, 1, 1);
		easilyfracturedMaterial.setFractureStrength(1, 1, 1, 1, 1);
		easilyfracturedMaterial.setDensity(1);
		easilydeformedMaterial.setMaxFractureAllowed(10);

		unbreakableMaterial.setDeformationStrenghts(1000000, 1000000, 1000000, 1000000, 1000000);
		unbreakableMaterial.setFractureStrength(1000000, 1000000, 1000000, 1000000, 1000000);
		unbreakableMaterial.setDensity(1);
	}
}

namespace DamageTraveralUnitTests
{


	UnitTestFileHandler ufhDamageTraversal("DamageTraversalUnitTestResults.txt");


	/*
	Objective:

	For n number of times, select a random depth and get all paths of depths n. Then apply a force that will fracture all of the materials at that depth

	Expected Result: We should have a list of AppliedForceEffects whose length is the same as the paths returned by the random depth selection. Each of these must be a fracture material effect.

	*/


	void Test_Traversal_Fracture_All_At_Depth()
	{
		//Apply

		Armor armor;
	
		armor.setMaterial(TraveralUnitTestValues::easilyfracturedMaterial);
		TraveralUnitTestValues::prepareBody();

		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
		}

		int depth = 3;

		ApplyingForce appForce;
		appForce.setInitialForce(10000);
		std::vector<int> randomPath;
		ForceEffectHandler forceEffectHandler;
		std::list<AppliedForceEffect> forceEffectList;
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			//Want to test for every force type
			for (int j = 0; j < TraveralUnitTestValues::forcetypeArray_size; j++)
			{
				appForce.setAppliedForceType(TraveralUnitTestValues::forcetypeArray[j]);
				



				DamageTraverser trav("Attack1", appForce, GraphTraversalTypes::depthTraversal);
				randomPath = trav.getRandomDepthPath(i, depth, TraveralUnitTestValues::basicHumanoidTestBody);

				forceEffectHandler = trav.ApplyForceToPath(randomPath, TraveralUnitTestValues::basicHumanoidTestBody);
				ForceEffectMap forceEffectMap = forceEffectHandler.getForceEffectMap();

				if (randomPath.size() != forceEffectHandler.getforceEffectMapSize())
				{

					ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. The forceEffectHandler map is not equal in size to the path.");
			
				}

				for (auto mapIt = forceEffectMap.begin(); mapIt != forceEffectMap.end(); ++mapIt)
				{
					//std::cout << mapIt->first->getBodyPartName();

					//Each of the elements in here should be a fracture effect
					forceEffectList = mapIt->second;
					for (auto forceIt = forceEffectList.begin(); forceIt != forceEffectList.end(); ++forceIt)
					{
						if (!ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()))
						{
							ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. A list element in the forceEffect map is not fracture effect.");
							break;
						}
					}
				}
			}
		}
	

	}

	/*
Objective:

For n number of times, select a random depth and get all paths of depths n. Then apply a force that will deform all of the materials at that depth

Expected Result: We should have a list of AppliedForceEffects whose length is the same as the paths returned by the random depth selection. Each of these must be a deformed material effect.

*/


	void Test_Traversal_Deformation_All_At_Depth()
	{
		//Apply

		Armor armor;

		armor.setMaterial(TraveralUnitTestValues::easilydeformedMaterial);

		TraveralUnitTestValues::prepareBody();
		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);

		}

		int depth = 3;

		ApplyingForce appForce;
		appForce.setInitialForce(100);
		std::vector<int> randomPath;
		ForceEffectHandler forceEffectHandler;
		std::list<AppliedForceEffect> forceEffectList;
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			//Want to test for every force type
			for (int j = 0; j < TraveralUnitTestValues::forcetypeArray_size; j++)
			{
				appForce.setAppliedForceType(TraveralUnitTestValues::forcetypeArray[j]);


				DamageTraverser trav("Attack1", appForce, GraphTraversalTypes::depthTraversal);
				randomPath = trav.getRandomDepthPath(i, depth, TraveralUnitTestValues::basicHumanoidTestBody);

				forceEffectHandler = trav.ApplyForceToPath(randomPath, TraveralUnitTestValues::basicHumanoidTestBody);
				ForceEffectMap forceEffectMap = forceEffectHandler.getForceEffectMap();

				if (randomPath.size() != forceEffectHandler.getforceEffectMapSize())
				{

					ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. The forceEffectHandler map is not equal in size to the path.");

				}

				for (auto mapIt = forceEffectMap.begin(); mapIt != forceEffectMap.end(); ++mapIt)
				{
					//std::cout << mapIt->first->getBodyPartName();

					//Each of the elements in here should be a deformation effect
					forceEffectList = mapIt->second;
					for (auto forceIt = forceEffectList.begin(); forceIt != forceEffectList.end(); ++forceIt)
					{
						if (ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()))
						{
							ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. A list element in the forceEffect map is not deformation effect.");
							break;
						}
					}
				}
			}
		}


	}

		/*
	Objective:

	For n number of times, select a random depth and get all paths of depths n. Then apply a force that will have no effect all of the materials at that depth

	Expected Result: We should have a list of AppliedForceEffects whose length is the same as the paths returned by the random depth selection. Each of these must be have no material effect.
	*/


	void Test_Traversal_NoEffect_All_At_Depth()
	{
		//Apply

		Armor armor;
	
		armor.setMaterial(TraveralUnitTestValues::unbreakableMaterial);

		TraveralUnitTestValues::prepareBody();
		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);

		}

		int depth = 3;

		ApplyingForce appForce;
		appForce.setInitialForce(100);
		std::vector<int> randomPath;
		ForceEffectHandler forceEffectHandler;
		std::list<AppliedForceEffect> forceEffectList;
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			//Want to test for every force type
			for (int j = 0; j < TraveralUnitTestValues::forcetypeArray_size; j++)
			{
				appForce.setAppliedForceType(TraveralUnitTestValues::forcetypeArray[j]);


				DamageTraverser trav("Attack1", appForce, GraphTraversalTypes::depthTraversal);
				randomPath = trav.getRandomDepthPath(i, depth, TraveralUnitTestValues::basicHumanoidTestBody);

				forceEffectHandler = trav.ApplyForceToPath(randomPath, TraveralUnitTestValues::basicHumanoidTestBody);
				ForceEffectMap forceEffectMap = forceEffectHandler.getForceEffectMap();

				if (randomPath.size() != forceEffectHandler.getforceEffectMapSize())
				{

					ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. The forceEffectHandler map is not equal in size to the path.");

				}

				for (auto mapIt = forceEffectMap.begin(); mapIt != forceEffectMap.end(); ++mapIt)
				{
					//std::cout << mapIt->first->getBodyPartName();

					//Each of the elements in here should be a no effect
					forceEffectList = mapIt->second;
					for (auto forceIt = forceEffectList.begin(); forceIt != forceEffectList.end(); ++forceIt)
					{
						if (ForceEffecectTypes::isFractureEffectType(forceIt->getMaterialEffectType()) || ForceEffecectTypes::isDeformationEffectType(forceIt->getMaterialEffectType()))
						{
							ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. A list element in the forceEffect map is not no effect.");
							break;
						}
					}
				}
			}
		}
	}

	void Test_Traversal_Gradual_All_At_Depth()
	{
		//Apply

		Armor armor;

		armor.setMaterial(TraveralUnitTestValues::unbreakableMaterial);

		TraveralUnitTestValues::prepareBody();
		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{

			armor.setMaterial(TraveralUnitTestValues::easilyfracturedMaterial);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
			TraveralUnitTestValues::easilydeformedMaterial.setDensity(5);
			armor.setMaterial(TraveralUnitTestValues::easilydeformedMaterial);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);
			TraveralUnitTestValues::unbreakableMaterial.setDensity(50);
			armor.setMaterial(TraveralUnitTestValues::unbreakableMaterial);
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i).addArmorToLayer(armor);

		}

		int depth = 3;

		ApplyingForce appForce;
		appForce.setInitialForce(100);
		std::vector<int> randomPath;
		ForceEffectHandler forceEffectHandler;
		std::list<AppliedForceEffect> forceEffectList;
		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			//Want to test for every force type
			for (int j = 0; j < TraveralUnitTestValues::forcetypeArray_size; j++)
			{
				appForce.setAppliedForceType(TraveralUnitTestValues::forcetypeArray[j]);


				DamageTraverser trav("Attack1", appForce, GraphTraversalTypes::depthTraversal);
				randomPath = trav.getRandomDepthPath(i, depth, TraveralUnitTestValues::basicHumanoidTestBody);

				forceEffectHandler = trav.ApplyForceToPath(randomPath, TraveralUnitTestValues::basicHumanoidTestBody);
				ForceEffectMap forceEffectMap = forceEffectHandler.getForceEffectMap();

				if (randomPath.size() != forceEffectHandler.getforceEffectMapSize())
				{

					ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. The forceEffectHandler map is not equal in size to the path.");

				}

				for (auto mapIt = forceEffectMap.begin(); mapIt != forceEffectMap.end(); ++mapIt)
				{
					//std::cout << mapIt->first->getBodyPartName();

					//We need to compare the current element end force to the previous element end force. The end force of the previous element must be less
					forceEffectList = mapIt->second;
					auto forceIt = forceEffectList.begin();
					float previousForce = forceIt->getEndForce();

					if (forceEffectList.size() >= 2)
					{
						std::advance(forceIt, 1);
						for (; forceIt != forceEffectList.end(); ++forceIt)
						{
							
							if (forceIt->getEndForce() > previousForce)
							{
								ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. The next element has a greater force than the previous element.");
								break;
							}
							previousForce = forceIt->getEndForce();
							
						}
					}
					else
					{
						ufhDamageTraversal.write_unit_test_fail_information(__FUNCTION__, "Test failed. We do not have enough elements.");
					}
				}
			}
		}
	}

}

namespace ForceEffectHandlerUnitTests
{

	UnitTestFileHandler ufhForceEffectHandler("DamageTraversalUnitTestResults.txt");

	/*
	Objective: Deform the material x number of times and confirm that it was deformed appropriately. 
	
	*/
	void Test_Deform_Material_ForceEffectHandler()
	{

		Armor armor;
		int numOfArmorLayers = 2;

		armor.setMaterial(TraveralUnitTestValues::easilydeformedMaterial);
		TraveralUnitTestValues::prepareBody();
		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int bpIndex = 0; bpIndex < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); bpIndex++)
		{
			armor.setItemName("Armor 1");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);
			armor.setItemName("Armor 2");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);
			armor.setItemName("Armor 3");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);




			ForceEffectHandler forceHandler;
			std::list<AppliedForceEffect> forceList;

			AppliedForceEffect forceEffect;
			forceEffect.setDamageAmount(1);
			AppliedForceTypes::AppliedForceType tempAppliedForceType;
			for (int i = 0; i <= numOfArmorLayers; i++)
			{
				tempAppliedForceType = TraveralUnitTestValues::forcetypeArray[rand() % TraveralUnitTestValues::forcetypeArray_size];


				forceEffect.setAppliedForceType(tempAppliedForceType);
				forceEffect.setMaterialEffect(ForceEffecectTypes::getDeformationEffectType(tempAppliedForceType));
				forceList.push_back(forceEffect);
			}

			BodyPart &currentBP = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex);
			forceHandler.addElementToForceEffectMap(TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartPointer(bpIndex), forceList);


			int numberOfTimesToApply = 5;
			for (int i = 0; i < numberOfTimesToApply; i++)
			{
				forceHandler.processAppliedForceEffects();
			}


			for (auto armorIt = currentBP.getLayerIterator(); armorIt != currentBP.getLayerIteratorEnd(); ++armorIt)
			{
				if (armorIt->getDeformationPercentage() != (forceList.front().getDamageAmount()*numberOfTimesToApply))
				{
					ufhForceEffectHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. The number of deformations doesn't match.");
					break;
				}
			}


		}
	}


	/*
	Objective: Fracture the material x number of times and confirm that it was fractured appropriately. 

	*/
	void Test_Fracture_Material_ForceEffectHandler()
	{

		Armor armor;
		int numOfArmorLayers = 2;

		armor.setMaterial(TraveralUnitTestValues::easilyfracturedMaterial);
		TraveralUnitTestValues::prepareBody();
		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int bpIndex = 0; bpIndex < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); bpIndex++)
		{
			armor.setItemName("Armor 1");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);
			armor.setItemName("Armor 2");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);
			armor.setItemName("Armor 3");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);




			ForceEffectHandler forceHandler;
			std::list<AppliedForceEffect> forceList;

			AppliedForceEffect forceEffect;
			forceEffect.setDamageAmount(1);
			AppliedForceTypes::AppliedForceType tempAppliedForceType;
			for (int i = 0; i <= numOfArmorLayers; i++)
			{
				tempAppliedForceType = TraveralUnitTestValues::forcetypeArray[rand() % TraveralUnitTestValues::forcetypeArray_size];


				forceEffect.setAppliedForceType(tempAppliedForceType);
				forceEffect.setMaterialEffect(ForceEffecectTypes::getFractureEffectType(tempAppliedForceType));
				forceList.push_back(forceEffect);
			}

			BodyPart &currentBP = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex);
			forceHandler.addElementToForceEffectMap(TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartPointer(bpIndex), forceList);


			int numberOfTimesToApply = 5;
			for (int i = 0; i < numberOfTimesToApply; i++)
			{
				forceHandler.processAppliedForceEffects();
			}


			for (auto armorIt = currentBP.getLayerIterator(); armorIt != currentBP.getLayerIteratorEnd(); ++armorIt)
			{
				if (armorIt->getFracturePercentage() != (forceList.front().getDamageAmount()*numberOfTimesToApply))
				{
					ufhForceEffectHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. The number of fractures doesn't match.");
					break;
				}
			}


		}
	}


	/*
Objective: Deform the material x number of times and confirm that it was deformed appropriately. Once it has been deformed the maxNumberOfTimes, ensure that the next formation
causes a fracture.

*/
	void Test_Deform_Then_Fracture_Material_ForceEffectHandler()
	{

		Armor armor;
		int numOfArmorLayers = 2;

		armor.setMaterial(TraveralUnitTestValues::easilydeformedMaterial);
		TraveralUnitTestValues::prepareBody();
		//Since we don't know which path we're going to take, set the armor on every body part.
		for (int bpIndex = 0; bpIndex < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); bpIndex++)
		{
			armor.setItemName("Armor 1");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);
			armor.setItemName("Armor 2");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);
			armor.setItemName("Armor 3");
			TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex).addArmorToLayer(armor);




			ForceEffectHandler forceHandler;
			std::list<AppliedForceEffect> forceList;
			AppliedForceEffect forceEffect;
			forceEffect.setDamageAmount(1);
			AppliedForceTypes::AppliedForceType tempAppliedForceType;
			for (int i = 0; i <= numOfArmorLayers; i++)
			{
				tempAppliedForceType = TraveralUnitTestValues::forcetypeArray[rand() % TraveralUnitTestValues::forcetypeArray_size];


				forceEffect.setAppliedForceType(tempAppliedForceType);
				forceEffect.setMaterialEffect(ForceEffecectTypes::getDeformationEffectType(tempAppliedForceType));
				forceList.push_back(forceEffect);
			}

			BodyPart &currentBP = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(bpIndex);
			forceHandler.addElementToForceEffectMap(TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartPointer(bpIndex), forceList);

			int numberOfFractures = 5;
			int numberOfTimesToApply = TraveralUnitTestValues::easilydeformedMaterial.getMaxDeformationAllowed();
			for (int i = 0; i < numberOfTimesToApply+numberOfFractures; i++)
			{
				forceHandler.processAppliedForceEffects();
			}


			for (auto armorIt = currentBP.getLayerIterator(); armorIt != currentBP.getLayerIteratorEnd(); ++armorIt)
			{
				if (armorIt->getDeformationPercentage() != (forceList.front().getDamageAmount()*numberOfTimesToApply))
				{
					ufhForceEffectHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. The number of deformations doesn't match.");
					break;
				}
				if (armorIt->getFracturePercentage() != (forceList.front().getDamageAmount()*numberOfFractures))
				{
					ufhForceEffectHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. The number of fractures doesn't match.");
					break;
				}
			}


		}
	}

}

namespace WoundHandlerUnitTests
{
	UnitTestFileHandler ufhWoundHandler("WoundHandlerUnitTests.txt");

	void Test_Basic_Damage_To_BodyPart_With_No_Armor()
	{
		TraveralUnitTestValues::prepareBody();

		ForceEffectHandler forceHandler;
	

		ApplyingForce appForce;
		appForce.setInitialForce(10000);
		std::vector<int> randomPath;
		DamageTraverser trav("Attack1", appForce, GraphTraversalTypes::depthTraversal);

		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			
			randomPath = trav.getRandomDepthPath(i, 3, TraveralUnitTestValues::basicHumanoidTestBody);

			for (auto pathIt = randomPath.begin(); pathIt != randomPath.end(); ++pathIt)
			{
				BodyPart &bpRef = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(*pathIt);
				bpRef.setDamageAmount(0); //Resetting the damage to make the unit test easier.
			}

			forceHandler = trav.ApplyForceToPath(randomPath, TraveralUnitTestValues::basicHumanoidTestBody);
			ForceEffectMap &forceEffectMap = forceHandler.getForceEffectMapRef();

			int damageAmount = 5; //Chosing an arbitrary amount of damage to apply. 

			for (auto it = forceEffectMap.begin(); it != forceEffectMap.end(); ++it)
			{
				if (it->second.size() != 1)
				{
					ufhWoundHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. There is more than one applied force effect");
					break;
				}
				for (auto forceIt = it->second.begin(); forceIt != it->second.end(); ++forceIt)
				{
					forceIt->setDamageAmount(damageAmount); //Using a predetermined damage amount so we can easily check to see if it has been applied correctly
				}
			}

			int numOfTimesToApplyDamage = 3; 
			for (int numApplied = 0; numApplied < numOfTimesToApplyDamage; numApplied++)
			{
				forceHandler.processAppliedForceEffects();
			}
			

			//Now we're checking to see if the damage was applied correctly to every body part in the path. 
			for (auto pathIt = randomPath.begin(); pathIt != randomPath.end(); ++pathIt)
			{
				BodyPart &bpRef = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(*pathIt);

				if (bpRef.getDamageAmount() != numOfTimesToApplyDamage * damageAmount)
				{
					ufhWoundHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. Damage not applied to body part correctly");
					break;
				}
			
			}


		}

	}

	void Test_Basic_Damage_To_BodyPart_With_Armor()
	{
		TraveralUnitTestValues::prepareBody();

		ForceEffectHandler forceHandler;
		Armor armor;
		int numOfArmorLayers = 2;
		armor.setMaterial(TraveralUnitTestValues::easilydeformedMaterial);

		ApplyingForce appForce;
		appForce.setInitialForce(10000);
		appForce.setAppliedForceType(AppliedForceTypes::enImpact); //Doesn't really matter which one we test with 
		std::vector<int> randomPath;
		DamageTraverser trav("Attack1", appForce, GraphTraversalTypes::depthTraversal);

		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{
			BodyPart &bpRef = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i);
			bpRef.addArmorToLayer(armor);
			bpRef.addArmorToLayer(armor);
		}

		for (int i = 0; i < TraveralUnitTestValues::basicHumanoidTestBody.getNumVertices(); i++)
		{

			randomPath = trav.getRandomDepthPath(i, 3, TraveralUnitTestValues::basicHumanoidTestBody);

			for (auto pathIt = randomPath.begin(); pathIt != randomPath.end(); ++pathIt)
			{
				BodyPart &bpRef = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(*pathIt);
				bpRef.setDamageAmount(0); //Resetting the damage to make the unit test easier.
			}

			BodyPart &bpRef = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(i);
			//Just use two layers
	
	
			forceHandler = trav.ApplyForceToPath(randomPath, TraveralUnitTestValues::basicHumanoidTestBody);
			ForceEffectMap &forceEffectMap = forceHandler.getForceEffectMapRef();

			int damageAmount = 5; //Chosing an arbitrary amount of damage to apply. 

			for (auto it = forceEffectMap.begin(); it != forceEffectMap.end(); ++it)
			{
				if (it->second.size() != bpRef.getNumberOfArmorLayers())
				{
					ufhWoundHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. We do not have a forceEffect for every layer");
					break;
				}
				for (auto forceIt = it->second.begin(); forceIt != it->second.end(); ++forceIt)
				{
					forceIt->setDamageAmount(damageAmount); //Using a predetermined damage amount so we can easily check to see if it has been applied correctly
				}
			}

			int numOfTimesToApplyDamage = 3;
			for (int numApplied = 0; numApplied < numOfTimesToApplyDamage; numApplied++)
			{
				forceHandler.processAppliedForceEffects();
			}


			//Now we're checking to see if the damage was applied correctly to every body part in the path. 
			for (auto pathIt = randomPath.begin(); pathIt != randomPath.end(); ++pathIt)
			{
				BodyPart &bpRef = TraveralUnitTestValues::basicHumanoidTestBody.getBodyPartRef(*pathIt);

				if (bpRef.getDamageAmount() != numOfTimesToApplyDamage * damageAmount)
				{
					ufhWoundHandler.write_unit_test_fail_information(__FUNCTION__, "Test failed. Damage not applied to body part correctly");
					break;
				}

			}


		}

	}

}




void RunDamageTraversalUnitTests()
{

	TraveralUnitTestValues::initializeDamageTraversalUnitTestValues();
	DamageTraveralUnitTests::Test_Traversal_Fracture_All_At_Depth();
	DamageTraveralUnitTests::Test_Traversal_Deformation_All_At_Depth();
	DamageTraveralUnitTests::Test_Traversal_NoEffect_All_At_Depth();
	DamageTraveralUnitTests::Test_Traversal_Gradual_All_At_Depth();
	ForceEffectHandlerUnitTests::Test_Deform_Material_ForceEffectHandler();
	ForceEffectHandlerUnitTests::Test_Fracture_Material_ForceEffectHandler();
	ForceEffectHandlerUnitTests::Test_Deform_Then_Fracture_Material_ForceEffectHandler();
	WoundHandlerUnitTests::Test_Basic_Damage_To_BodyPart_With_No_Armor();
	WoundHandlerUnitTests::Test_Basic_Damage_To_BodyPart_With_Armor();

	DamageTraveralUnitTests::ufhDamageTraversal.closefile();
	ForceEffectHandlerUnitTests::ufhForceEffectHandler.closefile();
	WoundHandlerUnitTests::ufhWoundHandler.closefile();





}

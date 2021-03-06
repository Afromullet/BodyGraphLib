//#include "Bodypart.hpp"

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
//#include "../Material/ForceEnums.hpp"
#include <boost/assign/list_of.hpp> 
//#include "../DamageTraverser.hpp"



namespace WoundTypes
{
	enum EnWoundTypes
	{
		cut,
		rupture,
		bruise,
		puncture,
		tear,
		dismember,
		fracture
	};

	enum EnWoundSeverity
	{
		light,
		medium,
		heavyWound,
		enErrorSeverity
	};

	/*
	This enumerated type is used for multiple things.
	1) In the wounds possible to a body part. Identifies what kind of a wound a bodypart can suffer from. 
	2) DamageTraversers, letting it know what kind of wounds it can cause.
	3) A bodypart (specifically the WoundSeverityPair), which identifies what kind of wounds a bodypart suffers from. 
	*/
	enum EnWoundProperties
	{
		enBleed,
		enFracture,
		enRupture
	};

	/*
	Once Wound Characteristics are ironed out, they won't change, so we can use some static types to help us. 
	*/
	//This is the template header for the wound characteristics file. Used for error checking
	static std::string WOUND_CHARACTERISTICS_HEADER = "BodygraphToken,CanBleed,CanFracture,CanRupture";
	
	//A bodypart has a WoundsPossible that tell it what kind of wounds are...possible. 
	//The enumerated types match up with the Wound Characteristics file
	typedef std::vector<std::pair<EnWoundProperties, bool>> WoundProperties;

	//The string is the bodypartToken. This will make it easier to find the correct body part after all the file reading is done. 
	//Don't want to store a pointer to the bodypart. I think that will make it easier to store down the line...Eventually I could store WoundPossibleMap
	//outside of bodyparet
	typedef std::map <std::string, WoundProperties> WoundPossibleMap; 

	//Used for the DamageTraverser. These are the wounds an attack can cause. 
	typedef std::vector<WoundProperties> CanCauseWounds;

	//Identifies how severe a wound is on a bodypart
	typedef std::pair< EnWoundProperties, EnWoundSeverity> WoundSeverityPair;

	WoundPossibleMap process_wound_characteristics_file(std::string fname);
	


}

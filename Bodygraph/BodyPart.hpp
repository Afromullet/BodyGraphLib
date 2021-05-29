

#pragma once

//#include <string>
//#include <list>
//#include "../Item/Armor.hpp"
#include "../Item/ArmorLayer.h"
#include "WoundHandler.hpp"


#include "ErrorValues.h"

#include <string>
#include <list>


class Armor;

/* 
 This is a generic class for a body part. Rather than breaking down the body part into separate clases,
 this class represents any sort of body part - each body part can have one or more functions. It's not modelled after
 regular anatomy, so should an arm still be able to breathe and see, it's up to the user, allowing "bizarre" creatures to be created. 

 Two important parts of a BodyPart is the ArmorLayer and the WoundHandler.

 ArmorLayer: When applying force to a bodypart, the force can be absorbed by the armor on the bodypart. The ArmorLayer handles that.
 WoundHandler: Lets us apply damage to a bodypart and determine how resistant and damaged a bodypart already is. Want to keep those calculations outside of the calss. 

 */
class BodyPart
{
    
private:
    
    int id;
    int health;
    std::string bodyPartToken; 
    std::string section;

	bool hasFineMotorSkills;
    bool canMoveCreature;
    bool canSee;
    bool canSmell;
    bool canBreathe;
    bool isInternal;
    bool isOrgan;
	std::string bodyPartName;
    
	float lightWoundTreshold;
	float mediumWoundThreshold;
	float heavyWoundThreshold;
	
    float relativeSize; //How large this part is in relation to the whole body. A percent..I.E, .53 is 53%

	ArmorLayers armorlayer; 

	std::vector<WoundTypes::WoundSeverityPair> woundsOnBodypart; //The kind of wounds that this bodypart is suffering from. 

	//WoundTypes::WoundProperties woundPropeties; //The kind of wounds that can be applied to this bodypart. 

public:

    bool operator==(BodyPart &other) const;
    bool operator!=(BodyPart &other) const;
    void operator=(const BodyPart &other);
                    
    BodyPart();
    BodyPart(std::string bptoken,std::string bpname);

    void setBodyPartToken(std::string val) ;
    void setSection(std::string val) ;
    void setCanMoveCreature(bool val) ;
    void setCanSee(bool val) ;
    void setCanSmell(bool val) ;
    void setCanBreathe(bool val);
    void setIsInternal(bool val) ;
    void setRelativeSize(float val) ;
    void setIsOrgan(bool val);
	void setBodyPartName(std::string bpname);
	void setHealth(int _health);
	void setDamageAmount(float amount);

	void setLightWoundThreshold(float amount);
	void setMediumWoundThreshold(float amount);
	void setHeavyWoundThreshold(float amount);

	void addToDamageAmount(float amount);

	void addArmorToLayer(Armor armor);
	//void setWoundProperties(WoundTypes::WoundProperties _woundPropeties);




	ArmorLayers& getArmorLayerRef();
	std::list<Armor>::iterator getLayerIterator();
	std::list<Armor>::iterator getLayerIteratorEnd();
    std::string getBodyPartToken() const;
    std::string getSection() const; 
	std::string getBodyPartName() const;    
	int getNumberOfArmorLayers() const;

	float getDamageAmount();
	float getLightwoundThreshold() const;
	float getMediumoundThreshold() const;
	float getHeavywoundThreshold() const;

	//WoundTypes::EnWoundSeverity calculateWoundSeverity(AppliedForceBodypartPair &appliedForcePair);

};




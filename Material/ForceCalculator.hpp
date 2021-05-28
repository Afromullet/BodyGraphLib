#pragma once

#include "AppliedForceEffect.hpp"
#include "Forces.hpp"
//#include "ForceEnums.hpp"

/*
Applying a Force to a material compares the kind of force that is being applied (tension, impact, shear, etc) to the strength of the material against that kind of force.
The deformation and fracture strength are passed as simple floats to keep things as non-coupled as possible.
ApplyingForce and ResistingForce are mostly self explanatory. ResistingForce reports how much of the ApplyingForce is absorbed. 
*/
AppliedForceEffect ApplyForceToMaterial(float materialDeformationStrength, float materialFractureStrength, ApplyingForce applyingForce, ResistingForce resistingForce);


/*
This is used to handle force propagation. The ApplyForceToMaterial function which takes ApplyingForce as an input is used for the initial calculations.
This one is used to continue applying the force (I.E, for layers)
*/
AppliedForceEffect ApplyForceToMaterial(float materialDeformationStrength, float materialFractureStrength, AppliedForceEffect appliedForceEffect, ResistingForce resistingForce);

/*
The AppliedForceEffect for when there is no ResistingForce to absorb anything
*/
AppliedForceEffect ApplyForceWithNoMaterial(ApplyingForce applyingForce);


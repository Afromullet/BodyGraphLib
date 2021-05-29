#pragma once

//#include <vector>
namespace AppliedForceTypes
{


	/*
	This is the kind of force that is being applied. Compression, impact, tension, torsion, or shearing. 

	When thinking AppliedForceType as a "type", its as simpleas "This is the kind of force that is being applied. When applying it to a Material, it can either deform it, fracture it, or do nthign at all
	Each AppliedForceType has a related MaterialEffect type. So something like enForcetypekind would have an enforcetypekind deformation and fracture effect.

	*/
	enum AppliedForceType {
		enCompression,
		enImpact,
		enTensile,
		enTorsion,
		enShear,
		enNoForceApplied
	};

}

namespace ForceEffecectTypes
{
		/*
		  This is an enum that identifies how a material is deformed. There are different kind of deformations from different forces (The AppliedForceType). 
		  The forces are comrpession,impact, tension, torsion, and shearing. Each kind of force can induce either "deformation" or "fracture"
		  This means that a material either deforms or changes shape in some way but stays together, or is broken

		*/
		enum MaterialEffectType
		{

			enCompDefEffect,
			enImpactDefEffect,
			enTensileDefEffect,
			enTorsionDefEffect,
			enShearDefEffect,
			enCompFracEffect,
			enImpactFracEffect,
			enTensileFracEffect,
			enTorsionFracEffect,
			enShearFracEffect,
			enNoEffect
		};


		//Simple getters and setters that determine the kind of deformation or fracturing that occured. 
		MaterialEffectType getDeformationEffectType(AppliedForceTypes::AppliedForceType appliedForceType);
		MaterialEffectType getFractureEffectType(AppliedForceTypes::AppliedForceType appliedForceType);

		bool isFractureEffectType(ForceEffecectTypes::MaterialEffectType materialEffectType);
		bool isDeformationEffectType(ForceEffecectTypes::MaterialEffectType materialEffectType);

}




/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok).  Not for disclosure or distribution without Havok's
 * prior written consent.This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2006 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_POINT_TO_PLANE_CONSTRAINT_H
#define HK_DYNAMICS2_POINT_TO_PLANE_CONSTRAINT_H

#include <hkconstraintsolver/solve/hkSolverResults.h>
#include <hkdynamics/constraint/hkConstraintData.h>
#include <hkconstraintsolver/constraint/atom/hkConstraintAtom.h>

extern const hkClass hkPointToPlaneConstraintDataClass;

/// A stiff spring constraint. It holds the constrained bodies apart at a specified distance, 
/// as if they were attached at each end of an invisible rod.
class hkPointToPlaneConstraintData : public hkConstraintData
{
	public:

		HK_DECLARE_REFLECTION();
			
		hkPointToPlaneConstraintData();

			/// Sets the point-to-plane up with world space information.
			/// \param pivotW Specifies both the origin point for the constraining plane, and bodyB's pivot point, specified in world space.
			/// \param planeNormalW Specifies the normal direction of the constraining plane, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, 
								const hkVector4& pivotW, const hkVector4& planeNormalW);

			/// Sets the point-to-plane up with body space information.
			/// \param pivotA Specifies the origin point for the constraining plane, specified in bodyA's space.
			/// \param pivotB BodyB's pivot point, specified in bodyB's space.
			/// \param planeNormalB Specifies the normal direction of the constraining plane, specified in bodyB's space.
		void setInBodySpace(	const hkVector4& pivotA, const hkVector4& pivotB, const hkVector4& planeNormalB);

				/// Check consistency of constraint
		hkBool isValid() const;

			/// Get type from this constraint	
 		virtual int getType() const;

	public:

		enum 
		{
			SOLVER_RESULT_LIN_0 = 0,		// linear constraint 
			SOLVER_RESULT_MAX = 1
		};

		struct Runtime
		{
			class hkSolverResults m_solverResults[1/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];
		};

		inline const Runtime* getRuntime( hkConstraintRuntime* runtime ){ return reinterpret_cast<Runtime*>(runtime); }


		struct Atoms
		{
			HK_DECLARE_REFLECTION();

			struct hkSetLocalTransformsConstraintAtom	m_transforms;
			struct hkLinConstraintAtom					m_lin;

			Atoms(){}

			// get a pointer to the first atom
			const hkConstraintAtom* getAtoms() const { return &m_transforms; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_lin+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_transforms(f), m_lin(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );


	public:

		// Internal functions
		
		// hkConstraintData interface implementations
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const ;

		// hkConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkConstraintData::RuntimeInfo& infoOut ) const;
	
	public:

		hkPointToPlaneConstraintData(hkFinishLoadedObjectFlag f) : hkConstraintData(f), m_atoms(f) {}
};

#endif // HK_DYNAMICS2_POINT_TO_PLANE_CONSTRAINT_H

/*
* Havok SDK - PUBLIC RELEASE, BUILD(#20060902)
*
* Confidential Information of Havok.  (C) Copyright 1999-2006 
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
*
* Use of this software for evaluation purposes is subject to and indicates 
* acceptance of the End User licence Agreement for this product. A copy of 
* the license is included with this software and is also available from salesteam@havok.com.
*
*/
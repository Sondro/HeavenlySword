/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok).  Not for disclosure or distribution without Havok's
 * prior written consent.This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2006 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VARIABLETIMESTEPPER_H
#define HK_VARIABLETIMESTEPPER_H

class hkWorld;

/// This class attempts to keep the stiffness of the system constant when the time step varies from frame to frame.
///
/// This class seems to work for timesteps between 15 fps and 750 fps.
/// If you fall below the min substep count, the time per substep will change, which will
/// result in a noticeable change to the system stiffness.  However, this is better than the alternative
/// where the system can totally fail if you fall bellow 2 substeps.
/// This scheme will result in noticeable jitter.  A better scheme that keeps jitter down is to 
/// always step at n*timePerSubstep, accumulate requestedTimeStep - n*timePerSubstep and do an
/// extra substep when that accumulated ammount > timePerSubstep.  Even more solid would be if 
/// you did the same, but with full timesteps rather than substeps.
class hkVariableTimestepper
{
	public:
		/// Constructor. 
		hkVariableTimestepper( hkReal timePerSubstep, hkReal minSubstepCount = 2.0f );

		/// Simulate forward in time.
		int step( hkWorld* world, hkReal timestep );

		/// Time per substep
		hkReal m_timePerSubstep;

		/// Min number of solver substeps allowable
		hkReal m_minSubstepCount;

};


#endif // HK_VARIABLETIMESTEPPER_H


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

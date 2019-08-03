/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok).  Not for disclosure or distribution without Havok's
 * prior written consent.This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2006 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <hkcollide/hkCollide.h>
#include <hkcollide/shape/phantomcallback/hkPhantomCallbackShape.h>

	
void hkPhantomCallbackShape::getAabb( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) const
{
	out.m_min.setZero4();
	out.m_max.setZero4();
}

hkShapeType hkPhantomCallbackShape::getType() const
{
	return HK_SHAPE_PHANTOM_CALLBACK;
}

hkBool hkPhantomCallbackShape::castRay( const hkShapeRayCastInput& input, hkShapeRayCastOutput& results ) const
{
	return false;
}


void hkPhantomCallbackShape::castRayWithCollector( const hkShapeRayCastInput& input, const hkCdBody& cdBody, hkRayHitCollector& collector ) const
{

}

void hkPhantomCallbackShape::calcStatistics( hkStatisticsCollector* collector ) const
{
	collector->beginObject("PhantomCb", collector->MEMORY_SHARED, this);
	collector->endObject();
}

/*
* Havok SDK - CLIENT RELEASE, BUILD(#20060902)
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
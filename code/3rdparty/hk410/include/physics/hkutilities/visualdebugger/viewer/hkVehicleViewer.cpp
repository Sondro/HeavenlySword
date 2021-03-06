/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok).  Not for disclosure or distribution without Havok's
 * prior written consent.This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2006 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <hkbase/hkBase.h>
#include <hkbase/class/hkClass.h>
#include <hkbase/memory/hkLocalBuffer.h>
#include <hkbase/monitor/hkMonitorStream.h>
#include <hkdynamics/world/hkWorld.h>
#include <hkdynamics/world/hkPhysicsSystem.h>
#include <hkdynamics/action/hkAction.h>
#include <hkvisualize/hkProcessFactory.h>
#include <hkvisualize/hkDebugDisplay.h>
#include <hkserialize/util/hkBuiltinTypeRegistry.h>
#include <hkserialize/util/hkVtableClassRegistry.h>
#include <hkserialize/util/hkClassNameRegistry.h>
#include <hkvehicle/hkVehicleInstance.h>
#include <hkutilities/visualdebugger/viewer/hkVehicleViewer.h>

int			hkVehicleViewer::m_tag				= 0;
const int	hkVehicleViewer::s_numWheelSegments	= 12;

// A Little macro to convert degrees to radians
#define DEGREES2RADIANS(deg) ((deg)*(0.0174532925f))

void HK_CALL hkVehicleViewer::registerViewer()
{
	m_tag = hkProcessFactory::getInstance().registerProcess( getName(), create );
}

hkProcess* HK_CALL hkVehicleViewer::create(const hkArray<hkProcessContext*>& contexts)
{
	return new hkVehicleViewer(contexts);
}

hkVehicleViewer::hkVehicleViewer(const hkArray<hkProcessContext*>& contexts)
: hkWorldViewerBase(contexts)
{	
}

void hkVehicleViewer::init()
{
	if (m_context)
	{
		for (int i=0; i < m_context->getNumWorlds(); ++i)
		{
			worldAddedCallback( m_context->getWorld(i));
		}
	}
}

hkVehicleViewer::~hkVehicleViewer()
{
	if (m_context)
	{
		for (int i=0; i < m_context->getNumWorlds(); ++i)
		{
			worldRemovedCallback( m_context->getWorld(i));
		}
	}
}

void hkVehicleViewer::worldAddedCallback( hkWorld* w)
{
	w->markForWrite();
	w->addActionListener(this);

	// This class is an action listener, and thus will be notified when actions
	// are added to	the world. However, since this class can be created by the
	// user in the VDB, there may already be actions added to the world when
	// this class is created. This loops finds any existing actions and passes
	// them to the action added callback.
	hkPhysicsSystem* system = w->getWorldAsOneSystem();
	const hkArray<hkAction*>& action = system->getActions();

	for(int i=0; i < action.getSize(); i++)
	{
		actionAddedCallback(action[i]);
	}

	system->removeReference();

	w->unmarkForWrite();
}

void hkVehicleViewer::worldRemovedCallback( hkWorld* w)
{
	w->markForWrite();
	w->removeActionListener(this);
	w->unmarkForWrite();
}


// HVK-3274
// This function ensures that hkVehicleViewer will fail to compiler if the name
// of hkVehicleInstance is ever changed. Should this ever happen the string
// below needs to be changed accordingly. This removes the dependency on
// hkvehicle from hkutilities.
static inline const char* getHkVehicleInstanceName(hkVehicleInstance*) { return "hkVehicleInstance"; }

void hkVehicleViewer::actionAddedCallback( hkAction* action )
{
	hkBuiltinTypeRegistry& registry = hkBuiltinTypeRegistry::getInstance();

	// Get the class of the action that was added to the world
	// NB: To get the class we use a string here, rather than
	// hkVehicleInstanceClass, so that building hkUtilities does
	// not have a dependency on the vehicle library. See HVK-3274.
	const hkClass* vehicleClass = registry.getClassNameRegistry()->getClassByName(getHkVehicleInstanceName(HK_NULL));
	const hkClass* actionClass	= registry.getVtableClassRegistry()->getClassFromVirtualInstance(action);


	// If the action is a vehicle instance, or it
	// is of a type derived from the vehicle class.
	if (vehicleClass && actionClass && vehicleClass->isSuperClass(*actionClass))
	{
		action->addReference();
		m_vehicles.pushBack(static_cast<hkVehicleInstance*>(action));
	}
}

void hkVehicleViewer::actionRemovedCallback( hkAction* action )
{
	// Search for the action that has just been removed from the
	// world. If the action is in our list of vehicles, remove it
	// from the list.
	const int index = m_vehicles.indexOf((hkVehicleInstance*)action);

	// If this action is not a member of m_vehicles, we ignore it and return.
	if (index == -1) return;

	m_vehicles.removeAt(index);
	action->removeReference();
}

static void drawStar(hkDebugDisplayHandler* display, const hkVector4& point, const int color, const int tag, const hkReal scale = 0.05f)
{
	// Copied from displayStar
	for (int k=0; k<3; k++)
	{
		hkVector4 star, pt1, pt2;
		star.setZero4();

		star(k) = scale;
		pt1.setAdd4(point,star);
		pt2.setSub4(point,star);
		display->displayLine(pt1, pt2, color, tag);
	}
}


void hkVehicleViewer::step(hkReal frameTimeInMs)
{
	HK_TIME_CODE_BLOCK("hkVehicleViewer::step", HK_NULL);

	const int numVehicles = m_vehicles.getSize();

	// Loop through each vehicle in the world
	for (int i=0; i<numVehicles; i++)
	{
		const hkVehicleInstance& vehicle = *m_vehicles[i];

		// Loop through each wheel on the vehicle
		const int numWheels = vehicle.m_wheelsInfo.getSize();
		for ( int w = 0; w < numWheels; w++ )
		{

			const hkVehicleData::WheelComponentParams& wheelParam = vehicle.m_data->m_wheelParams[w];
			const hkVehicleInstance::WheelInfo& wheel = vehicle.m_wheelsInfo[w];
			const hkVector4& contactPoint = wheel.m_contactPoint.getPosition();
			const hkVector4& hardPoint = wheel.m_hardPointWs;

			// Draw the ray, with a star at the top and bottom (hardpoint, contactpoint)
			m_displayHandler->displayLine(contactPoint, hardPoint, hkColor::RED, m_tag);
			drawStar(m_displayHandler, contactPoint, hkColor::YELLOW, m_tag);
			drawStar(m_displayHandler, hardPoint,    hkColor::YELLOW, m_tag);

			// Draw s star at the center of the wheel
			hkVector4 wheelPoint = contactPoint;
			wheelPoint.addMul4(-wheelParam.m_radius, wheel.m_suspensionDirectionWs);
			drawStar(m_displayHandler, wheelPoint, hkColor::YELLOWGREEN, m_tag);

			// Draw the wheel
			hkLocalBuffer<hkVector4> spokes(s_numWheelSegments);
			hkQuaternion rotation(wheel.m_suspensionDirectionWs, DEGREES2RADIANS(-90.0f));
			spokes[0].setRotatedDir(rotation, wheel.m_spinAxisWs);
			spokes[0].mul4(wheelParam.m_radius);

			// This loop defines a number of points on the circumference of the
			// wheel rim. It then draws a lines to connect all these points.
			hkQuaternion spokeRotation(wheel.m_spinAxisWs, DEGREES2RADIANS(360.0f/(float)s_numWheelSegments));
			for (int s=1; s<s_numWheelSegments+2; s++)
			{
				if (s < s_numWheelSegments)
				{
					spokes[s].setRotatedDir(spokeRotation, spokes[s-1]);
				}
				if (s <= s_numWheelSegments)
				{
					spokes[s-1].add4(wheelPoint);
				}
				if (s>=2)
				{
					m_displayHandler->displayLine(	spokes[(s-2)%s_numWheelSegments],
						spokes[(s-1)%s_numWheelSegments],
						hkColor::WHITE, m_tag);
				}
			}
		}

		// Draw the axles (Do this by drawing a line between all wheels on the
		// same axle).
		const int numAxles  = vehicle.m_data->m_numWheelsPerAxle.getSize();
		for (int axle=0; axle < numAxles; axle++)
		{
			// These two vectors will be used to store the point of connected with
			// the axle of the last two wheels that we have encountered.
			hkVector4 a;
			hkVector4 b;

			// This flag will be used so that we know when we are going through
			// this loop for the first time.
			bool firstTimeThrough = true;
			const int totalWheelsOnThisAxle = vehicle.m_data->m_numWheelsPerAxle[axle];
			int wheelsFoundOnThisAxle  = 0;

			for ( int w = 0; w < numWheels; w++ )
			{
				const hkVehicleData::WheelComponentParams& wheelParam = vehicle.m_data->m_wheelParams[w];

				// If this wheel is not connected to the axle that we are
				// concerned with, skip it.
				if (wheelParam.m_axle != axle) continue;

				wheelsFoundOnThisAxle++;

				const hkVehicleInstance::WheelInfo& wheel = vehicle.m_wheelsInfo[w];
				const hkVector4& contactPoint = wheel.m_contactPoint.getPosition();

				// Find the center of the wheel.
				hkVector4 wheelPoint = contactPoint;
				wheelPoint.addMul4(-wheelParam.m_radius, wheel.m_suspensionDirectionWs);

				a = b;
				b = wheelPoint;

				if (firstTimeThrough) 
				{
					// On this first time through this loop, the vector 'a'
					// would be uninitialized, so we do nothing.
					firstTimeThrough = false;
				}
				else
				{
					// Draw a line to connect the last 2 wheels that we
					// have encountered.
					m_displayHandler->displayLine(a, b, hkColor::YELLOWGREEN, m_tag);
				}

				// If we have already dealt with all the wheels on this axle,
				// we can break from the loop.
				if (wheelsFoundOnThisAxle == totalWheelsOnThisAxle) break;
			}
		}	
	}
}




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

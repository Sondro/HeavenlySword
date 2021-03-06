#include "speedtree/SpeedTreeDef_ps3.h"

#include "speedtree/SpeedTreeManager_ps3.h"
#include "objectdatabase/dataobject.h"



START_STD_INTERFACE(SpeedTreeXmlTreeInstance)
	PUBLISH_VAR_WITH_DEFAULT_AS( m_position, CPoint(0.0, 0.0, 0.0), Position )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_rotation, CQuat(0.0, 0.0, 0.0, 0.0), Orientation )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_needsCollision, false, NeedsCollision )
END_STD_INTERFACE

START_STD_INTERFACE(SpeedTreeXmlTree)
	PUBLISH_VAR_AS( m_templateName, TemplateName )
//	PUBLISH_VAR_WITH_DEFAULT_AS( m_fSize, 10.0f, Size )
//	PUBLISH_VAR_WITH_DEFAULT_AS( m_seed, 0, Seed )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_fVariance, 2.0f, Variance )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_position, CPoint(0.0, 0.0, 0.0), Position )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_rotation, CQuat(0.0, 0.0, 0.0, 0.0), Orientation )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_fNearLodDistance, 5.f, NearLODDistance )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_fFarLodDistance, 15.f, FarLODDistance )
	PUBLISH_PTR_CONTAINER_AS( m_list, List )
	PUBLISH_VAR_WITH_DEFAULT_AS( m_needsCollision, false, NeedsCollision )
END_STD_INTERFACE

START_STD_INTERFACE(SpeedTreeXmlForest)
	PUBLISH_VAR_WITH_DEFAULT_AS( m_sectorBits, 0, SectorBits )
	PUBLISH_VAR_AS( m_speedWindFilename, SpeedWindFilename )
	PUBLISH_PTR_CONTAINER_AS( m_list, List )
	DECLARE_POSTCONSTRUCT_CALLBACK(PostConstruct)
END_STD_INTERFACE

START_STD_INTERFACE(SpeedTreeXmlWind)
	PUBLISH_VAR( m_uiNumMatrices )
	PUBLISH_CONTAINER( m_afBendLowWindControl )
	PUBLISH_CONTAINER( m_afBendHighWindControl )
	PUBLISH_CONTAINER( m_afVibrationLowWindControl )
	PUBLISH_CONTAINER( m_afVibrationHighWindControl )
	PUBLISH_CONTAINER( m_afVibrationFrequency )
	PUBLISH_CONTAINER( m_afVibrationAngles )
	PUBLISH_VAR( m_fMaxBendAngle )
	PUBLISH_VAR( m_fStrengthAdjustmentExponent )
	PUBLISH_CONTAINER( m_afGustStrength )
	PUBLISH_CONTAINER( m_afGustDuration )
	PUBLISH_VAR( m_fGustFrequency )
	PUBLISH_CONTAINER( m_afGustControl )
	PUBLISH_VAR( m_uiNumLeafAngles )
	PUBLISH_VAR( m_fLeafStrengthExponent )

	PUBLISH_CONTAINER_AS( m_afLeafAngleLowWindControl[SpeedTreeXmlWind::ROCK], LeafAngleLowWind_ROCK)
	PUBLISH_CONTAINER_AS( m_afLeafAngleHighWindControl[SpeedTreeXmlWind::ROCK], LeafAngleHighWind_ROCK)
	PUBLISH_CONTAINER_AS( m_afLeafAngleLowWindControl[SpeedTreeXmlWind::RUSTLE], LeafAngleLowWind_RUSTLE)
	PUBLISH_CONTAINER_AS( m_afLeafAngleHighWindControl[SpeedTreeXmlWind::RUSTLE], LeafAngleHighWind_RUSTLE)

	PUBLISH_CONTAINER_AS( m_afLeafAngleFrequency[SpeedTreeXmlWind::ROCK], LeafAngleFrequency_ROCK)
	PUBLISH_CONTAINER_AS( m_afLeafAngleAngles[SpeedTreeXmlWind::ROCK], LeafAngleAngles_ROCK)
	PUBLISH_CONTAINER_AS( m_afLeafAngleFrequency[SpeedTreeXmlWind::RUSTLE], LeafAngleFrequency_RUSTLE)
	PUBLISH_CONTAINER_AS( m_afLeafAngleAngles[SpeedTreeXmlWind::RUSTLE], LeafAngleAngles_RUSTLE)
END_STD_INTERFACE





//#define pArray, a, b, c, d) pArray[0] = a; pArray[1] = b; pArray[2] = c; pArray[3] = d;
//#define pArray, a, b) pArray[0] = a; pArray[1] = b;



SpeedTreeXmlWind::SpeedTreeXmlWind( ) :
	m_uiNumMatrices(4),
	m_fMaxBendAngle(60.0f),
	m_fStrengthAdjustmentExponent(3.0f),
	m_fGustFrequency(15.0f),
//	m_uiNumLeafAngles(6),
	m_uiNumLeafAngles(4),
	m_fLeafStrengthExponent(5.0f)
{
	m_afBendLowWindControl = Vec4(3.0f, 0.0f, 0.0f, 0.1f);
	m_afBendHighWindControl = Vec4(3.0f, 0.0f, 0.0f, 0.1f);

	m_afVibrationLowWindControl = Vec4(1.0f, 0.0f, 0.0f, 0.001f);
	m_afVibrationHighWindControl = Vec4(10.0f, 0.0f, 0.0f, 0.1f);

	m_afVibrationFrequency = Vec2(50.0f, 1000.0f);
	m_afVibrationAngles = Vec2(4.0f, 3.0f);

	m_afGustStrength = Vec2(0.05f, 0.45f);
	m_afGustDuration = Vec2(0.5f, 5.0f);

	m_afGustControl = Vec4(2.0f, 0.0f, 0.0f, 0.001f);

	m_afLeafAngleLowWindControl[ROCK] = Vec4(0.2f, 0.01f, 0.0f, 0.0f);
	m_afLeafAngleHighWindControl[ROCK] = Vec4(0.2f, 0.01f, 1.0f, 0.0f);

	m_afLeafAngleFrequency[ROCK] = Vec2(10.0f, 50.0f);
	m_afLeafAngleAngles[ROCK] = Vec2(4.0f, 2.0f);

	m_afLeafAngleLowWindControl[RUSTLE] = Vec4(0.5f, 0.05f, 0.0f, 0.0f);
	m_afLeafAngleHighWindControl[RUSTLE] = Vec4(3.0f, 6.0f, 1.0f, 0.0f);

	m_afLeafAngleFrequency[RUSTLE] = Vec2(50.0f, 500.0f);
	m_afLeafAngleAngles[RUSTLE] = Vec2(3.0f, 5.0f);
}

//static int numForests = 0;


void SpeedTreeXmlForest::AddTree(SpeedTreeXmlTree* pTree)
{
	m_list.push_back(pTree);
}
void SpeedTreeXmlForest::PostConstruct()
{
	if(SpeedTreeManager::Exists())
	{
		//if (++numForests > 2) return;
		SpeedTreeManager::Get().AddForest(this);
	}
	else
	{
		ntPrintf("SpeedTreeManager does not exist");
	}
}

SpeedTreeXmlForest::~SpeedTreeXmlForest()
{
	if(SpeedTreeManager::Exists())
	{
		SpeedTreeManager::Get().RemoveForest(this);
	}
}


// crap:
void ForceLinkFunction17()
{
	ntPrintf("!ATTN! Calling ForceLinkFunction17() !ATTN!\n");
}

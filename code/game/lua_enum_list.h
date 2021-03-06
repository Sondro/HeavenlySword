#ifndef	_LUA_ENUM_LIST_INC
#define	_LUA_ENUM_LIST_INC

#define ENUM_XML 1
#define ENUM_LUA 2

#ifndef	ENUM_SET
	#define ENUM_STARTEX(n,t)              enum n {
	#define ENUM_STARTEX_PUBLISH_AS(n,t,a) enum n {
	#define ENUM_SET(n,v)	              n = v,
	#define ENUM_SET_AS(n,v,as)	          n = v,
	#define ENUM_AUTO(n)                  n,
	#define ENUM_AUTO_AS(n,as)	          n,
	#define ENUM_END()                    };
#endif

//#include "interactioncomponent.h"

// On Screen Debug Channels
ENUM_STARTEX_PUBLISH_AS(CHANNEL, ENUM_LUA, OSD)
	ENUM_AUTO(AICOMBAT)
	ENUM_AUTO(NETWORK)
	ENUM_AUTO(MOVEMENT)
	ENUM_AUTO(CAMERA)
	ENUM_AUTO(OBJECTS)
	ENUM_AUTO(HAIR)
	ENUM_AUTO(DEBUG_CHAN)
	ENUM_AUTO(AISTATES)
	ENUM_AUTO(AIFORMATIONS)
	ENUM_AUTO(SCRIPT)
	ENUM_AUTO(ARMY)
	ENUM_AUTO(COUNT)
ENUM_END()

//Component construction types
ENUM_STARTEX(COMPONENTTYPE, ENUM_LUA)
	ENUM_AUTO_AS(CT_HIERARCHY, HIERARCHY)
	ENUM_AUTO_AS(CT_MESSAGEHANDLER, MESSAGEHANDLER)
	ENUM_AUTO_AS(CT_ANIMATOR, ANIMATOR)
	ENUM_AUTO_AS(CT_AUDIO, AUDIO)
	ENUM_AUTO_AS(CT_DYNAMICS, DYNAMICS)
	ENUM_AUTO_AS(CT_ATTACK, ATTACK)
	ENUM_AUTO_AS(CT_AWARE, AWARE)
	ENUM_AUTO_AS(CT_MOVEMENT, MOVEMENT)
	ENUM_AUTO_AS(CT_INPUT, INPUT)
	ENUM_AUTO_AS(CT_AI, AI)
	ENUM_AUTO_AS(CT_LOD, LOD)
	ENUM_AUTO_AS(CT_NETWORK, NETWORK)
	ENUM_AUTO_AS(CT_SCENEELEMENT, SCENEELEMENT)
	ENUM_AUTO_AS(CT_HAIR, HAIR)
	ENUM_AUTO_AS(CT_AIMING, AIMING)
ENUM_END()

//Interaction types
/*ENUM_STARTEX(INTERACTION_TYPE, ENUM_LUA)
	ENUM_AUTO(ICF_NONE)
	ENUM_AUTO(ICF_PICKUPABLE)
	ENUM_AUTO(ICF_RUNNING_PICKUPABLE)
	ENUM_AUTO(ICF_CATCHABLE)
	ENUM_AUTO(ICF_USABLE)
	ENUM_AUTO(ICF_ATTACKABLE)
ENUM_END()*/

#endif // _LUA_ENUM_LIST_INC

#ifndef HEADER_H
#define HEADER_H

#define  INCLUDE_HAVOK_PHYSICS_CLASSES 
#define INCLUDE_HAVOK_ANIMATION_CLASSES 

// Classlists
#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_All.cxx>

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>

#include <Physics/Dynamics/hkpDynamics.h>  
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/Simulation/hkpSimulation.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Collide/hkpCollide.h>						
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>		

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>	
#include <physics/utilities/dynamics/inertia/hkpinertiatensorcomputer.h>
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Query/Collector/BodyPairCollector/hkpAllCdBodyPairCollector.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Visualize/Shape/hkDisplayGeometry.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Base/Types/Geometry/hkStridedVertices.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h"
#include<stdio.h>
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h"
#include"../sdks/Havok_Physics/hk660r1/Source/Common/Base/Types/Geometry/hkGeometry.h"
#include"../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Shape/Misc/ConvexList/hkpConvexListShape.h"
#include <windows.h>
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h"
#include <GL/gl.h>
#include"../sdks/Havok_Physics/hk660r1/Source/Common/Internal/MeshSimplifier/hkMeshSimplifier.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Serialize/Packfile/hkPackfileReader.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Base/System/Io/IStream/hkIStream.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Utilities/Serialize/hkpPhysicsData.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Utilities/Serialize/hkpHavokSnapshot.h"


#include "../sdks/Havok_Physics/hk660r1/Demo/Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h"
#include "../sdks/Havok_Physics/hk660r1/Demo/Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h"
#include "../sdks/Havok_Physics/hk660r1/Demo/Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiDemo.h"

#include "../sdks/Havok_Physics/hk660r1/Demo/Demos/DemoCommon/DemoFramework/hkdemo.h"

#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Collection/StorageExtendedMesh/hkpStorageExtendedMeshShape.h>

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>


// havok animation 
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Serialize/Util/hkloader.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Common/Serialize/Util/hkRootLevelContainer.h"
#include  "../sdks/Havok_Physics/hk660r1/Demo/Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h"
#include  "../sdks/Havok_Physics/hk660r1/Source/Animation/Animation/hkaAnimationContainer.h"
#include  "../sdks/Havok_Physics/hk660r1/Source/Animation/Animation/Rig/hkaSkeleton.h"

#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h"
#include <../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>

#include <../sdks/Havok_Physics/hk660r1/Source/Physics/Internal/Collide/BroadPhase/hkpBroadPhaseCastCollector.h>
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h"
#include "../sdks/Havok_Physics/hk660r1/Source/Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h"

#include <windows.h>
#include <math.h>
#include "vbo.h"
#include "Model_3DS.h"

#include "resourceManager.h"

#include "opengl.h"
#include "fmod.h"

#include "time.h"
#include <cmath>

hkpWorld* m_world;
hkp1dAngularFollowCam m_camera;

hkpRigidBody *mybox,*fixbox,*convex1,*table,*box[10];
vector<hkpRigidBody*> model_bodies_xml;


hkThreadMemory* threadMemory;
char* stackBuffer;

hkpShape *wheel[4];

static int camera=0;
int CURR_VIEW =0;

//functions........
void Render();
int KeyBoardFunc();
static void HK_CALL errorReport(const char* msg, void*)
{

}

#include "rigidbody.h"
vector<Model*> models;


//objests draw or not...
bool BANNER_SHOW=1;
bool SKYBOX_SHOW=1;
bool SIDE_WALL_SHOW=1;


#include "havokFuncs.h"
#include "road.h"
#include "aiCars.h"
#include "car.h"

MANUAL_CAR *mycar;
vector<AI_CAR*>AI_cars;

#include "menu.h"
#include "initHavok.h"
#include "level.h"

#endif

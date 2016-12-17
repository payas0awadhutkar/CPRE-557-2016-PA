#ifndef CARS_H
#define CARS_H

#include "havokFuncs.h"
#include "header.h"
#include "road.h"
#include "aiCars.h"
#include <Demos/Physics/Api/Vehicle/VehicleApi/FrictionMapVehicleRaycastWheelCollide.h>

#pragma region ray+collosion
class rayCaster : public hkpBroadPhaseCastCollector {   
			public:
				rayCaster( const hkpWorldRayCastInput& input, hkpWorldRayCastOutput* output );
				rayCaster(){}
			protected:
				virtual	hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadPhaseHandle, int castIndex );

			public:
				hkpWorldRayCastInput   m_input;
				hkpWorldRayCastOutput* m_output;
};

rayCaster::rayCaster( const hkpWorldRayCastInput& input, hkpWorldRayCastOutput* output) {
	m_input = input;
	m_output = output;
}
hkReal rayCaster::addBroadPhaseHandle(const hkpBroadPhaseHandle* broadphaseHandle, int castIndex) {
	const hkpCollidable* col = static_cast<hkpCollidable*>( static_cast<const hkpTypedBroadPhaseHandle*>(broadphaseHandle)->getOwner() );
	const hkpShape* shape = col->getShape();

	if (shape) {
		hkpShapeRayCastInput sinput;
		const hkTransform& trans = col->getTransform();

		// transform the ray into local space
		sinput.m_from.setTransformedInversePos(trans, m_input.m_from);
		sinput.m_to.setTransformedInversePos(  trans, m_input.m_to);

		// subshape filtering turned off
		sinput.m_rayShapeCollectionFilter = HK_NULL;

		if (shape->castRay(sinput, *m_output)) {
			// transform the normal back into worldspace
			m_output->m_rootCollidable = col;
			m_output->m_normal.setRotatedDir( trans.getRotation(), m_output->m_normal );
		}
	}
	return m_output->m_hitFraction;
}

hkpConvexVerticesShape* createCarChassisShape() {
	hkReal xSize = 2.50f;
	hkReal ySize = 0.35f;
	hkReal zSize = 1.0f;

	hkReal xBumper = 1.9f;
	hkReal yBumper = 0.25f;
	hkReal zBumper = 1.0f;

	hkReal xRoofFront = 0.4f;
	hkReal xRoofBack = -2.4f;
	hkReal yRoof = ySize + 0.45f;
	hkReal zRoof = 0.5f;

	hkReal xDoorFront = xRoofFront;
	hkReal xDoorBack = xRoofBack;
	hkReal yDoor = ySize;
	hkReal zDoor = zSize;

	int numVertices = 22;

	// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float).
	int stride = sizeof(float) * 4;

	float vertices[] = { 
		xSize, ySize, zSize, 0.0f,		// v0
		xSize, ySize, -zSize, 0.0f,		// v1
		xSize, -ySize, zSize, 0.0f,		// v2
		xSize, -ySize, -zSize, 0.0f,	// v3
		-xSize, -ySize, zSize, 0.0f,	// v4
		-xSize, -ySize, -zSize, 0.0f,	// v5

		xBumper, yBumper, zBumper, 0.0f,	// v6
		xBumper, yBumper, -zBumper, 0.0f,	// v7
		-xBumper, yBumper, zBumper, 0.0f,	// v8
		-xBumper, yBumper, -zBumper, 0.0f,	// v9

		xRoofFront, yRoof, zRoof, 0.0f,		// v10
		xRoofFront, yRoof, -zRoof, 0.0f,	// v11
		xRoofBack, yRoof, zRoof, 0.0f,		// v12
		xRoofBack, yRoof, -zRoof, 0.0f,		// v13

		xDoorFront, yDoor, zDoor, 0.0f,		// v14
		xDoorFront, yDoor, -zDoor, 0.0f,	// v15
		xDoorFront, -yDoor, zDoor, 0.0f,	// v16
		xDoorFront, -yDoor, -zDoor, 0.0f,	// v17

		xDoorBack, yDoor, zDoor, 0.0f,		// v18
		xDoorBack, yDoor, -zDoor, 0.0f,		// v19
		xDoorBack, -yDoor, zDoor, 0.0f,		// v20
		xDoorBack, -yDoor, -zDoor, 0.0f,	// v21
	};
	
	hkStridedVertices		stridedVerts;
	stridedVerts.m_numVertices	=	numVertices;
	stridedVerts.m_striding		=	stride;
	stridedVerts.m_vertices		=	vertices;
	
	return new hkpConvexVerticesShape(stridedVerts);
}

hkpVehicleInstance* createVehicle( VehicleSetup& vehicleSetup, hkpRigidBody* chassis) {
	hkpVehicleInstance* vehicle;
	{
		vehicle = new hkpVehicleInstance( chassis );
		vehicleSetup.buildVehicle( m_world, *vehicle );
	}

	{
		vehicle->addToWorld( m_world );
	}

	{
		m_world->addAction(vehicle);
	}
	return vehicle;
}

hkpRigidBody *setupVehicles(hkpVehicleInstance* &v,hkVector4 &pos) {
	hkpRigidBody* chassisRigidBody;
	m_world->lock();
	VehicleSetup setup;
	{
		
		wheel[0]=VehicleApiUtils::createDisc(.4, .2);
		// Create vehicle's chassis shape.
		hkpConvexVerticesShape* chassisShape = createCarChassisShape(); 

		//createDisplayWheels(.4);

		int chassisLayer = 1;
		for (int vehicleId = 0; vehicleId < 1; vehicleId++) {
			{
				{
					hkpRigidBodyCinfo chassisInfo;
					chassisInfo.m_mass = 500.0f;	
					chassisInfo.m_shape = chassisShape;
					chassisInfo.m_friction = 1.8f;
					chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
					// Position chassis on the ground.

					// Inertia tensor will be set by VehicleSetup.
					chassisInfo.m_position.set(pos(0),pos(1),pos(2));
					chassisInfo.m_inertiaTensor.setDiagonal(.1f, 1.0f, 1.0f);
					
					chassisInfo.m_centerOfMass.set( -.7f, -1.0f, 0.0f);
					chassisInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( chassisLayer, 0 );
					
					chassisRigidBody = new hkpRigidBody(chassisInfo);
				}
				hkpVehicleInstance *temp_v=createVehicle( setup, chassisRigidBody);
				v=temp_v;
	 
				if (vehicleId == 0) 	 
				{ 	 
						 
				}
				
				chassisRigidBody->removeReference();
			}
		}
		chassisShape->removeReference();
		v->removeReference();
	} 
	m_world->unlock();
	return chassisRigidBody;
}

class MyCollisionListener : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener {
   	public:
		MyCollisionListener(){};
		MyCollisionListener( hkpRigidBody* rigidBody);
		~MyCollisionListener();

		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event );

		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event);

		// Called before a contact point gets removed. We do not implement this for this demo.
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event );

		// Called just before the collisionResult is passed to the constraint system (solved).
		virtual void contactProcessCallback( hkpContactProcessEvent& event );

		// Called when the entity is deleted. Important to use this event to remove ourselves as a listener.
		void entityDeletedCallback( hkpEntity* entity );

		// Called when the entity is removed from the hkpWorld
		void entityRemovedCallback( hkpEntity* entity );

	protected:
		// a small structure, which gets attached to each contact point
		struct  ContactPointInfo
		{
			int m_uniqueId;
		};
		int m_uniqueIdCounter;
	public:
		int m_reportLevel;
};
MyCollisionListener::MyCollisionListener(hkpRigidBody* rigidBody) {
	rigidBody->addCollisionListener( this );
	rigidBody->addEntityListener( this );
	m_uniqueIdCounter = 0;
}


MyCollisionListener::~MyCollisionListener() {

}

void MyCollisionListener::contactPointAddedCallback(hkpContactPointAddedEvent& event) {
		const hkVector4& start = event.m_contactPoint->getPosition();
		for (int i = 0; i < 20; i++) {
			hkVector4 dir( hkMath::sin( i * 1.0f ), hkMath::cos( i * 1.0f ), hkMath::sin(i * 5.0f ) );
			dir.setMul4(0.3f, dir);
			hkVector4 end;		end.setAdd4(start, dir);
		}

	if (event.m_contactPointProperties->getUserData() == HK_NULL) {
		ContactPointInfo* info = new ContactPointInfo;
		info->m_uniqueId = m_uniqueIdCounter++;
		event.m_contactPointProperties->setUserData( reinterpret_cast<hkUlong>(info) );

			if ( event.isToi() )
			{
 				//printf("Toi userId=%i created\n", info->m_uniqueId );
			}
			else {
				int cpId = event.asManifoldEvent().m_contactPointId;
				//printf("Contact Point userId=%i created: contactId=%i\n", info->m_uniqueId, cpId );
			}
		}
	}

	event.m_callbackFiredFrom->setProcessContactCallbackDelay(0);
}

void MyCollisionListener::contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) {
	ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties->getUserData() );
	if ( !info )
		return;

		if ( event.isToi() ) {
			//printf("Toi userId=%i deleted\n", info->m_uniqueId );
		}
		else {
			int cpId = event.m_contactPointId;
			//printf("Contact Point userId=%i deleted. contactId=%i\n", info->m_uniqueId, cpId );
		}
	delete info;
	event.m_contactPointProperties->setUserData( HK_NULL );
}

void MyCollisionListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event) {
	ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties->getUserData() );
	if (!info)
		return;

		if ( event.isToi() )
		{
			//printf("Toi userId=%i confirmed\n", info->m_uniqueId );
		}
		else
		{
			hkContactPointId id = event.getContactPointId();
			// you can get the contactMgr here by calling:  event.getContactMgr();
			//printf("Contact userId=%i confirmed. contactId=%i\n", info->m_uniqueId, id );
		}
	}
}
void MyCollisionListener::contactProcessCallback( hkpContactProcessEvent& event ) {
	hkpProcessCollisionData& result = *event.m_collisionData;
	int size = result.getNumContactPoints();

	for (int i = 0; i < size; i++ ) {
		hkpProcessCdPoint& cp = result.m_contactPoints[i]; 
			ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties[i]->getUserData() );
			if ( (info) /*&& (m_reportLevel >= hkDemoEnvironment::REPORT_INFO)*/ )
			{
				int contactId = cp.m_contactPointId;
				if(event.m_contactPointProperties[i]->getImpulseApplied()>300)
					;//printf("Contact userId=%i processed. Impulse %f. Contact Point Id=%i\n", info->m_uniqueId, event.m_contactPointProperties[i]->getImpulseApplied(), contactId );
			}

		// draw the contact points and normals in white
		{
			const hkVector4& start = result.m_contactPoints[i].m_contact.getPosition();
			hkVector4 normal       = result.m_contactPoints[i].m_contact.getNormal();

			// For ease of display only, we'll always draw the normal "up" (it points from entity 'B'
			// to entity 'A', but the order of A,B is arbitrary) so that we can see it. Thus, if it's 
			// pointing "down", flip its direction (and scale), only for display.
			normal.mul4(5.0f * normal(1));
			//HK_DISPLAY_ARROW(start, normal, hkColor::WHITE);
		}
	}
}

void MyCollisionListener::entityDeletedCallback( hkpEntity* entity )
{
	entity->removeCollisionListener( this ); 
	delete this;
}


void MyCollisionListener::entityRemovedCallback( hkpEntity* entity )
{
	// remove the collision event listener only when the entity is deleted
	// not implemented as no entity is deleted in this particular game
}
#pragma endregion 


class LevelInfo {
public:
	int level_activated;
	int curr_lap;
	int max_laps;
	int currpoint;
	int position;
	int curr_checkPoint;
	bool pos_correcting;

	void Reset() {
		level_activated=1;
		max_laps=atoi(myData.getUserData("total_laps")->value.c_str());
		curr_lap=1;
		currpoint=0;
		curr_checkPoint=0;
	}
	LevelInfo() {
		Reset();	
	}
	int nextPointAhead(int i) {
		if( (currpoint+i) > roadPoints.size()-1)
			return (currpoint+i)%(roadPoints.size()-1)-1;
		else if( (currpoint+i) < 0)
			return (roadPoints.size()-1)+currpoint-i+1;
		else 
			return (currpoint+i);
	}
	int NearestPoint(hkVector4 &carpos,int start,int end) {
		float min_length=10000;
		float min_length_point=currpoint;
		
		if(start>end) {
			for(int i=0;i<=roadPoints.size()-1;i++) {
				if(lineLength( Vec3TohkVector(roadPoints[i].right),carpos)< min_length) {
					min_length=lineLength( Vec3TohkVector(roadPoints[i].right),carpos);
					min_length_point=i;
				}	
			}
		}
		else {
			for(int i=start;i<=end;i++) {
				if(lineLength( Vec3TohkVector(roadPoints[i].right),carpos)< min_length) {
					min_length=lineLength( Vec3TohkVector(roadPoints[i].right),carpos);
					min_length_point=i;
				}	
			}
		}
		return min_length_point;
	}
};

class CarModel {
public:
	Model_3DS *m;
	Model_3DS *wheelModel;

	char *path;
	char name[100];

	__Carmodel carModelData;
	carModelCommanArgs cdata;
	carModelCommanArgs wheeldata[4];

	CarModel(char *path=NULL,int scale=1) {
		//car model
		{
			this->path=new char[strlen(path)+1];
			this->path=path;
			this->m=g_resource.AddModel_3DS(path,"car_model");
		}

		//car wheels model
		{
			char Wheelpath[]="Data/models/Car1/Wheel_002.3ds";
			this->wheelModel=g_resource.AddModel_3DS(Wheelpath,"wheel_model");
		}

		string dirPath(path);
		dirPath=dirPath.substr(0,dirPath.rfind("/"));
		carModelData.Load(dirPath.append("/cardata.txt").c_str());
	
		cdata=*carDataTocarModelCommanArgs(*carModelData.getUserData("body"));
		wheeldata[0]=*carDataTocarModelCommanArgs(*carModelData.getUserData("wheel1"));
		wheeldata[1]=*carDataTocarModelCommanArgs(*carModelData.getUserData("wheel2"));
		wheeldata[2]=*carDataTocarModelCommanArgs(*carModelData.getUserData("wheel3"));
		wheeldata[3]=*carDataTocarModelCommanArgs(*carModelData.getUserData("wheel4"));
	}
	void Draw(hkVector4 &c=hkVector4(0,0,0)) {
		if(m==NULL) return;
		else {
			glPushMatrix();
				//cdata.
				glColor3f(c(0),c(1),c(2));
				glRotatef(cdata.rot.w,cdata.rot.x,cdata.rot.y,cdata.rot.z);
				glTranslatef(cdata.pos(0),cdata.pos(1),cdata.pos(2));

				m->scale.x=cdata.scale.x;
				m->scale.y=cdata.scale.y;
				m->scale.z=cdata.scale.z;

				glDisable(GL_LIGHTING);
				m->Draw();
			glPopMatrix();
		}
	}
	void DrawWheels() {
		hkVector4 wheels_displacement[]={
										hkVector4(1.7,0,-.9),
										hkVector4(1.7,0,.9),
										hkVector4(-1.5,0,.9),
										hkVector4(-1.5,0,-.9)
									};

		for(int i=0;i<4;i++) {
			glPushMatrix();	
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

				hkVector4 pos;
				pos(0)=wheeldata[i].pos.x+wheels_displacement[i](0);
				pos(1)=wheeldata[i].pos.y+wheels_displacement[i](1);
				pos(2)=wheeldata[i].pos.z+wheels_displacement[i](2);
				glTranslatef( pos(0),pos(1)-.2,pos(2));

				if(i==0)glRotatef(90,0,1,0);
				else if(i==1)glRotatef(-90,0,1,0);
				else if(i==2)glRotatef(-90,0,1,0);
				else if(i==3)glRotatef(90,0,1,0);

				wheelModel->scale.x=wheeldata[i].scale.x;
				wheelModel->scale.y=wheeldata[i].scale.y;
				wheelModel->scale.z=wheeldata[i].scale.z;

				wheelModel->Draw();
			glPopMatrix();	
		}
	}
};

class CAR : public CarModel,public LevelInfo,public MyCollisionListener {
public:
	hkpVehicleInstance *VehicleInst;
	hkpRigidBody *car;
	
	hkReal inputXPosition;
	hkReal inputYPosition;

	hkReal steeringLock;

	struct CAR_SOUNDS{
		Sound* music_crash;
		Sound* music_running;
		Sound* music_break;
		CAR_SOUNDS(){
			// add sounds later
		}
	}sounds;

	int SPEED_UP,SPEED_DOWN,TURN_LEFT,TURN_RIGHT,HAND_BREAK,SETTING,BREAK;

	CAR(char *path=NULL,int scale=1)
		:CarModel(path,scale)
	{		
		inputXPosition=0;
		inputYPosition=0;
		steeringLock=0;
	}
	void buildVehicle(const hkpWorld* world) {
		hkpVehicleInstance *vehicle=this->VehicleInst;
  
		vehicle->m_data				= new hkpVehicleData;
		vehicle->m_driverInput		= new hkpVehicleDefaultAnalogDriverInput;
		vehicle->m_steering			= new hkpVehicleDefaultSteering;
		vehicle->m_engine			= new hkpVehicleDefaultEngine;
		vehicle->m_transmission		= new hkpVehicleDefaultTransmission;
		vehicle->m_brake				= new hkpVehicleDefaultBrake;
		vehicle->m_suspension		= new hkpVehicleDefaultSuspension;
		vehicle->m_aerodynamics		= new hkpVehicleDefaultAerodynamics;
		vehicle->m_velocityDamper	= new hkpVehicleDefaultVelocityDamper;

		vehicle->m_wheelCollide		= new FrictionMapVehicleRaycastWheelCollide;

		VehicleSetup setup;
		setup.setupVehicleData( world, *vehicle->m_data );

		vehicle->m_tyreMarks		= new hkpTyremarksInfo( *vehicle->m_data, 128 );

		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultAnalogDriverInput* >(vehicle->m_driverInput) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultSteering*>(vehicle->m_steering));
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultEngine*>(vehicle->m_engine) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultTransmission*>(vehicle->m_transmission) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultBrake*>(vehicle->m_brake) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultSuspension*>(vehicle->m_suspension) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultAerodynamics*>(vehicle->m_aerodynamics) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultVelocityDamper*>(vehicle->m_velocityDamper) );

		setup.setupWheelCollide( world, *vehicle, *static_cast< hkpVehicleRayCastWheelCollide*>(vehicle->m_wheelCollide) );

		setup.setupTyremarks( *vehicle->m_data, *static_cast< hkpTyremarksInfo*>(vehicle->m_tyreMarks) );
		
		CarSettings();
		// Check that all components are present.
		HK_ASSERT(0x0, vehicle->m_data );
		HK_ASSERT(0x7708674a,  vehicle->m_driverInput );
		HK_ASSERT(0x5a324a2d,  vehicle->m_steering );
		HK_ASSERT(0x7bcb2aff,  vehicle->m_engine );
		HK_ASSERT(0x29bddb50,  vehicle->m_transmission );
		HK_ASSERT(0x2b0323a2,  vehicle->m_brake );
		HK_ASSERT(0x7a7ade23,  vehicle->m_suspension );
		HK_ASSERT(0x6ec4d0ed,  vehicle->m_aerodynamics );
		HK_ASSERT(0x67161206,  vehicle->m_wheelCollide );
		HK_ASSERT(0x295015f1,  vehicle->m_tyreMarks );

		vehicle->m_deviceStatus = new hkpVehicleDriverInputAnalogStatus;
		hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)vehicle->m_deviceStatus;
		deviceStatus->m_positionY = -0.4f;

		// Turn.
		deviceStatus->m_positionX = 0.3f;

		// Defaults
		deviceStatus->m_handbrakeButtonPressed = false;
		deviceStatus->m_reverseButtonPressed = false;

		vehicle->init();
	}

	int CreateCar(hkVector4 &pos) {
		this->inputXPosition=0;
		this->inputYPosition=0;
		car=setupVehicles(pos);
		//CarSettings();
		return car?1:0 ;
	}

	void CarSettings() {
		if(carModelData.getUserData("info")==NULL)return;
		//else printf("Car settings changed");

		//transmission.....
		int max_speed= atoi(carModelData.getUserData("info")->searchByName("topspeed")->value[0].c_str());
		float wheel_radius= atof(carModelData.getUserData("info")->searchByName("wheel_radius")->value[0].c_str());
		float max_engine_rpm= atof(carModelData.getUserData("info")->searchByName("max_engine_rpm")->value[0].c_str());
		int num_gears=atoi(carModelData.getUserData("info")->searchByName("num_gears")->value[0].c_str());

		//printf("%d %f %f %d",max_speed,wheel_radius,max_engine_rpm,num_gears);

		hkpVehicleDefaultTransmission *transmission=(hkpVehicleDefaultTransmission *)this->VehicleInst->m_transmission;
		transmission->m_primaryTransmissionRatio = hkpVehicleDefaultTransmission::calculatePrimaryTransmissionRatio( 
																						max_speed,
																						wheel_radius,
																						max_engine_rpm,
																						transmission->m_gearsRatio[num_gears - 1 ] );
		//wheels.....
		int wheel_mass= atoi(carModelData.getUserData("info")->searchByName("wheel_mass")->value[0].c_str());
		Vector3 wheel_friction( carModelData.getUserData("info")->searchByName("wheel_friction")->getToVec3());
		for(int i=0;i<4;i++) {
			this->VehicleInst->m_data->m_wheelParams[i].m_friction=wheel_friction(i);
			this->VehicleInst->m_data->m_wheelParams[i].m_mass=wheel_mass;
		}
		
		//steering.....
		hkpVehicleDefaultSteering *steering=(hkpVehicleDefaultSteering*)this->VehicleInst->m_steering;
		float max_steering_angle= atof(carModelData.getUserData("info")->searchByName("max_steering_angle")->value[0].c_str()) * ( HK_REAL_PI / 180 );
		steering->m_maxSteeringAngle=max_steering_angle;

		//data.....
		Vector3 RPY_factor(carModelData.getUserData("info")->searchByName("RPY_factor")->getToVec3());
		float friction_equalizer=atof(carModelData.getUserData("info")->searchByName("friction_equalizer")->value[0].c_str());
		this->VehicleInst->m_data->m_torqueRollFactor=RPY_factor(0);
		this->VehicleInst->m_data->m_torquePitchFactor=RPY_factor(1);
		this->VehicleInst->m_data->m_torqueYawFactor=RPY_factor(2);
		this->VehicleInst->m_data->m_frictionEqualizer=friction_equalizer;

	}
	int SetWheelsPos(hkVector4 &w1,hkVector4 &w2,hkVector4 &w3,hkVector4 &w4)
	{
		VehicleInst->m_suspension->m_wheelParams[0].m_hardpointChassisSpace=w1;
		VehicleInst->m_suspension->m_wheelParams[1].m_hardpointChassisSpace=w2;
		VehicleInst->m_suspension->m_wheelParams[2].m_hardpointChassisSpace=w3;
		VehicleInst->m_suspension->m_wheelParams[3].m_hardpointChassisSpace=w4;

		return 0;
	}
	void Draw(hkVector4 &c=hkVector4(0,0,0)) {
		hkTransform t=this->car->getTransform();
		hkReal *p=new hkReal[16];
		t.get4x4ColumnMajor(p);

		glPushMatrix();
			glMultMatrixf(p);
			CarModel::Draw();
		glPopMatrix();

		DrawWheels();
		delete p;
	}
	void DrawWheels() {
		for (int i = 0; i < 4; i++) {
			hkVector4    pos;
			hkQuaternion rot;

			VehicleInst->calcCurrentPositionAndRotation( VehicleInst->getChassis(), 
							VehicleInst->m_suspension,					
							i,
							pos, rot );
			hkVector4 x;x.setRotatedDir(this->car->getRotation(),hkVector4(wheeldata[i].pos.x,wheeldata[i].pos.y,wheeldata[i].pos.z));
			pos.add4(x);
			hkTransform trans(rot, pos);
			hkReal *p=new hkReal[16];
			trans.get4x4ColumnMajor(p);
			glPushMatrix();

				glMultMatrixf(p);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				if(i==0)glRotatef(90,0,1,0);
				else if(i==1)glRotatef(-90,0,1,0);
				else if(i==2)glRotatef(-90,0,1,0);
				else if(i==3)glRotatef(90,0,1,0);
				
				wheelModel->scale.x=wheeldata[i].scale.x;
				wheelModel->scale.y=wheeldata[i].scale.y;
				wheelModel->scale.z=wheeldata[i].scale.z;
				
				wheelModel->Draw();
			
			glPopMatrix();
			delete p;
		}
	}
	void AddCollosionListener() {
		car->addCollisionListener( this );
		car->addEntityListener( this );
		this->m_uniqueIdCounter = 0;
	}
	void contactProcessCallback( hkpContactProcessEvent& event) {
		hkpProcessCollisionData& result = *event.m_collisionData;
		int size = result.getNumContactPoints();

		for (int i = 0; i < size; i++ )
		{
			hkpProcessCdPoint& cp = result.m_contactPoints[i];
			{
				ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties[i]->getUserData() );
				if ( (info) /*&& (m_reportLevel >= hkDemoEnvironment::REPORT_INFO)*/ )
				{
					int contactId = cp.m_contactPointId;
					if(event.m_contactPointProperties[i]->getImpulseApplied()>300)
					{
						// this place could be used to play crashing sounds.
						// I'm not interested in doing it.
					}
					car->activate();
				}
			}

			// draw the contact points and normals in white
			{
				const hkVector4& start = result.m_contactPoints[i].m_contact.getPosition();
				hkVector4 normal       = result.m_contactPoints[i].m_contact.getNormal();
				normal.mul4(5.0f * normal(1));
				//HK_DISPLAY_ARROW(start, normal, hkColor::WHITE);
			}
		}
	}

	hkpRigidBody *setupVehicles(hkVector4 &pos)
	{
		hkpRigidBody* chassisRigidBody;
		m_world->lock();
		VehicleSetup setup;
		{
			wheel[0]=VehicleApiUtils::createDisc(.4, .2);
			
			hkpConvexVerticesShape* chassisShape = createCarChassisShape(); 

			int chassisLayer = 1;

			hkpRigidBodyCinfo chassisInfo;
			chassisInfo.m_mass = 1500.0f;	
			chassisInfo.m_shape = chassisShape;
			chassisInfo.m_friction = 2.8f;
			chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			chassisInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
			// Position chassis on the ground.

			// Inertia tensor will be set by VehicleSetup.
			chassisInfo.m_position.set(pos(0),pos(1),pos(2));
			chassisInfo.m_inertiaTensor.setDiagonal(1.0f, 1.0f, 1.0f);
			
			chassisInfo.m_centerOfMass.set( -.7f, -1.0f, 0.0f);
			chassisInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( chassisLayer, 0 );
			
			chassisRigidBody = new hkpRigidBody(chassisInfo);
		
			//VehicleInst=createVehicle( setup, chassisRigidBody);
			{
				VehicleInst=new hkpVehicleInstance(chassisRigidBody);
				//setup.buildVehicle( m_world, *VehicleInst);
				buildVehicle( m_world);
				
				VehicleInst->addToWorld( m_world );
				m_world->addAction(VehicleInst);
			}
						
			chassisRigidBody->removeReference();
			chassisShape->removeReference();
			VehicleInst->removeReference();
		} 
		m_world->unlock();
		return chassisRigidBody;
	}
	hkReal getRotationAngle(hkVector4 &axis)
	{
		hkQuaternion q(this->car->getRotation());
		hkReal angle;
		hkQuaternion x;
		q.decomposeRestAxis(axis,x,angle);
		return angle;  //in radian
	}
	enum FacingDirection
	{
		FORWARD,BACK,   //x
		UP,DOWN,		//y
		RIGHT,LEFT,		//z
	};

	bool pointInFacingDirection(const hkVector4 & point, FacingDirection dir)
	{
		hkVector4 pointBodyVec;
		pointBodyVec.setSub4(point, this->car->getPosition());

		hkVector4 bodyDir = this->car->getTransform().getColumn(dir / 2);
	    
		if(dir % 2 != 0)
			bodyDir.mul4(-1.f);

		return bodyDir.dot3(pointBodyVec) < 0.f;
	}

	void levelProcessing()
	{
		int max_checkPoints=roadPoints.size()/20;
		int last_checkPoint=max_checkPoints * 20;
		
		if( this->curr_checkPoint >= last_checkPoint-20)
		{
			if( this->currpoint == roadPoints.size()-1 )
			{
				this->curr_lap++;
				this->curr_checkPoint=20;
			}
		}
		if(this->curr_lap > max_laps )
		{
			this->steeringLock=1;
			hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)VehicleInst->m_deviceStatus;
			deviceStatus->m_handbrakeButtonPressed =true;
		}

		if(this->currpoint >= this->curr_checkPoint &&  this->currpoint <= this->curr_checkPoint+5)
		{
			this->curr_checkPoint+=20;
		}
	}

	void SetPosToRoadPoint(int i ,int heightFactor=15)
	{
		car->setRotation(hkQuaternion(hkVector4(0,1,0),roadPoints[i].angle));
		car->setPosition(   hkVector4( 
							(roadPoints[i].left.x+roadPoints[i].right.x)/2,
							roadPoints[i].left.y+heightFactor,
							(roadPoints[i].left.z+roadPoints[i].right.z)/2 ));
		car->activate();
	}
};


void RenderPoint(int i)
{
	glDisable(GL_LIGHTING);
		glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(roadPoints[i].right(0),roadPoints[i].right(1),roadPoints[i].right(2));
			glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
			glColor3f(1,1,0);
			glTranslatef(roadPoints[i].left(0),roadPoints[i].left(1),roadPoints[i].left(2));
			glutSolidCube(1.0);
		glPopMatrix();
	glEnable(GL_LIGHTING);
}



class MANUAL_CAR :public CAR
{

public:
	float scale;
	
	MANUAL_CAR(char *path=NULL,int scale=1):CAR(path,scale)
	{
		this->scale=scale;
		//pathTracking Settings..
		currpoint=0;

		//printf("\n\n--%f %f %f %f--\n",cdata.rot.x,cdata.rot.y,cdata.rot.z,cdata.rot.w);
	}
	

	void SetHandling(int up,int right,int down,int left)
	{
		SPEED_UP=up;
		SPEED_DOWN=down;
		TURN_LEFT=left;
		TURN_RIGHT=right;
	}

	int steer()
	{
		float currspeed=VehicleInst->calcKMPH();

		char *txt=ArgsString("Car Speed:%d",(int)currspeed);
		glutPrint(2.5f, 1.1f, glutFonts[5], txt, 1.0f, 0.0f, 0.0f, 1.0f);
		delete txt;

		hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)VehicleInst->m_deviceStatus;

		float timestep=.035;
		const float steerSpeed     = 4.0f * timestep;
		const float backSteerSpeed = 10.0f * timestep;
		float deltaY = -inputYPosition  * steerSpeed ;
		float deltaX = -inputXPosition  * backSteerSpeed;


		if( gKeys['r'] )
		{
			if(!steeringLock)
				SetPosToRoadPoint(currpoint);
		}

		if( gKeys[SPEED_UP] )
		{
			if(currspeed==0.00)
				if(!car->isActive())car->activate();
			//if(currspeed<10)
				deltaY = -25.f*timestep;
		}
		else if ( gKeys[SPEED_DOWN]  || gKeys[80])
		{
			if(currspeed==0.00)
				if(!car->isActive())car->activate();
			//if(currspeed<10)
				deltaY = 25.f*timestep;
		}

		if ( gKeys[TURN_LEFT] )
		{
			if ( inputXPosition <= 0.0f){	deltaX = 0.0f;	}
			deltaX -= steerSpeed * .13;
			//if(VehicleInst->m_currentGear>1)
			//	deltaY = 0.025f;
		}
		else if ( gKeys[TURN_RIGHT] )
		{
			// Turn right.
			if ( inputXPosition >= 0.0f){	deltaX = 0.0f;	}
			deltaX += (steerSpeed)* .13;
			//if(VehicleInst->m_currentGear>1)
			//	deltaY = 0.025f;
		}

		
		inputXPosition = hkMath::clamp( inputXPosition+deltaX, -1.0f, 1.0f);
		inputYPosition = hkMath::clamp( inputYPosition+deltaY, -1.0f, 1.0f);

		if(!steeringLock)
		{
			deviceStatus->m_positionY = inputYPosition;
			deviceStatus->m_positionX = inputXPosition;
		}
		else
		{
			deviceStatus->m_positionY = 0;
			deviceStatus->m_positionX = 0;
		}
		//printf("%f %f \n",deviceStatus->m_positionY,deviceStatus->m_positionX);

		return 0;
	}

	void pathTracking()
	{
		hkReal carAngle= StanderizeAngle(this->getRotationAngle(hkVector4(0,1,0)));
		hkVector4 carpos(car->getPosition());
		currpoint=NearestPoint(carpos,0,roadPoints.size()-1);
		//currpoint=NearestPoint(carpos,nextPointAhead(-10),nextPointAhead(10));
		//RenderPoint(currpoint);
				
		if(!isbetween(carAngle, StanderizeAngle(roadPoints[currpoint].angle)+(PI/2) ,StanderizeAngle(roadPoints[currpoint].angle)-(PI/2))) {
			//currpoint=currpoint==0?roadPoints.size()-1:currpoint-1;
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			char *txt=ArgsString("WRONG DIRECTION");
			glutPrint(-2.0f, 1.8f, glutFonts[3],txt, 1.0f, 0.0f, 0.0f, 1.0f);
			delete txt;
		}

		levelProcessing();
	}
};

#endif

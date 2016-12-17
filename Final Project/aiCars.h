#ifndef AICARS_H
#define AICARS_H
#include "header.h"
#include "car.h"

class AI_CAR : public CAR {
public:

	int CURR_STEER_FLAG1;
	int CURR_STEER_FLAG2;

	rayCaster rays[2];

	hkVector4 CarColor;

	int counter;
	int lastpoint;

	AI_CAR(char *path=NULL,int scale=1):CAR(path,scale) {
		CURR_STEER_FLAG1=SPEED_UP;
		CURR_STEER_FLAG2=0;

		SPEED_UP=1;
		SPEED_DOWN=2;
		TURN_LEFT=3;
		TURN_RIGHT=4;
		HAND_BREAK=5;

		rays[0].m_output=NULL;
		rays[1].m_output=NULL;

		currpoint=0;
		counter=0;
	}

	int steer(int key,int key2);

	void Makeray(rayCaster &rayCast,hkVector4 &tpos1,hkVector4 &tpos2) {
		hkTransform t=this->car->getTransform(),t2;
		
		hkVector4 aheadOfCar;

		aheadOfCar.setTransformedPos( t, tpos1);
		rayCast.m_input.m_from.set( aheadOfCar(0),aheadOfCar(1),aheadOfCar(2));

		aheadOfCar.setTransformedPos( t, tpos2);
		rayCast.m_input.m_to.set( aheadOfCar(0),aheadOfCar(1),aheadOfCar(2));

		if(rayCast.m_output!=NULL)delete rayCast.m_output;
		rayCast.m_output=new hkpWorldRayCastOutput();
		
		hkpBroadPhase* broadPhase = m_world->getBroadPhase();
		hkpBroadPhase::hkpCastRayInput rayInput;
		rayInput.m_from = rayCast.m_input.m_from;
		rayInput.m_toBase = &rayCast.m_input.m_to;
		rayInput.m_aabbCacheInfo = HK_NULL;

		broadPhase->castRay(rayInput, &rayCast , hkSizeOf(rayCaster));
	}

	void Make_Draw_Rays() {
		Makeray(this->rays[0],hkVector4(2,0,-1.4),hkVector4(20,0,-1.4));  
		Makeray(this->rays[1],hkVector4(2,0,1.4),hkVector4(20,0,1.4));
	}
	
	void pathTracking();
};

void AI_CAR ::pathTracking() {

	levelProcessing(); 
	float currspeed=this->VehicleInst->calcKMPH();
	
	hkVector4 carpos(car->getPosition());
	if( NearestPoint(carpos,nextPointAhead(-10),nextPointAhead(10)) > currpoint)
		currpoint=NearestPoint(carpos,nextPointAhead(-10),nextPointAhead(10));

	hkVector4 curr_left_point=Vec3TohkVector(roadPoints[currpoint].left);
	hkVector4 curr_right_point=Vec3TohkVector(roadPoints[currpoint].right);

	hkVector4 mid ((roadPoints[currpoint].left(0)+roadPoints[currpoint].right(0))/2,
				   (roadPoints[currpoint].left(1)+roadPoints[currpoint].right(1))/2,
				   (roadPoints[currpoint].left(2)+roadPoints[currpoint].right(2))/2 );

	if(this->pointInFacingDirection(curr_right_point,CAR::FacingDirection::LEFT) && 
		this->pointInFacingDirection(curr_left_point,CAR::FacingDirection::RIGHT) )
	{
		if(this->getRotationAngle(hkVector4(0,1,0))>roadPoints[currpoint].angle)
			CURR_STEER_FLAG2=TURN_RIGHT;
		else
			CURR_STEER_FLAG2=TURN_LEFT;

			CURR_STEER_FLAG1=SPEED_UP; 
	}
	else if( this->pointInFacingDirection(curr_left_point,CAR::FacingDirection::LEFT) ) {
		CURR_STEER_FLAG1=SPEED_UP;
		CURR_STEER_FLAG2=TURN_RIGHT;
	}
	else {
		CURR_STEER_FLAG1=SPEED_UP;
		CURR_STEER_FLAG2=TURN_LEFT;   
	}

	if(lineLength(mid,curr_left_point) < 4 ) CURR_STEER_FLAG2=TURN_LEFT;
	else if(lineLength(mid,curr_right_point) < 4 ) CURR_STEER_FLAG2=TURN_RIGHT;


	if(this->pointInFacingDirection(Vec3TohkVector(roadPoints[currpoint].left),CAR::FacingDirection::FORWARD)
		&& this->pointInFacingDirection(Vec3TohkVector(roadPoints[nextPointAhead(1)].left),CAR::FacingDirection::BACK)) {
		currpoint=nextPointAhead(1);
		CURR_STEER_FLAG1=SPEED_UP;
		CURR_STEER_FLAG2=0;
	}

	//position_correction
	if(pos_correcting) {
		if(counter>=400) {
			SetPosToRoadPoint(currpoint);
			lastpoint=this->currpoint;
			counter=0;
		}
		if( this->currpoint ==  lastpoint ) {
			counter++;
		}
		else {
			lastpoint=this->currpoint;
			counter=0;
		}
	}

	
	if(1) {
		//ray tracking
		hkBool hit1=rays[0].m_output->hasHit();
			hkBool hit2=rays[1].m_output->hasHit();
			
			hkVector4 intersectionPointWorld[2];
			
			intersectionPointWorld[0].setInterpolate4( rays[0].m_input.m_from, rays[0].m_input.m_to, rays[0].m_output->m_hitFraction);
			intersectionPointWorld[1].setInterpolate4( rays[1].m_input.m_from, rays[1].m_input.m_to, rays[1].m_output->m_hitFraction);
			
			if (hit1 || hit2) {
				hkReal h1=lineLength(rays[0].m_input.m_from,intersectionPointWorld[0]);
				hkReal h2=lineLength(rays[1].m_input.m_from,intersectionPointWorld[1]);
				if(!hit1)h1=lineLength(rays[0].m_input.m_from,rays[0].m_input.m_to);
				if(!hit2)h2=lineLength(rays[1].m_input.m_from,rays[1].m_input.m_to);

				BREAK=SPEED_DOWN;
				float currspeed=this->VehicleInst->calcKMPH();
				
				if(h1 > h2) {	
					if(currspeed >=50) {
						CURR_STEER_FLAG1=BREAK;
						CURR_STEER_FLAG2=TURN_LEFT;
					}
					else if(currspeed <=20) {
						this->car->setAngularDamping(0);
						this->car->setAngularVelocity(hkVector4(0,0,0));

						if(h1<10) {
							CURR_STEER_FLAG1=SPEED_DOWN;
							CURR_STEER_FLAG2=TURN_RIGHT;
						}
						else {
							CURR_STEER_FLAG1=SPEED_UP;
							CURR_STEER_FLAG2=TURN_LEFT;
						}
					}
					else {
						CURR_STEER_FLAG1=SPEED_UP;
						CURR_STEER_FLAG2=TURN_LEFT;
					}
				}
				else {
					if(currspeed >=50) {
						CURR_STEER_FLAG1=BREAK;
						CURR_STEER_FLAG2=TURN_RIGHT;
					}
					else if(currspeed <=20) {
						this->car->setAngularDamping(0);
						this->car->setAngularVelocity(hkVector4(0,0,0));

						if(h2<10) {
							CURR_STEER_FLAG1=SPEED_DOWN;
							CURR_STEER_FLAG2=TURN_LEFT;
						}
						else {
							CURR_STEER_FLAG1=SPEED_UP;
							CURR_STEER_FLAG2=TURN_RIGHT;
						}
					}
					else {
						CURR_STEER_FLAG1=SPEED_UP;
						CURR_STEER_FLAG2=TURN_RIGHT;
					}
				}
			}
			else {

			}
	}
}

int AI_CAR::steer(int key,int key2) {
	float currspeed=this->VehicleInst->calcKMPH();

	hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)this->VehicleInst->m_deviceStatus;

	float timestep=.02;
	const float steerSpeed     = 4.0f * timestep;
	const float backSteerSpeed = 10.0f * timestep;
	float deltaY = -inputYPosition  * steerSpeed ;
	float deltaX = -inputXPosition  * backSteerSpeed;

	if ( key==SPEED_UP) {
		if(currspeed==0.00)
		if(!car->isActive())car->activate();
		
		deltaY = -.9f;//-25.f*timestep;
	}
	else if (key==SPEED_DOWN) {
		if(currspeed==0.00)
				if(!car->isActive())car->activate();
			deltaY = 9.f;//25.f*timestep;
	}
	else if(key==BREAK)
		deltaY =5.0f;

	if (key2==TURN_LEFT) {
		if (inputXPosition <= 0.0f) {	
			deltaX = 0.0f;
		}
			deltaX -= steerSpeed * .3;
	}
	else if (key2==TURN_RIGHT) {
		// Turn right.
			if ( inputXPosition >= 0.0f){	deltaX = 0.0f;	}
			deltaX += (steerSpeed)* .3;
	}


	this->inputXPosition = hkMath::clamp( this->inputXPosition+deltaX, -1.0f, 1.0f);
	this->inputYPosition = hkMath::clamp( this->inputYPosition+deltaY, -1.0f, 1.0f);

	if(!steeringLock) {
		deviceStatus->m_positionY = inputYPosition;
		deviceStatus->m_positionX = inputXPosition;
	}
	else {
		deviceStatus->m_positionY = 0;
		deviceStatus->m_positionX = 0;
	}
	return 0;
}
#endif

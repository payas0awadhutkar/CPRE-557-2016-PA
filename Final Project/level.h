#ifndef LEVEL_H 
#define LEVEL_H

#include "header.h"
#include "car.h"
	
enum COMMANDS {
	WAIT_FOR_FIRE_TO_START,
	STARTING_TIMER,
	START,
	RUNNING,
	ENDING
};
int CURRENT_COMMAND=0;

void LevelInit() {
	SetRoadPointsFromFile();
	CreateRoadBlocks();
	
	CURRENT_COMMAND=0;

	for(int i=0;i<AI_cars.size();i++) {
		AI_cars[i]->steeringLock=1;
		hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)AI_cars[i]->VehicleInst->m_deviceStatus;
		deviceStatus->m_handbrakeButtonPressed =true;

		AI_cars[i]->car->setPosition(hkVector4(0,1,i*5));
		AI_cars[i]->car->setRotation(hkQuaternion(hkVector4(0,1,0),-PI/4));

		AI_cars[i]->pos_correcting=0;

		AI_cars[i]->Reset();
	}
	mycar->car->setRotation(hkQuaternion(hkVector4(0,1,0),-PI/4));
	mycar->car->setPosition(hkVector4(0,1,-8));
	mycar->steeringLock=1;
	mycar->Reset();
	hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mycar->VehicleInst->m_deviceStatus;
	deviceStatus->m_handbrakeButtonPressed =true;
}

bool OnStart(int i=0) {
	static int counter=0;
	if(CURRENT_COMMAND == 0) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glutPrint(-2.0f, 1.8f,glutFonts[3],"Press 'ENTER' to Start Race..", 1.0f, 0.0f, 0.0f, 1.0f);
	
		if( gKeys[13])
		{
			CURRENT_COMMAND=1;
		}
	}
	else if( CURRENT_COMMAND == 1 ) {
		counter++;
		if( counter/50 >=0  && counter/50 <=2 ) {
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			char *txt=ArgsString("%d", 3-(counter/50) );
			glutPrint(-2.0f, 1.8f, glutFonts[3],txt, 1.0f, 0.0f, 1.0f, 1.0f);
			delete txt;
		}
		else {
			CURRENT_COMMAND=2;
			counter=0;
		}
	}
	else if(CURRENT_COMMAND == 2 ) {
		counter++;
		if(counter/100 >=0 && counter/100 <=1) {
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glutPrint(-2.0f, 1.8f,glutFonts[3],"GO! GO! GO!", 1.0f, 1.0f, 0.0f, 1.0f);
			mycar->pathTracking();
		}
		else {
			CURRENT_COMMAND = 3;
			counter=0;
			for(int i=0;i<AI_cars.size();i++)
				AI_cars[i]->pos_correcting=1;
		}
		for(int i=0;i<AI_cars.size();i++) {
			AI_cars[i]->steeringLock=0;
			hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)AI_cars[i]->VehicleInst->m_deviceStatus;
			deviceStatus->m_handbrakeButtonPressed =false;
			AI_cars[i]->pathTracking();
		}

		mycar->steeringLock=0;
		hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mycar->VehicleInst->m_deviceStatus;
		deviceStatus->m_handbrakeButtonPressed =false;
	}
	else if(CURRENT_COMMAND == 3) {
		//counter++;
		int aipos[8]={-1,-1,-1,-1,-1,-1,-1,-1};
			for(int i=0;i<AI_cars.size();i++) {
				int this_pos=0;
				for(int j=0;j<AI_cars.size();j++) {	
					if(i==j)continue;
					if(AI_cars[i]->curr_lap < AI_cars[j]->curr_lap )
						this_pos++;
					else if( AI_cars[i]->curr_lap == AI_cars[j]->curr_lap &&  AI_cars[i]->currpoint < AI_cars[j]->currpoint) 
						this_pos++;
					else if(AI_cars[i]->curr_lap == AI_cars[j]->curr_lap && AI_cars[i]->currpoint == AI_cars[j]->currpoint)
					{
						if(i>j)this_pos++;
					}
				}
				if( AI_cars[i]->curr_lap < mycar->curr_lap )this_pos++;
				else if( AI_cars[i]->curr_lap == mycar->curr_lap && AI_cars[i]->currpoint < mycar->currpoint )this_pos++;
				
				aipos[this_pos]=i;
				AI_cars[i]->position=this_pos+1;
			}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glutPrint(-7.0f,2.0f, glutFonts[3],"-Positions-", 1.0f, 0.0f, 1.0f, 0.0f);

		if(mycar->curr_lap > mycar->max_laps ) {
			counter=0;
			CURRENT_COMMAND++;
			return 1;
		}
		else {
			char *txt=ArgsString("LAP:%d/%d",mycar->curr_lap,mycar->max_laps);
			glutPrint(4.0f,2.0f, glutFonts[3],txt, 0.0f, 1.0f, 1.0f, 1.0f);
			delete txt;
		}

		for(int i=0;i<AI_cars.size()+1;i++) {
			if( aipos[i]==-1) {
				mycar->position=i+1;
				glutPrint(-7.0f,1.8f-i*.2, glutFonts[3],"YOU", 0.0f, 1.0f, 1.0f, 1.0f);
			}
			else {
				char *txt=ArgsString("AICAR-%d",aipos[i]+1);
				glutPrint(-7.0f,1.8f-i*.2, glutFonts[3],txt, 0.0f, 0.0f, 1.0f, 1.0f);
				delete txt;
			}
		}

		mycar->pathTracking();
		for(int i=0;i<AI_cars.size();i++) {
			AI_cars[i]->pathTracking();
		}
	}
	else if(CURRENT_COMMAND == 4 ) {
		char *txt=ArgsString("Hay You Complete The race and get %d position",mycar->position);
		glutPrint(-4.0f,1.0f, glutFonts[3],txt, 0.0f, 1.0f, 0.0f, 1.0f);
		delete txt;
		
		txt=ArgsString("Press 'ENTER' for continue...");
		glutPrint(-4.0f,0.2f, glutFonts[3],txt, 0.0f, 1.0f, 0.0f, 1.0f);
		delete txt;

		txt=ArgsString("Made By:Dheeraj Patni");
		glutPrint(-5.0f,1.9f, glutFonts[3],txt, 0.0f, 1.0f, 1.0f, 1.0f);
		delete txt;

		hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mycar->VehicleInst->m_deviceStatus;
		deviceStatus->m_handbrakeButtonPressed =true;
		mycar->steeringLock=1;


		for(int i=0;i<AI_cars.size();i++) {
			AI_cars[i]->pos_correcting=0;
		}
		if( gKeys[13]) {
			//LevelInit();
			CURRENT_COMMAND=0;
			CURR_MENU=MENU_CONDITIONS::MAIN_MENU;
		}
		
	}
	return 0;
}

#endif

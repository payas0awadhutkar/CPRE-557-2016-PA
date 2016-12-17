#ifndef INIT_HAVOK
#define INIT_HAVOK
#include "header.h"

void InitHavokBodies() {
		for(int i=0;i<models_xml.size();i++) {
			SettingObjects *obj=s.GetModelDataByName(models_xml[i].name);
			hkpRigidBody *rbody=CreateXMLMeshBody(models_xml[i],m_world,hkVector4(obj->Model_Pos.x,obj->Model_Pos.y,obj->Model_Pos.z),obj->Model_Size.x);
			model_bodies_xml.push_back(rbody);
		}

		vector<SettingObjects> models_objs_3DS= s.GetAllModelsByType("3DS");
		for(int i=0;i<models_objs_3DS.size();i++) {
			SettingObjects *obj=s.GetModelDataByName(models_objs_3DS[i].Model_Name);

			Model *model=new Model(NULL);
			model->SetModel( g_resource.AddModel_3DS((char*)models_objs_3DS[i].Model_Path.c_str(),"map",0) );
			model->scale=hkVector4(obj->Model_Size.x,obj->Model_Size.y,obj->Model_Size.z);
			model->position=hkVector4(obj->Model_Pos.x,obj->Model_Pos.y,obj->Model_Pos.z);
			model->rotation=hkQuaternion(hkVector4(obj->Model_Rot.x,obj->Model_Rot.y,obj->Model_Rot.z), obj->Model_Rot.w /180 * 3.14);
			
			model->MakeBody(m_world);
			//rbody=Create3DSMeshBody(models_3ds[i],m_world,hkVector4(obj->Model_Pos.x,obj->Model_Pos.y,obj->Model_Pos.z),obj->Model_Size);

			if(VBO_ENABLE);//model->m->DeleteVNT();
			else model->BuildDisplayList();

			model->rbody->setFriction(.5);
			models.push_back(model);
		}  

		int NUM_AI_CARS=atoi(myData.getUserData("total_ai_cars")->value.c_str());
		srand(time(0));
		int AiCarModelNo=rand()%9;
		for(int i=0;i<NUM_AI_CARS;i++) {
			if(++AiCarModelNo > 9)AiCarModelNo=1;
			char car_path[255]="";
			sprintf(car_path,"data/models/Car%d/car.3ds",AiCarModelNo);

			AI_CAR *AICar=new AI_CAR(car_path,1);
			AICar->CreateCar(hkVector4(i*5,0,5));
			AICar->car->setRotation(hkQuaternion(hkVector4(0,1,0),-PI/4));
			AICar->SetWheelsPos( hkVector4(1.7,0,-.9) , hkVector4(1.7,0,.9) , hkVector4(-1.5,0,.9) , hkVector4(-1.5,0,-.9) );
			//AICar->collosion_listener=new MyCollisionListener(AICar->car);
			AICar->AddCollosionListener();
			AI_cars.push_back(AICar);  
		}
		char car_path[255];
		//sprintf(car_path,"data/models/Car%d/car.3ds",atoi(myData.getUserData("carModel")->value.c_str()));
		sprintf(car_path,"data/models/Car%d/car.3ds",MY_CAR_MODEL);
		mycar=new MANUAL_CAR(car_path,1);
		mycar->CreateCar(hkVector4(0,0,2));
		mycar->SetHandling('w','d','s','a');
		//mycar->SetWheelsPos(hkVector4(1.7,0,-1),hkVector4(1.7,0,1),hkVector4(-1.5,0,1),hkVector4(-1.5,0,-1));
		mycar->SetWheelsPos( hkVector4(1.7,0,-.9) , hkVector4(1.7,0,.9) , hkVector4(-1.5,0,.9) , hkVector4(-1.5,0,-.9) );
		mycar->AddCollosionListener();
}


void DeleteHavokBodies() {
	objs.faces.clear();
	objs.verts.clear();
	objr.faces.clear();
	objr.verts.clear();

	g_resource.models_3DS.clear();

	roadPoints.clear();
	models_xml.clear();
	models.clear();
	AI_cars.clear();
	if(mycar)delete mycar;
	
	if(m_world)
	{
		delete m_world;
	}
	if(threadMemory) {
		threadMemory->setStackArea(0, 0);
		hkDeallocate(stackBuffer);
		threadMemory->removeReference();
	}

	FMOD_System_Close( ::g_ssystem );
	delete ::g_ssystem;

	// following line was in one of the examples I referred
	// this must be enabling sound system
	// I'm not interested in implementing sounds
	//::g_ssystem=g_soundManager->GetSystem();
	
}

void InitHavok() {
		hkResult ret;
		hkPoolMemory* memoryManager = new hkPoolMemory();
		threadMemory = new hkThreadMemory(memoryManager);
		ret = hkBaseSystem::init( memoryManager, threadMemory, errorReport );
		memoryManager->removeReference();
		
		{
			int stackSize = 0x100000;
			stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE);
			hkThreadMemory::getInstance().setStackArea( stackBuffer, stackSize);
		}

		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 10000.0f );
		info.m_enableDeactivation = true;   
		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		InitHavokBodies();
} 

#endif

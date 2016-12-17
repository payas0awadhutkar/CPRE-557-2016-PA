#include "header.h"
#include "car.h"
#include "winuser.h"
#include "timer.h"
#define _WIN32_WINNT 0x0500

void SetCarCamera(hkpWorld* m_world,hkpRigidBody *car) {
	hkTransform t;
	
	hkReal time = m_world->getCurrentTime();
	m_world->unmarkForRead();

	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();   

	hkp1dAngularFollowCam::CameraInput in;
	
			hkpWorld* world =m_world;
			car->approxTransformAt( time, in.m_fromTrans );	

			in.m_linearVelocity = car->getLinearVelocity();
			in.m_angularVelocity = car->getAngularVelocity();			
			in.m_deltaTime =.015;

		hkp1dAngularFollowCam::CameraOutput out;
		m_camera.calculateCamera( in, out );

		hkVector4 x;
		//x.setRotatedDir(mycar->car->getRotation(),hkVector4(-2,2,0));
		x.setRotatedDir(mycar->car->getRotation(),hkVector4(0,1,0));
		out.m_positionWS.add4(x);

		gluLookAt(out.m_positionWS(0),out.m_positionWS(1),out.m_positionWS(2),
			out.m_lookAtWS(0),out.m_lookAtWS(1)+1,out.m_lookAtWS(2),
			0,out.m_upDirWS(1),0);
}

int SetPoints=0;

void RenderGamePlay(bool FORCE_INIT=0) {
	static int run_once=0;
	if(FORCE_INIT) {
		DeleteHavokBodies();
		run_once=0;
	}
	if(run_once++==0) {
		InitHavok();
		LevelInit();
		//GetPoints();
		for(int i=0;i<AI_cars.size();i++) {
			float x= ( rand()%255 )>100 ? 1:0;
			float y= ( rand()%255 )<100 ?1:0;
			float z= ( rand()%255 )>200 ? 1:0;
			AI_cars[i]->CarColor.set(x,y,z);
		}
	}
	
	m_world->stepDeltaTime(.032);

	if(run_once%5==0 && SetPoints==1) {
			FILE *f=fopen("position","a");

			hkTransform t=mycar->car->getTransform();
			
			hkVector4 sideOfCar;
			sideOfCar.setTransformedPos( t, hkVector4(0,0,-10.0));
			fprintf(f,"%f,%f,%f\t",sideOfCar(0),sideOfCar(1),sideOfCar(2));

			sideOfCar.setTransformedPos( t, hkVector4(0,0,10.0));
			fprintf(f,",%f,%f,%f\t",sideOfCar(0),sideOfCar(1),sideOfCar(2));

			fprintf(f,",%f\n",mycar->getRotationAngle(hkVector4(0,1,0)));

			fclose(f);
	}

	//camera  
	if(camera==0)
		SetCarCamera(m_world,mycar->car);
	else if(camera == -1)
		KeyBoardFunc();
	else
		SetCarCamera(m_world,AI_cars[camera-1]->car);


	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH );
	glShadeModel(GL_SMOOTH);

	//glClearDepth(100.0f);									// Depth Buffer Setup

	float ambient[] = {0.0f, 1.0f, 1.0f, 1.0f};
    float diffuse[] = {1.0f, 1.0f, 1.0f, .2f};
    float specular[] = {1.0f, 1.0f, 1.0f, .5f};
    float position[] = {0.0f, 50.0f, 0.0f, 0.0f};
   
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
   
    //light model properties
    float model_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    int model_two_side = 1;                                //0=2sided, 1=1sided
    int viewpoint = 1;                                     //0=infiniteViewpoint, 1=localViewpoint
 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);     //small white ambient light
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, viewpoint);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_COLOR_MATERIAL_FACE );
		glDisable( GL_BLEND);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//draw skybox
		{
			hkVector4 pos;
			if(camera>=0)
				pos=camera==0?mycar->car->getPosition():AI_cars[camera-1]->car->getPosition();
			else
				pos=hkVector4(xListenerPos,yListenerPos,zListenerPos);
			if(SKYBOX_SHOW)RenderSkybox(pos,hkVector4(300,300,300),&skybox_texture[0]);
		}

		/*DrawRigidBody(fixbox,hkVector4(0,0,1),texture);
		DrawRigidBody(mybox,hkVector4(0,0,1),NULL);*/

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		glEnable(GL_COLOR_MATERIAL_FACE );
	}

	for(int i=0;i<model_bodies_xml.size();i++)
		DrawRigidBody(model_bodies_xml[i],hkVector4(0,1,0),NULL,NULL,&models_xml[i]);

	for(int i=0;i<models.size();i++)
		models[i]->Draw();


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	mycar->Draw();
	
	if(camera==0)mycar->steer();

	for(int i=0;i<AI_cars.size();i++)
	{
		AI_cars[i]->Draw(AI_cars[i]->CarColor);
		AI_cars[i]->Make_Draw_Rays();
		AI_cars[i]->steer(AI_cars[i]->CURR_STEER_FLAG1,AI_cars[i]->CURR_STEER_FLAG2);
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	char *txt=ArgsString("FPS:%d",(int)CalculateFPS());
	glutPrint(2.5f, 1.2f, glutFonts[5], txt, 1.0f, 0.0f, 0.0f, 1.0f);
	delete txt;

	OnStart();
	RenderPoints();

}

void Render() {
	
	ShowCursor(0);
	static int run_once=0;
	if(run_once++==0) {
		TimerInit();
		
		if(myData.getUserData("vbo")->value=="1")VBO_ENABLE=1;
	    VBO_ENABLE=VBO_ENABLE && InitVBO();
		printf("\n\n----VBO:%d----\n\n",VBO_ENABLE);
	}

	BYTE gKeys[256];
	GetKeyboardState( gKeys );
	if(MenuORPlay && CURR_MENU==MENU_CONDITIONS::NO_MENU) {
		if( (gKeys[27] & 0x80) )
			CURR_MENU=MENU_CONDITIONS::MENU_DURING_PLAY;
		RenderGamePlay();
	}
	else {
		if( (gKeys[13] & 0x80) && CURR_MENU==MENU_CONDITIONS::SHOWCASE) {
			MenuORPlay=1;
			CURR_MENU=MENU_CONDITIONS::NO_MENU;
			if(!carinfo)delete carinfo,carinfo=NULL;
			if(!carmodel)delete carmodel,carmodel=NULL;
			RenderGamePlay(1);
			Sleep(500);
		}
		else if(  (gKeys[27] & 0x80) && 
			( CURR_MENU==MENU_CONDITIONS::SHOWCASE 
			|| CURR_MENU==MENU_CONDITIONS::CRADITS 
			|| CURR_MENU==MENU_CONDITIONS::CONTROLS) 
		)
		{
			if( !MenuORPlay )
				CURR_MENU=MENU_CONDITIONS::MAIN_MENU;
			else
				CURR_MENU=MENU_CONDITIONS::MENU_DURING_PLAY;
		}
		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		
		if(CURR_MENU==MENU_CONDITIONS::MAIN_MENU)
			DrawMenu();
		else if(CURR_MENU==MENU_CONDITIONS::SHOWCASE)
			DrawShowCase();
		else if(CURR_MENU==MENU_CONDITIONS::MENU_DURING_PLAY)
			DrawMenu();
		else if(CURR_MENU==MENU_CONDITIONS::CRADITS)
			DrawCradits();
		else if(CURR_MENU==MENU_CONDITIONS::CONTROLS)
			DrawControls();
	}

	if(1) {
		Time2 = TimerGetTime()/1000;
		DiffTime = abs(Time2-Time1);
		while (DiffTime < .025) {
			// .015 = 66 frames per second
		  Time2 = TimerGetTime()/1000;
		  DiffTime = abs(Time2-Time1);      
		}
		Time1 = TimerGetTime()/1000;     
	}
}

void KeyboardCallback(unsigned char key, int x, int y) {
	gKeys[key] = true;

	switch (key) {
		case '27':	{ exit(0);} break; 
	}
}
void KeyboardUpCallback(unsigned char key, int x, int y) {
	gKeys[key] = false;

	switch (key) {
		case 'c' : {
			if(camera==0)camera=-1;
			else camera=0;
			break;
		}

		case 'b': {
			BANNER_SHOW=!BANNER_SHOW;
			SIDE_WALL_SHOW=!SIDE_WALL_SHOW;
			SKYBOX_SHOW=!SKYBOX_SHOW;
		}
		case 'x':
			{
				SetPoints=SetPoints==0?1:0;
			}
			break;
		case 'n':
			{
				int total_views=AI_cars.size();
				if(++camera > total_views ) camera=-1;

				//printf("Camera View Changed...",camera);
			}
			break;

		case 'q':
				
			break;
	}
}

//other functions

void initScene() {
	
}


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	Render();
	glutSwapBuffers();
}

int KeyBoardFunc() {
	if(camera==-1) {

		glLoadIdentity();

		doListenerMovement();
		glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
		glTranslatef(-xListenerPos, -yListenerPos, -zListenerPos);
	}

	return 0;
}



void inputKey(int key, int x, int y) {

	switch (key) {
		//case GLUT_KEY_LEFT : angle -= 0.01f;orientMe(angle);break;
		//case GLUT_KEY_RIGHT : angle +=0.01f;orientMe(angle);break;
		//case GLUT_KEY_UP : moveMeFlat(1);break;
		//case GLUT_KEY_DOWN : moveMeFlat(-1);break;
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE );

	myData.Load();

	bool FULLSCREEN=0;
	if(myData.getUserData("fullscreen")!=NULL)
		if(atoi(myData.getUserData("fullscreen")->value.c_str())==1)FULLSCREEN=1;
	
	if(!FULLSCREEN) {
		glutInitWindowPosition(100,100);
		glutInitWindowSize(640,360);
		glutCreateWindow("Hello");
	}
	else {
		if(myData.getUserData("screen")!=NULL)
			glutGameModeString( myData.getUserData("screen")->value.c_str() ); 

		if (!glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			printf("The select screen mode is not available\n");
			Sleep(2000);
		}
		glutEnterGameMode();
		
		printf("Current Mode: Game Mode %dx%d at %d hertz, %d bpp",
				glutGameModeGet(GLUT_GAME_MODE_WIDTH),
				glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
				glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
				glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
	}

	initScene();
	//glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);

	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	glutKeyboardFunc(KeyboardCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);  

	glutReshapeFunc(changeSize);

	glLineWidth(10.0);

	InitGL();
	
	LoadModels();
	//InitHavok();
	//m2.Load("3dscar/car.3DS"); 
	
	VehicleApiUtils::createCamera( m_camera );
	glutMainLoop();
	
	exit(0);
	
	return(0);
}

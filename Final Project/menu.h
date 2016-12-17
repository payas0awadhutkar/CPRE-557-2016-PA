enum MENU_CONDITIONS
{
	MAIN_MENU,
	SHOWCASE,
	MENU_DURING_PLAY,
	NO_MENU,
	CREDITS,
	CONTROLS
};
int CURR_MENU=MENU_CONDITIONS::MAIN_MENU;

void DrawRect(float x1,float x2,float y1,float y2)
{
	float cz = -0.0f,cx = 1.0f;
	glBegin(GL_QUADS);	
			glTexCoord2f(cz,cz);  glVertex3f( x1, y1,-1.0f);
			glTexCoord2f( cx,cz); glVertex3f( x2, y1,-1.0f); 
			glTexCoord2f( cx,cx); glVertex3f( x2, y2,-1.0f);
			glTexCoord2f(cz, cx); glVertex3f( x1, y2,-1.0f);
	glEnd();
}

static int MenuORPlay=0;
GLTexture menu_textures[10];
void DrawMenu()
{
	static int xx=0,menu_pos=0;
	if(xx++==0)
	{
		menu_textures[0].Load("Data/showcase/11_1.JPG");
		menu_textures[1].Load("Data/showcase/11_2.JPG");
		menu_textures[2].Load("Data/showcase/11_3.JPG");
		menu_textures[3].Load("Data/showcase/11_4.JPG");
		menu_textures[4].Load("Data/showcase/bg.JPG");
		menu_textures[5].Load("Data/showcase/A 11_1.JPG");
		menu_textures[6].Load("Data/showcase/11_5.JPG");
	}
	BYTE gKeys[256];
	GetKeyboardState( gKeys );
	if( gKeys[38] & 0x80  )  //UP ARROW KEY
	{
		menu_pos--;
		Sleep(100);
	}
	else if( gKeys[40] & 0x80 )  //DOWN ARROW KEY
	{
		menu_pos++;
		Sleep(100);
	}

	int max_opt=5;
	if(CURR_MENU==MENU_CONDITIONS::MENU_DURING_PLAY)
	{
		menu_pos=menu_pos>=max_opt?0:menu_pos;
		menu_pos=menu_pos<0?max_opt-1:menu_pos;
	}
	else
	{
		menu_pos=menu_pos>=max_opt?1:menu_pos;
		menu_pos=menu_pos<1?max_opt-1:menu_pos;
	}

	if( gKeys[13] & 0x80  )
	{
		if( menu_pos==1)
		{
			CURR_MENU=MENU_CONDITIONS::SHOWCASE;
			Sleep(500);
		}
		else if( menu_pos==2 )
		{
			CURR_MENU=MENU_CONDITIONS::CREDITS;
			Sleep(500);
		}
		else if( menu_pos==3 )
		{
			CURR_MENU=MENU_CONDITIONS::CONTROLS;
			Sleep(500);
		}
		else if( menu_pos==0 && CURR_MENU==MENU_CONDITIONS::MENU_DURING_PLAY)
		{
			CURR_MENU=MENU_CONDITIONS::NO_MENU;
			MenuORPlay=1;
			Sleep(500);
		}
		else if( menu_pos==4)
		{
			exit(0);
		}
	}
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glClearColor(1,1,0,1);

	/*static float pos_x=0,pos_y=0;
	if((gKeys['I'] & 0x80) )pos_y+=.1f;
	else if((gKeys['K'] & 0x80) )pos_y-=.1f;
	else if((gKeys['J'] & 0x80) )pos_x+=.1f;
	else if((gKeys['L'] & 0x80) )pos_x-=.1f;
	printf("\n%f %f\n",pos_x,pos_y);*/
	glPushMatrix();
		glTranslatef(-1,-3,-10);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		menu_textures[4].Use();
		DrawRect(-10.4, 12.30, -3.39,9.40);
		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);

		if(CURR_MENU==MENU_CONDITIONS::MENU_DURING_PLAY)
		{
			glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				if(menu_pos==0)glColor3f(0,1,0);else glColor3f(1,1,1);
				menu_textures[5].Use();
				DrawRect(-3.5, 5.7 , 5.5, 6.5f);

				glBlendFunc(GL_DST_COLOR, GL_ONE);
				if(menu_pos==0)glColor3f(0,1,0);else glColor3f(1,1,1);
				menu_textures[3].Use();
				DrawRect(-3.5, 5.7 , 5.5, 6.5f);
			glDisable(GL_BLEND);
		}

		glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			if(menu_pos==1)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[5].Use();
			DrawRect(-3.5, 5.7 , 4.0, 5.0f);

			glBlendFunc(GL_DST_COLOR, GL_ONE);
			if(menu_pos==1)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[0].Use();
			DrawRect(-3.5, 5.7 , 4.0, 5.0f);
		glDisable(GL_BLEND);

		glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			if(menu_pos==2)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[5].Use();
			DrawRect(-3.5, 5.7 , 2.5, 3.5f);

			glBlendFunc(GL_DST_COLOR, GL_ONE);
			if(menu_pos==2)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[1].Use();
			DrawRect(-3.5, 5.7 , 2.5, 3.5f);
		glDisable(GL_BLEND);
	
		glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			if(menu_pos==3)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[5].Use();
			DrawRect(-3.5, 5.7 , 1.0, 2.0f);

			glBlendFunc(GL_DST_COLOR, GL_ONE);
			if(menu_pos==3)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[6].Use();
			DrawRect(-3.5, 5.7 , 1.0, 2.0f);
		glDisable(GL_BLEND);

		glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			if(menu_pos==4)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[5].Use();
			DrawRect(-3.5, 5.7 , -0.5, 0.5f);

			glBlendFunc(GL_DST_COLOR, GL_ONE);
			if(menu_pos==4)glColor3f(0,1,0);else glColor3f(1,1,1);
			menu_textures[2].Use();
			DrawRect(-3.5, 5.7 , -0.5, 0.5f);
		glDisable(GL_BLEND);

	glPopMatrix();

}

void DrawCredits()
{
	// empty function which came up with the one of the examples I referred.
	// Can be used to display credits when the game ends.
}
void DrawControls() {
	glLoadIdentity();
	glClearColor(0,0,0,1);

	glPushMatrix();
		glTranslatef(-1,-3,-10);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		menu_textures[4].Use();
		DrawRect(-10.4, 12.30, -3.39,9.40);
	glPopMatrix();

	glLineWidth(2);
	

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
		glTranslatef(0,0,-2);
		glScalef(.0005,.0005,.0005);

		glColor4f(0,1,0,1);
		renderStrokeCharacter(-.5/.0005 , .5/.0005,0,GLUT_STROKE_ROMAN,"Use All keys in small Letters(Caps Lock Off)");
		glColor4f(1,0,0,1);
		renderStrokeCharacter(-.5/.0005 , .4/.0005,0,GLUT_STROKE_ROMAN,"Move : W,A,S,D");
		renderStrokeCharacter(-.5/.0005 , .3/.0005,0,GLUT_STROKE_ROMAN,"Reset Car : R");
		renderStrokeCharacter(-.5/.0005 , .2/.0005,0,GLUT_STROKE_ROMAN,"Toggle b/w Car View vs 3rd Person View : C");
		renderStrokeCharacter(-.5/.0005 , .1/.0005,0,GLUT_STROKE_ROMAN,"Goto Main Menu : [Esc]");
		renderStrokeCharacter(-.5/.0005 ,  0/.0005,0,GLUT_STROKE_ROMAN,"<Left> or <Right>Arrows uses in Car Model Selection");
		renderStrokeCharacter(-.5/.0005 ,  -.1/.0005,0,GLUT_STROKE_ROMAN,"Camera to AI Cars : N");
		renderStrokeCharacter(-.5/.0005 ,  -.2/.0005,0,GLUT_STROKE_ROMAN,"Drag Mouse in 3rd person view to change direction");
		renderStrokeCharacter(-.5/.0005 ,  -.3/.0005,0,GLUT_STROKE_ROMAN,"Hide Posters and Wall : B");
	glPopMatrix();
	glEnable(GL_DEPTH);
}

struct CarASH_Info
{
	float accelaration;
	float speed;
	float handling;

	void SetASH(float accelaration,float speed,float handling)
	{
		this->accelaration=accelaration;
		this->speed=speed;
		this->handling=handling;
	}
};

CarASH_Info *GetASHData( CarModel *carmodel ) 
{
	strcpy(carmodel->name,carmodel->carModelData.getUserData("info")->searchByName("name")->value[0].c_str());
	int max_speed= atoi(carmodel->carModelData.getUserData("info")->searchByName("topspeed")->value[0].c_str());
	float wheel_radius= atof(carmodel->carModelData.getUserData("info")->searchByName("wheel_radius")->value[0].c_str());
	float max_engine_rpm= atof(carmodel->carModelData.getUserData("info")->searchByName("max_engine_rpm")->value[0].c_str());
	int num_gears=atoi(carmodel->carModelData.getUserData("info")->searchByName("num_gears")->value[0].c_str());

	//wheels.....
	int wheel_mass= atoi(carmodel->carModelData.getUserData("info")->searchByName("wheel_mass")->value[0].c_str());
	Vector3 wheel_friction( carmodel->carModelData.getUserData("info")->searchByName("wheel_friction")->getToVec3());
	float max_steering_angle= atof(carmodel->carModelData.getUserData("info")->searchByName("max_steering_angle")->value[0].c_str()) * ( HK_REAL_PI / 180 );
	Vector3 RPY_factor(carmodel->carModelData.getUserData("info")->searchByName("RPY_factor")->getToVec3());

	float accelation=0;
	float handling= (RPY_factor(0)+RPY_factor(1)+RPY_factor(2))*20 + ((wheel_friction(0)+wheel_friction(1)+wheel_friction(2)+wheel_friction(3))/30)*80;
	handling=handling>100.f?100.f:handling;

	CarASH_Info *info=new CarASH_Info();
	info->SetASH(60.f,((float)max_speed/400.f)*100.f,handling);

	return info;
}

int MY_CAR_MODEL=1;
CarModel *carmodel=NULL;
CarASH_Info *carinfo=NULL;
GLTexture showcase_textures[10];
void DrawShowCase()
{
	static int xx=0;

	if(xx++==0)
	{
		showcase_textures[0].Load("Data/showcase/0.JPG");
		showcase_textures[1].Load("Data/showcase/1.JPG");
		//showcase_textures[3].Load("Data/showcase/bg.JPG");
	}

	
	BYTE gKeys[256];
	GetKeyboardState( gKeys );
	if( carmodel==NULL  || (gKeys[37] & 0x80) || (gKeys[39] & 0x80)  )
	{
		if((gKeys[37] & 0x80))MY_CAR_MODEL--,Sleep(200);
		else if((gKeys[39] & 0x80))MY_CAR_MODEL++,Sleep(200);

		if(MY_CAR_MODEL>9)MY_CAR_MODEL=1;
		if(MY_CAR_MODEL<1)MY_CAR_MODEL=9;

		if(carmodel!=NULL)delete carmodel;
		if(carinfo!=NULL)delete carinfo;

		char path[500];sprintf(path,"data/models/Car%d/car.3ds",MY_CAR_MODEL);
		carmodel=new CarModel(path);
		carinfo=GetASHData(carmodel);
	}
	
	glPushMatrix();
		static float angle=0;
		glLoadIdentity();
		glTranslatef(0,-1.f,-8);
		glRotatef(10,1,0,0);
		glRotatef(angle-=.5f,0,1,0);
		carmodel->Draw();
		carmodel->DrawWheels();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-1,-3,-10);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		menu_textures[4].Use();
		DrawRect(-10.4, 12.30, -3.39,9.40);
	glPopMatrix();


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_DEPTH);
		char *txt=&carmodel->name[0];
		glPushMatrix();
			glLoadIdentity();
			//glTranslatef(-.05,.1,-1);
			//glutPrintX(GLUT_BITMAP_HELVETICA_18, txt,0,0);
			static float angle2=0;
			glColor3f(0,0,0);
			glTranslatef( (-.03*strlen(txt))+.1,.1,-1);
			glRotatef( 20 ,0,1,0);
			glScalef(.0005,.0005,.0005);
			renderStrokeCharacter(0,0,0,GLUT_STROKE_MONO_ROMAN,txt);
		glPopMatrix();
		
	glEnable(GL_DEPTH);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPushMatrix();
		glDisable(GL_DEPTH);
		glTranslatef(0,0,-10.f);
		showcase_textures[0].Use();
		//glEnable(GL_BLEND);
			//glBlendFunc( GL_SRC_ALPHA, GL_ONE);
			DrawRect(-7.3,7.3,-6.0,-4.5f);
		//glDisable(GL_BLEND);

		showcase_textures[1].Use();
		DrawRect(-3.5, 5.7 * (float)(carinfo->accelaration/100.f) ,-4.9,-4.5f);
		DrawRect(-3.5, 5.7 * (float)(carinfo->speed/100.f)		  ,-5.4,-5.0f);
		DrawRect(-3.5, 5.7 * (float)(carinfo->handling/100.f)	  ,-5.92,-5.52f);
		glEnable(GL_DEPTH);
	glPopMatrix();
}

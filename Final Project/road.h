#ifndef ROAD_H
#define ROAD_H
#include "havok_funcs.h"
#include "header.h"

struct Road_Points
{
	Vector3 left;
	Vector3 right;
	float angle;
	Road_Points(Vector3 l,Vector3 r,float a)
		:left(l),right(r),angle(a){}
};
vector<Road_Points>roadPoints;

hkVector4 Vec3TohkVector(Vector3 v)
{
	return hkVector4(v(0),v(1),v(2));
}
hkVector4 getRoadPointLeft(int i)
{
	return Vec3TohkVector(roadPoints[i].left);
}
hkVector4 getRoadPointRight(int i)
{
	return Vec3TohkVector(roadPoints[i].right);
}

void SetRoadPointsFromFile() {
	FILE *f=fopen("data/positions","r");
	if(!f)return;

	char *txt=new char[100];
	int c=0;
	while(fgets(txt,100,f)!=NULL) {
		if(c++%2==0)continue;

		string str=trim(txt);	
		string data;

		Vector3 v;
		Vector3 v1,v2;
		
		for(int i=0;i<3;i++) {
			data=trim(str.substr(0,str.find(",")));
			v.set(i,atof(data.c_str()));
			str=str.substr(str.find(",")+1);
		}
		v1.x=v(0);v1.y=v(1);v1.z=v(2);
		//pos_left.push_back(vx);

		for(int i=0;i<3;i++)
		{
			data=trim(str.substr(0,str.find(",")));
			//printf("%s;",data.c_str());
			v.set(i,atof(data.c_str()));
			str=str.substr(str.find(",")+1);
		}
		v2.x=v(0);v2.y=v(1);v2.z=v(2);

		data=trim(str.substr(0,str.find(",")));
		float angle=atof(data.c_str());

		hkVector4 x;
		hkQuaternion q=hkQuaternion(hkVector4(0,1,0),angle);
		
		x.setRotatedDir(q,hkVector4(0,0,-20));
		x.add4(hkVector4(v1(0),v1(1),v1(2)));
		//v1.x=x(0); v1.z=x(2);

		x.setRotatedDir(q,hkVector4(0,0,20));
		x.add4(hkVector4(v2(0),v2(1),v2(2)));
		//v2.x=x(0); v2.z=x(2);

		
		Road_Points rp(v1,v2,angle);
		roadPoints.push_back(rp);
	}
}



class RoadBlocks {
public:
	vector<float> verts;
	vector<short> faces;

	unsigned int Verts_VBO_No,Tex_VBO_No,Ind_VBO_No;
	void createRoadVBO() {
		// Generate And Bind The Vertex Buffer
		glGenBuffersARB( 1, &(Verts_VBO_No) );							// Get A Valid Name
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, Verts_VBO_No );			// Bind The Buffer
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*verts.size(),&verts[0], GL_STATIC_DRAW_ARB );
		
		// Generate And Bind The Texture Coordinate Buffer
		glGenBuffersARB( 1, &(Tex_VBO_No) );							// Get A Valid Name
		glBindBufferARB( GL_ARRAY_BUFFER_ARB,Tex_VBO_No );		// Bind The Buffer
		glBufferDataARB( GL_ARRAY_BUFFER_ARB,sizeof(float)*verts.size(),&verts[0], GL_STATIC_DRAW_ARB );
		
		// Generate And Bind The Texture Coordinate Buffer
		glGenBuffersARB( 1, & (Ind_VBO_No) );							// Get A Valid Name
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, Ind_VBO_No );		// Bind The Buffer
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(short)*faces.size() , &faces[0], GL_STATIC_DRAW_ARB );

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB,-1);		// Bind The Buffer
	}

}objs,objr;

GLTexture tex[2],slogan_tex[16];

void DrawStartingPoint() {
	int size=roadPoints.size()-1;
	int u=1,v=1;
	glPushMatrix();
		tex[1].Use();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_COLOR_MATERIAL_FACE );
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
			 glTexCoord2f(0,1 );glVertex3d(getRoadPointLeft(0)(0),getRoadPointLeft(0)(1)-.8,getRoadPointLeft(0)(2));
			 glTexCoord2f(0,0 );glVertex3d(getRoadPointLeft(size)(0),getRoadPointLeft(size)(1)-.8,getRoadPointLeft(size)(2));
			 glTexCoord2f(1,0 );glVertex3d(getRoadPointRight(size)(0),getRoadPointRight(size)(1)-.8,getRoadPointRight(size)(2));
			 glTexCoord2f(1,1); glVertex3d(getRoadPointRight(0)(0),getRoadPointRight(0)(1)-.8,getRoadPointRight(0)(2));
		glEnd();

	glPopMatrix();

}

void ShowStickOnWallPoint(int p,GLTexture &tex,RoadBlocks &objs,int h1,int h2)
{
	int u=1,v=1;

	p=p*6;
	tex.Use();
	glPushMatrix();
			glTranslatef(objs.verts[p],objs.verts[p+1]+h1/2,objs.verts[p+2]);
			glRotatef(0,1,0,45);
			glScalef(.1,(h1),.1);
			glutSolidCube(1);
	glPopMatrix();

}

void ShowPosterOnWall(int from,int to,GLTexture &tex ,RoadBlocks &objs,int h1,int h2,float side,int stick=0) {
	int u=1,v=1;
	glPushMatrix();
		tex.Use();
		
		from=from*6;
		to=to*6;
		
		glBegin(GL_QUADS);
			 glTexCoord2f(0,1 );glVertex3d(objs.verts[from]+side,objs.verts[from+1]+h1,objs.verts[from+2]);
			 glTexCoord2f(0,0 );glVertex3d(objs.verts[from]+side,objs.verts[from+1]+h2,objs.verts[from+2]);
			 glTexCoord2f(1,0 );glVertex3d(objs.verts[to]+side,objs.verts[to+1]+h2,objs.verts[to+2]);
			 glTexCoord2f(1,1); glVertex3d(objs.verts[to]+side,objs.verts[to+1]+h1,objs.verts[to+2]);
		glEnd();

		if(stick==1) {
			ShowStickOnWallPoint(from/6,tex,objs,h1,h2);
			ShowStickOnWallPoint(to/6,tex,objs,h1,h2);
		}
	glPopMatrix();
}

void Draw(RoadBlocks *obj) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	tex[0].Use();

	if(VBO_ENABLE) {		
		glBindBufferARB (GL_ARRAY_BUFFER_ARB, obj->Verts_VBO_No);
		glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );

		glBindBufferARB (GL_ARRAY_BUFFER_ARB, obj->Tex_VBO_No);
		glTexCoordPointer( 3, GL_FLOAT, 0, (char *) NULL );
		glPushMatrix();
			if(1)
			{
				glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB, obj->Ind_VBO_No);
					glDrawElements (GL_TRIANGLE_STRIP, obj->faces.size(), GL_UNSIGNED_SHORT, 0); 
				glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB,-1);
			}
			else
			{
				glDrawElements(GL_TRIANGLE_STRIP ,obj->faces.size(), GL_UNSIGNED_SHORT, &obj->faces[0]);
			}
		glPopMatrix();
	}
	else {
		glTexCoordPointer(3, GL_FLOAT, 0, &obj->verts[0]);
		glVertexPointer(3, GL_FLOAT, 0, &obj->verts[0]);

		glPushMatrix();
			int num= (int)obj->faces.size();
			glDrawElements(GL_TRIANGLE_STRIP ,num, GL_UNSIGNED_SHORT, &obj->faces[0]);
		glPopMatrix();
	}	

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


int roadDL,bannerDL;
GLuint createRoadMapDL();
GLuint createBannerDL();

void CreateRoadBlocks() {
	for(int i=0;i<roadPoints.size()-1;i++) {
		hkVector4 p1=hkVector4(roadPoints[i].left(0),roadPoints[i].left(1)+4,roadPoints[i].left(2));

		hkVector4 x;
		hkQuaternion q=hkQuaternion(hkVector4(0,1,0),roadPoints[i].angle);
		x.setRotatedDir(q,hkVector4(0,0,-20));
		x.add4(hkVector4(p1(0),p1(1),p1(2)));
		p1=x;

		hkVector4 p2=hkVector4(p1(0),p1(1)-10,p1(2));

		for(int i=0;i<3;i++)
			objs.verts.push_back(p1(i));
		for(int i=0;i<3;i++)
			objs.verts.push_back(p2(i));
	}

	int num= (objs.verts.size()/3);
	for(short i=0;i<num ;i++) {
	    objs.faces.push_back( (short) i);
		if(i+1 >= num) objs.faces.push_back((short)num-i+1);
		else objs.faces.push_back((short) i+1);
		if(i+2 >= num) objs.faces.push_back((short)num-i+2);
		else objs.faces.push_back((short) i+2);
	}
	hkpRigidBody *r=createMesh(&objs.verts[0],&objs.faces[0],hkVector4(0,0,0),1.0,objs.verts.size(),objs.faces.size());
	r->setFriction(.5);

	for(int i=0;i<roadPoints.size()-1;i++) {
		hkVector4 p1=hkVector4(roadPoints[i].right(0),roadPoints[i].right(1)+4,roadPoints[i].right(2));
		hkVector4 x;
		hkQuaternion q=hkQuaternion(hkVector4(0,1,0),roadPoints[i].angle);
		x.setRotatedDir(q,hkVector4(0,0,20));
		x.add4(hkVector4(p1(0),p1(1),p1(2)));
		p1=x;

		hkVector4 p2=hkVector4(p1(0),p1(1)-10,p1(2));

		for(int i=0;i<3;i++)
			objr.verts.push_back(p1(i));
		for(int i=0;i<3;i++)
			objr.verts.push_back(p2(i));
	}

	num= (objr.verts.size()/3);
	for(short i=0;i<num ;i++) {
	    objr.faces.push_back( (short) i);
		if(i+1 >= num) objr.faces.push_back((short)num-i+1);
		else objr.faces.push_back((short) i+1);
		if(i+2 >= num) objr.faces.push_back((short)num-i+2);
		else objr.faces.push_back((short) i+2);
	}
	r=createMesh(&objr.verts[0],&objr.faces[0],hkVector4(0,0,0),1.0,objr.verts.size(),objr.faces.size());
	r->setFriction(.5);

	FILE *f;
	tex[0].Load("data/images/grass.jpg");
	tex[1].Load("data/images/nehe.jpg");
	for(int i=0;i<16;i++) {
		char *txt=ArgsString("data/slogans/slogan_%d.jpg",i);
		if(f=fopen(txt,"r")) {
			fclose(f);
			slogan_tex[i].Load(txt);
		}
		delete txt;
	}

	if(VBO_ENABLE)
	{
			objs.createRoadVBO();
			objr.createRoadVBO();
	}
	else
	{
		bannerDL =createBannerDL();
		roadDL=createRoadMapDL();
	}
}

void DrawBanners() {
		ShowPosterOnWall(0,10,slogan_tex[0],objs,-1,-5,-.1);

		ShowPosterOnWall(25,18,slogan_tex[1],objr,10,0,-.4);
		
		ShowPosterOnWall(70,67,slogan_tex[2],objr,6,0,-.4,1);

		ShowPosterOnWall(90,94,slogan_tex[3],objs,8,0,-.4,0);

		ShowPosterOnWall(120,117,slogan_tex[4],objr,14,5,-.4,1);

		ShowPosterOnWall(148,150,slogan_tex[5],objs,10,5,-.4,1);
		
		ShowPosterOnWall(180,175,slogan_tex[6],objr,6,0,-.4,0);

		ShowPosterOnWall(196,200,slogan_tex[7],objs,6,0,-.4,0);

		ShowPosterOnWall(260,255,slogan_tex[8],objr,13,4,-.4,1);

		ShowPosterOnWall(280,285,slogan_tex[9],objs,10,5,-.4,1);

		ShowPosterOnWall(300,295,slogan_tex[10],objr,6,0,-.4,1);

		ShowPosterOnWall(320,322,slogan_tex[11],objs,10,5,-.4,1);
		
		ShowPosterOnWall(331,329,slogan_tex[12],objr,7,0,-.4,0);

		ShowPosterOnWall(350,353,slogan_tex[13],objs,14,5,-.4,1);

		ShowPosterOnWall(370,373,slogan_tex[14],objs,20,5,-.4,1);
}

void RenderPoints() {
	glDisable(GL_LIGHTING);
	for(int i=0;i<roadPoints.size();i++) {
		glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(roadPoints[i].right(0),roadPoints[i].right(1),roadPoints[i].right(2));
			glutSolidCube(1.0);
		glPopMatrix();
	}for(int i=0;i<roadPoints.size();i++)
	{
		glPushMatrix();
			glColor3f(1,1,0);
			glTranslatef(roadPoints[i].left(0),roadPoints[i].left(1),roadPoints[i].left(2));
			glutSolidCube(1.0);
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
	if(VBO_ENABLE) {
		DrawStartingPoint();
		if(SIDE_WALL_SHOW)Draw(&objs),Draw(&objr);
		if(BANNER_SHOW)DrawBanners();
	}
	else {
		DrawStartingPoint();
		if(SIDE_WALL_SHOW)glCallList(roadDL);
		if(BANNER_SHOW)glCallList(bannerDL);
	}

}

GLuint createRoadMapDL() {
	GLuint snowManDL;
	// Create the id for the list
	snowManDL = glGenLists(1);

	glNewList(snowManDL,GL_COMPILE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		DrawStartingPoint();

		Draw(&objs);
		Draw(&objr);
		
	glEndList();

	return(snowManDL);
}

GLuint createBannerDL() {
	GLuint snowManDL;

	// Create the id for the list
	snowManDL = glGenLists(1);

	// start list
	glNewList(snowManDL,GL_COMPILE);

	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		DrawBanners();
	// endList
	glEndList();

	return(snowManDL);
}

#endif

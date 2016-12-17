// open source loader for XML loading

#ifndef XML_MODEL_H
#define XML_MODEL_H

#include <windows.h>		// Header File For Windows
#include <stdlib.h>
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <GL/glut.h>


#include <vector>
#include <string>
using namespace std;

struct Vector3f
{
	float x,y,z;
};
struct Vertex {
		float x,y,z;
	};

struct Object
{
		vector<short> Faces;
		vector<float>Vertexes;
		vector<float>Normals;
};

struct XMLModel
{
	vector <Object> Objects;

	XMLModel();
	int Load(string filename);
	string path;
	string name;
	int Draw(float scale);
	float scale;
	void CalculateNormals();
};

void XMLModel::CalculateNormals()
{
	// Let's build some normals
	for (int i = 0; i < Objects.size(); i++)
	{
		for (int j = 0; j < Objects[i].Faces.size(); j++)
			Objects[i].Normals.push_back(0.0);

		for (int k = 0; k < Objects[i].Normals.size();  k+=3)
		{
			Vector3f n;
			Vertex v1;
			Vertex v2;
			Vertex v3;

			float vertA=Objects[i].Faces[k];
			float vertB=Objects[i].Faces[k+1];
			float vertC=Objects[i].Faces[k+2];

			v1.x = Objects[i].Vertexes[vertA*3];
			v1.y = Objects[i].Vertexes[vertA*3+1];
			v1.z = Objects[i].Vertexes[vertA*3+2];
			v2.x = Objects[i].Vertexes[vertB*3];
			v2.y = Objects[i].Vertexes[vertB*3+1];
			v2.z = Objects[i].Vertexes[vertB*3+2];
			v3.x = Objects[i].Vertexes[vertC*3];
			v3.y = Objects[i].Vertexes[vertC*3+1];
			v3.z = Objects[i].Vertexes[vertC*3+2];

			float u[3], v[3];

			// V2 - V3;
			u[0] = v2.x - v3.x;
			u[1] = v2.y - v3.y;
			u[2] = v2.z - v3.z;

			// V2 - V1;
			v[0] = v2.x - v1.x;
			v[1] = v2.y - v1.y;
			v[2] = v2.z - v1.z;

			n.x = (u[1]*v[2] - u[2]*v[1]);
			n.y = (u[2]*v[0] - u[0]*v[2]);
			n.z = (u[0]*v[1] - u[1]*v[0]);

			// Add this normal to its verts' normals
			Objects[i].Normals[vertA*3]   += n.x;
			Objects[i].Normals[vertA*3+1] += n.y;
			Objects[i].Normals[vertA*3+2] += n.z;
			Objects[i].Normals[vertB*3]   += n.x;
			Objects[i].Normals[vertB*3+1] += n.y;
			Objects[i].Normals[vertB*3+2] += n.z;
			Objects[i].Normals[vertC*3]   += n.x;
			Objects[i].Normals[vertC*3+1] += n.y;
			Objects[i].Normals[vertC*3+2] += n.z;

		}
		for (int g = 0; g < Objects[i].Normals.size()/3; g++)
		{
			// Reduce each vert's normal to unit
			float length;
			Vector3f unit;

			unit.x = Objects[i].Normals[g*3];
			unit.y = Objects[i].Normals[g*3+1];
			unit.z = Objects[i].Normals[g*3+2];

			length = (float)sqrt((unit.x*unit.x) + (unit.y*unit.y) + (unit.z*unit.z));

			if (length == 0.0f)
				length = 1.0f;

			unit.x /= length;
			unit.y /= length;
			unit.z /= length;

			Objects[i].Normals[g*3]   = unit.x;
			Objects[i].Normals[g*3+1] = unit.y;
			Objects[i].Normals[g*3+2] = unit.z;
		}
	}
}


XMLModel::XMLModel()
{
	
}

int XMLModel::Draw(float scale)
{
	glPushMatrix();
		//(glClearColor(0, 1, 0, .5));
		(glEnable(GL_DEPTH_TEST));
		//glEnable(GL_COLOR_MATERIAL_FACE );
		glEnable(GL_CULL_FACE);

		(glShadeModel(GL_FLAT));
		(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

		// Set a default lightning
		(glEnable(GL_LIGHTING));
		(glEnable(GL_LIGHT0));
		
		/*float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
		float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
		float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
		float mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
		float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		float no_shininess = 0.0f;
		float low_shininess = 5.0f;
		float high_shininess = 100.0f;
		float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);*/


		glScalef(scale, scale, scale);
		
		for (int i = 0; i < Objects.size(); i++)
		{
			if(1)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, &Objects[i].Normals[0]);

				glVertexPointer(3, GL_FLOAT, 0, &Objects[i].Vertexes[0]);
				//for (int j = 0; j < Objects[i].Faces.size(); j ++)
				{
						glDrawElements(GL_TRIANGLES, Objects[i].Faces.size(), GL_UNSIGNED_SHORT, &Objects[i].Faces[0]);
				}
			}
		}
	glPopMatrix();
	return 0;
}

char *Trim(char *a)
{
	int flag=0;int k=0;
	char *b=new char [500];
	for(int i=0;a[i]!='\n';i++)
	{
		if(flag==0)
		{
			if(a[i]==' ' || a[i]=='\t');
			else
				flag=1;
		}
		if(flag==1)
		{
			b[k]=a[i];
			k++;
		}
	}
	b[k]='\0';
	return b;
}

static Object *obj=new Object();
int XMLModel::Load(string filename)
{
	path=filename;
	FILE *f=fopen(filename.c_str(),"r");
	char str[500];

	int pflag=0;
	int fflag=0;
	while(fgets(str,500,f))
	{
		//printf("%s\n",Trim(str));

		if(!strcmp(Trim(str),"<points>")){
			pflag=1;
		}
		else if(!strcmp(Trim(str),"<faces>"))fflag=1;
		if(!strcmp(Trim(str),"</points>"))pflag=0;
		else if(!strcmp(Trim(str),"</faces>"))
		{
			fflag=0;
			Objects.push_back(*obj);
			obj=new Object();
		}

		if(pflag==1)
		{
			char *p=strtok(str,"\"");
			if((p=strtok(NULL,"\""))==NULL)continue;
			obj->Vertexes.push_back(atof(p));

			p=strtok(NULL,"\"");
			p=strtok(NULL,"\"");
			obj->Vertexes.push_back(atof(p));

			p=strtok(NULL,"\"");
			p=strtok(NULL,"\"");
			obj->Vertexes.push_back(atof(p));
		}
		else if(fflag==1)
		{
			char *p=strtok(str,"\"");
			if((p=strtok(NULL,"\""))==NULL)continue;
			obj->Faces.push_back(atoi(p));

			p=strtok(NULL,"\"");
			p=strtok(NULL,"\"");
			obj->Faces.push_back(atoi(p));

			p=strtok(NULL,"\"");
			p=strtok(NULL,"\"");
			obj->Faces.push_back(atoi(p));
		}
	}
	fclose(f);
	CalculateNormals();
	return 0;
}

#endif MODEL_3DS_H

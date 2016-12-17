
#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "header.h"

class Model {
public:
	Model_3DS *m;
	
	hkVector4 scale;
	hkVector4 position;
	hkQuaternion rotation;

	char *path;
	char name[60];

	hkpRigidBody* rbody;

	bool enable_DL;
	int display_list;

	void Load(char *path) {
		m=new Model_3DS();
		m->Load(path);

		this->path=new char[strlen(path)];
		this->path=path;
	}

	void SetModel(Model_3DS *m) {
		this->path=new char[strlen(m->path)];
		this->path=path;
		//this->m=new Model_3DS();
		this->m=m;
	}
	
	Model(char *path=NULL,hkVector4 &scale=hkVector4(1,1,1),hkVector4 &position=hkVector4(0,0,0),hkQuaternion &rotation=hkQuaternion(hkVector4(0,1,0),0))
	{
		this->position=position;
		this->rotation=rotation;
		if(path!=NULL)
				Load(path);
		this->scale=scale;

		this->rbody=NULL;

		this->enable_DL=0;
	}


	int MakeBody(hkpWorld *m_world) {
		hkpStorageExtendedMeshShape* extendedMesh = new hkpStorageExtendedMeshShape();
		for(int numobj=0;numobj<this->m->numObjects;numobj++) {
			float *verts=new float[this->m->Objects[numobj].numVerts*3];
			for(int i=0;i<this->m->Objects[numobj].numVerts*3;i++) {
				verts[i]=this->m->Objects[numobj].Vertexes[i];
				verts[i]*=this->scale(i%3);
			}   
			hkpExtendedMeshShape::TrianglesSubpart part;
			part.m_numTriangleShapes = this->m->Objects[numobj].numFaces/3;
			part.m_numVertices = this->m->Objects[numobj].numVerts;
			part.m_vertexBase = &verts[0];
			part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
			part.m_vertexStriding = sizeof(float) * 3;
			part.m_indexBase = &this->m->Objects[numobj].Faces[0];
			part.m_indexStriding = sizeof(short) * 3;
			extendedMesh->addTrianglesSubpart( part );

			delete[] verts;
		}
		hkpMoppCompilerInput mci;
		mci.m_enableChunkSubdivision = true;

		hkpMoppCode* code = hkpMoppUtility::buildCode( extendedMesh ,mci);

		hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(extendedMesh, code);
		code->removeReference();
		extendedMesh->removeReference();
		

		hkpRigidBodyCinfo info;
		info.m_friction = 2;	
		info.m_shape = moppShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_position = position;
		info.m_rotation = rotation;

		hkpRigidBody *r=new hkpRigidBody(info);
		m_world->addEntity(r);

		this->rbody=r;
		return 1;
	}

	void Draw() {
		if(this->enable_DL==1) {
			glCallList(this->display_list);
			return;
		}
		if(this->rbody) {
			glPushMatrix();
				hkTransform t=this->rbody->getTransform();
				hkReal *p=new hkReal[16];
				t.get4x4ColumnMajor(p);
				glMultMatrixf(p);

				this->m->scale.x=this->scale(0);
				this->m->scale.y=this->scale(1);
				this->m->scale.z=this->scale(2);

				m->Draw();
			glPopMatrix();

			delete p;
		}
	}

	void BuildDisplayList() {
		this->display_list = glGenLists(1);
		glNewList(this->display_list,GL_COMPILE);
		this->Draw();
		glEndList();
		this->enable_DL=1;
	}

};

#endif

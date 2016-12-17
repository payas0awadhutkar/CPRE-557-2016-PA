#ifndef HAVOKFUNCS_H
#define HAVOKFUNCS_H


class MyPhantomShape: public hkpPhantomCallbackShape {
	public:

		MyPhantomShape(){ }

		// hkpPhantom interface implementation
		virtual void phantomEnterEvent( const hkpCollidable* collidableA, const hkpCollidable* collidableB, const hkpCollisionInput& env) {
			// the color can only be changed once the entity has been added to the world
			hkpRigidBody* owner = hkGetRigidBody(collidableB);

			// the "Collidables" here are "faked" so it's necessary to get the owner first in order
			// to get the "real" collidable!
			//HK_SET_OBJECT_COLOR((hkUlong)owner->getCollidable(), hkColor::rgbFromChars(255, 0, 0));
			//HK_SET_OBJECT_COLOR((int)&collidableB, hkColor::rgbFromChars(255, 0, 0));
		}

		// hkpPhantom interface implementation
		virtual void phantomLeaveEvent(const hkpCollidable* collidableA, const hkpCollidable* collidableB) {
			// the color can only be changed once the entity has been added to the world
			hkpRigidBody* owner = hkGetRigidBody(collidableB);

			// the "Collidables" here are "faked" so it's necessary to get the owner first in order
			// to get the "real" collidable!
			//HK_SET_OBJECT_COLOR((hkUlong)owner->getCollidable(), hkColor::rgbFromChars(0, 255, 0));

			// If moving out AND falling down, apply impulse to fire it towards "wall"
			if(owner->getLinearVelocity()(1) < 0.0f)
			{
				hkVector4 impulse(-50.0f, 220.0f, 0.0f);
				owner->applyLinearImpulseAsCriticalOperation(impulse);
			}
		}
		
};

hkpRigidBody* BuildConvex(hkpWorld* m_world) {
		hkpShape* shape = 0;
		int numVertices = 3*13;

		// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
		int stride = 12;

		float *vertices ;//= gCylinderData;

			/////////////////// SHAPE CONSTRUCTION ////////////////
		hkStridedVertices stridedVerts;
		{
				stridedVerts.m_numVertices = numVertices;
				stridedVerts.m_striding = stride;
				stridedVerts.m_vertices = vertices;
		}    
			
		shape = new hkpConvexVerticesShape(stridedVerts);

			// Make sure that a shape was created
		HK_ASSERT(0, shape);

		// To illustrate using the shape, first define a rigid body template.
		hkpRigidBodyCinfo rigidBodyInfo;
		rigidBodyInfo.m_position.set(0.0f, 20.0f, 0.0f);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_mass=10.0f;
		// Compute the rigid body inertia.
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
		//hkpInertiaTensorComputer::setShapeVolumeMassProperties( rigidBodyInfo.m_shape, 100.0f, rigidBodyInfo );

		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Remove reference since the body now "owns" the Shape.
		shape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();

		m_world->unlock();

		return rigidBody;
}




hkpRigidBody* Table(hkpWorld *m_world) {
			const hkReal radius = 0.01f;
			hkVector4 tableExtents( 1.0f, 0.02f, 0.6f );
			hkpBoxShape* tableShape = new hkpBoxShape( tableExtents, radius );

			hkVector4 legExtents( 0.1f, 0.6f, 0.1f );
			hkpBoxShape* legShape = new hkpBoxShape( legExtents, radius );

			hkVector4 legInvertDist( 0.1f, 0.0f, 0.1f );
			hkVector4 l; l.setSub4( tableExtents, legInvertDist );
			l.sub4( legExtents );

			hkVector4 tablePos( 0, 2 * legExtents(1) + tableExtents(1), 0 ); 
			hkVector4 leg0Pos (  l(0), legExtents(1),  l(2) );
			hkVector4 leg1Pos ( -l(0), legExtents(1),  l(2) );
			hkVector4 leg2Pos (  l(0), legExtents(1), -l(2) );
			hkVector4 leg3Pos ( -l(0), legExtents(1), -l(2) );

			const hkpConvexShape* shapes[5];

			// Create the convex list elements
			shapes[0] = new hkpConvexTranslateShape( tableShape, tablePos );
			shapes[1] = new hkpConvexTranslateShape( legShape, leg0Pos );
			shapes[2] = new hkpConvexTranslateShape( legShape, leg1Pos );
			shapes[3] = new hkpConvexTranslateShape( legShape, leg2Pos );
			shapes[4] = new hkpConvexTranslateShape( legShape, leg3Pos);
			
			/////////////////// SHAPE CONSTRUCTION ////////////////
			hkpShape *shape = new hkpConvexListShape( &shapes[0], 5);

			// Remove temporary shapes
			{
				for (int i =0; i < 5; i++ ) shapes[i]->removeReference();
				legShape->removeReference();
				tableShape->removeReference();
			}
					// To illustrate using the shape, first define a rigid body template.
			hkpRigidBodyCinfo rigidBodyInfo;
			rigidBodyInfo.m_position.set(0.0f, 5.0f, 0.0f);
			rigidBodyInfo.m_angularDamping = 0.0f;
			rigidBodyInfo.m_linearDamping = 0.0f;

			rigidBodyInfo.m_shape = shape;

			// Compute the rigid body inertia.
			rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties( rigidBodyInfo.m_shape, 1000.0f, rigidBodyInfo );

			// Create a rigid body (using the template above).
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

			// Remove reference since the body now "owns" the Shape.
			shape->removeReference();

			// Finally add body so we can see it, and remove reference since the world now "owns" it.
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();

			//m_world->unlock();
			return rigidBody;
}

hkpRigidBody* loadRigidBodyFromXmlFile(char* path, hkPackfileReader::AllocatedData*& allocatedData)
{
	hkIstream infile(path);

	hkpPhysicsData* physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), &allocatedData);
	hkArray<hkpPhysicsSystem*> physicsSystems;
	physicsSystems = physicsData->getPhysicsSystems();
	HK_ASSERT(0xaf836275, physicsSystems.getSize() == 1);

	hkArray<hkpRigidBody*> rigidBodies;
	rigidBodies = physicsSystems[0]->getRigidBodies();
	HK_ASSERT(0xaf836271, rigidBodies.getSize() >= 1);

	return rigidBodies[0];
}




hkpRigidBody* CreateBox(hkpWorld *m_world,hkVector4 &size,hkQuaternion &rotation,hkVector4 &position,float friction,bool isstatic=0,float mass=0) {
		hkpBoxShape* boxShape = new hkpBoxShape( size , 0 );

		hkpRigidBodyCinfo info;
		info.m_friction = friction;	

		info.m_shape = boxShape;
		
		if(!isstatic) {
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(size, mass, massProperties);
			info.m_mass  = mass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		}
		else
			info.m_motionType = hkpMotion::MOTION_FIXED;

		info.m_position = position;
		info.m_rotation = rotation;

		// Create a box
		hkpRigidBody *box=new hkpRigidBody(info);
		m_world->addEntity(box);
		box->removeReference();

		return box;
}

hkpRigidBody *LoadMesh() {

	 float vertices[24] = {-1, -1, -1, 
						1,  -1, -1, 
						-1, 1,  -1, 
						1,  1,  -1,
						-1, -1, 1, 
						1,  -1, 1, 
						-1, 1,  1, 
						1,  1,  1,
						};

	// Triangles is 12*3
	 hkUint16 indices[12*3] = {
							1,2,3,        
							0,2,1,  
							5,4,1,    
							1,4,0,    
							1,3,5,    
							3,7,5,    
							3,2,7,    
							2,6,7,    
							2,0,6,    
							4,6,0,
							7,4,5,
							7,6,4
							};

	//	void* vertices = 0;
	//	meshBlock->LockVertexBuffer(0, (void**)&vertices);
	int numVertices = 5;

	//void* indices = 0;
	//meshBlock->LockIndexBuffer(0, (void**)&indices);
	int numTriangles = 1;

	hkpExtendedMeshShape* m_mesh = new hkpExtendedMeshShape();
	{
		hkpExtendedMeshShape::TrianglesSubpart part;
		part.m_numTriangleShapes	= numTriangles;
		part.m_numVertices			= numVertices;
		part.m_vertexBase			= &vertices[0];
		part.m_stridingType			= hkpExtendedMeshShape::INDICES_INT16;
		part.m_vertexStriding		= sizeof(float) * 3;
		part.m_indexBase			= &indices[0];
		part.m_indexStriding		= sizeof( hkUint16 ) * 3;

		m_mesh->addTrianglesSubpart(part);
	}

	hkpRigidBodyCinfo blockInfo;
	hkVector4 blockPosition(0.0f, 5.0f, 0.0f);
	hkReal blockMass = 10.0f;

	blockInfo.m_shape = m_mesh;
	blockInfo.m_position = blockPosition;
	blockInfo.m_mass = blockMass;
	blockInfo.m_motionType = hkpMotion::MOTION_FIXED;
	blockInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
			
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(m_mesh, blockMass, blockInfo);

	hkpRigidBody *pRigidBody = new hkpRigidBody(blockInfo);

	m_world->addEntity(pRigidBody);
	m_mesh->removeReference();
	pRigidBody->removeReference();
	return pRigidBody;
}



hkpRigidBody* createMoppBody(int side, const hkVector4& position, hkReal scale) {
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f ); 
	{
		meshShape->m_vertices.setSize( side * side );
		for(int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				hkVector4 vertex ( i * 1.0f - side * 0.5f,
					j * 1.0f - side * 0.5f,
					0.6f * hkMath::cos((hkReal)j + i) + 0.3f * hkMath::sin( 2.0f * i) );
				vertex.add4(position);
				vertex.mul4(scale);
				meshShape->m_vertices[i*side + j] = vertex ;
			}
		}

		meshShape->m_triangles.setSize( (side-1) * (side-1) * 2);
		int corner = 0;
		int curTri = 0;
		for(int i = 0; i < side - 1; i++) {
			for (int j = 0; j < side - 1; j++) {
				meshShape->m_triangles[curTri].m_a = corner;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+1;
				curTri++;

				meshShape->m_triangles[curTri].m_a = corner+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+side+1;
				curTri++;
				corner++;
			}
			corner++;
		}
	}

	hkpStorageExtendedMeshShape* extendedMesh = new hkpStorageExtendedMeshShape();

	hkpExtendedMeshShape::TrianglesSubpart part;
	part.m_numTriangleShapes = meshShape->m_triangles.getSize();
	part.m_numVertices = meshShape->m_vertices.getSize();
	part.m_vertexBase = (float*)meshShape->m_vertices.begin();
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;
	part.m_vertexStriding = sizeof(hkVector4);
	part.m_indexBase = meshShape->m_triangles.begin();
	part.m_indexStriding = sizeof(hkpSimpleMeshShape::Triangle);
	extendedMesh->addTrianglesSubpart( part );

	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = false;

	hkpMoppCode* code = hkpMoppUtility::buildCode( extendedMesh ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(extendedMesh, code);
	code->removeReference();
	extendedMesh->removeReference();
	meshShape->removeReference();

	hkpRigidBodyCinfo info;
	info.m_friction = 10;	

	info.m_shape = moppShape;
	
	info.m_motionType = hkpMotion::MOTION_FIXED;

	info.m_position = position;
	info.m_rotation = hkQuaternion(hkVector4(0,1,0),0);

	// Create a box
	hkpRigidBody *box=new hkpRigidBody(info);
	m_world->addEntity(box);
	moppShape->removeReference();
	
	return box;
}

float vertices[24] = {-1, -1, -1, 1,  -1, -1, -1, 1,  -1, 1,  1,  -1,-1, -1, 1, 1,  -1, 1, -1, 1,  1, 1,  1,  1,};		
short indices[12*3] = {1,2,3,        0,2,1,  5,4,1,    1,4,0,    1,3,5,    3,7,5,    3,2,7,    2,6,7,    2,0,6,    4,6,0,   7,4,5,	7,6,4};


hkpRigidBody* createMesh(float *vertices,short *indices,const hkVector4& position, hkReal scale,int side1,int side2) {

	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f ); {
		meshShape->m_vertices.setSize(side1/3);
			for (int j = 0; j < side1/3; j++ ) {
				hkVector4 vertex (vertices[j*3+0],
					vertices[j*3+1],
					vertices[j*3+2] );
				//vertex.add4(position);
				vertex.mul4(scale);
				meshShape->m_vertices[j] = vertex ;
			}
		
		meshShape->m_triangles.setSize( side2/3 );
			for (int j = 0; j < side2/3 ; j++) {
				meshShape->m_triangles[j].m_a = indices[3*j];
				meshShape->m_triangles[j].m_b = indices[3*j+1];
				meshShape->m_triangles[j].m_c = indices[3*j+2];
			}
	}

	hkpStorageExtendedMeshShape* extendedMesh = new hkpStorageExtendedMeshShape();

	hkpExtendedMeshShape::TrianglesSubpart part;
	part.m_numTriangleShapes = side2/3;
	part.m_numVertices = side1/3;
	part.m_vertexBase = &vertices[0];
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
	part.m_vertexStriding = sizeof(float) * 3;
	part.m_indexBase = &indices[0];
	part.m_indexStriding = sizeof(short) * 3;
	extendedMesh->addTrianglesSubpart( part );

	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = false;

	hkpMoppCode* code = hkpMoppUtility::buildCode( extendedMesh ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(extendedMesh, code);
	code->removeReference();

	hkpRigidBodyCinfo info;
	info.m_friction = 10;	
	info.m_shape = moppShape;
	info.m_motionType = hkpMotion::MOTION_FIXED;
	info.m_position = position;
	info.m_rotation = hkQuaternion(hkVector4(0,1,0),0);

	hkpRigidBody *r=new hkpRigidBody(info);
	m_world->addEntity(r);
	return r;
}



hkpRigidBody* CreateXMLMeshBody(XMLModel m, hkpWorld *m_world,const hkVector4& position, hkReal scale) {
	hkpStorageExtendedMeshShape* extendedMesh = new hkpStorageExtendedMeshShape();
	for(int numobj=0;numobj<m.Objects.size();numobj++) {
		float *verts=&m.Objects[numobj].Vertexes[0];
		for(int i=0;i<m.Objects[numobj].Vertexes.size();i++)
			verts[i]*=scale;
		hkpExtendedMeshShape::TrianglesSubpart part;
		part.m_numTriangleShapes = m.Objects[numobj].Faces.size()/3;
		part.m_numVertices = m.Objects[numobj].Vertexes.size()/3;
		part.m_vertexBase = &verts[0];
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
		part.m_vertexStriding = sizeof(float) * 3;
		part.m_indexBase = &m.Objects[numobj].Faces[0];
		part.m_indexStriding = sizeof(short) * 3;
		extendedMesh->addTrianglesSubpart( part );
	}
	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = true;

	hkpMoppCode* code = hkpMoppUtility::buildCode( extendedMesh ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(extendedMesh, code);
	code->removeReference();
	//extendedMesh->removeReference();


	hkpRigidBodyCinfo info;
	info.m_friction = 10;	
	info.m_shape = moppShape;
	info.m_motionType = hkpMotion::MOTION_FIXED;
	info.m_rotation = hkQuaternion(hkVector4(1,0,0),0);
	info.m_position = position;

	hkpRigidBody *r=new hkpRigidBody(info);
	m_world->addEntity(r);
	return r;
}

void CreateHavokAnimation() {
		hkArray<hkUlong> m_objectIds;

		hkLoader* m_loader = new hkLoader();
		static const char* rigAsset = "../sdks/Havok_Physics/hk660r1/Demo/Demos/Resources/Animation/Ragdoll/hires_ragdoll_L4101.hkx";

		
		hkString assetFile ;//= hkAssetManagementUtil::getFilePath( rigAsset );
		hkRootLevelContainer* container = m_loader->load( rigAsset);

		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");

		hkpPhysicsData* physics = reinterpret_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ));
		HK_ASSERT2(0x27343635, physics, "No physics loaded");

		// Physics
		if (physics) {
			const hkArray<hkpPhysicsSystem*>& psys = physics->getPhysicsSystems();

			// Tie the two together
			for (int i=0; i<psys.getSize(); i++) {
				hkpPhysicsSystem* system = psys[i];

				// Change the layer of the rigid bodies
				for (int rb=0; rb < system->getRigidBodies().getSize(); rb++) {
					const hkUlong id = hkUlong(system->getRigidBodies()[rb]->getCollidable());
					m_objectIds.pushBack(id);
				}
				
				// Associate the display and physics (by name)
				
				// add the lot to the world
				m_world->addPhysicsSystem(system);
			}
		}
}


//havok + opengl
int DrawByShape(hkpShape *shape,hkTransform &t3,hkVector4 &color,GLuint texture=NULL,Model_3DS *m=NULL) {
	static int x=0;
	hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
	hkpShapeDisplayBuilder GeometryBuilder(env);
	hkArray<hkDisplayGeometry*>* displayGeometries;
				
	displayGeometries = new hkArray<hkDisplayGeometry*>;
	GeometryBuilder.buildDisplayGeometries(shape, *displayGeometries );

	for (int j=0; j<displayGeometries->getSize(); j++) {   
		hkDisplayGeometry* geometry = (*displayGeometries)[j];
		geometry->buildGeometry();
		
		hkGeometry* mesh = geometry->getGeometry();
		if(!mesh)exit(0);
		

		float *v= new float[mesh->m_vertices.getSize()*3];
		for(int i=0;i<mesh->m_vertices.getSize()*3;i+=3) {
			v[i]=mesh->m_vertices[i/3](0);
			v[i+1]=mesh->m_vertices[i/3](1);
			v[i+2]=mesh->m_vertices[i/3](2);
		}

		GLushort *t= new GLushort[mesh->m_triangles.getSize()*3];
		//unsigned short *t= new unsigned short[mesh->m_triangles.getSize()*3];
		for(int i=0;i<mesh->m_triangles.getSize()*3;i+=3) {
			t[i]=mesh->m_triangles[i/3].m_a;
			t[i+1]=mesh->m_triangles[i/3].m_b;
			t[i+2]=mesh->m_triangles[i/3].m_c;
		}

		glPushMatrix();		
			
			if(texture!=NULL)
			{
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glEnable(GL_TEXTURE_2D);
				glBindTexture( GL_TEXTURE_2D, texture );
			}
			else
			{
				glColor3f(color(0),color(1),color(2));
			}
			//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			
			hkReal *p=new hkReal[16];
			t3.get4x4ColumnMajor(p);
			glMultMatrixf(p);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, v);
			
			glDrawElements(GL_TRIANGLES, mesh->m_triangles.getSize()*3, GL_UNSIGNED_SHORT, t);
			//glDrawElements(GL_TRIANGLES, mesh->m_triangles.getSize()*3, GL_UNSIGNED_SHORT, Objects[i].MatFaces[j].subFaces);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glPopMatrix();
		geometry->removeReference();
		delete t;
		delete v;
		delete p;
	}
	x++;
	delete displayGeometries;	
	return 1;
}


int DrawRigidBody(hkpRigidBody *rbody,hkVector4 &color,GLuint texture=NULL,Model_3DS *m=NULL,XMLModel *m_xml=NULL)
{
	if(!rbody)return 0;
	const hkpShape *shape=rbody->getCollidable()->getShape();
	hkTransform t=rbody->getTransform();

	glColor3f(color(0),color(1),color(2));
	if(m!=0)
	{
		glPushMatrix();		
				hkReal *p=new hkReal[16];
				t.get4x4ColumnMajor(p);
				glMultMatrixf(p);
				m->Draw();
				delete p;
		glPopMatrix();
	}
	else if(m_xml!=0)
	{
		glPushMatrix();		
				hkReal *p=new hkReal[16];
				t.get4x4ColumnMajor(p);
				glMultMatrixf(p);
				m_xml->Draw(m_xml->scale);
				delete p;
		glPopMatrix();
	}
	else 
		DrawByShape((hkpShape*)shape,t,color,texture,m);
	return 1;
}

#endif

class Cube;
class vpoint {
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;
	vpoint(){}
	vpoint(GLdouble x,GLdouble y,GLdouble z) {
		SetPoint(x,y,z);
	}
	void SetPoint(GLdouble x,GLdouble y,GLdouble z) {
		this->x=x;
		this->y=y;
		this->z=z;
	}
};

class Cube {
public:

	GLdouble x,y,z,length,height,width;
	int u,v;
	
	Cube(GLdouble x,GLdouble y,GLdouble z,GLdouble length,GLdouble height,GLdouble width) {
			this->x=x;
			this->y=y;
			this->z=z;
			this->length=length;
			this->height=height;
			this->width=width;
			u=1;v=1;
	}

	void SetUV(int u,int v) {
		this->u=u;
		this->v=v;
	}

	int draw(GLuint texture) {
		glPushMatrix();
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texture);

			glColor4d (1,1,1,1);

			glBegin(GL_QUADS);
			glTexCoord2d(u,v);  glVertex3d(x,y,z);
			glTexCoord2d(u,0);  glVertex3d(x,y,z+width);
			glTexCoord2d(0,0);  glVertex3d(x+length,y,z+width);
			glTexCoord2d(0,v);  glVertex3d(x+length,y,z);    
			glEnd();

			glBegin(GL_QUADS);
			glTexCoord2d(u,v);  glVertex3d(x,y,z+width);
			glTexCoord2d(u,0);  glVertex3d(x,y-height,z+width);
			glTexCoord2d(0,0);  glVertex3d(x+length,y-height,z+width);
			glTexCoord2d(0,v);  glVertex3d(x+length,y,z+width);    
			glEnd();

			glBegin(GL_QUADS);
			glTexCoord2d(u,v);  glVertex3d(x,y,z);
			glTexCoord2d(u,0);  glVertex3d(x,y-height,z);
			glTexCoord2d(0,0);  glVertex3d(x+length,y-height,z);
			glTexCoord2d(0,v);  glVertex3d(x+length,y,z);    
			glEnd();

			glBegin(GL_QUADS);
			glTexCoord2d(u,v);  glVertex3d(x+length,y,z);
			glTexCoord2d(u,0);  glVertex3d(x+length,y-height,z);
			glTexCoord2d(0,0);  glVertex3d(x+length,y-height,z+width);
			glTexCoord2d(0,v);  glVertex3d(x+length,y,z+width);    
			glEnd();

			glBegin(GL_QUADS);
			glTexCoord2d(u,v);  glVertex3d(x,y,z);
			glTexCoord2d(u,0);  glVertex3d(x,y-height,z);
			glTexCoord2d(0,0);  glVertex3d(x,y-height,z+width);
			glTexCoord2d(0,v);  glVertex3d(x,y,z+width);    
			glEnd();
		      
			glBegin(GL_QUADS);
			glTexCoord2d(u,v);  glVertex3d(x,y-height,z);
			glTexCoord2d(u,0);  glVertex3d(x,y-height,z+width);
			glTexCoord2d(0,0);  glVertex3d(x+length,y-height,z+width);
			glTexCoord2d(0,v);  glVertex3d(x+length,y-height,z);    
			glEnd();
		glPopMatrix();
		return 0;
	}

};


class object {
public:
	vpoint v;
	GLdouble height;
	GLdouble width;
	GLdouble length;
	GLuint texture;
	object(){}
	object(GLdouble x,GLdouble y,GLdouble z,GLdouble l,GLdouble h,GLdouble w,GLuint texture) {
		v.x=x;
		v.y=y;
		v.z=z;
		height=h;
		width=w;
		length=l;
		this->texture=texture;
	}
};

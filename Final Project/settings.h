#include<stdio.h>
#include<windows.h>
#include<string>
#include<vector>
#include <algorithm>
#include "header.h"
using namespace std;

string trim(const string& txt) {
	string str=txt;
    string whitespaces (" \t\f\v\n\r");
	 
	int start=str.find_first_not_of(whitespaces);
	int end=str.find_last_not_of(whitespaces)+1;

	if(start== string::npos) return "";
	return str.substr(start,end);
}

struct Vector3 {
	float x,y,z,w;

	Vector3(){w=0;}
	Vector3(float x,float y,float z,float w=0){
		this->x=x;
		this->y=y;
		this->z=z;
		this->w=w;
	}
	Vector3 operator()(Vector3 v){
		x=v.x;
		y=v.y;
		z=v.z;
		w=v.w;
		return *this;
	}
	float operator()(int i)
	{
		if (i==0)return x;
		else if(i==1)return y;
		else if(i==2)return z;
		else if(i==3)return w;
		else return NULL;
	}
	void set(int n,float value)
	{
		if(n==0)	 x=value;
		else if(n==1)y=value;
		else if(n==2)z=value;
		else if(n==3)w=value;
	}
};

struct Vector4
{
	float x,y,z;
	float w;
};

struct SettingObjects
{
	string Model_Type;
	string Model_Name;
	string Model_Path;
	Vector3 Model_Size;
	Vector3 Model_Pos;
	Vector4 Model_Rot;
};

class Setting {
public:
	vector<SettingObjects>objects;
	void GetSettings(char *fname) {
		SettingObjects obj;
		char *txt=new char[100];
		FILE *f=fopen(fname,"r");
		if(f==NULL)return;

		int flag=-1;
		while(fgets(txt,100,f)!=NULL) {
			string str=trim(txt);
			
			if(str=="START")flag=1;
			else if(str=="END")flag=0;

			if(flag==1) {
				if(str.find("=")==-1)continue;
				string data=trim(str.substr(0,str.find("=")));
				string value=trim(str.substr((int)str.find("=")+1));

				if(data=="NAME") {
						obj.Model_Name=value;
				}
				else if(data=="PATH") obj.Model_Path=value;
				else if(data=="TYPE") obj.Model_Type=value;
				else if(data=="POS") {
					obj.Model_Pos.x=atof(value.substr(0,value.find(",")).c_str());
					obj.Model_Pos.z=atof(value.substr(value.rfind(",")+1).c_str());

					value=value.substr(value.find(",")+1);
					value=value.substr(0,value.find(","));
					obj.Model_Pos.y=atof(value.c_str());

				}
				else if(data=="SIZE") {
					obj.Model_Size.x=atof(value.substr(0,value.find(",")).c_str());
					obj.Model_Size.z=atof(value.substr(value.rfind(",")+1).c_str());

					value=value.substr(value.find(",")+1);
					value=value.substr(0,value.find(","));
					obj.Model_Size.y=atof(value.c_str());
				}
				else if(data=="ROT") {
					obj.Model_Rot.x=atof(value.substr(0,value.find(",")).c_str());
					obj.Model_Rot.z=atof(value.substr(value.rfind(",")+1).c_str());
					obj.Model_Rot.w=atof(value.substr(value.find(":")+1,value.size()).c_str());

					value=value.substr(value.find(",")+1);
					value=value.substr(0,value.find(","));
					obj.Model_Rot.y=atof(value.c_str());
				}
			}
			else if(flag==0) {
				if(obj.Model_Name.size()>1)
					objects.push_back(obj);
				flag=-1;
			}
		}
		fclose(f);
	}
	vector<SettingObjects> GetAllModelsByType(char* type) {
		vector<SettingObjects>s;
		for(int i=0;i<objects.size();i++) {
			if(!strcmp(objects[i].Model_Type.c_str(),type))
				s.push_back(objects[i]);
		}
		return s;
	}

	SettingObjects *GetModelDataByName(string name) {
		for(int i=0;i<objects.size();i++) {
			if(name==objects[i].Model_Name)
				return &objects[i];
		}
		return NULL;
	}

	int GetModelDataNumber(SettingObjects s,char* type=NULL) {
		int j=0;
		for(int i=0;i<objects.size();i++) {
			if( s.Model_Name==objects[i].Model_Name) {
				return i;
			}
		}
		return -1;
	}

	int IsDuplicateOf(SettingObjects s,char* type=NULL) {
		int flag=0,j=0;
		for(int i=0;i<GetModelDataNumber(s);i++) {
			if(!strcmp(s.Model_Type.c_str(),type)) {
				if( s.Model_Path==objects[i].Model_Path) {
						return i;
				}
			}
		}
		return -1;
	}

	SettingObjects *GetModelDataByPath(string path) {
		for(int i=0;i<objects.size();i++) {
			if(path==objects[i].Model_Path)
				return &objects[i];
		}
		return NULL;
	}
};

class userSetting {
public:
	struct data {
		string type;
		string value;
		string name;
	};

	vector<data>userData;

	void Load()
	{
		FILE *f=fopen("userSettings","r");
		if(f==NULL)VBO_ENABLE=0;
		else
		{
			char *txt=new char[100];
			while(fgets(txt,100,f)!=NULL)
			{
				data userdata;
				string str=trim(txt);
				if(str.find("=")==-1)continue;
				string data=trim(str.substr(0,str.find("=")));
				userdata.value=trim(str.substr((int)str.find("=")+1));

				userdata.type=trim(data.substr(0,str.find(" ")));
				userdata.name=trim(data.substr((int)str.find(" ")+1));

				userData.push_back(userdata);
			}
		}
		fclose(f);

		//printf("+++%d+++\n",userData.size());
		for(int i=0;i<userData.size();i++) {
			//printf("+++++++++++%s %s %s++++++++++++\n",userData[i].name.c_str(),userData[i].type.c_str(),userData[i].value.c_str());
		}
	}

	data *getUserData(char *name) {
		for(int i=0;i<userData.size();i++) {
			if(name==userData[i].name) {
				return &userData[i];
			}
		}
		return NULL;
	}

}myData;

class __Carmodel {
public:
	
	struct data {
		string type;
		string name;
		vector<string> value;
		Vector3 getToVec3() {
			Vector3 v(0,0,0);
			for(int i=0;i<4&&i<value.size();i++) {
				v.set(i,atof(value[i].c_str()));
			}
			return v;
		}
	};

	struct __varData {
		string name;
		vector<data>userData;

		data *searchByName(string name) {
			for(int i=0;i<userData.size();i++) {
				if(name==userData[i].name) {
					return &userData[i];
				}
			}
			return NULL;
		}
	};
	vector<__varData>varData;
	void Load(string path) {
		bool flag=0;
		FILE *f=fopen(path.c_str(),"r");
		if(!f)exit(0);
		char *txt=new char[100];
		__varData vData;
		while(fgets(txt,100,f)!=NULL) {
			string str=trim(txt);
			data userdata;

			if(flag==1 && str=="#end") {
				varData.push_back(vData);
				flag=0;
				vData.userData.clear();
				continue;
			}
			if(flag==0 && str[0]=='#') {
				vData.name=str.substr(1,str.size()-1);
				flag=1;
			}

			if(str.find("=")==-1)continue;
			string data=trim(str.substr(0,str.find("=")));

			userdata.name=trim(data.substr((int)data.find(" ")+1));
			userdata.type=trim(str.substr(0,str.find(userdata.name)));
			if(strlen(userdata.type.c_str())<1)userdata.type="string";

			string value=trim(str.substr((int)str.find("=")+1));
			do {
				string str=",";
				if(value.find(",")<value.find(":"))str=",";
				else str=":";
				
				userdata.value.push_back(trim(value.substr(0,value.find(str))));
				if(value.find(str)==-1)break;
				
				value=trim(value.substr(value.find(str)+1));
			}while(1);
			vData.userData.push_back(userdata);
		}
		fclose(f);
	}

	__varData *getUserData(char *name) {
		for(int i=0;i<varData.size();i++) {
			if(varData[i].name==name)
				return &varData[i];
		}
		return NULL;
	}
};


struct carModelCommanArgs {
	Vector3 rot;
	Vector3 pos;
	Vector3 scale;

	carModelCommanArgs operator =(carModelCommanArgs t) {
		rot(t.rot);
		pos(t.pos);
		scale(t.scale);

		return *this;
	}
};

carModelCommanArgs *carDataTocarModelCommanArgs(__Carmodel::__varData d) {
	carModelCommanArgs c;
	c.pos(d.searchByName("pos")->getToVec3());
	c.rot(d.searchByName("rot")->getToVec3());
	c.scale(d.searchByName("scale")->getToVec3());
	return &c;
}

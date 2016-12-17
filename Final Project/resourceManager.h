#include<vector>
using namespace std;

class ResourceManager {
public:
	vector<Model_3DS*>models_3DS;
	Model_3DS *AddModel_3DS(char *fileName,char *model_name,bool delete_VNT=1) {
		Model_3DS *model;
		if( (model=GetModel_3DS_ByPath(fileName))!=0)
			return model;

		printf("\nModels:%s",fileName);
		model=new Model_3DS();
		model->Load(fileName);
		model->modelname=model_name;
		
		if(VBO_ENABLE)model->BuildVBO(delete_VNT);
		
		models_3DS.push_back(model);
		return model;
	}
	Model_3DS *GetModel_3DS_ByName(char *model_name) {
		for(int i=0;i<models_3DS.size();i++) {
			if(!stricmp(models_3DS[i]->modelname,model_name))
				return models_3DS[i];
		}
		return NULL;
	}
	Model_3DS *GetModel_3DS_ByPath(char *model_path) {
		for(int i=0;i<models_3DS.size();i++)
		{
			if( !stricmp(models_3DS[i]->fullpath,model_path))
			{
				return models_3DS[i];
			}
		}
		return NULL;
	}


	ResourceManager()
	{
		// stub activator
		// not doing anything fancy so can keep it empty
	}
};

ResourceManager g_resource;


void InitResources() {
	g_resource=new ResourceManager();
}

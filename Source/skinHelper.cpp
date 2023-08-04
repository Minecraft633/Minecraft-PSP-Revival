#include "skinHelper.h"
#include <Aurora/Graphics/RenderManager.h>

skinHelper::skinHelper()
{
	defaulPatch = "Assets/skins/";
	defaultFolder = "Steve/";
	skinPatch = defaulPatch+defaultFolder;
}

skinHelper::~skinHelper()
{

}

void skinHelper::Init()
{
    LoadskinPackPath();
    skinPatch = defaulPatch+defaultFolder;

    FastLoadTexture("skin.png");
}

int skinHelper::Getskin(skins skin)
{
	switch(skin)
	{
        case Steve:
		{
			fileName = skinPatch + "skin.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
	}
	return 0;
}

void skinHelper::SetskinPack(std::string name)
{
    TextureManager::Instance()->RemoveTextures();

	SetskinFolderName(name+"/");

    FastLoadTexture("skin.png");
}

void skinHelper::FastRemoveTexture(std::string name)
{

}

void skinHelper::RemoveConstTextures()
{
    TextureManager::Instance()->RemoveConstTextures();
}

void skinHelper::FastLoadTexture(std::string name)
{
    std::string endName = skinPatch+name;
    TextureManager::Instance()->LoadTexture(endName);
}

void skinHelper::SetskinFolderName(std::string name)
{
	defaultFolder = name;
	skinPatch = defaulPatch + defaultFolder;
	SaveskinPackPath();
}

void skinHelper::SaveskinPackPath()
{
	unsigned int pathLength = defaultFolder.size();

	FILE * pFile;
	pFile = fopen("Save/skin.cf","wb");

	if(pFile == NULL)
		return;

    fwrite(&pathLength,sizeof(unsigned int),1,pFile);
	fwrite(defaultFolder.c_str(),1,pathLength,pFile);

	//close file
	fclose(pFile);
}

void skinHelper::LoadskinPackPath()
{
	unsigned int pathLength = 0;

	FILE * pFile;
	pFile = fopen("Save/skin.cf","rb");

    if(pFile == NULL)
    {
        defaultFolder = "Steve/";
        return;
    }


    fread(&pathLength,sizeof(unsigned int),1,pFile);
	char* c = new char[pathLength+1];
    fread(c,1,pathLength,pFile);
    c[pathLength] = '\0';
    defaultFolder = c;
    delete[] c;

	//close file
	fclose(pFile);

	if(fileExists(defaulPatch+defaultFolder+"skin.png") == false) // if this skin pack doesn't exist at the moment
    {
        defaultFolder = "Steve/"; // return to default one
    }
}

inline bool skinHelper::fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

skinHelper skinHelper::m_skinHelper;

skinHelper* skinHelper::Instance()
{
	return &m_skinHelper;
}

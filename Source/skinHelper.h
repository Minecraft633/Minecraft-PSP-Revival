#ifndef skinHELPER_H_
#define skinHELPER_H_

#include <string>
#include <pspiofilemgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

class skinHelper
{
private:

	std::string defaulPatch;
	std::string fileName;

public:

	enum skins
	{
         Steve,
	};
    std::string skinPatch;
    std::string defaultFolder;

	static skinHelper m_skinHelper;
	static skinHelper* Instance();


	skinHelper();
	~skinHelper();

	int Getskin(skins skin);
    void Init();
    void FastRemoveTexture(std::string name);
    void FastLoadTexture(std::string name);
	void SetskinFolderName(std::string name);
	void SetskinPack(std::string name);

    void RemoveConstTextures();

    void LoadskinPackPath();
    void SaveskinPackPath();
    inline bool fileExists(const std::string& name);
};
#endif
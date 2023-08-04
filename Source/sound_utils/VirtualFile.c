#include "oslib.h"

VIRTUAL_FILE_SOURCE* VirtualFileSources[VF_MAX_SOURCES];
int VirtualFileSourcesNb = 0;
int VF_MEMORY = -1;
OSL_VIRTUALFILENAME* osl_virtualFileList;
int osl_virtualFileListNumber = 0;
int osl_virtualFileListSize;
int osl_defaultVirtualFileSource = -1;
const char* osl_tempFileName = "*tmp*";
OSL_VIRTUALFILENAME osl_tempFile;

#define BLOCK_SIZE (4 << 10)
#define DEFAULT_TABLE_SIZE 128

int VirtualFileRegisterSource(VIRTUAL_FILE_SOURCE* vfs)
{
	if (VirtualFileSourcesNb >= VF_MAX_SOURCES)
		return -1;
	VirtualFileSources[VirtualFileSourcesNb] = vfs;
	return VirtualFileSourcesNb++;
}

VIRTUAL_FILE* VirtualFileOpen(void* param1, int param2, int type, int mode)
{
	VIRTUAL_FILE* f = NULL;
	if (type == VF_AUTO)
	{
		if (param2 == 0)
		{
			OSL_VIRTUALFILENAME* file = oslFindFileInVirtualFilenameList((const char*)param1, type);
			if (file)
			{
				param1 = file->data;
				param2 = file->size;
				type = *file->type;
			}
			else
				type = osl_defaultVirtualFileSource;
		}
	}

	if (type >= 0)
	{
		f = (VIRTUAL_FILE*)malloc(sizeof(*f));
		if (f)
		{
			memset(f, 0, sizeof(*f));
			f->type = type;
			if (!VirtualFileGetSource(f)->fOpen(param1, param2, type, mode, f))
			{
				free(f);
				f = NULL;
			}
		}
	}
	return f;
}

int VirtualFileClose(VIRTUAL_FILE* f)
{
	int result = VirtualFileGetSource(f)->fClose(f);
	if (result)
		free(f);
	return result;
}

int vfsMemOpen(void* param1, int param2, int type, int mode, VIRTUAL_FILE* f)
{
	if (param2 == 0)
	{
		OSL_VIRTUALFILENAME* file = oslFindFileInVirtualFilenameList((const char*)param1, type);
		if (file)
		{
			param1 = file->data;
			param2 = file->size;
		}
	}

	f->offset = 0;
	f->ioPtr = param1;
	f->maxSize = param2;
	return 1;
}

int vfsMemClose(VIRTUAL_FILE* f)
{
	return 1;
}

int vfsMemWrite(const void* ptr, size_t size, size_t n, VIRTUAL_FILE* f)
{
	int realSize = size * n, writeSize = 0;
	if (f->ioPtr)
	{
		writeSize = oslMin(realSize, f->maxSize - f->offset);
		if (writeSize > 0)
		{
			memcpy((char*)f->ioPtr + f->offset, ptr, writeSize);
			f->offset += writeSize;
		}
	}
	return writeSize;
}

int vfsMemRead(void* ptr, size_t size, size_t n, VIRTUAL_FILE* f)
{
	int readSize = 0, realSize = size * n;

	if (f->ioPtr)
	{
		readSize = oslMin(realSize, f->maxSize - f->offset);
		if (readSize > 0)
		{
			memcpy(ptr, (char*)f->ioPtr + f->offset, readSize);
			f->offset += readSize;
		}
	}
	return readSize;
}

int vfsMemGetc(VIRTUAL_FILE* f)
{
	unsigned char car;
	if (VirtualFileRead(&car, sizeof(car), 1, f) < 1)
		return -1;
	else
		return (int)car;
}

int vfsMemPutc(int caractere, VIRTUAL_FILE* f)
{
	unsigned char car = caractere;
	if (VirtualFileWrite(&car, sizeof(car), 1, f) < 1)
		return -1;
	else
		return caractere;
}

char* vfsMemGets(char* str, int maxLen, VIRTUAL_FILE* f)
{
	const int blockSize = 16;
	int offset = 0, i, size;
	while (1)
	{
		size = VirtualFileRead(str + offset, 1, oslMin(maxLen - offset, blockSize), f);
		if (offset + size < maxLen)
			str[offset + size] = 0;
		for (i = offset; i < offset + blockSize; i++)
		{
			if (str[i] == 0)
				return str;
			if (str[i] == '\r')
			{
				str[i] = 0;
				if (i + 1 >= offset + blockSize)
				{
					char temp[1];
					int tempSize;
					tempSize = VirtualFileRead(temp, 1, 1, f);
					if (!(tempSize > 0 && temp[0] == '\n'))
						i--;
				}
				else
				{
					if (str[i + 1] == '\n')
						i++;
				}
				VirtualFileSeek(f, -size + (i - offset) + 1, SEEK_CUR);
				return str;
			}
			else if (str[i] == '\n')
			{
				str[i] = 0;
				VirtualFileSeek(f, -blockSize + i + 1, SEEK_CUR);
				return str;
			}
		}
		offset += blockSize;
	}
	return str;
}

void vfsMemPuts(const char* s, VIRTUAL_FILE* f)
{
	VirtualFileWrite(s, strlen(s), 1, f);
}

void vfsMemSeek(VIRTUAL_FILE* f, int offset, int whence)
{
	if (f->ioPtr)
	{
		if (whence == SEEK_SET)
			f->offset = offset;
		else if (whence == SEEK_CUR)
			f->offset += offset;
		else if (whence == SEEK_END)
			f->offset = f->maxSize + offset;
		f->offset = oslMax(oslMin(f->offset, f->maxSize), 0);
	}
}

int vfsMemTell(VIRTUAL_FILE* f)
{
	return f->offset;
}

int vfsMemEof(VIRTUAL_FILE* f)
{
	return (f->offset < f->maxSize);
}

VIRTUAL_FILE_SOURCE vfsMemory =
{
	vfsMemOpen,
	vfsMemClose,
	vfsMemRead,
	vfsMemWrite,
	vfsMemGetc,
	vfsMemPutc,
	vfsMemGets,
	vfsMemPuts,
	vfsMemSeek,
	vfsMemTell,
	vfsMemEof,
};

int oslAddVirtualFileList(OSL_VIRTUALFILENAME* vfl, int numberOfEntries)
{
	//Need to allocate more?
	if (numberOfEntries + osl_virtualFileListNumber > osl_virtualFileListSize)
	{
		int finalSize = numberOfEntries + osl_virtualFileListNumber;
		OSL_VIRTUALFILENAME* v;
		if (finalSize % DEFAULT_TABLE_SIZE > 0)
			finalSize = finalSize - finalSize % DEFAULT_TABLE_SIZE + DEFAULT_TABLE_SIZE;
		v = (OSL_VIRTUALFILENAME*)realloc(osl_virtualFileList, finalSize * sizeof(OSL_VIRTUALFILENAME));
		if (v) {
			osl_virtualFileList = v;
			osl_virtualFileListSize = finalSize;
		}
		else
			return 0;
	}
	memcpy(osl_virtualFileList + osl_virtualFileListNumber, vfl, numberOfEntries * sizeof(OSL_VIRTUALFILENAME));
	osl_virtualFileListNumber += numberOfEntries;
	return 1;
}

void oslRemoveVirtualFileList(OSL_VIRTUALFILENAME* vfl, int numberOfEntries)
{
	int i;
	for (i = 0; i <= osl_virtualFileListNumber - numberOfEntries; i++)
	{
		if (!memcmp(osl_virtualFileList + i, vfl, numberOfEntries * sizeof(OSL_VIRTUALFILENAME)))
		{
			if (osl_virtualFileListNumber - i - numberOfEntries > 0)
				memmove(osl_virtualFileList + i, osl_virtualFileList + i + numberOfEntries, osl_virtualFileListNumber - i - numberOfEntries);
			osl_virtualFileListNumber -= numberOfEntries;
		}
	}
}

OSL_VIRTUALFILENAME* oslFindFileInVirtualFilenameList(const char* fname, int type)
{
	int i;
	OSL_VIRTUALFILENAME* file;
	if (fname)
	{
		if (fname[0] == '/')
			fname++;
		for (i = -1; i < osl_virtualFileListNumber; i++)
		{
			if (i == -1)
				file = &osl_tempFile;
			else
				file = osl_virtualFileList + i;
			if (!file->type)
				continue;
			if ((type == *file->type || type == VF_AUTO)
				&& !strcmp(fname, file->name))
				return file;
		}
	}
	return NULL;
}

void VirtualFileInit()
{
	if (VF_MEMORY < 0)
		VF_MEMORY = VirtualFileRegisterSource(&vfsMemory);
	if (VF_FILE < 0)
		VF_FILE = oslInitVfsFile();
	osl_defaultVirtualFileSource = VF_FILE;

	osl_virtualFileListSize = DEFAULT_TABLE_SIZE;
	osl_virtualFileListNumber = 0;
	osl_virtualFileList = (OSL_VIRTUALFILENAME*)malloc(DEFAULT_TABLE_SIZE * sizeof(OSL_VIRTUALFILENAME));

	osl_tempFile.name = osl_tempFileName;
	osl_tempFile.type = NULL;
}

void* oslReadEntireFileToMemory(VIRTUAL_FILE* f, int* fileSize)
{
	void* block = NULL;
	int add = 0;
	int size = 0, readSize, finalSize = 0;

	do
	{
		size += BLOCK_SIZE;
		if (block)
			block = realloc(block, size);
		else
			block = malloc(size);
		if (!block)
			return NULL;

		readSize = VirtualFileRead((char*)block + add, 1, BLOCK_SIZE, f);
		add += BLOCK_SIZE;
		finalSize += readSize;
	} 
	while (readSize >= BLOCK_SIZE);
	if (fileSize)
		*fileSize = finalSize;
	return block;
}

void oslSetTempFileData(void* data, int size, int* type)
{
	osl_tempFile.data = data;
	osl_tempFile.size = size;
	osl_tempFile.type = type;
}
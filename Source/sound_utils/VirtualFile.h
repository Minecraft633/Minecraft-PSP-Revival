#ifndef __OSL_VIRTUALFILE_H__
#define __OSL_VIRTUALFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define VirtualFileWrite(ptr, size, n, f)	(VirtualFileGetSource(f)->fWrite(ptr, size, n, f))
#define VirtualFileRead(ptr, size, n, f)	(VirtualFileGetSource(f)->fRead(ptr, size, n, f))
#define VirtualFileGetc(f)					(VirtualFileGetSource(f)->fGetc(f))
#define VirtualFilePutc(caractere, f)		(VirtualFileGetSource(f)->fPutc(caractere, f))
#define VirtualFileGets(str, maxLen, f)		(VirtualFileGetSource(f)->fGets(str, maxLen, f))
#define VirtualFilePuts(s, f)				(VirtualFileGetSource(f)->fPuts(s, f))
#define VirtualFileSeek(f, offset, whence)	(VirtualFileGetSource(f)->fSeek(f, offset, whence))
#define VirtualFileTell(f)					(VirtualFileGetSource(f)->fTell(f))
#define VirtualFileEof(f)					(VirtualFileGetSource(f)->fEof(f))
#define VF_MAX_SOURCES 16
#define VirtualFileGetSource(vf)		(VirtualFileSources[(vf)->type])
#define VF_AUTO -2

	typedef struct
	{
		void* ioPtr;		   //!< User data for IO processing (usually a pointer to data, FILE*, usw.)
		unsigned short type;   //!< Virtual file type (source number).
		unsigned long userData;//!< Additional data
		int offset, maxSize;   //!< Internal variables for memory-based (RAM / ROM) sources
	} VIRTUAL_FILE;

	enum VF_OPEN_MODES
	{
		VF_O_READ, //!< Read only
		VF_O_READWRITE, //!< Read & Write
		VF_O_WRITE //!< Write only
	};

	typedef struct
	{
		int (*fOpen)(void* param1, int param2, int type, int mode, VIRTUAL_FILE* f);
		int (*fClose)(VIRTUAL_FILE* f);
		int (*fRead)(void* ptr, size_t size, size_t n, VIRTUAL_FILE* f);
		int (*fWrite)(const void* ptr, size_t size, size_t n, VIRTUAL_FILE* f);
		int (*fGetc)(VIRTUAL_FILE* f);
		int (*fPutc)(int caractere, VIRTUAL_FILE* f);
		char* (*fGets)(char* str, int maxLen, VIRTUAL_FILE* f);
		void (*fPuts)(const char* s, VIRTUAL_FILE* f);
		void (*fSeek)(VIRTUAL_FILE* f, int offset, int whence);
		int (*fTell)(VIRTUAL_FILE* f);
		int (*fEof)(VIRTUAL_FILE* f);

	} VIRTUAL_FILE_SOURCE;

	typedef struct
	{
		const char* name; //!< Virtual file name
		void* data;	//!< RAM data block
		int size;	//!< Block data size
		int* type;	//!< Associated source (e.g. &VF_MEMORY). Don't forget the &, which is there so you can pass a variable which is not known at compile time (virtual file sources are registered upon start, so the compiler doesn't know the ID it will be given in advance).
	} OSL_VIRTUALFILENAME;
	void VirtualFileInit();
	VIRTUAL_FILE* VirtualFileOpen(void* param1, int param2, int type, int mode);
	int VirtualFileClose(VIRTUAL_FILE* f);
	int VirtualFileRegisterSource(VIRTUAL_FILE_SOURCE* vfs);
	OSL_VIRTUALFILENAME* oslFindFileInVirtualFilenameList(const char* fname, int type);

	extern VIRTUAL_FILE_SOURCE* VirtualFileSources[VF_MAX_SOURCES];
	extern int VirtualFileSourcesNb;
	extern OSL_VIRTUALFILENAME* osl_virtualFileList;
	extern int osl_virtualFileListNumber;
	extern int osl_defaultVirtualFileSource;
	extern const char* osl_tempFileName;
	extern void* oslReadEntireFileToMemory(VIRTUAL_FILE* f, int* size);
	extern int vfsMemOpen(void* param1, int param2, int type, int mode, VIRTUAL_FILE* f);
	extern int vfsMemClose(VIRTUAL_FILE* f);
	extern int vfsMemWrite(const void* ptr, size_t size, size_t n, VIRTUAL_FILE* f);
	extern int vfsMemRead(void* ptr, size_t size, size_t n, VIRTUAL_FILE* f);
	extern int vfsMemGetc(VIRTUAL_FILE* f);
	extern int vfsMemPutc(int caractere, VIRTUAL_FILE* f);
	extern char* vfsMemGets(char* str, int maxLen, VIRTUAL_FILE* f);
	extern void vfsMemPuts(const char* s, VIRTUAL_FILE* f);
	extern void vfsMemSeek(VIRTUAL_FILE* f, int offset, int whence);
	extern int vfsMemTell(VIRTUAL_FILE* f);
	extern int vfsMemEof(VIRTUAL_FILE* f);
	extern VIRTUAL_FILE_SOURCE vfsMemory;
	int oslInitVfsFile();
	extern inline void oslSetDefaultVirtualFileSource(int source)
	{
		osl_defaultVirtualFileSource = source;
	}
	extern int VF_MEMORY;
	extern int VF_FILE;
	extern inline char* oslGetTempFileName()
	{
		return (char*)osl_tempFileName;
	}
	extern void oslSetTempFileData(void* data, int size, int* type);
	extern int oslAddVirtualFileList(OSL_VIRTUALFILENAME* vfl, int numberOfEntries);
	extern void oslRemoveVirtualFileList(OSL_VIRTUALFILENAME* vfl, int numberOfEntries);

#ifdef __cplusplus
}
#endif
#endif
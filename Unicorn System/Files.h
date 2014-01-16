/*

	Pyramid DX8 System - File System (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier & Nico Vrouwe

*/
#ifndef __FILES_H__
#define __FILES_H__

#define LIBFILE_SIZE 25

#ifdef UNI_LOAD_JPEG
dword * uniLoadImage( char *szFile, int &nWidth, int &nHeight );
dword * uniLoadImage(char *szFile);
#endif

#ifdef UNI_LOAD_RAW
dword * uniLoadRAW(char * filename, int size);
#endif

#ifdef UNI_FILE_SYS

class UniFile {
public:
	char	*data;
	dword   size;
	dword   position;
	char    status;

public:
	UniFile(char * filename);
	~UniFile();

public:
	int		getData(void *data, dword aantal);
	char	getChar();
	short	getShort();
	float	getFloat();
	int		getInt();
	int		getPosition();
	int		getSize();
	void	setPosition(dword pos);
};
	
struct UniPAKFile {
	char    filename[13];
	dword   pos;
	dword   paksize;
	dword   unpaksize;
};


#ifdef UNI_PAK_SYS
void uniLoadPAK(char * pakfile);
void aP_depack_asm_fast(unsigned char * src, unsigned char * dest);
#endif

#endif

#endif

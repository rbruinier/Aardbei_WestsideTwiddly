/*

	Pyramid DX8 System - File System (source file)


	(c) 2001, Robert Jan Bruinier & Nico Vrouwe

*/

#include "Unicorn.h"


#ifdef UNI_LOAD_JPEG

#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <ocidl.h>
#include <olectl.h>

// loads jpg/gif/bmp/ico/emf/wmf (sick indeed :)
dword * uniLoadImage(char *szFile) {
	int x, y;
	return uniLoadImage(szFile, x, y);
}

// loads jpg/gif/bmp/ico/emf/wmf (sick indeed :) (1k upx --best)
dword * uniLoadImage(char *szFile, int &nWidth, int &nHeight) {
	static int antialias = 4;

	bool useAntialias = false;
	if( strstr(szFile,".wmf") || strstr(szFile,".emf") || strstr(szFile,".WMF") || strstr(szFile,".EMF") ) useAntialias = true;

	static HDC hdcnull = GetDC(NULL);
	static LPPICTURE gpPicture;
	
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		uniQuit("Image not found");

	DWORD dwFileSize = GetFileSize(hFile, NULL);

	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);

	pvData = GlobalLock(hGlobal);

	DWORD dwBytesRead = 0;
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	LPSTREAM pstm = NULL;
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);

	if (gpPicture) gpPicture->Release();

	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
	pstm->Release();

	long hmWidth;
	long hmHeight;
	gpPicture->get_Width(&hmWidth);
	gpPicture->get_Height(&hmHeight);

	HDC hdc = CreateCompatibleDC( hdcnull );

	#define HIMETRIC_INCH 2540

	int scale = 1;
	if( useAntialias ) scale = antialias;
	nWidth	= scale * MulDiv( hmWidth, GetDeviceCaps(hdc,LOGPIXELSX), 2540 );
	nHeight	= scale * MulDiv( hmHeight, GetDeviceCaps(hdc,LOGPIXELSY), 2540 );


	HBITMAP bmp = CreateCompatibleBitmap( hdcnull, nWidth, nHeight);
	SelectObject(hdc, bmp);

	// zeer vuig gefixt :)
	int pixel1 = (scale * 2540) / GetDeviceCaps(hdc, LOGPIXELSY);
	RECT rc = {0,0,nWidth,nHeight};
	gpPicture->Render( hdc, 0, -1, nWidth, nHeight+1, 0, 0, hmWidth, hmHeight+pixel1, NULL);

	BITMAPINFOHEADER lpbi;
	ZeroMemory(&lpbi,sizeof(BITMAPINFOHEADER));
	lpbi.biSize = sizeof(BITMAPINFOHEADER);
	lpbi.biWidth = nWidth;
	lpbi.biHeight = nHeight;
	lpbi.biPlanes = 1;
	lpbi.biBitCount = 32;
	lpbi.biCompression = BI_RGB;

	dword *ret = new dword[ nWidth * nHeight ];
	for( int i=0; i<nHeight; i++ )
		GetDIBits( hdc, bmp, i, 1, &ret[i*nWidth], (LPBITMAPINFO)&lpbi, DIB_RGB_COLORS );

	for (i = 0; i < nWidth * nHeight; i++)
		ret[i] |= (DWORD) 255 << 24;

//	for (i = i; i < nWidth * nHeight; i++)
//		ret[i] |= 128 << 24;

	if( useAntialias )
	{
		dword *src = ret;
		int w = nWidth/scale;
		int h = nHeight/scale;
		ret = new dword[ w*h ];
		for( int y=0; y<h; y++ )
		{
			int tsrc = y*scale*nWidth;
			int tdest = y*w;
			for( int x=0; x<w; x++ )
			{
				int ttsrc = tsrc;
				int c,r,g,b;
				c=r=g=b=0;
				for( int yy=0; yy<scale; yy++ )
				{
					for( int xx=0; xx<scale; xx++ )
					{
						c = src[ttsrc];
						r += (c>>16)&255;
						g += (c>>8)&255;
						b += c&255;
						ttsrc++;
					}
					ttsrc += (nWidth - scale);
				}
				r /= (scale*scale);
				g /= (scale*scale);
				b /= (scale*scale);
				ret[tdest] = (r<<16) | (g<<8) | b;
				tsrc+=scale;
				tdest++;
			}
		}
		delete[] src;
		nWidth = w;
		nHeight = h;
	}

	return ret;
}

#endif

#ifdef UNI_LOAD_RAW

dword * uniLoadRAW(char * filename, int size) {
	dword * dest = new dword[size];
	UniFile * f = new UniFile(filename);
	for (int i = 0; i < size; i++) {
		byte r,g,b = 0;
		r = f->getChar();
		g = f->getChar();
		b = f->getChar();
		dest[i] = (r << 16) | (g << 8) | b;
	}
	delete f;
	return dest;
}

#endif

#ifdef UNI_FILE_SYS

#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <ocidl.h>
#include <olectl.h>

UniPAKFile libfile[1024];
dword	   pakNumFiles  = 0;
dword	   pakTotalSize = 0;
char	   pakFileName[13];

UniFile::UniFile(char * filename) {
#ifdef UNI_LOGHTML
	uniLog("   Loading file %s\n", filename);
#endif
	int filenr = -1;
	dword h;
	data	 = NULL;
	position = 0;
	status	 = 1;
	size	 = 0;

	if ((h = open(filename, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) != -1) {
		size = filelength(h);
		data = new char[size];
		lseek(h, 0, SEEK_SET);
		read(h, data, size);
		close(h);
#ifdef UNI_PAK_SYS
	} else {
		for(dword i=0; i<pakNumFiles; i++)
			if(_stricmp(filename,libfile[i].filename)==0) 
				filenr=i;
		if(filenr != -1) {
			int h;
			if ((h=open(pakFileName, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD))!=-1) {
				int fsize=filelength(h);
				size=libfile[filenr].unpaksize;
				status=1;
				data= new char[ libfile[filenr].unpaksize ];
				_lseek(h,fsize-8-pakNumFiles*LIBFILE_SIZE-libfile[filenr].pos,SEEK_SET);
				char* unpack=new char[libfile[filenr].paksize];
				read(h, unpack, libfile[filenr].paksize);
				aP_depack_asm_fast((byte*)unpack,(byte*)data);
				free(unpack);
				close(h);
			}
		} else
		uniQuit("File not found");
#endif
	} 
}

UniFile::~UniFile() {
	delete data;
}

int UniFile::getData(void *buffer, dword aantal) {
    dword lees = aantal;
    if (status != 1) 
		uniQuit("File not opened!");
    if (position + lees > size)
        lees = size - position;
    memcpy(buffer, data + position, lees);
    position += lees;
    return lees;
}

char UniFile::getChar() {
	char temp;
	getData(&temp, sizeof(temp));
	return temp;
}

short UniFile::getShort() {
	short temp;
	getData(&temp, sizeof(temp));
	return temp;
}

float UniFile::getFloat() {
	float temp;
	getData(&temp, sizeof(temp));
	return temp;
}

int UniFile::getInt() {
	int temp;
	getData(&temp, sizeof(temp));
	return temp;
}

int UniFile::getPosition() {
	return position;
}

int UniFile::getSize() {
	return size;
}

void UniFile::setPosition(dword pos) {
	position = pos;
}

#ifdef UNI_PAK_SYS

void uniLoadPAK(char * pakfile) {
#ifdef UNI_LOGHTML
	uniLog("   Loading pakfile %s\n", pakfile);
#endif

	char    snqcomp[6]={0,0,0,0,0,0};
	char    snqlib[6]={'s','n','q','l','i','b'};

	dword fsize;
    int h;
    if ((h=open(pakfile, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD))!=-1) {
        fsize=filelength(h);
        _lseek(h,fsize-6,SEEK_SET);
        read(h,snqcomp,6);
        if(stricmp(snqlib,snqcomp)==-1) {
			uniQuit("Not a valid PAK file");
        }
        _lseek(h,fsize-8,SEEK_SET);
        read(h,&pakNumFiles,2);
        _lseek(h,fsize-8-pakNumFiles*LIBFILE_SIZE,SEEK_SET);
				for(dword k=0; k<pakNumFiles; k++)
				{
					read(h,libfile[k].filename,13);
					read(h,&libfile[k].pos,4);
					read(h,&libfile[k].paksize,4);
					read(h,&libfile[k].unpaksize,4);
				}
        close(h);
    }
	else
		uniQuit("Pak file not found");
    strcpy((char*)pakFileName,pakfile);
}

void aP_depack_asm_fast(unsigned char * src, unsigned char * dest) {
	__asm
	{
		mov			esi, src
		mov			edi, dest
		push    ebp                 
		cld                         
		mov     dh, 1               
	nexttag:                          
		dec     dh                  
		jnz     stillbitsleftA      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleftA:                   
		add     dl, dl              
		jc      not_literal         
		movsb                       
		jmp     short nexttag       
	not_literal:                      
		dec     dh                  
		jnz     stillbitsleftB      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleftB:                   
		add     dl, dl              
		jnc     codepair            
		dec     dh                  
		jnz     stillbitsleftC      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleftC:                   
		add     dl, dl              
		jnc     shortmatch          
		xor     eax, eax            
		dec     dh                  
		jnz     stillbitsleft_eaxA    
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eaxA:               
		add     dl, dl              
		adc     eax, eax            
		dec     dh                  
		jnz     stillbitsleft_eaxB     
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eaxB:               
		add     dl, dl              
		adc     eax, eax            
		dec     dh                  
		jnz     stillbitsleft_eaxC     
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eaxC:               
		add     dl, dl              
		adc     eax, eax            
		dec     dh                  
		jnz     stillbitsleft_eaxD     
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eaxD:               
		add     dl, dl              
		adc     eax, eax            
		neg     eax                 
		jz      thewrite            
		mov     al, [edi+eax]       
	thewrite:                         
		stosb                       
		jmp     short nexttag       
	codepair:                         
		jmp     short getgamma_eax     
	getgamma_eax_ret:                 
		sub     eax, 2              
		jnz     normalcodepair      
		call    getgamma            
		push    esi                 
		mov     esi, edi            
		sub     esi, ebp            
		rep     movsb               
		pop     esi                 
		jmp     short nexttag       
	normalcodepair:                   
		dec     eax                 
		shl     eax, 8              
		lodsb                       
		mov     ebx, eax            
		call    getgamma            
		cmp     ebx, 127            
		ja      domatch3            
		add     ecx, 2              
	domatch3:                         
		mov     ebp, ebx            
		push    esi                 
		mov     esi, edi            
		sub     esi, ebx            
		rep     movsb               
		pop     esi                 
		jmp     short nexttag       
	shortmatch:                       
		xor     eax, eax            
		lodsb                       
		xor     ecx, ecx
		shr			al,  1
		jz      donedepacking       
		adc     ecx, 2              
		mov     ebx, eax            
		push    esi                 
		mov     esi, edi            
		sub     esi, ebx            
		rep     movsb               
		pop     esi                 
		jmp     short nexttag       
	getgamma_eax:                     
		mov     eax, 1              
	getgammaloop_eax:                 
		dec     dh                  
		jnz     stillbitsleft_eax   
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eax:                
		add     dl, dl              
		jnc     getgammabit_eax     
		dec     dh                  
		jnz     stillbitsleft_eax2     
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eax2:               
		add     dl, dl              
		adc     eax, eax            
		jmp     short getgammaloop_eax  
	getgammabit_eax:                  
		dec     dh                  
		jnz     stillbitsleft_eax4      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft_eax4:               
		add     dl, dl              
		adc     eax, eax            
		jmp     short getgamma_eax_ret  
	getgamma:                         
		mov     ecx, 1              
	getgammaloop:                     
		dec     dh                  
		jnz     stillbitsleft2      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft2:                   
		add     dl, dl              
		jnc     getgammabit         
		dec     dh                  
		jnz     stillbitsleft3      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft3:                   
		add     dl, dl              
		adc     ecx, ecx            
		jmp     short getgammaloop      
	getgammabit:                      
		dec     dh                  
		jnz     stillbitsleft4      
		mov     dl, [esi]           
		inc     esi                 
		mov     dh, 8               
	stillbitsleft4:                   
		add     dl, dl              
		adc     ecx, ecx            
		ret                         
	donedepacking:                    
		pop     ebp                 
	}
}

#endif

#endif
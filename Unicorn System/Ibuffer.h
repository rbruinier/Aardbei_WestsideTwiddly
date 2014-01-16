/*

	Pyramid DX8 System - Index Buffer Code


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/


#ifndef __IBUFFER_H__
#define __IBUFFER_H__

class UniIBuffer {
private:
	IDirect3DIndexBuffer8 *d3d8IBuffer;

public:
	bool		dynamic;
	dword		*data;
	D3DFORMAT	type;
	dword		sizeTotal;
	dword		sizeFace;
	dword		usage;
	D3DPOOL		pool;

public:
	UniIBuffer(dword * data, dword sizeFace, dword sizeTotal, D3DFORMAT type, dword usage);
	~UniIBuffer();

public:
	void set();
	void fill(dword *data);
	void release();
	void recreate();

};

struct IBufferContainer {
	UniIBuffer			*uniIBuffer;			
	IBufferContainer	*next;
};

void uniIBufferDestroy();
void uniIBufferReleaseDynamics();
void uniIBufferRecreateDynamics();

#endif
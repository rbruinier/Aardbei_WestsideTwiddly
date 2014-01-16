/*

	Pyramid DX8 System - Vertex Buffer Code


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __VBUFFER_H__
#define __VBUFFER_H__

class UniVBuffer {
private:
	IDirect3DVertexBuffer8 *d3d8VBuffer;

public:
	dword	*data;
	dword	type;
	dword	sizeTotal;
	dword	sizeVertex;
	dword	usage;
	D3DPOOL	pool;
	bool	dynamic;

public:
	UniVBuffer(dword * data, dword sizeVertex, dword sizeTotal, dword type, dword usage);
	~UniVBuffer();

public:
	void set();
	void fill(dword *data);
	void release();
	void recreate();
};

struct VBufferContainer {
	UniVBuffer			*uniVBuffer;			
	VBufferContainer	*next;
};

void uniVBufferDestroy();
void uniVBufferReleaseDynamics();
void uniVBufferRecreateDynamics();

#endif

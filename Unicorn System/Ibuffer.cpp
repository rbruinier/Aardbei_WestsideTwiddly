/*

	Pyramid DX8 System - Vertex Buffer Code


	(c) 2001, Robert Jan Bruinier

*/

#include "Unicorn.h"

IBufferContainer * ibufferList = NULL;

#define FOREACHIBUFFER(ptc) for (IBufferContainer* ptc = ibufferList; NULL != ptc; ptc=ptc->next)

UniIBuffer::UniIBuffer(dword * data, dword sizeFace, dword sizeTotal, D3DFORMAT type, dword usage) {
	this->sizeFace = sizeFace;
	this->sizeTotal = sizeTotal;
	this->data = data;
	this->type = type;
	this->usage = usage;

	this->d3d8IBuffer = NULL;

	pool = D3DPOOL_MANAGED;

	if (usage & D3DUSAGE_DYNAMIC) {
		this->dynamic = true;
		pool = D3DPOOL_DEFAULT;
	} else {
		this->dynamic = false;
	}

	if (D3D_OK != uniSystem->d3d8Device->CreateIndexBuffer(sizeTotal, usage, type,
														 pool, &d3d8IBuffer))
		uniQuit("Failed to create Index Buffer");

	if (data != NULL)
		fill(data);

	IBufferContainer * actIB = new IBufferContainer();

	actIB->next = ibufferList;
	actIB->uniIBuffer = this;
	ibufferList = actIB;

}

UniIBuffer::~UniIBuffer() {
	release();
}

void UniIBuffer::release() {
	if (d3d8IBuffer != NULL)
		d3d8IBuffer->Release();
}

void UniIBuffer::recreate() {
	if (D3D_OK != uniSystem->d3d8Device->CreateIndexBuffer(sizeTotal, usage, type,
														 pool, &d3d8IBuffer))
		uniQuit("Failed to create Index Buffer");
}


void UniIBuffer::fill(dword *data) {
	this->data = data;

	dword flags = D3DLOCK_NOSYSLOCK;

	if (dynamic)
		flags |= D3DLOCK_DISCARD;

	VOID* pVertices;
	if (D3D_OK != d3d8IBuffer->Lock(0, 0, (BYTE**)&pVertices, flags))
		uniQuit("Failed to lock Vertex Buffer");

	memcpy(pVertices, data, sizeTotal);

	if (D3D_OK != this->d3d8IBuffer->Unlock())
		uniQuit("Failed to unlock Vertex Buffer");

}

void UniIBuffer::set() {
	uniSystem->d3d8Device->SetIndices(d3d8IBuffer, 0);
}

void uniIBufferDestroy() {
	if (ibufferList != NULL) {
		FOREACHIBUFFER (ptcIBuffer) {
			if (ptcIBuffer->uniIBuffer != NULL)
				delete ptcIBuffer->uniIBuffer;
		}
		do {
			IBufferContainer * next_ptr = ibufferList->next;
			delete ibufferList;
			ibufferList = next_ptr;
		} while (ibufferList != NULL);
	}
}

void uniIBufferReleaseDynamics() {
	if (ibufferList != NULL) {
		FOREACHIBUFFER (ptcIBuffer) {
			if (ptcIBuffer->uniIBuffer->pool == D3DPOOL_DEFAULT)
				ptcIBuffer->uniIBuffer->release();
		}
	}
}

void uniIBufferRecreateDynamics() {
	if (ibufferList != NULL) {
		FOREACHIBUFFER (ptcIBuffer) {
			if (ptcIBuffer->uniIBuffer->pool == D3DPOOL_DEFAULT)
				ptcIBuffer->uniIBuffer->recreate();
		}
	}
}
/*

	Pyramid DX8 System - Vertex Buffer Code


	(c) 2001, Robert Jan Bruinier

*/


#include "Unicorn.h"

VBufferContainer * vbufferList = NULL;

#define FOREACHVBUFFER(ptc) for (VBufferContainer* ptc = vbufferList; NULL != ptc; ptc=ptc->next)

UniVBuffer::UniVBuffer(dword * data, dword sizeVertex, dword sizeTotal, dword type, dword usage) {
	this->sizeTotal  = sizeTotal;
	this->sizeVertex = sizeVertex;
	this->data       = data;
	this->type       = type;
	this->usage      = usage;

	this->d3d8VBuffer = NULL;

	pool = D3DPOOL_MANAGED;

	
	if (usage & D3DUSAGE_DYNAMIC) {
		this->dynamic = true;
		pool = D3DPOOL_DEFAULT;
	} else {
		this->dynamic = false;
	}

	if (usage & D3DUSAGE_POINTS) {
		this->usage |= D3DUSAGE_SOFTWAREPROCESSING;
	}

	if (D3D_OK != uniSystem->d3d8Device->CreateVertexBuffer(sizeTotal, usage, type, pool, 
														    &d3d8VBuffer))
		uniQuit("Failed to create Vertex Buffer");

	if (data != NULL)
		fill(data);

	VBufferContainer * actVB = new VBufferContainer();

	actVB->next = vbufferList;
	actVB->uniVBuffer = this;
	vbufferList = actVB;
}

UniVBuffer::~UniVBuffer() {
	release();
}

void UniVBuffer::release() {
	if (d3d8VBuffer != NULL)
		d3d8VBuffer->Release();
}

void UniVBuffer::recreate() {
	if (D3D_OK != uniSystem->d3d8Device->CreateVertexBuffer(sizeTotal, usage, type, pool, 
														    &d3d8VBuffer))
		uniQuit("Failed to create Vertex Buffer");
}

void UniVBuffer::fill(dword *data) {
	VOID* pVertices;

	dword flags = D3DLOCK_NOSYSLOCK;

	if (dynamic)
		flags |= D3DLOCK_DISCARD;

	if (D3D_OK != d3d8VBuffer->Lock(0, 0, (BYTE**)&pVertices, flags))
		uniQuit("Failed to lock Vertex Buffer");

	memcpy(pVertices, data, sizeTotal);

	if (D3D_OK != this->d3d8VBuffer->Unlock())
		uniQuit("Failed to unlock Vertex Buffer");

}

void UniVBuffer::set() {
    uniSystem->d3d8Device->SetStreamSource(0, this->d3d8VBuffer, this->sizeVertex);
    uniSystem->d3d8Device->SetVertexShader(this->type);
}

void uniVBufferDestroy() {
	if (vbufferList != NULL) {
		FOREACHVBUFFER (ptcVBuffer) {
			if (ptcVBuffer->uniVBuffer != NULL)
				delete ptcVBuffer->uniVBuffer;
		}
		do {
			VBufferContainer * next_ptr = vbufferList->next;
			delete vbufferList;
			vbufferList = next_ptr;
		} while (vbufferList != NULL);
	}
}

void uniVBufferReleaseDynamics() {
	if (vbufferList != NULL) {
		FOREACHVBUFFER (ptcVBuffer) {
			if (ptcVBuffer->uniVBuffer->pool == D3DPOOL_DEFAULT)
				ptcVBuffer->uniVBuffer->release();
		}
	}
}

void uniVBufferRecreateDynamics() {
	if (vbufferList != NULL) {
		FOREACHVBUFFER (ptcVBuffer) {
			if (ptcVBuffer->uniVBuffer->pool == D3DPOOL_DEFAULT)
				ptcVBuffer->uniVBuffer->recreate();
		}
	}
}
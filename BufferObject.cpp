#include "stdafx.h"
#include "BufferObject.h"

MBufferObject::MBufferObject()
{
	pUVBuffer = NULL;
	BufferOffset = 0;
	TextureId = 0;
	DirectionRight = true;
}

MBufferObject::~MBufferObject()
{
	pUVBuffer = NULL;
	BufferOffset = 0;
	TextureId = 0;
}

void MBufferObject::SetDirectionRight(bool Right)
{
	if(Right != DirectionRight) FlipTextureByX();
	DirectionRight = Right;
}

bool MBufferObject::Create(MUVBuffer* inpUVBuffer, float pX0, float pY0, float pW, float pH, float tX0, float tY0, float tW, float tH, unsigned int inTextureId)
{
	if(!inpUVBuffer)
	{
		LogFile<<"Buffer object: NULL buffer"<<endl;
		return false;
	}
	if(!inTextureId)
	{
		LogFile<<"Buffer object: Null texture"<<endl;
		return false;
	}
	if(pW < 0.001 && pH > -0.001) return false;
	if(tW < 0.001 && tH > -0.001) return false;
	pUVBuffer = inpUVBuffer;
	TextureId = inTextureId;
	
	stQuad Vertex;
	stQuad UV;
	SetQuad(Vertex, pX0, pY0, pW, pH);
	SetQuad(UV, tX0, tY0, tW, tH);
	SetUVQuad(UVQuad, Vertex, UV);
	
//	SetBox(Box, pX0, pY0, pX0 + pW - 4, pY0 + pH - 4); //can add edge here
	
	BufferOffset = pUVBuffer->AddUVQuad(UVQuad, false);
	if(BufferOffset == -1) return false;
	
	LogFile<<"Buffer object create: Offset: "<<BufferOffset<<" Data: x: "<<pX0<<" y: "<<pY0<<" w: "<<pW<<" h: "<<pH<<" Buffer id: "<<pUVBuffer->GetId()<<endl;

	return true;
}

bool MBufferObject::Create(float pX0, float pY0, float pW, float pH, float tX0, float tY0, float tW, float tH)
{
	if(!pUVBuffer) 
	{
		LogFile<<"Buffer object: NULL buffer"<<endl;
		return false;
	}
	if(pW < 0.001 && pH > -0.001) return false;
	if(tW < 0.001 && tH > -0.001) return false;
	
	stQuad Vertex;
	stQuad UV;
	SetQuad(Vertex, pX0, pY0, pW, pH);
	SetQuad(UV, tX0, tY0, tW, tH);
	SetUVQuad(UVQuad, Vertex, UV);
	
//	SetBox(Box, pX0, pY0, pX0 + pW - 4, pY0 + pH -4); //can add edge here
	
	BufferOffset = pUVBuffer->AddUVQuad(UVQuad, false);
	if(BufferOffset == -1) return false;
	
	LogFile<<"Buffer object create: Offset: "<<BufferOffset<<" Data: x: "<<pX0<<" y: "<<pY0<<" w: "<<pW<<" h: "<<pH<<" Buffer id: "<<pUVBuffer->GetId()<<endl;
	
	return true;
}

bool MBufferObject::SetBuffer(MUVBuffer* inpUVBuffer)
{
	if(!inpUVBuffer)
	{
		LogFile<<"Buffer object: NULL buffer"<<endl;
		return false;
	}
	pUVBuffer = inpUVBuffer;
	
	return true;
}

bool MBufferObject::SetTexture(unsigned int inTextureId)
{
	if(!inTextureId)
	{
		LogFile<<"Buffer object: Null texture"<<endl;
		return false;
	}
	TextureId = inTextureId;
	
	return true;
}

bool MBufferObject::SetVertex(stQuad Vertex)
{
	if(!pUVBuffer || !pUVBuffer->GetId()) return false;
	SetUVQuad(UVQuad, Vertex, GetUVQuad(UVQuad));
	
	return pUVBuffer->UpdateUVQuad(BufferOffset, UVQuad, true);
}

bool MBufferObject::SetUV(stQuad UV)
{
	if(!pUVBuffer || !pUVBuffer->GetId()) return false;
	SetUVQuad(UVQuad, GetVertexQuad(UVQuad), UV);
	
	return pUVBuffer->UpdateUVQuad(BufferOffset, UVQuad, true);
}

bool MBufferObject::Set(stUVQuad inUVQuad)
{
	if(!pUVBuffer || !pUVBuffer->GetId()) return false;
	UVQuad = inUVQuad;
	
	return pUVBuffer->UpdateUVQuad(BufferOffset, inUVQuad, true);
}

bool MBufferObject::Update()
{
	if(!pUVBuffer || !pUVBuffer->GetId()) return false;
	
	return pUVBuffer->UpdateUVQuad(BufferOffset, UVQuad, true);
}

bool MBufferObject::FlipTextureByX()
{
	if(!pUVBuffer) return false;
	
	swap(UVQuad.p[0][1], UVQuad.p[3][1]);
	swap(UVQuad.p[1][1], UVQuad.p[2][1]);
	
	return pUVBuffer->UpdateUVQuad(BufferOffset, UVQuad, true);
}

bool MBufferObject::FlipTextureByY()
{
	if(!pUVBuffer) return false;
	
	swap(UVQuad.p[0][1], UVQuad.p[1][1]);
	swap(UVQuad.p[2][1], UVQuad.p[3][1]);
	
	return pUVBuffer->UpdateUVQuad(BufferOffset, UVQuad, true);
}

void MBufferObject::Draw()
{
	if(!pUVBuffer || !pUVBuffer->GetId() || !TextureId) return;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	//vertex
	glBindBuffer(GL_ARRAY_BUFFER, pUVBuffer->GetId());
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//uv
	glBindBuffer(GL_ARRAY_BUFFER, pUVBuffer->GetId());
	glVertexAttribPointer(SHR_LAYOUT_UV, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)8);
	//draw
	glDrawArrays(GL_QUADS, BufferOffset * 4, 4);
}


stQuad MBufferObject::GetVertex()
{
	return GetVertexQuad(UVQuad);
}

stQuad MBufferObject::GetUV()
{
	return GetUVQuad(UVQuad);
}

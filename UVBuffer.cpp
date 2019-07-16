#include "stdafx.h"
#include "UVBuffer.h"

MUVBuffer::MUVBuffer()
{
	Id = 0;
}

MUVBuffer::~MUVBuffer()
{
}

bool MUVBuffer::Initialize(GLenum inType)
{
	if(inType != GL_STATIC_DRAW && inType != GL_STREAM_DRAW)
	{
		LogFile<<"UVBuffer: Given buffer type is not supported"<<endl;
		return false;
	}
	Type = inType;
	
	//generate buffer
	glGenBuffers(1, &Id);
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		LogFile<<"UVBuffer: "<<(char*)gluErrorString(error)<<" "<<error<<endl;
		return false;
	}
	LogFile<<"UVBuffer: Empty buffer created"<<endl;
	
	return true;
}

int MUVBuffer::AddUVQuad(stUVQuad inUVQuad, bool Force)
{
	int Number = UVQuad.size();
	UVQuad.push_back(inUVQuad);
	if(Force) 
	{
		if(!UpdateAll()) return -1;
	}
	
	return Number;
}

bool MUVBuffer::UpdateUVQuad(unsigned int Number, stUVQuad inUVQuad, bool Force)
{
	if(Number >= UVQuad.size())
	{
		LogFile<<"UVBuffer: UVQuad number more that array size"<<endl;
		return false;
	}
	
	UVQuad[Number] = inUVQuad;
	
	if(Force)
	{
		glBindBuffer(GL_ARRAY_BUFFER, Id);
		glBufferSubData(GL_ARRAY_BUFFER, Number * sizeof(stUVQuad), sizeof(stUVQuad), &UVQuad[Number].p[0][0]);
	}
	return true;
}

bool MUVBuffer::UpdateAll()
{
	glBindBuffer(GL_ARRAY_BUFFER, Id);
	glBufferData(GL_ARRAY_BUFFER, UVQuad.size() * sizeof(stUVQuad), NULL, Type);
	glBufferData(GL_ARRAY_BUFFER, UVQuad.size() * sizeof(stUVQuad), &UVQuad[0].p[0][0], Type);
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		LogFile<<"UVBuffer: "<<(char*)gluErrorString(error)<<" "<<error<<endl;
		return false;
	}
	
	return true;
}
	
void MUVBuffer::Close()
{
	UVQuad.clear();
	if(Id) glDeleteBuffers(1, &Id);
	LogFile<<"UVBuffer: Closed. Id: "<<Id<<endl;
}

unsigned int MUVBuffer::GetId()
{
	return Id;
}

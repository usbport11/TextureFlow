#ifndef bufferobjectH
#define bufferobjectH

#include "UVBuffer.h"

class MBufferObject
{
private:
	unsigned int TextureId;
	bool DirectionRight;
protected:
	MUVBuffer* pUVBuffer;
	unsigned int BufferOffset;
	stUVQuad UVQuad;
public:
	MBufferObject();
	~MBufferObject();
	bool Create(MUVBuffer* inpUVBuffer, float pX0, float pY0, float pW, float pH, float tX0, float tY0, float tW, float tH, unsigned int inTextureId);
	
	bool SetBuffer(MUVBuffer* inpUVBuffer);
	bool Create(float pX0, float pY0, float pW, float pH, float tX0, float tY0, float tW, float tH);
	bool SetTexture(unsigned int inTextureId);
	bool SetVertex(stQuad Vertex);
	bool SetUV(stQuad UV);
	bool Set(stUVQuad inUVQuad);
	
	bool Update();
	
	stQuad GetVertex();
	stQuad GetUV();
	
	bool FlipTextureByX();
	bool FlipTextureByY();
	void SetDirectionRight(bool Right);
	
	void Draw();
};

#endif

#include "stdafx.h"
#include "FlowObject.h"

map<UINT_PTR, MFlowObject*> TimerMap;

void CALLBACK MFlowObject::TimerFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	MFlowObject* pFlowObject = TimerMap[idEvent];
	if(idEvent == pFlowObject->Timer.Id)
	{
		if(pFlowObject->Timer.Enabled)
		{
			pFlowObject->Size();
		}
	}
}

MFlowObject::MFlowObject()
{
	Timer.Id = 0;
	Timer.Enabled = false;
	
	FlowVelocity = glm::vec2(0, 0);
	SizeVelocity = glm::vec2(0, 0);
	FlowLimit[0] = glm::vec2(0, 1);
	FlowLimit[1] = glm::vec2(0, 1);
	SizeLimit = glm::vec2(0, 0);
	ReturnValue[0] = glm::vec2(0, 1);
	ReturnValue[1] = glm::vec2(0, 1);
	TextureSize = glm::vec2(0, 0);
}

MFlowObject::~MFlowObject()
{
}

bool MFlowObject::Create(MUVBuffer* inpUVBuffer, float pX0, float pY0, float pW, float pH, float tX0, float tY0, float tW, float tH, unsigned int inTextureId)
{
	ReturnValue[0] = glm::vec2(tX0, tW);
	ReturnValue[1] = glm::vec2(tY0, tH);
	
	return MBufferObject::Create(inpUVBuffer, pX0, pY0, pW, pH, tX0, tY0, tW, tH, inTextureId);
}

bool MFlowObject::SetSizeTimer(HWND hWnd, unsigned int TimerValue, bool Enable)
{
	if(!Timer.Set(hWnd, 0, TimerValue, TimerFunction)) return false;
	Timer.Enabled = Enable;
	TimerMap[Timer.Id] = this;
	
	return true;
}

bool MFlowObject::StartSizeTimer()
{
	return Timer.Start();
}

void MFlowObject::StopSizeTimer()
{
	Timer.Stop();
}

bool MFlowObject::UpdateSizeTimer(unsigned int TimerValue, bool Enable)
{
	Timer.Value = TimerValue;
	Timer.Enabled = Enable;
	return true;
}

bool MFlowObject::SetFlowVelocity(float VelocityX, float VelocityY)
{
	if(TextureSize.x <= 0 || TextureSize.y <= 0 )
	{
		LogFile<<"FlowObject: Texture size is not set or wrong"<<endl;
		return false;
	}
	FlowVelocity = glm::vec2(VelocityX, VelocityY);
	//if flowvelocity < 0 uv coords must be at top of texture
	//if(FlowVelocity.x < 0) ReturnValue[0] = glm::vec2(TextureSize.x - ReturnValue[0][1], TextureSize.x);
	//if(FlowVelocity.y < 0) ReturnValue[1] = glm::vec2(TextureSize.y - ReturnValue[1][1], TextureSize.y);
	FlowLimit[0] = glm::vec2(0, 1); //always 0-1
	FlowLimit[1] = glm::vec2(0, 1); //always 0-1
	
	cout<<"Start ret: "<<ReturnValue[0][1]<<" "<<ReturnValue[1][1]<<endl;
	
	stQuad UV;
	UV = GetUVQuad(UVQuad);
	UV.p[3][1] = UV.p[0][1] = ReturnValue[1][0];
	UV.p[2][1] = UV.p[1][1] = ReturnValue[1][1];
	SetUVQuad(UVQuad, GetVertexQuad(UVQuad), UV);
	//do not update here - will be error in buffer
		
	return true;
}

void MFlowObject::SetSizeVelocity(float VelocityX, float VelocityY)
{
	SizeVelocity = glm::vec2(VelocityX, VelocityY);
}

bool MFlowObject::SetTextureSize(float Width, float Height)
{
	if(Width <= 0 || Height <= 0) 
	{
		LogFile<<"FlowObject: Texture width or height is wrong"<<endl;
		return false;
	} 
	TextureSize = glm::vec2(Width, Height);
	
	return true;
}

void MFlowObject::SetSizeLimit(float SizeLimitX, float SizeLimitY)
{
	SizeLimit = glm::vec2(SizeLimitX, SizeLimitY);
}

void MFlowObject::Flow()
{
	if(!FlowVelocity[0] && !FlowVelocity[1]) return;
	
	//change UV data and update buffer
	stQuad UV;
	UV = GetUVQuad(UVQuad);
	MoveQuad(UV, FlowVelocity[0], FlowVelocity[1]);
	//check
	if((FlowVelocity[0] > 0 && UV.p[0][0] >= FlowLimit[0][1]) || (FlowVelocity[0] < 0 && UV.p[0][0] <= -1))
	{
		UV.p[1][0] = UV.p[0][0] = ReturnValue[0][0];
		UV.p[3][0] = UV.p[2][0] = ReturnValue[0][1];
	}
	if((FlowVelocity[1] > 0 && UV.p[0][1] >= FlowLimit[1][1]) || (FlowVelocity[1] < 0 && UV.p[0][1] <= -1))
	{
		UV.p[3][1] = UV.p[0][1] = ReturnValue[1][0];
		UV.p[2][1] = UV.p[1][1] = ReturnValue[1][1];
	}
	//set and update in buffer
	SetUVQuad(UVQuad, GetVertexQuad(UVQuad), UV);
	Update();
}

void MFlowObject::Size()
{
	if(!SizeVelocity[0] && !SizeVelocity[1]) return;
	
	//change size
	Vertex = GetVertexQuad(UVQuad);
	UV = GetUVQuad(UVQuad);
	
	Vertex.p[1] += SizeVelocity;
	Vertex.p[2] += SizeVelocity;
	//calculate UV
	NewUV = glm::vec2(GetQuadSize(Vertex).x / TextureSize[0], GetQuadSize(Vertex).y / TextureSize[1]);
	UV.p[2][1] += fabs(NewUV.y - ReturnValue[1][1]);
	UV.p[1][1] = UV.p[2][1];
	ReturnValue[1][1] = NewUV.y;
		
	//set and update in buffer
	SetUVQuad(UVQuad, Vertex, UV);
	Update();
}

void MFlowObject::Close()
{
	if(Timer.Id) 
	{
		Timer.Enabled = false;
		Timer.Stop();
	}
}

#ifndef flowobjectH
#define flowobjectH

#include "BufferObject.h"

class MFlowObject: public MBufferObject
{
private:
	glm::vec2 FlowVelocity;
	glm::vec2 SizeVelocity;
	glm::vec2 FlowLimit[2];//flow limit by x(left right) and y(bottom top)
	glm::vec2 SizeLimit;//size limit (right, top)
	glm::vec2 ReturnValue[2];
	glm::vec2 TextureSize;
	
	//size
	stQuad Vertex;
	stQuad UV;
	glm::vec2 NewUV;
	
	//timer may be used (if size is increase/descrease by time) or not used
	stTimer Timer;
	static void CALLBACK TimerFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
public:
	MFlowObject();
	~MFlowObject();
	
	bool SetSizeTimer(HWND hWnd, unsigned int TimerValue, bool Enable);
	bool StartSizeTimer();
	void StopSizeTimer();
	bool UpdateSizeTimer(unsigned int TimerValue, bool Enable);
	
	bool Create(MUVBuffer* inpUVBuffer, float pX0, float pY0, float pW, float pH, float tX0, float tY0, float tW, float tH, unsigned int inTextureId);
	bool SetFlowVelocity(float VelocityX, float VelocityY);
	void SetSizeVelocity(float VelocityX, float VelocityY);
	bool SetTextureSize(float Width, float Height);
	void SetSizeLimit(float SizeLimitX, float SizeLimitY);
	void Flow();
	void Size();
	void Close();
};

#endif

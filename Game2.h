#ifndef game2H
#define game2H

#include "Window.h"
#include "TextureLoader.h"
#include "FlowObject.h"

class MGame2;

typedef void (MGame2::*VoidFunc) ();

class MGame2: public MWindow
{
private:
	VoidFunc pDrawFunc;
	VoidFunc pFrameFunc;
	
	//statuses
	bool Pause;
	bool* Key;
	
	//classes
	MTextureLoader TextureLoader;
	MUVBuffer UVBuffer;
	MFlowObject FlowObject;
	
	//textures
	stTexture* txFlow;
	unsigned int txFlow_cnt;
	
	//shader data
	GLuint VertexArrayId;
	GLuint ProgramId;
	GLuint MVPId;
	GLuint ColorId;
	GLuint Sampler2DId;
	GLuint UseTextureId;
	GLuint UseOffsetVertexId;
	GLuint UseOffsetUVId;
	
	glm::vec4 WhiteColor[4];
	
	//matrixes and view
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;
	glm::vec3 CameraPosition;
	glm::vec3 CameraDirection;
	
	//draw functions
	void DrawGame();
	void FrameGame();
	
	//overload virtual functions
	void OnDraw();
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnSize();
	
	//local
	void Start();
	void Stop();
	
public:
	MGame2();
	~MGame2();
	bool Initialize();
	void OnClose();
};

#endif

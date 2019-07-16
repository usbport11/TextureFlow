#include "stdafx.h"
#include "Game2.h"

MGame2::MGame2():MWindow()
{
	Pause = true;
    Key = new bool [256];
    memset(Key, 0, 256);
	
	pDrawFunc = NULL;
	pFrameFunc = NULL;

	txFlow = NULL;
}

MGame2::~MGame2()
{
	pDrawFunc = NULL;
	pFrameFunc = NULL;
}

bool MGame2::Initialize()
{	
    LogFile<<"Game2: Initialize"<<endl;
    
    if(!WindowInitialized)
    {
    	LogFile<<"Game2: Window was not initialized"<<endl;
    	return 0;
	}
	
	//randomize
    LogFile<<"Game2: randomize rand by time"<<endl; 
    srand(time(NULL));
    
    //prepare view
    CameraPosition = glm::vec3(0, 0, 1);
	CameraDirection = glm::vec3(0, 0, 0);
	//Projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
	Projection = glm::ortho(0.0f, 8.0f, 0.0f, 6.0f, -1.0f, 1.0f); //8 on 6 meters
	Model = glm::mat4(1.0f);
	View = glm::lookAt(CameraPosition, CameraDirection, glm::vec3(0,1,0));
    MVP = Projection * View * Model;
    
    //prepare vertex array - for all programs
    glGenVertexArrays(1, &VertexArrayId);
	glBindVertexArray(VertexArrayId);
	GLenum Error = glGetError();
	if(Error != GL_NO_ERROR)
	{
		LogFile<<"Game2: "<<(char*)gluErrorString(Error)<<" "<<Error<<endl;
		return false;
	}
	
	LogFile<<"Game2: Load shaders"<<endl;
	ProgramId = LoadShaders((char*)"shaders/main2.vertexshader.glsl", (char*)"shaders/main2.fragmentshader.glsl");
	if(!ProgramId) return false;
	MVPId = glGetUniformLocation(ProgramId, "MVP");
	if(MVPId == -1)
	{
		LogFile<<"Game2: Can't get MVP uniform"<<endl;
		return false;
	}
	ColorId = glGetUniformLocation(ProgramId, "sameColor");
	if(ColorId == -1)
	{
		LogFile<<"Game2: Can't get Color uniform"<<endl;
		return false;
	}
	Sampler2DId = glGetUniformLocation(ProgramId, "mainSampler");
	if(Sampler2DId == -1)
	{
		LogFile<<"Game2: Can't get texture uniform"<<endl;
		return false;
	}
	UseTextureId = glGetUniformLocation(ProgramId, "useSampler");
	if(UseTextureId == -1)
	{
		LogFile<<"Game2: Can't get usetexture uniform"<<endl;
		return false;
	}
	UseOffsetVertexId = glGetUniformLocation(ProgramId, "useOffsetVertex");
	if(UseOffsetVertexId == -1)
	{
		LogFile<<"Game2: Can't get useoffset vertex uniform"<<endl;
		return false;
	}
	UseOffsetUVId = glGetUniformLocation(ProgramId, "useOffsetUV");
	if(UseOffsetUVId == -1)
	{
		Error = glGetError();
		if(Error != GL_NO_ERROR) LogFile<<"Game2: "<<(char*)gluErrorString(Error)<<" "<<Error<<endl;
		LogFile<<"Game2: Can't get useoffset uv uniform"<<endl;
		return false;
	}
	
	//textures
	txFlow = TextureLoader.LoadTexture("textures/tex06.png", 1, 1, 0, txFlow_cnt, GL_NEAREST, GL_REPEAT);
	if(!txFlow) return false;
	
	//base data
	WhiteColor[3] = WhiteColor[2] = WhiteColor[1] = WhiteColor[0] = glm::vec4(1, 1, 1, 1);
	
	Error = glGetError();
	if(Error != GL_NO_ERROR)
	{
		LogFile<<"Game2: "<<(char*)gluErrorString(Error)<<" "<<Error<<endl;
		return false;
	}
	
	//UVBuffer
	if(!UVBuffer.Initialize(GL_STREAM_DRAW)) return false;
	//flow object
    if(!FlowObject.Create(&UVBuffer, 0, 0, 6, 2, 0, 0, 1, 1, txFlow->Id)) return false;
    if(!FlowObject.SetTextureSize(6.0f, 2.0f)) return false;//0.64,0.64
    if(!FlowObject.SetFlowVelocity(0, -0.005)) return false;
    FlowObject.SetSizeVelocity(0, 1);
    FlowObject.SetSizeLimit(0, 4);
	if(!UVBuffer.UpdateAll()) return false;
	
	//set draw and frame functions
    pDrawFunc = &MGame2::DrawGame;
    pFrameFunc = &MGame2::FrameGame;
    //turn pause off
    Pause = false;
    LogFile<<"Game2: Started"<<endl;
    
    return true;
}

void MGame2::Start()
{
    Pause = false;
}

void MGame2::Stop()
{
    Pause = true;
}

void MGame2::DrawGame()
{
	//math operations
	if(!Pause)
	{
		FlowObject.Flow();
	}
	
	//prepare shader, matrix, color
	glUseProgram(ProgramId);
	glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);
	glUniform4fv(ColorId, 1, &WhiteColor[0][0]);//set white color
	glUniform1i(UseOffsetVertexId, 0);//disable vertex offset
	glUniform1i(UseOffsetUVId, 0);//disable uv offset
	
	//flowobject
	glUniform1i(UseTextureId, 1);//enable texture
	glEnableVertexAttribArray(SHR_LAYOUT_VERTEX);
	glEnableVertexAttribArray(SHR_LAYOUT_UV);
	FlowObject.Draw();
	glDisableVertexAttribArray(SHR_LAYOUT_VERTEX);
	glDisableVertexAttribArray(SHR_LAYOUT_UV);
}

void MGame2::FrameGame()
{
}


void MGame2::OnDraw()
{
	if(pDrawFunc) ((*this).*(pDrawFunc))();
}


void MGame2::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	Key[wParam] = 1;
	if(pDrawFunc == &MGame2::DrawGame)
	{
		if(Key[VK_ESCAPE])
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
			return;
		}
		if(!Pause)
		{
			if(Key[VK_UP]) FlowObject.Size();
		}
	}
}

void MGame2::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	Key[wParam] = 0;
	if(pDrawFunc == &MGame2::DrawGame)
	{
	}
}

void MGame2::OnClose()
{
	Stop();
	LogFile<<"Game2: Stopped"<<endl;
	
	FlowObject.Close();
	UVBuffer.Close();
	LogFile<<"Game2: Objects free"<<endl;
	
	TextureLoader.DeleteTexture(txFlow, txFlow_cnt);
	TextureLoader.Close();
	
	LogFile<<"Game2: World free"<<endl;
	
	glDeleteProgram(ProgramId);
	glDeleteVertexArrays(1, &VertexArrayId);
	LogFile<<"Game2: Shaders free"<<endl;
	
	if(Key) delete [] Key;
	LogFile<<"Game2: Keys free"<<endl;
}

void MGame2::OnSize()
{
	//Projection = glm::ortho(0.0f, (float)WindowWidth, 0.0f, (float)WindowHeight, -1.0f, 1.0f);
	//MVP = Projection * View * Model;
}

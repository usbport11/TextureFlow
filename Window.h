#ifndef windowH
#define windowH

class MWindow
{
private:
	const char* sWindowClass;
	const char* sTitle;
	
	float fOpenGlVersion;
	string OpenGLVersion;
	string OpenGLVendor;
	string OpenGLRenderer;
	
	HINSTANCE m_hInstance;
	SYSTEM_INFO sysinfo;
	
    int SetUpWindow(HWND hWnd);
	int InitGL();
	int CloseGL();
	void ResizeScene(int width, int height);
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CreateConsole();
	void SetVSync(bool Enable);
	
protected:
	bool Exit;
	HDC m_hDC;
	HGLRC m_hGLRC;
	HWND m_hWnd;
	bool WindowInitialized;
	
	int WindowWidth;
    int WindowHeight;
    
    DWORD CurrTime;
	float DiffTime;
	
	//virtual functions
	virtual void OnDraw();
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void OnClose();
	virtual void OnSize();
	
public:
	MWindow();
	~MWindow();
	int CreateMainWindow(HINSTANCE hInstance);
	int CreateMainWindow(HINSTANCE hInstance, int Width, int Height);
	bool ChangeWindowSize(int Width, int Height);
	LRESULT CALLBACK WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
	void Run();
	HWND GetHWND();
};

#endif

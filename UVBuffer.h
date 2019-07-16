#ifndef uvbufferH
#define uvbufferH

class MUVBuffer
{
private:
	GLenum Type;
	vector<stUVQuad> UVQuad;
	unsigned int Id;
public:
	MUVBuffer();
	~MUVBuffer();
	bool Initialize(GLenum inType);
	int AddUVQuad(stUVQuad inUVQuad, bool Force);
	bool UpdateUVQuad(unsigned int Number, stUVQuad inUVQuad, bool Force);
	bool UpdateAll();
	void Close();
	unsigned int GetId();
};

#endif

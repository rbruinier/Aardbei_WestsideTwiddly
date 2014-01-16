#ifndef __CLOUDS_H__
#define __CLOUDS_H__

class Clouds: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
//	void	Receive(dword Message, dword timer);

private:
	Scene	*scene;
};

#endif
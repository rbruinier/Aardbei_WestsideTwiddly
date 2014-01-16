#ifndef __HOSPITAL_H__
#define __HOSPITAL_H__

class Hospital: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	Scene   *scene2;
};

#endif

#ifndef _COMCENTERCLASS_H_
#define _COMCENTERCLASS_H_
#include "building.h"
enum PlayerSides
{
	Red = 1,
	Blue = 2
};

class ComCenter : public Building
{
public:
	ComCenter();
	UINT Side;
};

#endif
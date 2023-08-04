#ifndef CHEST_H
#define CHEST_H

#include "InputHelper.h"

class Chest
{
public:
	InputHelper* mIhelper;
	Chest(int x, int y, int z);
	virtual ~Chest();

	int chestSlotId[27];
	int chestSlotAm[27];
	bool chestSlotSt[27];

	int chestAddress;

	short id;
};
#endif
#include "Chest.h"

Chest::Chest(int x, int y,  int z)
{
    for(char i = 0; i < 27; i++)
    {
        chestSlotId[i] = -1;
        chestSlotAm[i] = -1;
        chestSlotSt[i] = 0;
    }

	if (mIhelper->Limit_RAM == false)
	{
        chestAddress = x + y * 266 + z * 266 * 98;
	}
	if (mIhelper->Limit_RAM == true)
	{
        chestAddress = x + y * 112 + z * 112 * 98;
	}  
    id = 0;
}
Chest::~Chest()
{
}
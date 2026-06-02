#pragma once
#include "EnemyData.h"



class Enemy
{
public:
	EnemyData Data;

	Enemy(const EnemyData& data)
		: Data(data)
	{
	}
};

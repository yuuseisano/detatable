#include "Enemy.h"
#include "EnemyFactory.h"

const EnemyData EnemyFactory::EnemyTable[] = {
	{ 1, "Slime", 10, 5, 2, 1, 5, 10,0, Element::Water },
	{ 2, "Goblin", 20, 10, 5, 2, 10, 20,10, Element::Earth },
	{ 3, "Orc", 100, 50, 30, 10, 1000, 5000,50, Element::Fire },
	{ 4, "Dragon", 1000, 200, 100, 50, 10000, 50000,25, Element::Wind},
};

const int EnemyFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(EnemyData);

Enemy* EnemyFactory::CreateEnemy(int ID)
{
	for (int i = 0; i < EnemyTableSize; ++i)
	{
		if (EnemyTable[i].ID == ID)
		{
			return new Enemy(EnemyTable[i]);
		}
	}
	return nullptr;
}

// 追加実装: テーブル情報の読み取り用関数（インスタンスは生成しない）
int EnemyFactory::GetEnemyTableSize()
{
	return EnemyTableSize;
}

const EnemyData* EnemyFactory::GetEnemyDataByIndex(int index)
{
	if (index < 0 || index >= EnemyTableSize) return nullptr;
	return &EnemyTable[index];
}
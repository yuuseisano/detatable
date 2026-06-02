#include <iostream>
#include <iomanip>
#include <random>
#include <memory>
#include "Enemy.h"
#include "EnemyFactory.h"
#include "EnemyData.h"

static const char* ElementToString(Element e)
{
	switch (e)
	{
	case Element::None:  return "None";
	case Element::Fire:  return "Fire";
	case Element::Water: return "Water";
	case Element::Earth: return "Earth";
	case Element::Wind:  return "Wind";
	case Element::Light: return "Light";
	case Element::Dark:  return "Dark";
	default:             return "Unknown";
	}
}

int main()
{
	int tableSize = EnemyFactory::GetEnemyTableSize();
	if (tableSize <= 0)
	{
		std::cout << "敵データが存在しません。" << std::endl;
		return 0;
	}

	int totalHP = 0;
	int totalATK = 0;
	for (int i = 0; i < tableSize; ++i)
	{
		const EnemyData* d = EnemyFactory::GetEnemyDataByIndex(i);
		if (d)
		{
			totalHP += d->HP;
			totalATK += d->ATK;
		}
	}

	// ランダムで 1 体の ID を選び、unique_ptr で生成して表示
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, tableSize - 1);
	int chosenIndex = dist(gen);

	const EnemyData* chosenData = EnemyFactory::GetEnemyDataByIndex(chosenIndex);
	if (!chosenData)
	{
		std::cout << "敵の選択に失敗しました。" << std::endl;
		return 0;
	}

	// CreateEnemy は std::unique_ptr<Enemy> を返す想定
	std::unique_ptr<Enemy> chosenEnemy = EnemyFactory::CreateEnemy(chosenData->ID);
	if (!chosenEnemy)
	{
		std::cout << "敵インスタンスの生成に失敗しました。" << std::endl;
		return 0;
	}

	std::cout << "=== ランダム選択された敵 ===" << std::endl;
	std::cout << "ID: " << chosenEnemy->Data.ID << std::endl;
	std::cout << "Name: " << chosenEnemy->Data.Name << std::endl;
	std::cout << "HP: " << chosenEnemy->Data.HP << std::endl;
	std::cout << "ATK: " << chosenEnemy->Data.ATK << std::endl;
	std::cout << "DEF: " << chosenEnemy->Data.DEF << std::endl;
	std::cout << "SPD: " << chosenEnemy->Data.SPD << std::endl;
	std::cout << "Gold: " << chosenEnemy->Data.Gold << std::endl;
	std::cout << "EXP: " << chosenEnemy->Data.EXP << std::endl;
	std::cout << "CriticalRate: " << chosenEnemy->Data.CriticalRate << std::endl;
	std::cout << "Element: " << ElementToString(chosenEnemy->Data.Element) << std::endl;

	double averageATK = static_cast<double>(totalATK) / static_cast<double>(tableSize);
	std::cout << std::endl;
	std::cout << "合計 HP: " << totalHP << std::endl;
	std::cout << "平均 ATK: " << std::fixed << std::setprecision(2) << averageATK << std::endl;

	// unique_ptr により自動で解放される
	return 0;
}


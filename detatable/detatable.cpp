#include <iostream>
#include <iomanip>
#include <random>
#include <string>
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
	// テーブル全体から合計を計算（インスタンス生成なし）
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

	// ランダムで1体選択（ここでもインスタンスは生成しない）
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, tableSize - 1);
	int chosenIndex = dist(gen);

	const EnemyData* chosen = EnemyFactory::GetEnemyDataByIndex(chosenIndex);
	if (!chosen)
	{
		std::cout << "敵の選択に失敗しました。" << std::endl;
		return 0;
	}

	// 選択された敵の全ステータスを表示
	std::cout << "=== ランダム選択された敵 ===" << std::endl;
	std::cout << "ID: " << chosen->ID << std::endl;
	std::cout << "Name: " << chosen->Name << std::endl;
	std::cout << "HP: " << chosen->HP << std::endl;
	std::cout << "ATK: " << chosen->ATK << std::endl;
	std::cout << "DEF: " << chosen->DEF << std::endl;
	std::cout << "SPD: " << chosen->SPD << std::endl;
	std::cout << "Gold: " << chosen->Gold << std::endl;
	std::cout << "EXP: " << chosen->EXP << std::endl;
	std::cout << "CriticalRate: " << chosen->CriticalRate << std::endl;
	std::cout << "Element: " << ElementToString(chosen->Element) << std::endl;

	// 合計 HP と 平均 ATK を表示
	double averageATK = static_cast<double>(totalATK) / static_cast<double>(tableSize);
	std::cout << std::endl;
	std::cout << "合計 HP: " << totalHP << std::endl;
	std::cout << "平均 ATK: " << std::fixed << std::setprecision(2) << averageATK << std::endl;

	return 0;
}


#include <iostream>
#include <iomanip>
#include <random>
#include <memory>
#include <vector>
#include <string>
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

struct Hero
{
	std::string Name;
	int HP;
	int ATK;
	int DEF;
	int SPD;
	int CriticalRate; // パーセンテージ
};

static int CalcDamage(int atk, int def)
{
	int dmg = atk - def;
	if (dmg < 1) dmg = 1;
	return dmg;
}

int main()
{
	int tableSize = EnemyFactory::GetEnemyTableSize();
	if (tableSize <= 0)
	{
		std::cout << "敵データが存在しません。" << std::endl;
		return 0;
	}

	// ランダムに4体生成（重複あり）。生成と同時に合計を計算する
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, tableSize - 1);
	std::uniform_int_distribution<> percentDist(1, 100);

	std::vector<std::unique_ptr<Enemy>> enemies;
	enemies.reserve(4);

	int totalHP = 0;
	int totalATK = 0;

	for (int i = 0; i < 4; ++i)
	{
		int idx = dist(gen);
		const EnemyData* d = EnemyFactory::GetEnemyDataByIndex(idx);
		if (!d)
		{
			std::cout << "敵データの取得に失敗しました (index=" << idx << ")。" << std::endl;
			continue;
		}

		std::unique_ptr<Enemy> e = EnemyFactory::CreateEnemy(d->ID);
		if (!e)
		{
			std::cout << "敵インスタンスの生成に失敗しました (ID=" << d->ID << ")。" << std::endl;
			continue;
		}

		// 生成と同時に合計値に加算
		totalHP += e->Data.HP;
		totalATK += e->Data.ATK;

		enemies.push_back(std::move(e));
	}

	// 勇者を作成
	Hero hero;
	hero.Name = "勇者";
	hero.HP = 800;
	hero.ATK = 80;
	hero.DEF = 25;
	hero.SPD = 20;
	hero.CriticalRate = 10; // 10%

	// 生成された敵を表示（戦闘前）
	if (enemies.empty())
	{
		std::cout << "敵を生成できませんでした。" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "=== 生成された敵（ランダム4体） ===" << std::endl;
		for (size_t i = 0; i < enemies.size(); ++i)
		{
			const Enemy& e = *enemies[i];
			std::cout << "[" << (i + 1) << "] ID: " << e.Data.ID
				<< "  Name: " << e.Data.Name
				<< "  HP: " << e.Data.HP
				<< "  ATK: " << e.Data.ATK
				<< "  DEF: " << e.Data.DEF
				<< "  SPD: " << e.Data.SPD
				<< "  Gold: " << e.Data.Gold
				<< "  EXP: " << e.Data.EXP
				<< "  Crit: " << e.Data.CriticalRate
				<< "  Element: " << ElementToString(e.Data.Element)
				<< std::endl;
		}
	}

	// 生成された敵の合計/平均を表示（生成された敵のもの）
	double averageATK = 0.0;
	if (!enemies.empty())
	{
		averageATK = static_cast<double>(totalATK) / static_cast<double>(enemies.size());
	}
	std::cout << std::endl;
	std::cout << "合計 HP (生成された敵): " << totalHP << std::endl;
	std::cout << "平均 ATK (生成された敵): " << std::fixed << std::setprecision(2) << averageATK << std::endl;

	// 勇者と順に戦わせる
	std::cout << std::endl;
	std::cout << "=== 戦闘開始: " << hero.Name << " vs ランダム4体 ===" << std::endl;

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		Enemy& enemy = *enemies[i];
		std::cout << std::endl << "=== 敵と対峙: " << enemy.Data.Name << " (HP:" << enemy.Data.HP << ") ===" << std::endl;

		// 戦闘ループ
		while (hero.HP > 0 && enemy.Data.HP > 0)
		{
			// 先攻判定：SPD が高い方が先攻（同値なら英雄が先攻）
			bool heroFirst = (hero.SPD >= enemy.Data.SPD);

			if (heroFirst)
			{
				// 勇者攻撃
				int dmg = CalcDamage(hero.ATK, enemy.Data.DEF);
				if (percentDist(gen) <= hero.CriticalRate) dmg *= 2;
				enemy.Data.HP -= dmg;
				std::cout << hero.Name << " の攻撃! " << enemy.Data.Name << " に " << dmg << " ダメージ (" << std::max(0, enemy.Data.HP) << " HP 残)" << std::endl;
				if (enemy.Data.HP <= 0) break;

				// 敵反撃
				int edmg = CalcDamage(enemy.Data.ATK, hero.DEF);
				if (percentDist(gen) <= enemy.Data.CriticalRate) edmg *= 2;
				hero.HP -= edmg;
				std::cout << enemy.Data.Name << " の攻撃! " << hero.Name << " に " << edmg << " ダメージ (" << std::max(0, hero.HP) << " HP 残)" << std::endl;
			}
			else
			{
				// 敵先攻
				int edmg = CalcDamage(enemy.Data.ATK, hero.DEF);
				if (percentDist(gen) <= enemy.Data.CriticalRate) edmg *= 2;
				hero.HP -= edmg;
				std::cout << enemy.Data.Name << " の攻撃! " << hero.Name << " に " << edmg << " ダメージ (" << std::max(0, hero.HP) << " HP 残)" << std::endl;
				if (hero.HP <= 0) break;

				int dmg = CalcDamage(hero.ATK, enemy.Data.DEF);
				if (percentDist(gen) <= hero.CriticalRate) dmg *= 2;
				enemy.Data.HP -= dmg;
				std::cout << hero.Name << " の攻撃! " << enemy.Data.Name << " に " << dmg << " ダメージ (" << std::max(0, enemy.Data.HP) << " HP 残)" << std::endl;
			}
		}

		// 勝敗判定
		if (hero.HP <= 0)
		{
			std::cout << std::endl << hero.Name << " は力尽きた..." << std::endl;
			break;
		}
		else
		{
			std::cout << std::endl << enemy.Data.Name << " を倒した!" << std::endl;
			// 次の敵へ（必要ならここで経験値や回復処理を追加可能）
		}
	}

	if (hero.HP > 0)
	{
		std::cout << std::endl << hero.Name << " はすべての敵を撃破した！ 残りHP: " << hero.HP << std::endl;
	}
	else
	{
		std::cout << std::endl << hero.Name << " は敗北した。" << std::endl;
	}

	return 0;
}


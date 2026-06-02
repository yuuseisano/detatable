#pragma once
#include "EnemyData.h"

class Enemy;

//　@brief 敵の生成を担当するファクトリークラス
class EnemyFactory
{
	// @brief 敵のデータを格納するテーブル
	static const EnemyData EnemyTable[];
	// @brief 敵のデータテーブルのサイズ
	static const int EnemyTableSize;

public:
	// @brief 指定されたIDに基づいて敵を生成します
	// @param [int]ID 敵のID
	// @return 生成された敵のポインタ。IDが見るからなかった場合はnullptrを返します。
	// @details
	// このメソッドは、指定されたIDに基づいて敵を生成します。
	// IDがつからなかった場合はnullptrを返します。
	static Enemy* CreateEnemy(int ID);

	// 追加: テーブルへの読み取り専用アクセス（インスタンス生成なしでデータを参照するため）
	static int GetEnemyTableSize();
	static const EnemyData* GetEnemyDataByIndex(int index);
};

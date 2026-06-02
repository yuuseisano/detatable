#include <iostream>
#include "Enemy.h"
#include "EnemyFactory.h"

int main()
{
		Enemy* enemy = EnemyFactory::CreateEnemy(1);

		std::cout << enemy->Data.Name << "   EXP :" << enemy->Data.EXP << std::endl;

		delete enemy;


		return 0;

	
}


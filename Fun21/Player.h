#pragma once
#include "Entity.h"
namespace GEX
{
	class Player : public Entity
	{
	public:
		Player(int totalMoney);

		int		getTotalMoney() { return _totalMoney; }
		void	addMoney(int amount) { _totalMoney += amount; }
		void	betMoney(int amount) { _totalMoney -= amount; }

	private:
		int		_totalMoney;
	};	
}


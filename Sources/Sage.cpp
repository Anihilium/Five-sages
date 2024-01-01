#include <Sage.hpp>

Sage::Sage(std::mutex* lc, std::mutex* rc, const int timeBoundaries[4], const int etl) :
	Left_chopstick(lc),
	Right_chopstick(rc),
	thinkDistribution(std::uniform_int_distribution<int>(timeBoundaries[0], timeBoundaries[1])),
	eatDistribution(std::uniform_int_distribution<int>(timeBoundaries[2], timeBoundaries[3])),
	eatingTimeLimit(etl)
{
}

void Sage::Think()
{
	State = S_STATE::SS_THINKING;
	std::this_thread::sleep_for(std::chrono::seconds(thinkDistribution(generator)));
	Eat();
}

void Sage::Eat()
{
	if (DailyEatingTime >= eatingTimeLimit)
	{
		Sleep();
		return;
	}

	bool leftLock = Left_chopstick->try_lock(), rightLock = Right_chopstick->try_lock();
	if (leftLock && rightLock)
	{
		State = S_STATE::SS_EATING;
		int curEatingTime = eatDistribution(generator);
		DailyEatingTime += curEatingTime;
		std::this_thread::sleep_for(std::chrono::seconds(curEatingTime));
	}
	else
	{
		State = S_STATE::SS_HUNGRY;
	}

	if(leftLock)
		Left_chopstick->unlock();
	if (rightLock)
		Right_chopstick->unlock();

	if (State == S_STATE::SS_HUNGRY)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Eat();
	}
	else if (State == S_STATE::SS_EATING)
	{
		Think();
	}
}


void Sage::Sleep()
{
	State = S_STATE::SS_SLEEPING;
}

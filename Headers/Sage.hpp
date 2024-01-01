#include <mutex>
#include <chrono>
#include <random>

enum class S_STATE
{
	SS_EATING,
	SS_HUNGRY,
	SS_SLEEPING,
	SS_THINKING,
};

class Sage
{
public:
	std::mutex* Left_chopstick;
	std::mutex* Right_chopstick;
	S_STATE State = S_STATE::SS_THINKING;
	int DailyEatingTime;
	
public:
	Sage(std::mutex* lc, std::mutex* rc, const int timeBoundaries[4], const int eatingTimeLimit);
	void Eat();
	void Think();
	void Sleep();

private:
	std::default_random_engine generator;
	std::uniform_int_distribution<int> thinkDistribution;
	std::uniform_int_distribution<int> eatDistribution;
	int eatingTimeLimit;
};
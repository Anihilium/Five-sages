#include <windows.h>
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

#include <Sage.hpp>

class App
{
public:
	bool EveryoneIsSatiated;
	int DayAmount;
	int CurDay;

public:
	App();
	void Update();
	void DailyInit();
	~App();

private:
	HANDLE hConsole;
	std::vector<std::mutex*> Chopsticks;
	std::vector<Sage> sages;
	std::vector<std::thread> threads;
	std::thread drawThread;

private:
	void PrintState(S_STATE s);
	void Draw();
};
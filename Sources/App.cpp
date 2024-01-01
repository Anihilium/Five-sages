#include <App.hpp>

App::App():
	hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
	EveryoneIsSatiated(false),
	CurDay(0)
{
	SetConsoleTextAttribute(hConsole, 7);
	int sageAmount;
	int actionTimeLimit[4];
	int eatingTime;

	std::cout << "Sage amount: ";
	std::cin >> sageAmount;

	std::cout << "Number of days: ";
	std::cin >> DayAmount;

	std::cout << "Thinking time (min/max): ";
	std::cin >> actionTimeLimit[0] >> actionTimeLimit[1];

	std::cout << "Eating time (min/max): ";
	std::cin >> actionTimeLimit[2] >> actionTimeLimit[3];

	std::cout << "Daily eating time: ";
	std::cin >> eatingTime;

	for (int i = 0; i < sageAmount; ++i)
	{
		Chopsticks.push_back(new std::mutex);
		threads.push_back(std::thread());
	}

	for (int i = 0; i < sageAmount; ++i)
		sages.push_back(Sage(Chopsticks[i], Chopsticks[(i + 1) % sageAmount], actionTimeLimit, eatingTime));

	DailyInit();

	drawThread = std::thread(&App::Draw, std::ref(*this));
}

App::~App()
{
	system("cls");
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Complete!";

	for (std::thread& thread : threads)
		thread.join();

	drawThread.join();

	for (std::mutex* m : Chopsticks)
		delete m;

	SetConsoleTextAttribute(hConsole, 7);
}

void App::DailyInit()
{
	for (int i = 0; i < sages.size(); ++i)
	{
		sages[i].DailyEatingTime = 0;
		threads[i] = std::thread(&Sage::Think, std::ref(sages[i]));
	}

}

void App::PrintState(S_STATE s)
{
	switch (s)
	{
	case S_STATE::SS_EATING:
		SetConsoleTextAttribute(hConsole, 2);
		std::cout << 'E';
		break;
	case S_STATE::SS_HUNGRY:
		SetConsoleTextAttribute(hConsole, 4);
		std::cout << 'H';
		break;
	case S_STATE::SS_SLEEPING:
		SetConsoleTextAttribute(hConsole, 1);
		std::cout << 'S';
		break;
	case S_STATE::SS_THINKING:
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << 'T';
		break;
	default:
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << '#';
		break;
	}
}

void App::Draw()
{
	while (!EveryoneIsSatiated || CurDay < DayAmount)
	{
		system("cls");

		if (EveryoneIsSatiated)
		{
			++CurDay;
			if (CurDay >= DayAmount)
				return;

			for (std::thread& thread : threads)
				thread.join();

			DailyInit();
			continue;
		}

		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "Day " << CurDay + 1 << ": [";
		for (int i = 0; i < sages.size() - 1; ++i)
		{
			PrintState(sages[i].State);
			SetConsoleTextAttribute(hConsole, 7);
			std::cout << '|';
		}
		PrintState(sages[sages.size() - 1].State);
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << ']';
		SetConsoleTextAttribute(hConsole, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void App::Update()
{
	bool tmpSatiation = true;
	for (Sage& sage : sages)
	{
		if (sage.State != S_STATE::SS_SLEEPING)
		{
			tmpSatiation = false;
			break;
		}
	}
	EveryoneIsSatiated = tmpSatiation;
}
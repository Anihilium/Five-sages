#include <App.hpp>

int main()
{
	{
		App app;

		while (!app.EveryoneIsSatiated || app.CurDay < app.DayAmount)
			app.Update();
	}
	return 0;
}
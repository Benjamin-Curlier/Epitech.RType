#include "ServeurTCP.h"
#include <iostream>

int	main()
{
	ServeurTCP *server = new ServeurTCP();

	try
	{
		server->Init(9001);
		server->Start();
		server->End();
	}
	catch (const std::runtime_error *e)
	{
		std::cout << e->what() << std::endl;
		system("PAUSE");
	}
	catch (std::exception* e)
	{
		std::cout << e->what() << std::endl;
		system("PAUSE");
	}
	return 0;
}
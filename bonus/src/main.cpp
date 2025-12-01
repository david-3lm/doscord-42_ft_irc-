#include "Botini.hpp"

int main(int argc, char const *argv[])
{
	if (argc != 4)
	{
		std::cout << "NECESITO 4 ARGUMENTOS!!!!"<< std::endl;
	}

	Botini b(argv[1], argv[2], argv[3]);

	b.botConnect();
	b.update();
	return 0;
}
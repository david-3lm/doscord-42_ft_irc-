#include <Server.hpp>
#include <iostream>


int	main_error(const char *message, int error_code)
{
	std::cerr << RED << message << NO_COLOR << std::endl;
	std::cout << GREEN << "./ircserv <port> <password>" << NO_COLOR << std::endl;
	return (error_code);
}


int main(int argc, char const *argv[])
{
	if (argc != 3)
		return main_error("Invalid number of arguments", EXIT_FAILURE);
	

	std::string port = argv[1];
	//TODO: ERRORES

	std::string pass = argv[2];
	//TODO: ERRORES

	try
	{
		Server serv(to_int(port), pass);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << "error:" << e.what() << std::endl << '\n';
	}
	
	
	return 0;
}

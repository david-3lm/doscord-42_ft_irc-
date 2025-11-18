#include <Server.hpp>
#include <iostream>


int	main_error(const char *message, int error_code)
{
	std::cerr << RED << message << NO_COLOR << std::endl;
	std::cout << GREEN << "./ircserv <port> <password>" << NO_COLOR << std::endl;
	return (error_code);
}

bool parse_args(std::string port, std::string pass)
{
	size_t port_len = std::count_if(port.begin(), port.end(), isdigit);
	if (port_len != port.length())
		return main_error("Only digits in <port> 🔢", EXIT_FAILURE), false;
	if (pass.empty())
		return main_error("<password> empty. Try put something 😉", EXIT_FAILURE), false;
	return true;
}

int main(int argc, char const *argv[])
{
	if (argc != 3)
		return main_error("Invalid number of arguments 🥸", EXIT_FAILURE);
	
	std::string port = argv[1];
	std::string pass = argv[2];
	if (!parse_args(port, pass))
		return EXIT_FAILURE;

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

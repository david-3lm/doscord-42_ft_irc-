#include "Server.hpp"

void Server::reg_pass(std::string line, size_t pos, size_t cl_idx)
{
	std::string pass = line.substr(pos + 1, line.find("\r") - (pos + 1));
	if (pass.empty())
	{
		std::string msg = "Empty Password\r\n";
		std::cout << RED << msg << NO_COLOR;
		send(cl_idx + 1, msg.c_str(), msg.size(), 0);
		return;
	}
	if (pass != _pass)
	{
		std::cout << _clients[cl_idx].getPass() << " BAd Pass"<< std::endl;
		//kick_client(cl_idx);
		return;
	}
	_clients[cl_idx].setPass(pass);
}

void Server::reg_nick(std::string line, size_t pos, size_t cl_idx)
{
	std::string nick = line.substr(pos + 1, line.find("\r") - (pos + 1));
	//TODO: ERRORES
	if (nick.empty())
	{
		std::string msg = "Empty Nick. No name? 🥲\r\n";
		std::cout << RED << msg << NO_COLOR;
		send(cl_idx + 1, msg.c_str(), msg.size(), 0);
		return;
	}
	_clients[cl_idx].setNick(nick);
}

void Server::reg_user(std::string line, size_t pos, size_t cl_idx)
{
	std::string user = line.substr(pos + 1, line.find(' ', pos + 1) - pos - 1);
	//TODO: ERRORES
	if (user.empty())
	{
		std::string msg = "Empty User. No name? 🥲\r\n";
		std::cout << RED << msg << NO_COLOR;
		send(cl_idx + 1, msg.c_str(), msg.size(), 0);
		return;
	}
	_clients[cl_idx].setUser(user);
	std::cout << "USER [" << _clients[cl_idx].getUser() << "]" << std::endl;
}
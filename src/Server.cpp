#include "Server.hpp"

Server::Server(int port, std::string pass) : _port(port), _pass(pass)
{
	std::cout << CYAN << "EMO INISIAO EL SERVER" << std::endl;
	std::cout << "PORT => " << _port << std::endl;
	std::cout << std::endl;
	std::cout << "PASS => " << _pass << std::endl;
	std::cout << std::endl;
	std::cout << "***************************" << NO_COLOR << std::endl;


	init();

	server_loop();
}

Server::~Server() 
{
	std::cout << YELLOW << "Server Closed :(" << NO_COLOR << std::endl;
}

Server::Server(const Server &other) 
{
    (void)other;
}

Server &Server::operator=(const Server &other) 
{
    if (this != &other) {
    }
    return *this;
}

void Server::init()
{
	int tcp_socket;
	sockaddr_in addr;
	pollfd poll;

	tcp_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (tcp_socket == -1)
		throw ErrorExcept("Error with the socket");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = INADDR_ANY;
	std::cout << GREEN << "Port => " << ntohs(addr.sin_port) << NO_COLOR << std::endl;
	
	if (bind(tcp_socket, (sockaddr *)&addr, sizeof(addr)) == -1)
		throw ErrorExcept("Error binding the socket");

	if (listen(tcp_socket, 4096) == -1)
		throw ErrorExcept("Error listening the socket");

	poll.fd = tcp_socket;
	poll.events = POLLIN;
	poll.revents = 0;

	_polls.push_back(poll);
	_address = addr;

}

void Server::server_loop()
{
	int n_polls;

	while (true)
	{
		std::cout << GREEN << "————————————— CLIENTS ————————————" << std::endl;
		for (size_t i = 0; i < _clients.size(); i++)
			std::cout << "Name (" << i << ")" << _clients[i] << std::endl;
		std::cout << "——————————————————————————————————" << NO_COLOR << std::endl;

		n_polls = poll(&_polls[0], _polls.size(), 7000);
		if (n_polls == -1)
			std::cerr << RED << "Error with the poll" << NO_COLOR <<std::endl;
		else if(n_polls > 0)
		{
			std::cout << GREEN << "Number of polls: " << n_polls << std::endl;
			for (size_t i = 0; i < _polls.size() && n_polls; i++)
			{
				if (_polls[i].revents & POLLIN)
				{
					//TODO: SERVER COSAS
					if (i == 0)
						poll_server();
					else
						poll_client(i);
					n_polls--;
				}
			}
		}
		//TODO: clients to auth & register clients

		//TODO: CHANNEL COSAS
	}
	
}

void Server::poll_server()
{
	int new_socket;
	pollfd new_poll;
	sockaddr addr;
	socklen_t addr_len = sizeof(sockaddr);

	_polls[0].revents = 0;

	new_socket = accept(_polls[0].fd, &addr, &addr_len);
	if (new_socket == -1)
	{
		std::cerr << RED << "Error accepting a new client" << NO_COLOR << std::endl;
		return;
	}
	if (addr_len != sizeof(addr))
	{
		std::cerr << RED << "Error with address length" << NO_COLOR << std::endl;
		return;
	}
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << RED << "Error changing client to NON-BLOCK" << NO_COLOR << std::endl;
		return;
	}


	new_poll.fd = new_socket;
	new_poll.events = POLLIN;
	new_poll.revents = 0;

	_polls.push_back(new_poll);
	_clients.push_back(Client(new_poll, addr));
	//TODO: clients to auth
}

void Server::poll_client(size_t p_idx)
{
	ssize_t read;

	read = recv(_polls[p_idx].fd, _buff, 4096, 0);
	if (read <= 0)
	{
		//TODO: TRALALERO
		return;
	}
	_polls[p_idx].revents = 0;
	std::cout << BLUE << "Message from client: "<< _clients[p_idx - 1] << "\n"
		<< "read => " << read << "\n" << "[" << _buff << " ]" << NO_COLOR << std::endl;
	parse_buff(std::string(_buff), (p_idx - 1));
	std::fill_n(_buff, read, 0);
}

void Server::parse_buff(std::string buff, size_t cl_idx)
{
	size_t pos;
	std::string line;
	std::istringstream is(buff);
	std::string com;

	//std::cout<< "buff => " <<  << std::endl;
	while (getline(is, line))
	{
		pos = line.find(" ");
		com = line.substr(0, pos);
		std::cout << "com[" << com << "]" << std::endl;
		if (com == "PASS")
		{
			_clients[cl_idx].setPass(line.substr(pos + 1, line.find("\r")));
		}
		else if (com == "NICK")
		{
			_clients[cl_idx].setNick(line.substr(pos + 1, line.find("\r")));
		}
		else if (com == "USER")
		{
			_clients[cl_idx].setUser(line.substr(pos + 1, pos + line.find_first_of(' ', pos)));
			std::cout << "USER [" << _clients[cl_idx].getUser() << "]" << std::endl;
		}
	}
	
	
}
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
		for (size_t i = 0; i < _registered_clients.size(); i++)
			std::cout << "Name (" << i << ")" << _registered_clients[i] << std::endl;
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
					if (i == 0)
						poll_server();
					else
						poll_client(i);
					n_polls--;
				}
			}
		}
		if (_clients_to_auth)
			register_clients();

		//TODO: CHANNEL COSAS
	}
	
}

void Server::register_clients()
{
	size_t i = 0;
	std::string nick;

	while (i < _clients.size() && _clients_to_auth)
	{
		std::cout << i << " - Clientes registrados:  " << _registered_clients.size()<< std::endl;
		std::cout << "i = " << i << "[" << _clients[i].getNick() << "]" << "[" << _clients[i].getUser() << "]" << "[" << _clients[i].getPass() << "]" << "[" << _clients[i].getRegistered() << "]" << std::endl;
		if (_clients[i].tryToRegister(_registered_clients))
		{
			std::cout << "Try to register" << std::endl;

			nick = _clients[i].getNick();
			_registered_clients.push_back(nick);
			_clients_to_auth--;
			std::cout << "Enviamos mensaje a pollfd => " << i << std::endl;

			std::string art =
			" ⠀⠀⠀⠀⢀⣤⡀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"
			" ⠀⠀⠀⠀⣿⠉⢻⠟⢹⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"
			" ⠀⠀⠀⢀⣿⡄⠀⠀⣼⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣄⣠⣤⣄⠀⠀⠀⠀\r\n"
			" ⠀⠀⣰⡿⠋⠀⣀⣀⠈⣿⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣇⠘⠋⠀⣿⠇⠀⠀⠀\r\n"
			" ⠀⣠⡟⠀⢀⣾⠟⠻⠿⠿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣀⡀⠀⠀⣾⠋⢀⣀⠈⠻⢶⣄⠀⠀\r\n"
			" ⢠⣿⠁⣰⡿⠁⠀⣀⣤⣶⣶⡶⢶⣤⣄⡀⢀⣠⠴⠚⠉⠉⠉⠉⠉⠙⢶⡄⠛⠒⠛⠙⢳⣦⡀⠹⣆⠀\r\n"
			" ⢸⡇⢠⣿⣠⣴⣿⡟⢉⣠⠤⠶⠶⠾⠯⣿⣿⣧⣀⣤⣶⣾⣿⡿⠿⠛⠋⢙⣛⡛⠳⣄⡀⠙⣷⡀⢹⡆\r\n"
			" ⢸⠀⢸⣿⣿⣿⣿⠞⠉⠀⠀⠀⠀⣀⣤⣤⠬⠉⠛⠻⠿⠟⠉⢀⣠⢞⣭⣤⣤⣍⠙⠺⢷⡀⢸⡇⠀⣿\r\n"
			" ⢸⠀⢸⣿⣿⡟⠀⠀⠀⢀⣠⠞⣫⢗⣫⢽⣶⣤⣀⠉⠛⣶⠖⠛⠀⣾⡷⣾⠋⣻⡆⠀⠀⡇⣼⠇⠀⣿\r\n"
			" ⢸⠀⠀⣿⣿⡇⢠⡤⠔⣋⡤⠞⠁⢸⣷⣾⣯⣹⣿⡆⢀⣏⠀⠈⠈⣿⣷⣼⣿⠿⠷⣴⡞⠀⣿⠀⠀⣿\r\n"
			" ⢸⠀⠀⢿⣿⡇⠀⠀⠘⠻⠤⣀⡀⠸⣿⣯⣿⣿⡿⠷⠚⠉⠛⠛⠛⠛⠉⠉⠀⣠⡾⠛⣦⢸⡏⠀⠀⣿\r\n"
			" ⢸⠀⠀⢸⣿⡇⠀⣠⠶⠶⠶⠶⠿⣿⣭⣭⣁⣀⣠⣤⣤⣤⣤⣤⣤⡶⠶⠛⠋⢁⣀⣴⠟⣽⠇⠀⠀⣿\r\n"
			" ⢸⠀⠀⢸⣿⡇⢾⣅⠀⠀⠶⠶⢦⣤⣤⣀⣉⣉⣉⣉⣁⣡⣤⣤⣴⡶⠶⠶⠚⠉⢉⡿⣠⠟⠀⠀⣰⡟\r\n"
			" ⢸⡀⠀⠀⢿⣇⠀⠈⠛⠳⠶⠤⠤⢤⣀⣉⣉⣉⣉⣉⣉⣁⣀⣠⣤⡤⠤⠤⠶⠞⢻⡟⠃⠀⠀⣰⠟⠀\r\n"
			" ⢸⣧⠀⠀⠘⣿⣦⣄⡀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⣠⣤⣶⣿⣧⣀⣴⠟⠃⠀⠀\r\n"
			" ⠀⢻⣆⠀⠀⠈⢻⣿⣿⣷⣶⣤⣄⣀⣀⣀⣠⣤⣶⣶⣶⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣟⡉⠀⠀⠀⠀⠀\r\n"
			" ⠀⠀⢻⣦⡄⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠀⠀⠀\r\n"
			" ⠀⢀⣿⣿⣿⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡧⠀⠀⠀\r\n"
			"\r\n"
			" ⠀⠀⠀⠀⠀⠀⠀⠀⠀✨ **Conectado!!** ✨⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n";



			send(_polls[i+1].fd, art.c_str(), art.length(), 0);
			//_clients[i].sendToClient(art);
		}
		i++;
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
	std::cout << RED;
	_clients.push_back(Client(new_poll, addr));
	std::cout << RED << "clients size = [" << _clients.size() << "]" << NO_COLOR << std::endl;
	_clients_to_auth++;
}

void Server::poll_client(size_t p_idx)
{
	ssize_t read;

	read = recv(_polls[p_idx].fd, _buff, 4096, 0);
	if (read <= 0)
	{
		if (read == -1)
			std::cerr << RED << "Error trying to read" << NO_COLOR << std::endl;
		std::cout << CYAN << "cerramos clientes" << NO_COLOR << std::endl;
		_clients[p_idx - 1].quitClient();
		if (!_clients[p_idx - 1].getRegistered())
			_clients_to_auth--;
		else
		{
			std::string nick = _clients[p_idx - 1].getNick();
			std::deque<std::string>::iterator it = std::find(_registered_clients.begin(), _registered_clients.end(), nick);
			if (it != _registered_clients.end())
				_registered_clients.erase(it);
			//TODO: eliminar de los canales
		}
		_polls.erase(_polls.begin() + p_idx);
		_clients.erase(_clients.begin() + p_idx - 1);
		return;
	}
	_polls[p_idx].revents = 0;
	std::cout << BLUE << p_idx << " <=pollfd | Message from client: "<< _clients[p_idx - 1] << "\n"
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

	while (getline(is, line))
	{
		pos = line.find(" ");
		com = line.substr(0, pos);
		std::cout << "com[" << com << "]" << std::endl;
		if (com == "PASS")
			reg_pass(line, pos, cl_idx);
		else if (com == "NICK")
			reg_nick(line, pos, cl_idx);
		else if (com == "USER")
			reg_user(line, pos, cl_idx);
		else if (com == "JOIN")
			ch_join(line, pos, cl_idx);
		else if (com == "KICK")
			ch_kick(line, pos, cl_idx);
		else if (com == "INVITE")
			ch_invite(line, pos, cl_idx);
		else if (com == "TOPIC")
			ch_topic(line, pos, cl_idx);
		else if (com == "MODE")
			ch_mode(line, pos, cl_idx);
	}
	
	
}

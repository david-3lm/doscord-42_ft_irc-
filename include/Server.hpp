#pragma once

#ifndef _COLORS
# define _COLORS
# define BLACK    "\033[1;30m"
# define RED      "\033[1;31m"
# define GREEN    "\033[1;32m"
# define YELLOW   "\033[1;33m"
# define BLUE     "\033[1;34m"
# define MAGENTA  "\033[1;35m"
# define CYAN     "\033[1;36m"
# define WHITE    "\033[1;37m"
# define BOLD	  "\033[1m"
# define BOLD_OFF "\033[22m"
# define ITALIC   "\033[3m"
# define NO_COLOR       "\033[0m"
#endif

#include <algorithm>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <Client.hpp>
#include <Server.hpp>


template<class T> std::string	to_string(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template<class T> int	to_int(const T& value)
{
	std::stringstream ss;
	int val;
	ss << value;
	ss >> val;
	return val;
}

class Server 
{
private:
	int _port;
	sockaddr_in _address;
	struct tm *_init_time;
	std::string _time_str;
	std::string _pass;
	size_t _clients_to_auth;
	char _buff[4096];

	std::vector<struct pollfd> _polls;
	std::vector<Client> _clients;

	void init();
	void server_loop();
	void poll_server();
	void poll_client(size_t p_idx);
	void parse_buff(std::string buff, size_t cl_idx);
	
public:
    Server(int port, std::string pass);
    ~Server();
    Server(const Server &other);
    Server &operator=(const Server &other);

	class ErrorExcept : public std::exception
	{
		private:
			const char *_msg;
		public:
			ErrorExcept(const char *msg): _msg(msg){};
			virtual const char * what() const throw ()
			{
				return (_msg);
			}
	};
};


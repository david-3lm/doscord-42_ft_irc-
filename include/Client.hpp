#pragma once


#include <algorithm>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Client 
{
private:
	pollfd _poll_client;
	sockaddr _addr;
	std::string _pass;
	std::string _nick;
	std::string _user;
public:
    Client(pollfd poll, sockaddr addr);
    ~Client();
    Client(const Client &other);
    Client &operator=(const Client &other);

	std::string getPass() const;
	std::string getNick() const;
	std::string getUser() const;

	void setPass(std::string pass);
	void setNick(std::string nick);
	void setUser(std::string user);
};

std::ostream& operator<<(std::ostream& os, const Client& c);

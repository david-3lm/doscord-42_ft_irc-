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
#include <deque>

class Client 
{
private:
	pollfd _poll_client;
	sockaddr _addr;
	std::string _pass;
	std::string _nick;
	std::string _user;
	bool _registered;
	bool _invited;
	std::string _chan_invited;

public:
    Client(pollfd poll, sockaddr addr);
    ~Client();
    Client(const Client &other);
    Client &operator=(const Client &other);

	std::string getPass() const;
	std::string getNick() const;
	std::string getUser() const;
	bool getRegistered() const;

	void setPass(std::string pass);
	void setNick(std::string nick);
	void setUser(std::string user);
	void setRegistered(bool reg);
	void setInvited(bool inv, std::string chan);

	bool tryToRegister(std::deque<std::string> registered);
	void sendToClient(std::string &msg);
	bool getInvited(std::string chan);


	void quitClient();
};

std::ostream& operator<<(std::ostream& os, const Client& c);

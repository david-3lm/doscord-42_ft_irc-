#include "Client.hpp"

Client::Client(pollfd poll, sockaddr addr) : _poll_client(poll), _addr(addr) 
{
	
}

Client::~Client() 
{
}

Client::Client(const Client &other) 
{
    (void)other;
}

Client &Client::operator=(const Client &other) 
{
    if (this != &other) {
    }
    return *this;
}

std::string Client::getPass() const
{
	return _pass;
}

std::string Client::getNick() const
{
	return _nick;
}

std::string Client::getUser() const
{
	return _user;
}

void Client::setPass(std::string pass)
{
	_pass = pass;
}

void Client::setNick(std::string nick)
{
	_nick = nick;
}

void Client::setUser(std::string user)
{
	_user = user;
}

std::ostream &operator<<(std::ostream &os, const Client &c)
{
	os << c.getNick();
	return os;
}

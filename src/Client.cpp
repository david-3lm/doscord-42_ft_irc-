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

bool Client::getRegistered() const
{
	return _registered;
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

void Client::setRegistered(bool reg)
{
	_registered = reg;
}

bool Client::tryToRegister(std::vector<std::string> registered)
{
	if (this->getRegistered())
		return (false);
	if (_user.empty() || _nick.empty() || _pass.empty())
		return (false);

	if (std::find(registered.begin(), registered.end(), _nick) != registered.end())
		return (false);

	std::cout << "Cliente [" << _nick << "] Registrado" << std::endl;
	_registered = true;
	return true;
}

std::ostream &operator<<(std::ostream &os, const Client &c)
{
	os << c.getNick();
	return os;
}

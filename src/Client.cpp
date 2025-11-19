#include "Client.hpp"

Client::Client(pollfd poll, sockaddr addr) : _poll_client(poll), _addr(addr) 
{
	
	std::cout << "Cliente creado con fd: " << _poll_client.fd << std::endl;
	_registered = false;
	
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
	return this->_pass;
}

std::string Client::getNick() const
{
	return this->_nick;
}

std::string Client::getUser() const
{
	return this->_user;
}

bool Client::getRegistered() const
{
	return this->_registered;
}

void Client::setPass(std::string pass)
{
	this->_pass = pass;
}

void Client::setNick(std::string nick)
{
	this->_nick = nick;
}

void Client::setUser(std::string user)
{
	this->_user = user;
}

void Client::setRegistered(bool reg)
{
	this->_registered = reg;
}

bool Client::tryToRegister(std::deque<std::string> registered)
{
	std::cout << "DENTRO DE TRY TO REGISTER" << std::endl;
	std::cout << "POLLFD: " << this->_poll_client.fd <<std::endl;
	std::cout << "BOOL: " << this->getRegistered() <<std::endl;
	std::cout << "USER: " << this->getUser() << std::endl;
	std::cout << "NICK: " << this->getNick() << std::endl;
	std::cout << "PASS: " << this->getPass() << std::endl;
	std::cout << "DENTRO DE TRY TO REGISTER" << std::endl;
	if (this->getRegistered())
		return (false);
	if (_user.empty() || _nick.empty() || _pass.empty())
		return (false);

	if (std::find(registered.begin(), registered.end(), _nick) != registered.end())
		return (false);

	std::cout << "Cliente [" << _nick << "] Registrado" << std::endl;
	this->setRegistered(true);
	return true;
}

void Client::quitClient()
{
	close(_poll_client.fd);
	std::cout << "DESCONECTAMOS CLIENTE ["<< _nick << "]" << std::endl;
}

void Client::sendToClient(std::string &msg)
{
	send(this->_poll_client.fd, msg.c_str(), msg.length(), 0);

}

std::ostream &operator<<(std::ostream &os, const Client &c)
{
	os << c.getNick();
	return os;
}

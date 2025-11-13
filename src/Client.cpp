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

std::string Client::getName() const
{
	return _name;
}

std::ostream &operator<<(std::ostream &os, const Client &c)
{
	os << c.getName();
	return os;
}

#include "Channel.hpp"


Channel::Channel(const std::string &name, const std::string &pass) : _channel_name(name), _channel_pass(pass)
{
}

Channel::~Channel() 
{
}


Channel::Channel(const Channel &other)
{
    (void)other;
}

Channel &Channel::operator=(const Channel &other) 
{
    if (this != &other) {
    }
    return *this;
}

std::string Channel::getName()
{
	return std::string();
}

std::string Channel::getPass()
{
	return std::string();
}

std::string Channel::getTopic()
{
	return std::string();
}

size_t Channel::getLimit()
{
	return _channel_limit;
}

bool Channel::getIniviteMode()
{
	return _invite_mode;
}

void Channel::setPass(std::string pass)
{
	_channel_pass = pass;
}

void Channel::setTopic(std::string topic)
{
	_channel_topic = topic;
}

void Channel::setLimit(size_t l)
{
	_channel_limit = l;
}

void Channel::setIniviteMode(bool mode)
{
	_invite_mode = mode;
}

void Channel::addMember(std::string member)
{
	_members.push_back(member);
}

void Channel::kickMember(std::string member)
{
	std::deque<std::string>::iterator it;
	it = std::find(_members.begin(), _members.end(), member);
	if (it != _members.end())
		_members.erase(it);
}

void Channel::addOperator(std::string member)
{
	_operators.push_back(member);
}

void Channel::kickOperator(std::string member)
{
	std::deque<std::string>::iterator it;
	it = std::find(_operators.begin(), _operators.end(), member);
	if (it != _operators.end())
		_operators.erase(it);
}
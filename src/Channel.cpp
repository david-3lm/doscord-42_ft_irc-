#include "Channel.hpp"


Channel::Channel(const std::string &name, const std::string &pass)
{
	_channel_name = name;
	_channel_pass = pass;
	_invite_mode = false;
}

Channel::~Channel() 
{
}


Channel::Channel(const Channel &other)
{
    (void)other;
	*this = other;
}

Channel &Channel::operator=(const Channel &other) 
{
    if (this != &other) {
		_channel_name = other._channel_name;
		_channel_pass = other._channel_pass;
		_channel_topic = other._channel_topic;
		_members = other._members;
		_operators = other._members;
		_channel_limit = other._channel_limit;
		_invite_mode = other._invite_mode;
    }
    return *this;
}

std::string Channel::getName()
{
	return this->_channel_name;
}

std::string Channel::getPass()
{
	return this->_channel_pass;
}

std::string Channel::getTopic()
{
	return this->_channel_topic;
}

std::deque<std::string> Channel::getMembers()
{
	return this->_members;
}

size_t Channel::getLimit()
{
	return _channel_limit;
}

bool Channel::getIniviteMode()
{
	return _invite_mode;
}

void Channel::setName(std::string name)
{
	_channel_name = name;
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
	kickOperator(member);
}

bool Channel::isMember(std::string member)
{
	std::deque<std::string>::iterator it;
	it = std::find(_members.begin(), _members.end(), member);
	if (it != _members.end())
		return (true);
	return (false);
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

bool Channel::isOperator(std::string member)
{
	std::deque<std::string>::iterator it;
	it = std::find(_operators.begin(), _operators.end(), member);
	if (it != _operators.end())
		return (false);
	return (true);
}
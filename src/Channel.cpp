#include "Channel.hpp"


Channel::Channel(const std::string &name, const std::string &pass)
{
	_channel_name = name;
	_channel_pass = pass;
	_invite_mode = false;
	_channel_topic = "";
	_channel_limit = INT_MAX;
}

Channel::~Channel() 
{
}


Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other) 
{
    if (this != &other) {
		_channel_name = other._channel_name;
		_channel_pass = other._channel_pass;
		_channel_topic = other._channel_topic;
		_members = other._members;
		_operators = other._operators;
		_channel_limit = other._channel_limit;
		_invite_mode = other._invite_mode;
    }
    return *this;
}

std::string Channel::getName() const
{
	return this->_channel_name;
}

std::string Channel::getPass() const
{
	return this->_channel_pass;
}

std::string Channel::getTopic() const
{
	return this->_channel_topic;
}

std::deque<std::string> Channel::getMembers() const
{
	return this->_members;
}

size_t Channel::getLimit() const
{
	return _channel_limit;
}

bool Channel::getIniviteMode() const
{
	return _invite_mode;
}

bool Channel::getTopicRestict() const
{
	return _topic_restrict;
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

void Channel::setTopicRestirct(bool mode)
{
	_topic_restrict = mode;
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
		return (true);
	return (false);
}

void Channel::addInvite(std::string name)
{
	std::deque<std::string>::iterator it;
	it = std::find(_invites.begin(), _invites.end(), name);
	if (it != _operators.end())
		_invites.push_back(name);
}

void Channel::eraseInvite(std::string name)
{
	std::deque<std::string>::iterator it;
	it = std::find(_invites.begin(), _invites.end(), name);
	if (it != _operators.end())
		_invites.erase(it);
}

bool Channel::isInvited(std::string member)
{
	std::deque<std::string>::iterator it;
	it = std::find(_invites.begin(), _invites.end(), member);
	if (it != _invites.end())
		return (true);
	return (false);
}

std::ostream &operator<<(std::ostream &os, const Channel &c)
{
	os << "***Información importante del canal " << c.getName() << "***"<< std::endl;
	os << "\n\t Miembros: " << std::endl; 
	for (size_t i = 0; i < c.getMembers().size(); i++)
	{
		os << "\n\t\t -" << c.getMembers()[i] <<"-" << std::endl; 
	}
	return os;
}

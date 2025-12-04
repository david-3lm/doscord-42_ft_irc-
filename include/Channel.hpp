#pragma once

#include <poll.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <deque>
#include <iostream>
#include <limits.h>

class Channel 
{
private:
	std::string _channel_name;
	std::string _channel_pass;
	std::string _channel_topic;
	std::deque<std::string> _members;
	std::deque<std::string> _operators;
	std::deque<std::string> _invites;
	size_t _channel_limit;
	bool _invite_mode;
	bool _topic_restrict;
	bool _is_new;

public:
    Channel(const std::string &name, const std::string &pass);
    Channel(const Channel &other);
    ~Channel();
    Channel &operator=(const Channel &other);

	std::string getName()const;
	std::string getPass()const;
	std::string getTopic()const;
	std::deque<std::string> getMembers()const;
	size_t getLimit()const;
	bool getIniviteMode()const;
	bool getIsNew()const;

	void setName(std::string);
	void setPass(std::string);
	void setTopic(std::string);
	void setLimit(size_t);
	void setIniviteMode(bool);
	void setTopicRestirct(bool);
	void setIsNew(bool mode);

	void addMember(std::string);
	void addOperator(std::string);
	void addInvite(std::string);

	void kickMember(std::string);
	void kickOperator(std::string);
	void eraseInvite(std::string);

	bool isMember(std::string);
	bool isOperator(std::string);
	bool isInvited(std::string);

};

std::ostream &operator<<(std::ostream &os, const Channel &c);



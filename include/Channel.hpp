#pragma once

#include <poll.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <deque>

class Channel 
{
private:
	std::string _channel_name;
	std::string _channel_pass;
	std::string _channel_topic;
	std::deque<std::string> _members;
	std::deque<std::string> _operators;
	size_t _channel_limit;
	bool _invite_mode;

public:
    Channel(const std::string &name, const std::string &pass);
    Channel(const Channel &other);
    ~Channel();
    Channel &operator=(const Channel &other);

	std::string getName();
	std::string getPass();
	std::string getTopic();
	std::deque<std::string> getMembers();
	size_t getLimit();
	bool getIniviteMode();

	void setPass(std::string);
	void setTopic(std::string);
	void setLimit(size_t);
	void setIniviteMode(bool);

	void addMember(std::string);
	void addOperator(std::string);

	void kickMember(std::string);
	void kickOperator(std::string);

	bool isMember(std::string);
	bool isOperator(std::string);

};


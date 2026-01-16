#pragma once

#ifndef _COLORS
# define _COLORS
# define BLACK    "\033[1;30m"
# define RED      "\033[1;31m"
# define GREEN    "\033[1;32m"
# define YELLOW   "\033[1;33m"
# define BLUE     "\033[1;34m"
# define MAGENTA  "\033[1;35m"
# define CYAN     "\033[1;36m"
# define WHITE    "\033[1;37m"
# define BOLD	  "\033[1m"
# define BOLD_OFF "\033[22m"
# define ITALIC   "\033[3m"
# define NO_COLOR       "\033[0m"
#endif

#include <algorithm>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <deque>
#include <deque>
#include <map>


class Botini 
{
private:
	std::string _host;
	std::string _port;
	std::string _pass;
	std::string _nick;
	int _socket_fd;
	void onConnect();
	void onDisconnect();
	void sendMsg(std::string msg);
	void sendAnswer(std::string msg, std::string chan);
	void sendLetrero(std::string msg, std::string chan);
	void sendCommand(std::string cmd, std::string chan);

	std::map<int, std::string> _mapPoke;
	std::map<int, std::string> _mapJoke;
	std::map<int, std::string> _mapQuestion;
	std::map<int, std::string> _mapSaying1;
	std::map<int, std::string> _mapSaying2;
	void startPokeQuiz(std::string chan);
	void readJSONPoke();
	void readJSONJoke();
	void readJSONQuestion();
	void readJSONSaying();
	void readJSONSaying2();

	std::string _pokeChan;
	int _pokeInt;
	bool _pokeActive;

	void sendJoke(std::string chan);
	void sendQuestion(std::string chan);
	void sendSaying(std::string chan);

public:
    Botini(std::string h, std::string p, std::string pass);
    ~Botini();

	void botConnect();
	void update();
	
};


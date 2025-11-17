#include "Server.hpp"

void Server::ch_join(std::string line, size_t pos, size_t cl_idx)
{
	/*TODO: CREAR SERVER revisando que empiece por #
		/join #channel
	*/
}

void Server::ch_kick(std::string line, size_t pos, size_t cl_idx)
{
	/*TODO:
	echar a una persona de un canal
	comprobar que estas en el canal
	que existe el canal	
	*/
}

void Server::ch_invite(std::string line, size_t pos, size_t cl_idx)
{
	/*TODO:
	/invite <nickname> #channel
	*/
}

void Server::ch_topic(std::string line, size_t pos, size_t cl_idx)
{
	/*
	/topic this is the new channel topic
	*/
}

void Server::ch_mode(std::string line, size_t pos, size_t cl_idx)
{
	/*TODO:
	/mode #channel +/-attribute [data]
		· i: Set/remove Invite-only channel
		· t: Set/remove the restrictions of the TOPIC command to channel operators
		· k: Set/remove the channel key (password)
		· o: Give/take channel operator privilege
		· l: Set/remove the user limit to channe
	*/
}

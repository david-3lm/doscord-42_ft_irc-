#include "Server.hpp"

void mode_invite(size_t cl_idx, bool mode)
{

}

void mode_topic(size_t cl_idx, bool mode)
{

}

void mode_key(size_t cl_idx, bool mode)
{

}

void mode_operator(size_t cl_idx, bool mode)
{

}

void mode_limit(size_t cl_idx, bool mode)
{

}

void Server::ch_mode(std::string line, size_t cl_idx)
{
	/*TODO:
	/mode #channel +/-attribute [data]
		· i: Set/remove Invite-only channel
		· t: Set/remove the restrictions of the TOPIC command to channel operators
		· k: Set/remove the channel key (password)
		· o: Give/take channel operator privilege
		· l: Set/remove the user limit to channe
	*/

	std::string chan;
	std::string mode;
	std::string msg;
	size_t pos_sp;

	pos_sp = line.find(" ");
	if (pos_sp == line.npos)
	{
		chan = line.substr(0, line.find("\r"));
		mode = "";
	}
	else
	{
		chan = line.substr(0, line.find(" "));
		mode = line.substr(pos_sp + 1, line.find("\r") - (pos_sp + 1));
	}
	
	if (mode.empty())
	{
		std::cout << RED << "Not parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + chan + ":Empty parameter\r\n";
		return ;
	}

	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	if ((mode.find("+", 0) == std::string::npos && mode.find("-", 0) == std::string::npos) ||
		mode.find("+", 0) != std::string::npos && mode.find("-", 0) != std::string::npos)
	{
		std::cout << RED << "Unknown MODE flag" << NO_COLOR << std::endl;
		msg = ":doscord.irc 501 " + chan + " :Unknown MODE flag\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (mode.find("+", 0) != std::string::npos)
	{
		if (mode.find("i") != std::string::npos)
			mode_invite(cl_idx, true);
		if (mode.find("t") != std::string::npos)
			mode_topic(cl_idx, true);
		if (mode.find("k") != std::string::npos)
			mode_key(cl_idx, true);
		if (mode.find("o") != std::string::npos)
			mode_operator(cl_idx, true);
		if (mode.find("l") != std::string::npos)
			mode_limit(cl_idx, true);
		return;
	}
	if (mode.find("-", 0) != std::string::npos)
	{
		if (mode.find("i") != std::string::npos)
			mode_invite(cl_idx, false);
		if (mode.find("t") != std::string::npos)
			mode_topic(cl_idx, false);
		if (mode.find("k") != std::string::npos)
			mode_key(cl_idx, false);
		if (mode.find("o") != std::string::npos)
			mode_operator(cl_idx, false);
		if (mode.find("l") != std::string::npos)
			mode_limit(cl_idx, false);
		return ;
	}
}

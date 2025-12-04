#include "Server.hpp"

void Server::mode_invite(Channel& c, size_t cl_idx, bool mode)
{
	c.setIniviteMode(mode);
}

void Server::mode_topic(Channel& c, size_t cl_idx, bool mode)
{
	c.setTopicRestirct(mode);
}

void Server::mode_key(Channel& c, size_t cl_idx, bool mode, std::string arg)
{
	//TODO RECIBIR EL ARGUMENTO
	if (arg.empty())
	{
		std::cout << RED << "Empty parameter" << NO_COLOR << std::endl;
		std::string msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + c.getName() + ": Empty parameter\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if(mode == true)
		c.setPass(arg);
	else
		c.setPass("");
}

void Server::mode_operator(Channel& c, size_t cl_idx, bool mode, std::string arg)
{
	//TODO RECIBIR ARG
	if (arg.empty())
	{
		std::cout << RED << "Not parameters" << NO_COLOR << std::endl;
		std::string msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + c.getName() + ": Empty parameter\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (!c.isMember(arg))
	{
		std::cout << RED << arg << " They aren't on that channel " << c.getName() << NO_COLOR << std::endl;
		std::string msg = ":doscord.irc 441 " + _clients[cl_idx].getNick() + " " + c.getName() + " : " + arg + " aren't on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (mode == true && !c.isOperator(arg))
	{
		c.addOperator(arg);
		std::string msg = ":doscord.irc 381 " + _clients[cl_idx].getNick() + " " + c.getName() + " : " + _clients[cl_idx].getNick() + " You are now an IRC operator";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}
	else if (mode == false && c.isOperator(arg))
		c.kickOperator(arg);

}

void Server::mode_limit(Channel& c, size_t cl_idx, bool mode, std::string arg)
{
	//TODO: RECIBIR ARG
	if (arg.empty())
	{
		std::cout << RED << "Not parameters" << NO_COLOR << std::endl;
		std::string msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + c.getName() + " : Empty parameter\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	for (size_t i = 0; i < arg.size(); i++)
	{
		if (!isdigit(arg[i]))
		{
			std::cout << RED << "Ivalid Parameter. Only numbers" << NO_COLOR << std::endl;
			std::string msg = ":doscord.irc 696 " + _clients[cl_idx].getNick() + " " + c.getName() + " : Ivalid Parameter. Only numbers\r\n";
			send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
			return ;
		}
	}
	
	std::stringstream t(arg);
	size_t limit;
	t >> limit;
	if (mode == true)
		c.setLimit(limit);
	else
		c.setLimit(INT_MAX);
	
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
	std::string arg;
	size_t pos_sp;

	pos_sp = line.find(" ");
	if (pos_sp == line.npos)
	{
		chan = line.substr(0, line.find("\r"));
		mode = "";
		arg = "";
	}
	else
	{
		chan = line.substr(0, pos_sp);
		line = line.substr(pos_sp + 1, line.size() - (pos_sp + 1));
		pos_sp = line.find(" ");
		if (pos_sp == line.npos)
		{
			mode = line.substr(0, line.find("\r"));
			arg = "";
		}
		else
		{
			mode = line.substr(0, line.find(" "));
			arg = line.substr(line.find(" ") + 1, line.find("\r") - (line.find(" ") + 1));
		}
	}
	
	std::cout << YELLOW << "MODE: chan = -" << chan << "-\nmode = -" << mode << "-\narg = -" << arg << "-" << NO_COLOR << std::endl;
	
	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + _clients[cl_idx].getNick() + " " + chan + " : No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	Channel &c = find_channel(chan);

	if (mode.empty())
	{
		if (!c.getIsNew())
		{
			c.setIsNew(true);
			return;
		}
		std::cout << RED << "Not parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + " : Empty parameter\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	if (((mode.find("+", 0) == std::string::npos && mode.find("-", 0) == std::string::npos) ||
	(mode.find("+", 0) != std::string::npos && mode.find("-", 0) != std::string::npos)) && c.getIsNew())
	{
		std::cout << RED << "Unknown MODE flag" << NO_COLOR << std::endl;
		msg = ":doscord.irc 501 " + _clients[cl_idx].getNick() + " " + chan + " : Unknown MODE flag\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	
	if (!c.isOperator(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "Permission Denied -" << _clients[cl_idx].getNick() << " You're not an IRC operator" << NO_COLOR << std::endl;
		msg = ":doscord.irc 481 " + _clients[cl_idx].getNick() + " " + chan + " : " + _clients[cl_idx].getNick() + " Permission Denied- You're not an IRC operator\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (mode.find("+", 0) != std::string::npos)
	{
		if (mode.find("i") != std::string::npos)
			mode_invite(c, cl_idx, true);
		else if (mode.find("t") != std::string::npos)
			mode_topic(c, cl_idx, true);
		else if (mode.find("k") != std::string::npos)
			mode_key(c, cl_idx, true, arg);
		else if (mode.find("o") != std::string::npos)
			mode_operator(c, cl_idx, true, arg);
		else if (mode.find("l") != std::string::npos)
			mode_limit(c, cl_idx, true, arg);
		else
		{
			std::cout << RED << "Unknown MODE flag" << NO_COLOR << std::endl;
			msg = ":doscord.irc 501 " + _clients[cl_idx].getNick() + " " + chan + " : Unknown MODE flag\r\n";
			send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		}
		return;
	}
	if (mode.find("-", 0) != std::string::npos)
	{
		if (mode.find("i") != std::string::npos)
			mode_invite(c, cl_idx, false);
		else if (mode.find("t") != std::string::npos)
			mode_topic(c, cl_idx, false);
		else if (mode.find("k") != std::string::npos)
			mode_key(c, cl_idx, false, arg);
		else if (mode.find("o") != std::string::npos)
			mode_operator(c, cl_idx, false, arg);
		else if (mode.find("l") != std::string::npos)
			mode_limit(c, cl_idx, false, arg);
		else
		{
			std::cout << RED << "Unknown MODE flag" << NO_COLOR << std::endl;
			msg = ":doscord.irc 501 " + _clients[cl_idx].getNick() + " " + chan + " : Unknown MODE flag\r\n";
			send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		}
		return ;
	}
}

#include "Server.hpp"

void Server::ch_join(std::string line, size_t cl_idx)
{
	std::string chan;
	std::string pass;
	std::string msg;
	size_t pos_sp;
	pos_sp = line.find(" ");

	if (pos_sp == line.npos)
	{
		chan = line.substr(0, line.find("\r"));
		pass = "";
	}
	else
	{
		chan = line.substr(0, line.find(" "));
		pass = line.substr(pos_sp + 1, line.find("\r") - (pos_sp + 1));
	}

	if (chan.empty())
	{
		std::cout << RED << "Not parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + ":Empty parameter\r\n";
		return ;
	}

	if (chan[0] != '#')
	{
		std::cout << RED << "Bad Channel mask (#)" << NO_COLOR << std::endl;
		msg = ":doscord.irc 476 " + _clients[cl_idx].getNick() + " " + chan + " :Bad Channel Mask\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (!_clients[cl_idx].getRegistered())
	{
		std::cout << RED << "Not registered" << NO_COLOR << std::endl;
		msg = ":doscord.irc 451 " + _clients[cl_idx].getNick() + " " + chan + " :Client not registered\r\n" ;
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);
	
	if (!pass.empty() && c.getPass().empty() && c.getMembers().size() == 0)
		c.setPass(pass);

	if (!exist_channel(chan))
	{
		_channels.push_back(c);
		c = _channels.back();
		std::cout << MAGENTA << "c getname = -" << c.getName() << "- Channels = -" << _channels.back().getName() <<"-" << NO_COLOR<<std::endl;
	}


	if (c.getMembers().size() >= c.getLimit())
	{
		std::cout << RED << chan << "is full\r\n";
		msg = ":doscrod.irc 471 " + _clients[cl_idx].getNick() + " " + chan + " :Channel [" + chan + "] is full\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (!c.getPass().empty() && pass != c.getPass())
	{
		std::cout << RED << "Wrong Password for " << chan << NO_COLOR << std::endl;
		msg = ":doscord.irc 475 " + _clients[cl_idx].getNick() + " " + chan + " :Wrong_Password\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (c.getIniviteMode() && !_clients[cl_idx].getInvited(chan))
	{
		std::cout << RED << chan << " in only invitation mode" << NO_COLOR << std::endl;
		msg = ":doscord.irc 473 " + _clients[cl_idx].getNick() + " " + chan + " :In Only invitation mode\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		c.addMember(_clients[cl_idx].getNick());
		if (c.getMembers().size() == 1)
			c.addOperator(_clients[cl_idx].getNick());
	}

	msg = ":" + _clients[cl_idx].getNick() + " JOIN " + c.getName() + "\r\n";
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (c.isMember(_clients[i].getNick()) && i != cl_idx)
		{
			msg = ":doscord.irc PRIVMSG "+ chan + " " + _clients[cl_idx].getNick() + " joined the chat.\r\n";
			std::cout << "Complete = " << msg << std::endl;
			send(_polls[i + 1].fd, msg.c_str(), msg.size(), 0);
		}
	}
	
	if (!c.getTopic().empty())
	{
		msg = ":doscrod.irc 332 " + _clients[cl_idx].getNick() + " " + chan + " :" + c.getTopic() + "\r\n";
		// msg = ":doscrod.irc TOPIC " + chan + " : " + c.getTopic() + "\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}
}

void Server::ch_kick(std::string line, size_t cl_idx)
{
	std::string msg = "";
	std::string chan;
	std::string nick;
	std::string reason;
	size_t pos_dd;

	chan = line.substr(0, line.find(' '));
	pos_dd = line.find(':')	;
	if (pos_dd != std::string::npos)
	{
		nick = line.substr(line.find(' ') + 1, (pos_dd - 1) - (line.find(" ") + 1));
		reason = line.substr(pos_dd + 1, line.find("\r") - (pos_dd + 1));
	}
	else
	{
		nick = line.substr(line.find(' ') + 1, line.find("\r") - (line.find(' ') + 1));
		reason = "";
	}

	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + _clients[cl_idx].getNick() + " " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (nick.empty())
	{
		std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + " :Not enough parameters\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + _clients[cl_idx].getNick() + " " + chan + " : You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (!c.isOperator(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "Permission Denied -" << _clients[cl_idx].getNick() << "You're not an IRC operator" << NO_COLOR << std::endl;
		msg = ":doscord.irc 481 " + _clients[cl_idx].getNick() + " " + chan + " : " + _clients[cl_idx].getNick() + " Permission Denied- You're not an IRC operator\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (!c.isMember(nick))
	{
		std::cout << RED << nick << " They aren't on that channel " << chan << NO_COLOR << std::endl;
		msg = ":doscord.irc 441 " + _clients[cl_idx].getNick() + " " + chan + " :They aren't on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	std::string ban = ASCII_BAN;

	size_t idx_kick = search_id_nick(nick);
	// msg = ":" + nick + " PART " + c.getName() + " :Ban!\r\n";
	// send(_polls[idx_kick + 1].fd, msg.c_str(), msg.size(), 0);
	msg = ":" + _clients[cl_idx].getNick() + " KICK " + chan + " " + nick + " : " + reason + "\r\n";
	send(_polls[idx_kick + 1].fd, msg.c_str(), msg.size(), 0);
	for (int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNick() == nick) continue;
		send(_polls[i + 1].fd, msg.c_str(), msg.size(),0);
	}
	msg = ":doscord.irc 0 " + c.getName() + " :" + ban + "\t\tFrom channel: " + chan + "\r\n";
	send(_polls[idx_kick + 1].fd, msg.c_str(), msg.size(), 0);
	c.kickMember(nick);
}

void Server::ch_invite(std::string line, size_t cl_idx)
{
	std::string nick_invite;
	std::string chan;
	std::string msg;
	size_t pos_sp;

	pos_sp = line.find(" ");
	nick_invite = line.substr(0, line.find(" "));
	chan = line.substr(pos_sp + 1, line.find("\r") - (pos_sp + 1));

	if (chan.empty() && !exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + _clients[cl_idx].getNick() + " " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (search_id_nick(nick_invite) == _clients.size())
	{
		std::cout << RED << "Nick no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 401 " + _clients[cl_idx].getNick() + " " + chan + " :No such nick " + nick_invite + "\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + _clients[cl_idx].getNick() + " " + chan + " :You're not on that channel\r\n";
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

	if (c.isMember(nick_invite))
	{
		std::cout << RED << nick_invite << "Is already on channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 443 " + _clients[cl_idx].getNick() + " " + chan + " : " + nick_invite + " is already on channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	c.addInvite(nick_invite);

	msg = ":" + _clients[cl_idx].getNick() + " INVITE " + nick_invite + " " + chan + "\r\n";
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);

	for (int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNick() == nick_invite)
		{
			send(_polls[i + 1].fd, msg.c_str(), msg.size(), 0);
			_clients[i].setInvited(true, chan);
			return ;
		}
	}
}

void Server::ch_topic(std::string line, size_t cl_idx)
{
	/*
	/topic this is the new channel topic
	*/
	std::string chan;
	size_t pos_dd;
	std::string topic;
	std::string msg;

	pos_dd = line.find(":");
	if (pos_dd != std::string::npos)
	{
		chan = line.substr(0, line.find(" "));
		topic = line.substr(pos_dd + 1, line.find("\r") - (pos_dd + 1));
	}
	else
	{
		chan = line.substr(0, line.find("\r"));
		topic = "";
	}
	
	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + _clients[cl_idx].getNick() + " " + chan + " : No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + _clients[cl_idx].getNick() + " " + chan + " : You're not on that channel\r\n";
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

	if (topic.empty())
	{
		if (c.getTopic().empty())
		{
			std::cout << RED << "No topic is set in " << chan << NO_COLOR << std::endl;
			msg = ":doscord.irc 331 " + _clients[cl_idx].getNick() + " " + chan + " : No topic is set\r\n";
			send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
			return ;
		}
		msg = ":doscrod.irc 332 " + _clients[cl_idx].getNick() + " " + chan + " : " + c.getTopic() + "\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	c.setTopic(topic);
	msg = ":" + _clients[cl_idx].getNick() + " TOPIC " + chan + " : " + topic + "\r\n";
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (c.isMember(_clients[i].getNick()))
			send(_polls[i + 1].fd, msg.c_str(), msg.size(), 0);
	}
	

}

void Server::ch_msg(std::string line, size_t cl_idx)
{
	std::string chan;
	size_t pos_dd;
	std::string msg;
	std::string send_msg;

	chan = line.substr(0, line.find(" "));
	pos_dd = line.find(":") + 1;
	if (chan.empty())
	{
		std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + " : Not enough parameters\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (pos_dd == line.npos)//In channel
	{
		msg = line.substr(pos_dd, line.find("\r") - pos_dd);
		Channel &c = find_channel(chan);

		for (size_t i = 0; i < _clients.size(); i++)
		{
			std::cout << "Envio msg a: -" << _clients[i].getNick() << "-" << std::endl;
			if (c.isMember(_clients[i].getNick()))
			{
				send_msg = ":" + _clients[cl_idx].getNick() + " PRIVMSG "+ chan + " " + msg + "\r\n";
				std::cout << "Complete = " << send_msg << std::endl;
				send(_polls[i + 1].fd, send_msg.c_str(), send_msg.size(), 0);
			}
		}
		return ;
	}


	if (chan[0] != '#')//To user
	{
		size_t id_user = search_id_nick(chan);
		if (id_user == _clients.size())
		{
			std::cout << RED << "No such nick/channel" << NO_COLOR << std::endl;
			send_msg = ":doscord.irc 401 " + _clients[cl_idx].getNick() + " " + chan + " : No such nick/channel\r\n";
			send(_polls[cl_idx + 1].fd, send_msg.c_str(), send_msg.size(), 0);
			return ;
		}
		msg = line.substr(line.find(" ") + 1, line.find("\r") - line.find(" ") + 1);
		if (msg.empty())
		{
			std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
			send_msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + " : Not enough parameters\r\n";
			send(_polls[cl_idx + 1].fd, send_msg.c_str(), send_msg.size(), 0);
			return ;
		}
		send_msg = ":" +  _clients[cl_idx].getNick() + " PRIVMSG " + _clients[id_user].getNick() + " " + msg + "\r\n";
		send(_polls[id_user + 1].fd, send_msg.c_str(), send_msg.size(), 0);
		return ;		
	}

	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + _clients[cl_idx].getNick() + " " + chan + " : No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + _clients[cl_idx].getNick() + " " + chan + " : You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	msg = line.substr(line.find(" ") + 1, line.find("\r") - line.find(" ") + 1);
	if (msg.empty())
	{
		std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
		send_msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + " : Not enough parameters\r\n";
		send(_polls[cl_idx + 1].fd, send_msg.c_str(), send_msg.size(), 0);
		return ;
	}

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (c.isMember(_clients[i].getNick()) && i != cl_idx)
		{
			send_msg = ":" + _clients[cl_idx].getNick() + " " + "PRIVMSG" + " "+ chan + " " + msg + "\r\n";
			std::cout << "Complete = " << send_msg << std::endl;
			send(_polls[i + 1].fd, send_msg.c_str(), send_msg.size(), 0);
		}
	}
}

void Server::ch_part(std::string line, size_t cl_idx)
{
	std::string chan;
	std::string msg;
	std::string send_msg;

	chan = line.substr(0, line.find(" "));
	if (chan.empty())
	{
		std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + _clients[cl_idx].getNick() + " " + chan + " : Not enough parameters\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + _clients[cl_idx].getNick() + " " + chan + " : You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	c.kickMember(_clients[cl_idx].getNick());
	msg = _clients[cl_idx].getNick() + " left the chat.";
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (c.isMember(_clients[i].getNick()) && i != cl_idx)
		{
			std::cout << "Envio msg a: -" << _clients[i].getNick() << "-" << std::endl;
			send_msg = ":doscord.irc PRIVMSG "+ chan + " " + msg + "\r\n";
			std::cout << "Complete = " << send_msg << std::endl;
			send(_polls[i + 1].fd, send_msg.c_str(), send_msg.size(), 0);
		}

	}

}
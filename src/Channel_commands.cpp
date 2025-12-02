#include "Server.hpp"

void Server::ch_join(std::string line, size_t cl_idx)
{
	/*TODO: CREAR SERVER revisando que empiece por #
		/join #channel
	*/
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
		msg = ":doscord.irc 461 " + chan + ":Empty parameter\r\n";
		return ;
	}
	if (chan[0] != '#')
	{
		std::cout << RED << "Bad Channel mask (#)" << NO_COLOR << std::endl;
		msg = ":doscord.irc 476 " + chan + " :Bad Channel Mask\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (!_clients[cl_idx].getRegistered())
	{
		std::cout << RED << "Not registered" << NO_COLOR << std::endl;
		msg = ":doscord.irc 451 " + chan + " :Client not registered\r\n" ;
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

		std::cout << GREEN << "————————————— CHANNELs "<< _channels.size() <<" ————————————" << std::endl;
		for (size_t i = 0; i < _channels.size(); i++)
			std::cout << "Name (" << i << ")" << _channels[i].getName() << std::endl;
		std::cout << "——————————————————————————————————" << NO_COLOR << std::endl;


	// NO BORRAR ------------------------------------------------------
	// if (c.getMembers().size() == c.getLimit())
	// {
	// 	std::cout << RED << chan << "is full\r\n";
	// 	msg = "doscrod.irc 471 " + chan + ":Channel [" + chan + "] is full\r\n";
	// 	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	// 	return ;
	// }

	if (!c.getPass().empty() && pass != c.getPass())
	{
		std::cout << RED << "Wrong Password for " << chan << NO_COLOR << std::endl;
		msg = ":doscord.irc 475 " + chan + " :Wrong_Password\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (c.getIniviteMode())
	{
		std::cout << RED << chan << " in only invitation mode" << NO_COLOR << std::endl;
		msg = ":doscord.irc 473 " + chan + " :In Only invitation mode\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		c.addMember(_clients[cl_idx].getNick());
		c.addOperator(_clients[cl_idx].getNick());
	}

	msg = ":" + _clients[cl_idx].getNick() + " JOIN " + c.getName() + "\r\n";
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	if (!c.getTopic().empty())
	{
		msg = ":doscord.irc TOPIC " + chan + " :" + c.getTopic() + "\r\n";

		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}
}

void Server::ch_kick(std::string line, size_t cl_idx)
{
	/*TODO:
	echar a una persona de un canal
	comprobar que estas en el canal
	que existe el canal	
	*/

	std::string msg;
	std::string chan;
	std::string nick;
	std::string reason;
	size_t pos_dd;

	chan = line.substr(0, line.find(' '));
	pos_dd = line.find(':')	;
	if (pos_dd != std::string::npos)
	{
		nick = line.substr(line.find(' ') + 1, pos_dd - (line.find(" ") + 1));
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
		msg = ":doscord.irc 403 " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (nick.empty())
	{
		std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + chan + " :Not enough parameters\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);
	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + chan + " :You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (!c.isMember(nick))
	{
		std::cout << RED << nick << " They aren't on that channel " << chan << NO_COLOR << std::endl;
		msg = ":doscord.irc 441 " + chan + " :They aren't on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	std::string ban =
	"⡶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⢶\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀                ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡿⠿⠿⣿⣶⡄⠀⠀⠀⣼⣿⣿⣧⠀⠀⠀⠀⣿⣿⣿⡄⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣇⡀⣀⣼⣿⠇⠀⠀⢰⣿⡇⢹⣿⣆⠀⠀⠀⣿⡟⢻⣿⣆⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡿⠛⠻⢿⣷⣄⠀⢀⣿⣿⠀⠀⣿⣿⡄⠀⠀⣿⣷⠀⢻⣿⣆⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⣸⣿⡿⠀⣾⣿⠿⠿⠿⠿⣿⣿⡀⠀⣿⡟⠀⠀⢻⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠿⠿⠿⠿⠿⠛⠁⠸⠿⠏⠀⠀⠀⠀⠹⠿⠧⠀⠿⠿⠀⠀⠀⠻⠿⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n"
	"⡷⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⢶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⡾\n"
	"⠀⠀⠀⠉⠉⠉⠉⣽⡿⠁⣠⣾⡿⠾⣯⣿⠿⠯⢭⣉⠉⠉⠉⠉⣉⣭⣿⡯⠽⠯⢭⣟⡫⣽⣿⣁⣠⣦⣄⠉⠳⣿⣍⠉⠉⠉⠉⠉⠁⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⣿⠃⢠⣿⣿⣴⣾⠿⠛⠋⠉⠛⠲⠯⣵⣶⡯⠟⠋⢁⣀⣠⣤⣤⣬⣽⣾⣿⣿⣻⣿⣿⣷⣄⠘⣿⡄⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⣿⠀⢸⣿⡿⠛⢁⣤⠶⠛⠋⠉⠉⠛⠻⢿⠤⡴⠞⠛⣉⣽⠿⠛⢉⣠⣤⣤⢤⣝⣿⣿⣿⣿⡄⢸⣿⡀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⢸⡏⠀⠰⠋⠁⠀⠀⢀⣤⣶⣾⣿⣿⡤⠤⠤⣾⣋⢀⡤⠞⢫⣿⣿⣿⣷⡄⠉⠻⣿⣿⡇⠀⣿⠁⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⢸⣇⠀⠀⠀⢀⣠⠾⢋⡵⢋⣿⢻⣿⠿⣶⡄⠀⢹⠋⠀⠀⣿⣿⢿⣧⣿⣿⣀⣠⣿⣿⠁⠀⣿⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⢸⣿⠀⠀⠚⠛⣷⡞⠋⠀⢸⣿⣟⢿⣶⣿⣧⣠⡿⣤⣤⣤⣽⡿⠿⠛⠛⢉⣿⣿⣿⡿⠀⠀⣿⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⠈⣿⠀⠀⠀⣀⣀⣉⣻⣶⣤⣿⣿⠿⠟⠛⠉⠁⠀⠀⠀⠀⣀⣀⣠⣤⠾⠋⢉⣿⣿⡇⠀⠀⣿⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⣿⡆⢠⣾⠋⠉⣩⣉⣉⣙⠛⠛⠛⠛⠛⠛⠛⠛⠛⢛⣉⣉⣡⣤⡶⠾⣿⣿⣿⠁⠀⢠⣿⡄⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⣇⠀⠀⢻⡇⠈⠛⠶⣦⣬⣉⣉⣉⣙⡛⠛⠛⠛⠛⠛⠛⠛⢛⣛⣉⣉⣉⣠⣤⣶⣿⠟⠁⠀⢀⣾⡿⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠘⣿⣤⡀⠀⠀⠈⠉⠉⠉⠛⠛⠛⠛⠛⠛⠛⠛⠋⠉⠉⠉⢁⣠⣼⣏⠀⣀⣴⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⠈⣿⣧⠀⠀⠘⢿⣿⢷⣦⣄⣀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⣀⣀⣠⣤⣶⠿⠛⠉⢨⣿⣿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⠀⠘⣿⣧⡀⠀⠀⣿⠀⠈⠉⠛⠛⠿⠿⠿⠿⠛⠛⠛⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠈⠛⢾⣢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	"⠀⠀⠀⠀⠀⠀⠀⠀⣾⡿⢿⣦⣴⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀\n";

	size_t idx_kick = search_id_nick(nick);
	
	msg = ":doscord.irc 0 " + c.getName() + " :" + ban + "\t\tFrom channel: " + chan + "\r\n";
	send(_polls[idx_kick + 1].fd, msg.c_str(), msg.size(), 0);
	msg = ":doscord.irc KICK " + c.getName() + " " + nick + " " + reason + "\r\n";
	send(_polls[idx_kick + 1].fd, msg.c_str(), msg.size(), 0);
	c.kickMember(nick);
}

void Server::ch_invite(std::string line, size_t cl_idx)
{
	/*TODO:
	/invite <nickname> #channel
	*/
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
		msg = ":doscord.irc 403 " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + chan + " :You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (c.isMember(nick_invite))
	{
		std::cout << RED << nick_invite << "Is already on channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 443 " + chan + " : " + nick_invite + " is already on channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	c.addInvite(nick_invite);

	msg = ":" + _clients[cl_idx].getNick() + " INVITE " + nick_invite + " " + chan + "\r\n";
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);

	for (int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNick() == nick_invite)
			send(_polls[i + 1].fd, msg.c_str(), msg.size(), 0);
	}
	
	// msg = ":doscord.irc INVITE " + chan + " :" + nick_invite + "\r\n";
	// send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
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

	chan = line.substr(0, line.find(" "));
	pos_dd = line.find(":");
	if (pos_dd != std::string::npos)
		topic = line.substr(pos_dd + 1, line.find("\r") - (pos_dd + 1));
	else
		topic = "";
	
	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel &c = find_channel(chan);

	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 442 " + chan + " :You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}	

	if (topic.empty())
	{
		if (c.getTopic().empty())
		{
			std::cout << RED << "No topic is set in " << chan << NO_COLOR << std::endl;
			msg = ":doscord.irc 331 " + chan + " :No topic is set\r\n";
			send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
			return ;
		}
		msg = ":doscord TOPIC " + chan + " : " + c.getTopic() + "\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	c.setTopic(topic);
	msg = ":doscord TOPIC " + chan + " :" + topic + "\r\n";
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (c.isMember(_clients[i].getNick()))
			send(_polls[i + 1].fd, msg.c_str(), msg.size(), 0);
	}
	

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

	std::cout<< RED << "MODO"<< NO_COLOR <<  std::endl;
}

void Server::ch_msg(std::string line, size_t cl_idx)
{
	std::string chan;
	size_t pos;
	std::string msg;
	std::string complete_msg;

	std::cout << GREEN << "Entro ch_msg" << NO_COLOR << std::endl;
	chan = line.substr(0, line.find(" "));
	pos = line.find(":") + 1;
	msg = line.substr(pos, line.find("\r"));

	std::cout << YELLOW << "CHAN = -" << chan << "- msg = -" << msg << "-" << NO_COLOR << std::endl;
	Channel &c = find_channel(chan);
	std::cout << c << std::endl;
	//int chIdx;

	/*for (int i = 0; i< _channels.size(); i++)
	{
		if(_channels[i].getName() == chan)
			chIdx = i;
	}*/

	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << "Envio msg a: -" << _clients[i].getNick() << "-" << std::endl;
		if (c.isMember(_clients[i].getNick()) && i != cl_idx)
		{
			complete_msg = ":" + _clients[cl_idx].getNick() + " " + "PRIVMSG" + " "+ chan + " " + msg + "\r\n";
			std::cout << "Complete = " << complete_msg << std::endl;
			send(_polls[i + 1].fd, complete_msg.c_str(), complete_msg.size(), 0);
		}

	}
	
}
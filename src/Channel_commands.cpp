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
		std::cout << YELLOW << "no pass" << NO_COLOR << std::endl;
	}
	else
	{
		chan = line.substr(0, line.find(" "));
		pass = line.substr(pos_sp + 1, line.find("\r") - (pos_sp + 1));
		std::cout << YELLOW << "Pass = -" << pass << "-" << NO_COLOR << std::endl;
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
	
	std::cout << RED<< c << NO_COLOR <<std::endl;
	// Channel c(chan, pass);

	//size_t ch = find_channel_it(chan);
	if (!pass.empty() && c.getPass().empty() && c.getMembers().size() == 0)
		c.setPass(pass);
	if (!exist_channel(chan))
	{
		std::cout << RED << "NO EXISTE" << NO_COLOR << std::endl;
		// if (!pass.empty())
		// 	c.setPass(pass);
		_channels.push_back(c);
		//_channels.back().setPass(pass);
		c = _channels.back();
		std::cout << MAGENTA << "c getname = -" << c.getName() << "- Channels = -" << _channels.back().getName() <<"-" << NO_COLOR<<std::endl;
	}

		std::cout << GREEN << "————————————— CHANNELs "<< _channels.size() <<" ————————————" << std::endl;
		for (size_t i = 0; i < _channels.size(); i++)
			std::cout << "Name (" << i << ")" << _channels[i].getName() << std::endl;
		std::cout << "——————————————————————————————————" << NO_COLOR << std::endl;

	std::cout << CYAN << "chan = -" << chan << "- c name = -" << c.getName() << "-" << NO_COLOR << std::endl;
	// if (c.getMembers().size() == c.getLimit())
	// {
	// 	std::cout << RED << chan << "is full\r\n";
	// 	msg = "doscrod.irc 471 " + chan + ":Channel [" + chan + "] is full\r\n";
	// 	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	// 	return ;
	// }
	std::cout << "pass = -" << pass << "- ch_pass = -" << c.getPass() << "-" << std::endl;
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
		std::cout << RED << "NO ES MIEMBRO..... AÑADO" << _clients[cl_idx].getNick() << NO_COLOR << std::endl;
		c.addMember(_clients[cl_idx].getNick());
		c.addOperator(_clients[cl_idx].getNick());
	}

	msg = ":" + _clients[cl_idx].getNick() + " JOIN " + c.getName() + "\r\n";
	std::cout << CYAN << "c pass = -" << c.getPass() << "-" << NO_COLOR << std::endl;
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
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
		nick = line.substr(line.find(' ') + 1, pos_dd - 2);
		reason = line.substr(pos_dd + 1, line.find("\r") - (pos_dd + 1));
	}
	else
	{
		nick = line.substr(line.find(' ') + 1, line.find('\r'));
		reason = "";
	}

	if (!exist_channel(chan))
	{
		std::cout << RED << "Channel no exist" << NO_COLOR << std::endl;
		msg = ":doscord.irc 403 " + chan + " :No such channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}
	if (nick.empty())
	{
		std::cout << RED << "Not enough parameters" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + chan + " :Not enough parameters\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}

	Channel &c = find_channel(chan);
	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		std::cout << RED << "You're not on that channel" << NO_COLOR << std::endl;
		msg = ":doscord.irc 461 " + chan + " :You're not on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}
	if (!c.isMember(nick))
	{
		std::cout << RED << nick << " They aren't on that channel " << chan << NO_COLOR << std::endl;
		msg = ":doscord.irc 441 " + chan + " :They aren't on that channel\r\n";
		send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	}

	std::string ban =
	"⡶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⢶\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀                ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡿⠿⠿⣿⣶⡄⠀⠀⠀⣼⣿⣿⣧⠀⠀⠀⠀⣿⣿⣿⡄⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣇⡀⣀⣼⣿⠇⠀⠀⢰⣿⡇⢹⣿⣆⠀⠀⠀⣿⡟⢻⣿⣆⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡿⠛⠻⢿⣷⣄⠀⢀⣿⣿⠀⠀⣿⣿⡄⠀⠀⣿⣷⠀⢻⣿⣆⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⣸⣿⡿⠀⣾⣿⠿⠿⠿⠿⣿⣿⡀⠀⣿⡟⠀⠀⢻⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠿⠿⠿⠿⠿⠛⠁⠸⠿⠏⠀⠀⠀⠀⠹⠿⠧⠀⠿⠿⠀⠀⠀⠻⠿⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡷⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⢶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⡾\r\n"
	"⠀⠀⠀⠉⠉⠉⠉⣽⡿⠁⣠⣾⡿⠾⣯⣿⠿⠯⢭⣉⠉⠉⠉⠉⣉⣭⣿⡯⠽⠯⢭⣟⡫⣽⣿⣁⣠⣦⣄⠉⠳⣿⣍⠉⠉⠉⠉⠉⠁⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⣿⠃⢠⣿⣿⣴⣾⠿⠛⠋⠉⠛⠲⠯⣵⣶⡯⠟⠋⢁⣀⣠⣤⣤⣬⣽⣾⣿⣿⣻⣿⣿⣷⣄⠘⣿⡄⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⣿⠀⢸⣿⡿⠛⢁⣤⠶⠛⠋⠉⠉⠛⠻⢿⠤⡴⠞⠛⣉⣽⠿⠛⢉⣠⣤⣤⢤⣝⣿⣿⣿⣿⡄⢸⣿⡀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⢸⡏⠀⠰⠋⠁⠀⠀⢀⣤⣶⣾⣿⣿⡤⠤⠤⣾⣋⢀⡤⠞⢫⣿⣿⣿⣷⡄⠉⠻⣿⣿⡇⠀⣿⠁⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⢸⣇⠀⠀⠀⢀⣠⠾⢋⡵⢋⣿⢻⣿⠿⣶⡄⠀⢹⠋⠀⠀⣿⣿⢿⣧⣿⣿⣀⣠⣿⣿⠁⠀⣿⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⢸⣿⠀⠀⠚⠛⣷⡞⠋⠀⢸⣿⣟⢿⣶⣿⣧⣠⡿⣤⣤⣤⣽⡿⠿⠛⠛⢉⣿⣿⣿⡿⠀⠀⣿⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⠈⣿⠀⠀⠀⣀⣀⣉⣻⣶⣤⣿⣿⠿⠟⠛⠉⠁⠀⠀⠀⠀⣀⣀⣠⣤⠾⠋⢉⣿⣿⡇⠀⠀⣿⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⣿⡆⢠⣾⠋⠉⣩⣉⣉⣙⠛⠛⠛⠛⠛⠛⠛⠛⠛⢛⣉⣉⣡⣤⡶⠾⣿⣿⣿⠁⠀⢠⣿⡄⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⣇⠀⠀⢻⡇⠈⠛⠶⣦⣬⣉⣉⣉⣙⡛⠛⠛⠛⠛⠛⠛⠛⢛⣛⣉⣉⣉⣠⣤⣶⣿⠟⠁⠀⢀⣾⡿⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠘⣿⣤⡀⠀⠀⠈⠉⠉⠉⠛⠛⠛⠛⠛⠛⠛⠛⠋⠉⠉⠉⢁⣠⣼⣏⠀⣀⣴⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⠈⣿⣧⠀⠀⠘⢿⣿⢷⣦⣄⣀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⣀⣀⣠⣤⣶⠿⠛⠉⢨⣿⣿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⠀⠘⣿⣧⡀⠀⠀⣿⠀⠈⠉⠛⠛⠿⠿⠿⠿⠛⠛⠛⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠈⠛⢾⣢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"
	"⠀⠀⠀⠀⠀⠀⠀⠀⣾⡿⢿⣦⣴⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀\r\n";

	msg = ":doscord.irc KICK " + c.getName() + " " + nick + " " + reason + "\r\n";
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	msg = ":" + nick + ban;
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
}

void Server::ch_invite(std::string line, size_t cl_idx)
{
	/*TODO:
	/invite <nickname> #channel
	*/
}

void Server::ch_topic(std::string line, size_t cl_idx)
{
	/*
	/topic this is the new channel topic
	*/
	std::string chan;
	size_t pos;
	std::string topic;
	std::string complete_msg;

	std::cout << GREEN << "Entro ch_topic" << NO_COLOR << std::endl;
	chan = line.substr(0, line.find(" "));
	pos = line.find(":") + 1;
	topic = line.substr(pos, line.find("\r"));

	Channel &c = find_channel(chan);

	complete_msg = ":doscord TOPIC " + chan + " :" + topic + "\r\n";
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (c.isMember(_clients[i].getNick()))
			send(_polls[i + 1].fd, complete_msg.c_str(), complete_msg.size(), 0);
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
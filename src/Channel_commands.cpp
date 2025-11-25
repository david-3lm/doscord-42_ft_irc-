#include "Server.hpp"

void Server::ch_join(std::string line, size_t cl_idx)
{
	/*TODO: CREAR SERVER revisando que empiece por #
		/join #channel
	*/

	
	std::string chan = line.substr(0, line.find("\r"));
	std::cout << YELLOW << "JOIN\nEn line : [" << chan << "]" << NO_COLOR << std::endl;
	//TODO: no # no win
	Channel c = find_channel(chan);
	
	if (!c.isMember(_clients[cl_idx].getNick()))
	{
		c.addMember(_clients[cl_idx].getNick());
		c.addOperator(_clients[cl_idx].getNick());
		_channels.push_back(c);
	}

	std::string msg = ":" + _clients[cl_idx].getNick() + " JOIN " + c.getName() + "\r\n";
	send(_polls[cl_idx + 1].fd, msg.c_str(), msg.size(), 0);
	std::cout << "CHANNEL CREADO CON EL NOMBRE "<< chan<< std::endl;
}

void Server::ch_kick(std::string line, size_t pos, size_t cl_idx)
{
	/*TODO:
	echar a una persona de un canal
	comprobar que estas en el canal
	que existe el canal	
	*/


	std::string ban =
	"⡶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⠶⢶\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀    ⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡿⠿⠿⣿⣶⡄⠀⠀⠀⣼⣿⣿⣧⠀⠀⠀⠀⣿⣿⣿⡄⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣇⡀⣀⣼⣿⠇⠀⠀⢰⣿⡇⢹⣿⣆⠀⠀⠀⣿⡟⢻⣿⣆⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡿⠛⠻⢿⣷⣄⠀⢀⣿⣿⠀⠀⣿⣿⡄⠀⠀⣿⣷⠀⢻⣿⣆⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⣸⣿⡿⠀⣾⣿⠿⠿⠿⠿⣿⣿⡀⠀⣿⡟⠀⠀⢻⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠿⠿⠿⠿⠿⠛⠁⠸⠿⠏⠀⠀⠀⠀⠹⠿⠧⠀⠿⠿⠀⠀⠀⠻⠿⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\r\n"
	"⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨\r\n"
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
	// Channel c = find_channel(chan);
	int chIdx;

	for (int i = 0; i< _channels.size(); i++)
	{
		if(_channels[i].getName() == chan)
			chIdx = i;
	}

	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << "Envio msg a: -" << _clients[i].getNick() << "-" << std::endl;
		if (_channels[chIdx].isMember(_clients[i].getNick()) && i != cl_idx)
		{
			complete_msg = ":" + _clients[cl_idx].getNick() + " " + "PRIVMSG" + " "+ chan + " " + msg + "\r\n";
			std::cout << "Complete = " << complete_msg << std::endl;
			send(_polls[i + 1].fd, complete_msg.c_str(), complete_msg.size(), 0);
		}

	}
	
}
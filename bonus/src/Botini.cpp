#include "Botini.hpp"


#define _realname "MANUEH"
#define PONG "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣠⣤⣤⣄⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠤⠖⠊⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠙⠲⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⡤⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⡜⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢢⠀⠀⠀⠀⠀⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⣸⠁⠀⠀⠀⠀⠀⠀⠀⠱⡀⠀⠀⠀⠀⠀⠀⠀⡀⠈⠀⡀⠀⠀⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⡏⠀⠀⠀⠀⠀⠀⠀⠀⡰⠁⠀⠀⠀⠀⠀⠀⠀⠘⡆⡜⠁⠀⠀⠀⠀⢧⡀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠸⡀⠀⠀⠀⠀⠀⣀⣤⡂⠀⠇⠱⠀⡀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⢇⠀⠀⠀⠀⠀⠀⠀⠀⠈⢄⡀⢠⣟⢭⣥⣤⠽⡆⠀⡶⣊⣉⣲⣤⢀⡞⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠘⣆⠀⠀⠀⠀⠀⠀⡀⠀⠐⠂⠘⠄⣈⣙⡡⡴⠀⠀⠙⣄⠙⣛⠜⠘⣆⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠈⢦⡀⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠄⠊⠀⠀⠀⠀⡸⠛⠀⠀⠀⢸⠆⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⠦⢄⣘⣄⠀⠀⠀⠀⠀⠀⠀⡠⠀⠀⠀⠀⣇⡀⠀⠀⣠⠎⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠁⠈⡟⠒⠲⣄⠀⠀⡰⠇⠖⢄⠀⠀⡹⡇⢀⠎⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡇⠀⠀⡇⠀⠀⠹⠀⡞⠀⠀⢀⠤⣍⠭⡀⢱⢸⠀⠀⠀⠀pong⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⢀⣀⣀⣠⠞⠀⠀⢠⡇⠀⠀⠀⠀⠁⠀⢴⠥⠤⠦⠦⡼⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⣀⣤⣴⣶⣿⣿⡟⠁⠀⠋⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠀⠑⣠⢤⠐⠁⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠬⠥⣄⠀⠀⠈⠲⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠙⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠈⢳⠀⠀⢀⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀\n⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠒⠦⠤⢤⣄⣀⣠⠤⢿⣶⣶⣿⣿⣿⣶⣤⡀⠀⠀⠀⠀⠀\n⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⠁⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀⠀⠀\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣤⣤⣤⣤⣤⣤⣤⣤⣤⣴⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀\n";


#define CMD_PASS(password) std::string("PASS ") + password + "\r\n"
#define CMD_NICK(nickname) std::string("NICK ") + nickname + "\r\n"
#define CMD_USER(username, realname) std::string("USER ") + username + " 0 * " + realname + "\r\n"
#define CMD_JOIN(channel) std::string("JOIN ") + channel + "\r\n"
#define CMD_PRIVMSG(target, message) std::string("PRIVMSG ") + target + " :" + std::string(message) + "\r\n"



void Botini::onConnect()
{
	std::cout << GREEN"CONECTAMOS :D" << NO_COLOR<< std::endl;

}

void Botini::onDisconnect()
{
	std::cout << RED"DESCONECTAMOS :(" << NO_COLOR<< std::endl;
}

void Botini::sendMsg(std::string msg)
{
	//std::cout<< BLUE << "Envio => [" << msg <<"]" << NO_COLOR << std::endl;
	send(_socket_fd, msg.c_str(), msg.size(), 0);
}

Botini::Botini(std::string h, std::string p, std::string pass) : _host(h), _port(p), _pass(pass)
{
	_nick = "Manolo";

}

Botini::~Botini()
{
}

void Botini::botConnect()
{
	sockaddr_in addr;
	std::stringstream ss(_port);
	int p;
	ss >> p;


	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
	{
		std::cerr << RED "ERROOOOOOOOOR"<< NO_COLOR<< std::endl;
		return ;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(p);
	if (inet_pton(AF_INET, _host.c_str(), &addr.sin_addr) <= 0)
	{
		std::cerr << RED "MALA IP" << NO_COLOR <<std::endl;
		return ;
	}

	if (connect(_socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		std::cerr << RED "NO ME PUDE CONECTAR"<< NO_COLOR << std::endl;
		return ;
	}

	std::cout << GREEN << "Conectado al servidor :D " << NO_COLOR << std::endl;

	sendMsg(CMD_PASS(_pass));
	sendMsg(CMD_NICK(_nick));
	sendMsg(CMD_USER(_nick, _realname));
	usleep(3000);
	sendMsg(CMD_JOIN("#bot"));
	readJSON();
	_pokeInt = 1;
	_pokeChan = "";
	_pokeActive = false;
}

void Botini::sendAnswer(std::string msg, std::string chan)
{
				std::string ascii =
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣠⣤⣤⣄⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠤⠖⠊⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠙⠲⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⡤⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⡜⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢢⠀⠀⠀⠀⠀⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⣸⠁⠀⠀⠀⠀⠀⠀⠀⠱⡀⠀⠀⠀⠀⠀⠀⠀⡀⠈⠀⡀⠀⠀⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⡏⠀⠀⠀⠀⠀⠀⠀⠀⡰⠁⠀⠀⠀⠀⠀⠀⠀⠘⡆⡜⠁⠀⠀⠀⠀⢧⡀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠸⡀⠀⠀⠀⠀⠀⣀⣤⡂⠀⠇⠱⠀⡀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢇⠀⠀⠀⠀⠀⠀⠀⠀⠈⢄⡀⢠⣟⢭⣥⣤⠽⡆⠀⡶⣊⣉⣲⣤⢀⡞⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠘⣆⠀⠀⠀⠀⠀⠀⡀⠀⠐⠂⠘⠄⣈⣙⡡⡴⠀⠀⠙⣄⠙⣛⠜⠘⣆⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠈⢦⡀⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠄⠊⠀⠀⠀⠀⡸⠛⠀⠀⠀⢸⠆⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⠦⢄⣘⣄⠀⠀⠀⠀⠀⠀⠀⡠⠀⠀⠀⠀⣇⡀⠀⠀⣠⠎⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠁⠈⡟⠒⠲⣄⠀⠀⡰⠇⠖⢄⠀⠀⡹⡇⢀⠎⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡇⠀⠀⡇⠀⠀⠹⠀⡞⠀⠀⢀⠤⣍⠭⡀⢱⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⢀⣀⣀⣠⠞⠀⠀⢠⡇⠀⠀⠀⠀⠁⠀⢴⠥⠤⠦⠦⡼⠀⢸⠀⠀"+msg+" ⠀⠀⠀⠀\n"
"⣀⣤⣴⣶⣿⣿⡟⠁⠀⠋⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠀⠑⣠⢤⠐⠁⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠬⠥⣄⠀⠀⠈⠲⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠙⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠈⢳⠀⠀⢀⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠒⠦⠤⢤⣄⣀⣠⠤⢿⣶⣶⣿⣿⣿⣶⣤⡀⠀⠀⠀⠀⠀\n"
"⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⠁⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀⠀⠀\n"
"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣤⣤⣤⣤⣤⣤⣤⣤⣤⣴⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀\n";
	std::stringstream ss(ascii);
	std::string m;


	while(std::getline(ss, m))
	{
		sendMsg(CMD_PRIVMSG(chan, m) + "\r\n");
	}
}

void Botini::update()
{
	while (true)
	{
		int readed_count = 0;
		char buff[1025];

		readed_count = read(_socket_fd, buff, 1024);
		if (readed_count == 0)
		{
			close(_socket_fd);
			onDisconnect();
			return;
		}
		else if (readed_count < 0)
		{
			close(_socket_fd);
			onDisconnect();
		}

		buff[readed_count + 1] = '\0';
		std::cout << buff << std::flush;

		if (std::string(buff).find("PRIVMSG #bot !ping") != std::string::npos)
		{
			try {
				sendAnswer("Pong!", "#bot");
				
			} catch (std::exception &e) {
				close(_socket_fd);
				onDisconnect();
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}

		else if (std::string(buff).find("#bot") != std::string::npos)
		{
			try {
				std::deque<std::string> _replies;
				_replies.push_back("¡Aquí estoy!");
				_replies.push_back("¿Me llamabas?");
				_replies.push_back("¿Qué pasa?");
				_replies.push_back("Estoy vivo.");
				_replies.push_back("✨ beep boop ✨");
				_replies.push_back("¿Sí?");
				int r = rand() % _replies.size();
				sendAnswer(_replies[r], "#bot");
				
			} catch (std::exception &e) {
				close(_socket_fd);
				onDisconnect();
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
		else if (std::string(buff).find("INVITE") != std::string::npos)
		{
			size_t it = std::string(buff).find("#");
			size_t it2 = std::string(buff).find("\r");
			std::string chan = std::string(buff).substr(it, it2 - it);
			sendMsg(CMD_JOIN(chan));
			usleep(3000);
			sendAnswer("¿Me llamabas?", chan);
		}
		else if (std::string(buff).find(":.") != std::string::npos)
		{
			size_t it = std::string(buff).find("#");
			size_t it2 = std::string(buff).find(" :");
			std::string chan = std::string(buff).substr(it, it2 - it);

			
			size_t it_final = std::string(buff).find("\r");
			std::string com = std::string(buff).substr(it, it_final - it);
			com = com.substr(com.find(":") + 1, it_final - com.find(":") + 1);

			if (com[0] == '.')
				sendCommand(&com[1], chan);
		}
		else if (_pokeActive)
		{
			if (std::string(buff).find(_mapPoke[_pokeInt]) != std::string::npos)
			{
				sendAnswer("POKEMON ACERTADO!", _pokeChan);
				_pokeActive = false;
			}
		}
	}	
}

void Botini::sendCommand(std::string cmd, std::string chan)
{
	if (cmd == "weather")
		sendAnswer("El día está brillante porque ya estoy aquí!", chan);
	else if (cmd == "ping")
		sendAnswer("Pong!", chan);
	else if (cmd == "cat")
		sendAnswer("nya... 🐱", chan);
	else if (cmd == "dog")
		sendAnswer("Woof! 🐶", chan);
	else if (cmd == "pokemon")
		startPokeQuiz(chan);
	//TODO: lo que se te ocurra. Vuelvete loco
}


static std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

void Botini::startPokeQuiz(std::string chan)
{
	_pokeActive = true;
	_pokeChan = chan;
	srand(time(0));

	_pokeInt = rand() % _mapPoke.size() + 1;
	if (_pokeInt == 0)
		_pokeInt++;
	std::stringstream ss;

	ss<< _pokeInt;
	std::string msg = "¿Cuál es este Pokémon? " + ss.str();
	sendAnswer(msg, chan);
}

void Botini::readJSON()
{
    std::ifstream file("./include/pokemon.json");
    if (!file.is_open())
        return;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    size_t pos = 0;
    while ((pos = content.find("\"index\"", pos)) != std::string::npos)
    {
        size_t pos_dd = content.find(":", pos);
        size_t comma = content.find(",", pos_dd);
        int index = std::atoi(content.substr(pos_dd + 1, comma - pos_dd - 1).c_str());

        size_t namePos = content.find("\"name\"", comma);
        pos_dd = content.find(":", namePos);
        size_t quote1 = content.find("\"", pos_dd + 1);
        size_t quote2 = content.find("\"", quote1 + 1);
        std::string name = content.substr(quote1 + 1, quote2 - quote1 - 1);

        _mapPoke[index] = name;
        pos = quote2;
    }
}
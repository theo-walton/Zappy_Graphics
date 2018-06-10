#pragma once

#include "zappy.hpp"
#include "Player.hpp"
#include "Egg.hpp"
#include "Sound.hpp"
#include "Time.hpp"
#include "Grid.hpp"
#include "ServerMonitor.hpp"

class	Map
{
	std::map<std::string, std::function<void(std::string)>> _events;
	std::vector<Player*> _players;
	std::vector<Egg*> _eggs;
	std::vector<Sound*> _sounds;
	glm::vec2 _size;
	std::vector<std::vector<std::vector<int>>> _resources;
	double _timeUnit;
	Grid _grid;
	ServerMonitor _serverMonitor;
	
	static const glm::vec2 _directions[4];
	
	Player	*getPlayer(int ID);
	Egg	*getEgg(int ID);
	
public:
	Map(int fd);
	~Map(void);

	void	Render(std::pair<glm::mat4, glm::mat4> perspective, double dt);
};

#include "Player.hpp"

Model	*Player::_partyModel = nullptr;
Model	*Player::_normalModel = nullptr;

std::list<Player*> Player::_players;
std::map<uint64_t, std::list<Player*>> Player::_staticPlayers;

Sprite2D	*Player::_guiBackground = nullptr;

static uint64_t	keyHash(glm::vec2 pos)
{
	uint64_t out;

	((float*)&out)[0] = (uint32_t)glm::round(pos.x);
	((float*)&out)[1] = (uint32_t)glm::round(pos.y);
	return out;
}

void	Player::stopMoving(void)
{
	//add itself to the list of static players and call spaceOutPlayers()

	uint64_t key = keyHash(_pos);

	if (_staticPlayers.count(key) == 0)
	{
		std::list<Player*> l = {this};
		_staticPlayers[key] = l;
	}
	else
	{
		std::list<Player*> &l = _staticPlayers[key];
		l.push_back(this);
	}
	spaceOutPlayers(key);
	_moving = false;
}

void	Player::startMoving(void)
{
	//remove itself from list of static players and call spaceOutPlayers() and set own height to 0

	uint64_t key = keyHash(_pos);

	if (_staticPlayers.count(key) == 0) //can happen if player move 2 time before updata
		return;
	else
	{
		std::list<Player*> &l = _staticPlayers[key];
		auto iter = std::find(l.begin(), l.end(), this);
		assert(iter != l.end());
		l.erase(iter);
		if (l.empty())
			_staticPlayers.erase(key);
	}
	spaceOutPlayers(key);
	_height = 0;
	_moving = true;
}

void	Player::spaceOutPlayers(uint64_t key)
{
	if (_staticPlayers.count(key) == 0)
		return;
	
	std::list<Player*>& list = _staticPlayers[key];
	float i = 0;

	for (auto p : list)
	{
		p->_height = i;
		i++;
	}
}

void	Player::RayCastGUI(glm::vec3 rayPoint, glm::vec3 rayDir)
{
	std::vector<Player*> candidates;
	for (auto p : _players)
	{
		p->_guiActive = false;
		glm::vec3 pos(p->_modelPos.x, p->_height * 0.5 + 0.25, -p->_modelPos.y);
		float dist = glm::length(glm::cross(rayDir, pos - rayPoint));

		if (dist < 0.3)
			candidates.push_back(p);
	}
	//find closest candidate to rayPoint

	Player *playerHit = nullptr;
	float minDist = 10000000;
	
	for (auto p : candidates)
	{
		glm::vec3 v = glm::vec3(p->_modelPos.x, p->_height * 0.5, -p->_modelPos.y) - rayPoint;
		if (glm::dot(v, rayDir) < 0) // ray intersection happened behind ray
			continue;
		float dist = glm::length(v);
		if (dist < minDist)
		{
			dist = minDist;
			playerHit = p;
		}
	}
	if (playerHit == nullptr) // no player was hit by the ray
		return;

	playerHit->_guiActive = true;
}

void	Player::DrawGUI(Window& window)
{
	//draw GUI box:

	window.SetRenderMask(0.8, 0.5, 0.2, 0.5);	
	_guiBackground->Render();

	window.SetRenderMask(0.82, 0.95, 0.08, 0.05);
	Text food("Food:");
	food.Render(window.GetAspect());

	window.SetRenderMask(0.9, 0.95, 0.08, 0.05);
	Text foodAmount(std::to_string(_resources[0]));
	foodAmount.Render(window.GetAspect());

	window.SetRenderMask(0.82, 0.9, 0.08, 0.05);
	Text linemate("Linemate:");
	linemate.Render(window.GetAspect());
	
	window.SetRenderMask(0.9, 0.9, 0.08, 0.05);
	Text linemateAmount(std::to_string(_resources[1]));
	linemateAmount.Render(window.GetAspect());

	window.SetRenderMask(0.82, 0.85, 0.08, 0.05);
	Text deraumere("Deraumere:");
	deraumere.Render(window.GetAspect());

	window.SetRenderMask(0.9, 0.85, 0.08, 0.05);
	Text deraumereAmount(std::to_string(_resources[2]));
	deraumereAmount.Render(window.GetAspect());

	window.SetRenderMask(0.82, 0.8, 0.08, 0.05);
	Text sibur("Sibur:");
	sibur.Render(window.GetAspect());

	window.SetRenderMask(0.9, 0.8, 0.08, 0.05);
	Text siburAmount(std::to_string(_resources[3]));
	siburAmount.Render(window.GetAspect());

	window.SetRenderMask(0.82, 0.75, 0.08, 0.05);
	Text mendiane("Mendiane:");
	mendiane.Render(window.GetAspect());

	window.SetRenderMask(0.9, 0.75, 0.08, 0.05);
	Text mendianeAmount(std::to_string(_resources[4]));
	mendianeAmount.Render(window.GetAspect());

	window.SetRenderMask(0.82, 0.7, 0.08, 0.05);
	Text phirus("Phirus:");
	phirus.Render(window.GetAspect());

	window.SetRenderMask(0.9, 0.7, 0.08, 0.05);
	Text phirusAmount(std::to_string(_resources[5]));
	phirusAmount.Render(window.GetAspect());

	window.SetRenderMask(0.82, 0.65, 0.08, 0.05);
	Text thystame("Thystame:");
	thystame.Render(window.GetAspect());

	window.SetRenderMask(0.9, 0.65, 0.08, 0.05);
	Text thystameAmount(std::to_string(_resources[6]));
	thystameAmount.Render(window.GetAspect());
	
	window.RemoveRenderMask();
}

void	Player::RenderGUI(Window& window)
{
	for (auto p : _players)
		if (p->_guiActive)
			p->DrawGUI(window);
}

Player::Player(glm::vec2 pos, glm::vec2 dir, const std::string& name, int ID, int level, glm::vec2 mapSize) :
_pos(pos),
_dir(dir),
_ritualTime(0),
_level(level),
_teamName(name),
_ID(ID),
_mapSize(mapSize)
{
	_resources.resize(7);
	_modelPos = _pos;
	_modelDir = _dir;
	_moveTime = 0;
	_modelDirChange = glm::vec2(0, 0);
	stopMoving();
	if (!_partyModel)
		_partyModel = new Model("assets/party_player.model");
	if (!_normalModel)
		_normalModel = new Model("assets/normal_player.model");
	_model = _normalModel;
	
	_guiActive = false;
	if (!_guiBackground)
		_guiBackground = new Sprite2D("assets/textures/gui_background.png");
	
	_players.push_back(this);
}

Player::~Player(void)
{
	_players.remove(this);
	
	uint64_t key = keyHash(_modelPos);
	if (_staticPlayers.count(key) == 0)
		return;
	std::list<Player*> &l = _staticPlayers[key];
	auto iter = std::find(l.begin(), l.end(), this);
	if (iter != l.end())
		l.erase(iter);
	if (l.empty())
		_staticPlayers.erase(key);
	spaceOutPlayers(key);
}

void	Player::MoveTo(glm::vec2 pos)
{
	if (pos != _pos)
	{
		startMoving();
		glm::vec2 moveDir;
		if (pos == glm::mod(_pos + glm::vec2(1, 0), _mapSize))
			moveDir = glm::vec2(1, 0);
		else if (pos == glm::mod(_pos + glm::vec2(0, 1), _mapSize))
			moveDir = glm::vec2(0, 1);
		else if (pos == glm::mod(_pos + glm::vec2(-1, 0), _mapSize))
			moveDir = glm::vec2(-1, 0);
		else if (pos == glm::mod(_pos + glm::vec2(0, -1), _mapSize))
			moveDir = glm::vec2(0, -1);
		else
			assert(0 && "player movement was invalid");
		
		_pos = pos;
		_moveTime = 1;
		_moveDir = moveDir;
	}
}

void	Player::SetDir(glm::vec2 dir)
{
	if (dir != _dir)
	{
		assert(glm::dot(dir, _dir) == 0 && "player did not turn 90 degrees");
		
		_dir = dir;
		_modelDirChange = _dir - _modelDir;
	}
}

void	Player::UpdateResources(std::vector<int> resources)
{
	assert(resources.size() == 7);
	for (int i = 0; i < 7; i++)
	{
		assert(resources[i] >= 0);
		_resources[i] = resources[i];
	}
}

void	Player::BeginRitual(void)
{
	_ritualTime = 50;
}

void	Player::SetLevel(int level)
{
	_level = level;
	assert(level >= 0 && level <= 8);
}

const std::string&	Player::Name(void)
{
	return _teamName;
}

int	Player::ID(void)
{
	return _ID;
}

glm::vec2	Player::GetPosition(void)
{
	return _pos;
}

void	Player::Update(double dt)
{
	if (_moveTime > 0)
	{
		glm::vec2 newpos = _modelPos + _modelDir * dt;
		newpos = glm::mod(newpos, _mapSize);
		_modelPos = newpos;
		_moveTime -= dt;
		if (_moveTime < 0)
		{
			_moveTime = 0;
			_modelPos = _pos;
			stopMoving();
		}
	}

	if (_modelDirChange.x || _modelDirChange.y)
	{
		_modelDir += _modelDirChange * dt;
		glm::vec2 dirError = _dir - _modelDir;
		if (glm::dot(dirError, _modelDirChange) < 0)
		{
			_modelDir = _dir;
			_modelDirChange = glm::vec2(0, 0);
		}
	}

	if (_ritualTime > 0)
	{
		_ritualTime -= dt;
		_model = _partyModel;		
	}
	else
		_model = _normalModel;
}

void	Player::Render(std::pair<glm::mat4, glm::mat4> perspective)
{
	float angle = acos(glm::dot(_modelDir, glm::vec2(1, 0)));
	
	if (glm::dot(_modelDir, glm::vec2(0, 1)) < 0)
	{
		angle = 2 * M_PI - angle;
	}

	glm::mat4 rot = glm::rotate(angle, glm::vec3(0, 1, 0));

	glm::vec3 outlineCol;
	if (_guiActive)//_ritualTime > 0)
		outlineCol = glm::vec3(0.7, 0.2, 0.2);
	else
		outlineCol = glm::vec3(0, 0, 0);

	_model->Render(perspective,
		       rot,
		       glm::vec3(_modelPos.x, _height * 0.5, -_modelPos.y),
		       outlineCol,
		       0.02);

	Character3D::AddToBuffer(glm::vec3(_modelPos.x ,
					   _height * 0.5 + 0.6,
					   -_modelPos.y),
				 glm::vec3(1, 0, 0),
				 0.07,
				 _level + '0');
}

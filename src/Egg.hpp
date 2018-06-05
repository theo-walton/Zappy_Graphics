#pragma once

#include "zappy.hpp"
#include "Model.hpp"

class	Egg
{
	glm::vec2 _pos;
	bool _hatched;

	Model *_model;
	
public:
	Egg(glm::vec2 pos);
	~Egg(void);

	void	Crack(void);

	void	Render(std::pair<glm::mat4, glm::mat4> perspective);
};

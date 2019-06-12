#pragma once
class Game
{
public:
	Game();
	~Game();
	int run();
	void stop();
private:
	bool active;
};


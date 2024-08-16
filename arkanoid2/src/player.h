#pragma once

class Player : public Entity
{
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;
private:

};
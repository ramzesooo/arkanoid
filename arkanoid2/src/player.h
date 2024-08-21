#pragma once
#include "perk.h"

class Player : public Entity
{
public:
	Player();

	void Update() override;
	void Draw() override;

	void SetAffect(PerkTypes perkType);

	const PerkTypes& GetAffect() const { return m_Affect; }
	void CancelAffect();
private:
	static constexpr SDL_Rect player_source{ 0, 0, 64, 32 };

	// Used to count passed time after catching appropriate perk
	//double m_StartTime = NULL;
	//double m_EndTime = NULL;
	uint32_t m_EndTime = NULL;

	PerkTypes m_Affect = PerkTypes::none;
};
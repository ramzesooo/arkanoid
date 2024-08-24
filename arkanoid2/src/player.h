#pragma once
#include "entity.h"
#include "perk.h"

class Player : public Entity
{
public:
	Player();

	void Update() override;
	void Draw() override;

	void SetAffect(PerkType perkType);

	const PerkType& GetAffect() const { return m_Affect; }
	void CancelAffect();

	void SetInvisible() { m_Invisible = !m_Invisible; }
	bool GetInvisible() const { return m_Invisible; }
private:
	static constexpr SDL_Rect player_source{ 0, 0, 64, 32 };

	// Used to count passed time after catching appropriate perk
	//double m_StartTime = NULL;
	//double m_EndTime = NULL;
	uint32_t m_EndTime = NULL;

	PerkType m_Affect = PerkType::none;

	bool m_Invisible = false;
};
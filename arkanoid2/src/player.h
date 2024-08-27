#pragma once
#include "entity.h"
#include "perk.h"

class Player : public Entity
{
public:
	Player();

	void Update() override;
	void Draw() override;
private:
	static constexpr uint32_t s_OriginalWidth = 64;
	static constexpr uint32_t s_OriginalHeight = 32;
	static constexpr SDL_Rect player_source{ 0, 0, 64, 32 };
// AFFECTS
public:
	void SetAffect(PerkType perkType);
	const PerkType& GetAffect() const { return m_Affect; }
	void CancelAffect();
private:
	PerkType m_Affect = PerkType::none;

	// Used to count passed time after catching appropriate perk
	uint32_t m_EndTime = NULL;
// INVISIBLE
public:
	void SwitchInvisible() { m_Invisible = !m_Invisible; }
	bool GetInvisible() const { return m_Invisible; }
private:
	bool m_Invisible = false;
};
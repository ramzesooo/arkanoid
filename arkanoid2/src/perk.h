#pragma once
#include "entity.h"

#include <string_view>

enum class PerkType
{
	none = 0,
	shrink,
	supersize,
	addball,
	duplicateball,
	floor,
	size
};

class Perk : public Entity
{
public:
	Perk(std::string_view textureID, float startX, float startY, PerkType perkType);

	void Update() override;
	void Draw() override;

	const PerkType& GetType() const { return m_PerkType; }
private:
	PerkType m_PerkType;
	std::string_view m_TextureID;
	static constexpr SDL_Rect perk_source{ 0, 0, 64, 64 };
};
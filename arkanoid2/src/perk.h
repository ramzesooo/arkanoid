#pragma once
#include "entity.h"

#include <string>

enum class PerkType
{
	none = 0,
	shrink,
	supersize,
	addball,
	duplicateball,
	size
};

class Perk : public Entity
{
public:
	Perk(const std::string_view& textureID, float startX, float startY, PerkType perkType);

	void Update() override;
	void Draw() override;

	const PerkType& GetType() const { return m_PerkType; }
private:
	PerkType m_PerkType;
	std::string m_TextureID = "";
	static constexpr SDL_Rect perk_source{ 0, 0, 64, 64 };
};
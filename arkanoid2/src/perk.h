#pragma once

class Entity;

enum class PerkTypes
{
	none = 0,
	shrink,
	supersize,
	addball,
	duplicateball,
	unknownperk
};

class Perk : public Entity
{
public:
	Perk(const std::string& textureID, float startX, float startY, PerkTypes perkType);
	~Perk();

	void Update() override;
	void Draw() override;

	const PerkTypes& GetType() const { return m_PerkType; }
private:
	PerkTypes m_PerkType;
	std::string m_TextureID;
	static constexpr SDL_Rect perk_source{ 0, 0, 64, 64 };
};
#define _CRT_SECURE_NO_WARNINGS
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include <ctime>
background::background(sf::Texture& tex) : texture(tex)
{
	this->texture.setRepeated(true);
	this->rect.width = 800;
	this->rect.height = 600;
	this->sprite.setTextureRect(rect);
	this->sprite.setTexture(this->texture);
}
void background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprite, states);
}
Field::Field()
{
	this->rec.setSize(sf::Vector2f(209, 419));
	this->rec.setPosition(sf::Vector2f(50, 50));
	this->rec.setOutlineThickness(2);
	this->rec.setOutlineColor(sf::Color::White);
	this->rec.setFillColor(sf::Color(0, 133, 173));
	this->Gor_Lines.setPrimitiveType(sf::PrimitiveType::Lines);
	this->Gor_Lines.resize(38);
	for (int i = 0; i < 38; i += 2)
	{
		this->Gor_Lines[i].position = sf::Vector2f(50, 20 + 50 + (10.5 * i));
		this->Gor_Lines[i].color = sf::Color::White;
		this->Gor_Lines[i + 1].position = sf::Vector2f(50 + 209, 21 + 50 + (10.5 * i));
		this->Gor_Lines[i + 1].color = sf::Color::White;
	}
	this->Ver_Lines.setPrimitiveType(sf::PrimitiveType::Lines);
	this->Ver_Lines.resize(18);
	for (int i = 0; i < 18; i += 2)
	{
		this->Ver_Lines[i].position = sf::Vector2f(71 + (i * 10.5), 50);
		this->Ver_Lines[i].color = sf::Color::White;
		this->Ver_Lines[i + 1].position = sf::Vector2f(71 + (i * 10.5), 50 + 419);
		this->Ver_Lines[i + 1].color = sf::Color::White;
	}
}
void Field::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(this->rec);
	target.draw(this->Ver_Lines, states);
	target.draw(this->Gor_Lines, states);

	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		if ((*It).Ypos <= 19)
		{
			sf::Sprite sprt;
			sprt.setPosition(sf::Vector2f(50 + 21 * (*It).Xpos, 51 + 398 - 21 * (*It).Ypos));
			sprt.setTexture(Tile_Textures[(*It).color]);
			sprt.setScale(sf::Vector2f(2.f, 2.f));
			target.draw(sprt, states);
		}
	}
	for (auto It = points.begin(); It != points.end(); It++)
	{
		if ((*It).Ypos <= 19)
		{
			sf::Sprite sprt;
			sprt.setPosition(sf::Vector2f(50 + 21 * (*It).Xpos, 51 + 398 - 21 * (*It).Ypos));
			sprt.setTexture(Tile_Textures[(*It).color]);
			sprt.setScale(sf::Vector2f(2.f, 2.f));
			target.draw(sprt, states);
		}
	}
}

Next::Next()
{
	this->rec.setSize(sf::Vector2f(104, 104));
	this->rec.setPosition(sf::Vector2f(350, 50));
	this->rec.setOutlineThickness(2);
	this->rec.setOutlineColor(sf::Color::White);
	this->rec.setFillColor(sf::Color(0, 133, 173));
	this->Gor_Lines.setPrimitiveType(sf::PrimitiveType::Lines);
	this->Gor_Lines.resize(8);
	for (int i = 0; i < 8; i += 2)
	{
		this->Gor_Lines[i].position = sf::Vector2f(351, 71 + 10.5 * i);
		this->Gor_Lines[i].color = sf::Color::White;
		this->Gor_Lines[i + 1].position = sf::Vector2f(351 + 104, 71 + 10.5 * i);
		this->Gor_Lines[i + 1].color = sf::Color::White;
	}
	this->Ver_Lines.setPrimitiveType(sf::PrimitiveType::Lines);
	this->Ver_Lines.resize(8);
	for (int i = 0; i < 8; i += 2)
	{
		this->Ver_Lines[i].position = sf::Vector2f(371 + 10.5 * i, 50);
		this->Ver_Lines[i].color = sf::Color::White;
		this->Ver_Lines[i + 1].position = sf::Vector2f(371 + 10.5 * i, 50 + 104);
		this->Ver_Lines[i + 1].color = sf::Color::White;
	}
	srand(time(NULL));
	this->Add_Fig(1 + rand() % 7, rand() % 4);
}
void Next::Add_Fig(int type, int color)
{
	this->next.clear();
	this->next.push_back({ StX, StY, true, color });
	auto It = Fig_map.find(type);
	for (auto iterator = (*It).second.begin(); iterator != (*It).second.end(); iterator++)
	{
		this->next.push_back({ StX + (*iterator)[0], StY + (*iterator)[1], false, color });
	}
}
std::vector<Points> Next::Get_Figure()
{
	return next;
}
void Next::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(this->rec);
	target.draw(this->Ver_Lines, states);
	target.draw(this->Gor_Lines, states);
	for (auto It = this->next.begin(); It != this->next.end(); It++)
	{
		sf::Sprite sprt;
		sprt.setPosition(sf::Vector2f(350 + 42 + 21 * ((*It).Xpos - StX), 50 + 42 - 21 * ((*It).Ypos - StY)));
		sprt.setTexture(Tile_Textures[(*It).color]);
		sprt.setScale(sf::Vector2f(2.f, 2.f));
		target.draw(sprt, states);
	}
}
GOWin::GOWin()
{
	this->rec.setSize(sf::Vector2f(400, 200));
	this->rec.setPosition(sf::Vector2f(200, 150));
	this->rec.setFillColor(sf::Color::Red);
	this->rec.setOutlineThickness(2);
	this->rec.setOutlineColor(sf::Color::Black);
	if (!this->font.loadFromFile("font.ttf"))
		std::cout << "Font open error" << std::endl;
	this->text[0].setCharacterSize(24);
	this->text[0].setPosition(sf::Vector2f(350, 170));
	this->text[0].setString("You Lose!");
	this->text[0].setFont(this->font);
	this->text[1].setCharacterSize(16);
	this->text[1].setPosition(sf::Vector2f(285, 250));
	this->text[1].setString("Press 'Y' to restart or 'N' to exit");
	this->text[1].setFont(this->font);
}
void GOWin::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(this->rec, states);
	for (int i = 0; i < 2; i++)
	{
		target.draw(this->text[i]);
	}
}
Interface::Interface()
{
	this->rec.setSize(sf::Vector2f(180, 150));
	this->rec.setPosition(sf::Vector2f(500, 50));
	this->rec.setFillColor(sf::Color(0, 133, 173));
	this->rec.setOutlineThickness(2);
	this->rec.setOutlineColor(sf::Color::White);
	if (!this->font.loadFromFile("font.ttf"))
		std::cout << "Font open error" << std::endl;
	char buffer[5];
	this->text[0].setCharacterSize(26);
	this->text[0].setPosition(sf::Vector2f(510, 70));
	this->text[0].setString("Score:");
	this->text[0].setFont(this->font);
	this->text[1].setCharacterSize(26);
	this->text[1].setPosition(sf::Vector2f(510, 135));
	this->text[1].setString("Speed:");
	this->text[1].setFont(this->font);
	this->text[2].setCharacterSize(26);
	this->text[2].setPosition(sf::Vector2f(595, 70));
	this->text[2].setString(_itoa(Score, buffer, 10));
	this->text[2].setFont(this->font);
	this->text[3].setCharacterSize(26);
	this->text[3].setPosition(sf::Vector2f(595, 135));
	this->text[3].setString(_itoa(Speed, buffer, 10));
	this->text[3].setFont(this->font);
}
void Interface::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(this->rec);
	for (int i = 0; i < 4; i++)
	{
		target.draw(this->text[i]);
	}
}
void Interface::update()
{
	char buffer[5];
	this->text[2].setString(_itoa(Score, buffer, 10));
	this->text[3].setString(_itoa(Speed, buffer, 10));
}

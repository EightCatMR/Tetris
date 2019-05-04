#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
extern struct Points
{
	int Xpos;
	int Ypos;
	bool Major;
	int color;
};
extern std::vector<Points> points;
extern std::vector<Points> InUse;
extern std::vector<sf::Texture> Tile_Textures;
extern std::map<int, std::vector<std::vector<int>>> Fig_map;
extern unsigned int Score;
extern unsigned int Speed;
const int StX = 5;
const int StY = 22;
class background : public sf::Drawable
{
public:
	background(sf::Texture& tex);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::IntRect rect;
	sf::Sprite sprite;
	sf::Texture texture;
};
class Field : public sf::Drawable
{
public:
	Field();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::RectangleShape rec;
	sf::VertexArray Gor_Lines;
	sf::VertexArray Ver_Lines;
};
class Next : public sf::Drawable
{
public:
	Next();
	void Add_Fig(int, int);
	std::vector<Points> Get_Figure();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::RectangleShape rec;
	sf::VertexArray Gor_Lines;
	sf::VertexArray Ver_Lines;
	std::vector<Points> next;
};
class GOWin : public sf::Drawable
{
public:
	GOWin();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::RectangleShape rec;
	sf::Font font;
	sf::Text text[2];
};
class Interface : public sf::Drawable
{
public:
	Interface();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update();
private:
	sf::RectangleShape rec;
	sf::Font font;
	sf::Text text[4];
};
#endif // !_GRAPHICS_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Graphics.h"
#include <ctime> // for random seed choice
sf::RenderWindow Win(sf::VideoMode(800, 600), "Tetris 1.0");
struct Points; //defined in graphics.h (may be it's don't right aproach to define extern variable)
std::vector<Points> points; //all tiles except tiles which you handle in current time
std::vector<Points> InUse; //tiles that current figure uses
std::vector<sf::Texture> Tile_Textures;
std::map<int, std::vector<std::vector<int>>> Fig_map; //all our figure's patterns stores here
unsigned int Score; //Score =)
unsigned int Speed; //Speed =)
float fall_time = 1; //period of one step
void Read_Fig() //function that read patterns from .txt file 
{
	std::ifstream file("points.txt");
	if (!file.good())
	{
		std::cout << "file access error" << std::endl; exit(0);
	}
	char buffer[10];
	while (!file.eof())
	{
		std::vector<std::vector<int>> Vector;
		file.getline(buffer, 20, ' ');
		if (strcmp(buffer, "type") == 0)
		{
			file.getline(buffer, 20, ' ');
			int type = atoi(buffer);
			file.getline(buffer, 3, '\n');
			int count = atoi(buffer);
			for (int n = 0; n < 2; n++)
			{
				for (int i = 0; i < count; i++)
				{
					if (n == 0)
						Vector.push_back({ 0, 0 });
					file.getline(buffer, 5, ' ');
					Vector[i][n] = atoi(buffer);
				}
				file.getline(buffer, 5, '\n');
			}
			Fig_map.insert(std::pair<int, std::vector<std::vector<int>>>(type, Vector));
		}
	}
}
void Read_Tex() //cut .bmp file on separated tiles and write them to conrainer
{
	sf::Texture tex;
	for (int i = 0; i < 5; i++)
	{
		if (!tex.loadFromFile("tile.bmp", sf::IntRect(sf::Vector2i(i * 10, 0), sf::Vector2i(10, 10))))
		{
			std::cout << "tile texture loading error" << i << std::endl;
		}
		Tile_Textures.push_back(tex);
	}
}
void Erase()
{
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		points.push_back(*It);
	}
	InUse.clear();
}
bool Condition()
{
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		if ((*It).Ypos == 0)
		{
			return false;
		}
		for (auto Jt = points.begin(); Jt != points.end(); Jt++)
		{
			if ((*Jt).Ypos + 1 == (*It).Ypos && (*Jt).Xpos == (*It).Xpos)
			{
				return false;
			}
		}
	}
	return true;
}
void Add_Fig(int type, int color)
{
	InUse.push_back({ StX, StY, true, color });
	auto It = Fig_map.find(type);
	for (auto iterator = (*It).second.begin(); iterator != (*It).second.end(); iterator++)
	{
		InUse.push_back({ StX + (*iterator)[0], StY + (*iterator)[1], false, color });
	}
}
void Down()
{
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		--(*It).Ypos;
	}
}
void Left()
{
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		--(*It).Xpos;
	}
}
void Right()
{
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		++(*It).Xpos;
	}
}
bool Rotate()
{
	std::vector<Points> Check = InUse;
	std::vector<Points>::iterator Major;
	for (auto It = Check.begin(); It != Check.end(); It++)
	{
		if ((*It).Major == true)
			Major = It;
	}
	for (auto It = Check.begin(); It != Check.end(); It++)
	{
		if ((*It).Major != true)
		{
			int Xbuf = (*It).Xpos;
			(*It).Xpos = 0 * ((*It).Xpos - (*Major).Xpos) + 1 * ((*It).Ypos - (*Major).Ypos) + (*Major).Xpos;
			(*It).Ypos = -1 * (Xbuf - (*Major).Xpos) + 0 * ((*It).Ypos - (*Major).Ypos) + (*Major).Ypos;
		}
		if (((*It).Xpos < 0 || (*It).Xpos > 9) && (*It).Ypos > 0)
			return false;
		for (auto Jt = points.begin(); Jt != points.end(); Jt++)
		{
			if ((*It).Xpos == (*Jt).Xpos && (*It).Ypos == (*Jt).Ypos)
			{
				return false;
			}

		}
	}
	InUse = Check;
	return true;
}
void Reset()
{
	std::vector<Points>::iterator Major;
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		if ((*It).Major == true)
			Major = It;
	}
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		if ((*It).Major != true)
		{
			(*It).Xpos = (*It).Xpos - (*Major).Xpos + StX;
			(*It).Ypos = (*It).Ypos - (*Major).Ypos + StY;
		}
	}
	(*Major).Xpos = StX;
	(*Major).Ypos = StY;
}
void Restart()
{
	points.clear();
	InUse.clear();
}
void Check_Line()
{
	int streak = 0;
	for (int i = 0; i < 18; i++)
	{
		int Sum = 0;
		for (auto It = points.begin(); It != points.end(); It++)
		{
			if ((*It).Ypos == i)
			{
				Sum++;
			}
		}
		if (Sum == 10)
		{
			Score += (100);
			if (Score % 1000 < 100)
			{
				Speed += 1;
				fall_time -= 0.1;
			}
			Score += streak * 50;
			std::vector<Points>::iterator Iter = points.begin();
			while (Iter != points.end())
			{
				if ((*Iter).Ypos == i)
				{
					points.erase(Iter);
					Iter = points.begin();
				}
				else Iter++;
			}
			for (auto It = points.begin(); It != points.end(); It++)
			{
				if ((*It).Ypos > i)
					(*It).Ypos--;
			}
			i--;
			streak++;
		}
	}
}
bool Side_movement(bool sign)
{
	for (auto It = InUse.begin(); It != InUse.end(); It++)
	{
		if (sign == 0) // left
		{
			if ((*It).Xpos == 0)
				return false;
		}
		if (sign == 1) // right
		{
			if ((*It).Xpos == 9)
				return false;
		}
		for (auto Jt = points.begin(); Jt != points.end(); Jt++)
		{
			if (sign == 0) // left
			{
				if ((*It).Ypos == (*Jt).Ypos && (*It).Xpos == (*Jt).Xpos + 1)
					return false;
			}
			if (sign == 1) // right
			{
				if ((*It).Ypos == (*Jt).Ypos && (*It).Xpos == (*Jt).Xpos - 1)
					return false;
			}
		}
	}
	return true;
}
bool Game_Over()
{
	for (auto It = points.begin(); It != points.end(); It++)
	{
		if ((*It).Ypos == 19)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	setlocale(0, "");
	Read_Fig();
	Read_Tex();
	Win.setFramerateLimit(30);
	Speed = 1;
	Score = 0;
	sf::Texture texture;
	if (!texture.loadFromFile("back.bmp"))
		std::cout << "error" << std::endl;
	background back(texture);
	Field field;
	Next next;
	Interface interface;
	while (Win.isOpen())
	{
		sf::Clock timer;
		sf::Event event;
		InUse = next.Get_Figure();
		srand(time(NULL) + 24);
		next.Add_Fig(1 + rand() % 7, rand() % 4);
		while (Condition())
		{
			Down();
			while (timer.getElapsedTime() < sf::seconds(fall_time))
			{
				while (Win.pollEvent(event))
				{
					if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
					{
						if (Condition())
						{
							Down();
							sf::sleep(sf::milliseconds(75));
							timer.restart();
							break;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					{
						if (Side_movement(0))
						{
							Left();
							sf::sleep(sf::milliseconds(75));
							break;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					{
						if (Side_movement(1))
						{
							Right();
							sf::sleep(sf::milliseconds(75));
							break;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					{
						Rotate();
						sf::sleep(sf::milliseconds(75));
						break;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
					{
						Reset();
					}
					if (event.type == sf::Event::Closed)
					{
						Win.close();
						exit(0);
					}
				}
				//std::cout << sf::Mouse::getPosition(Win).x << '\t' << sf::Mouse::getPosition(Win).y << std::endl;
				Win.clear(sf::Color::White);
				Win.draw(back);
				Win.draw(field);
				Win.draw(next);
				Win.draw(interface);
				Win.display();
			}
			timer.restart();
		}
		Erase();
		Check_Line();
		interface.update();
		if (Game_Over()) //something that better replace with windows.h messageBox
		{
			GOWin obj;
			bool R = false;
			while (true)
			{
				while (Win.pollEvent(event))
				{
					if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::N))
					{
						Win.close();
						exit(0);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
					{
						R = true;
						sf::sleep(sf::milliseconds(75));
					}
				}
				Win.draw(back);
				Win.draw(field);
				Win.draw(next);
				Win.draw(interface);
				Win.draw(obj);
				Win.display();
				if (R)
				{
					Restart();
					break;
				}
			}
		}
	}
}

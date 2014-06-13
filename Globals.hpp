#ifndef CONST_H
#define CONST_H

#include <SFML/Graphics.hpp>

using namespace sf;

const int H = 12;
const int W = 40;

float offsetX = 0;
float offsetY = 0;
float screen_speed = 0.03;

int tile_size = 50;

int rect_width = 92;
int rect_height = 50;

int ground = tile_size*H;

Time TimePerFrame = seconds(1.f/60.f);

String TileMap[H] = {

	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0                                      0",
	"0                        00000         0",
	"0     0000    0                        0",
	"0    0        00                       0",
	"0          0000                        0",
	"00000          0000000000     00       0",
	"0                                    0 0",
	"0       0000                        0  0",
	"0                                  0   0",
	"0000000000000000000000000000000000000000",

};

#endif CONST_H
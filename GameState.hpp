#pragma once
#include <iostream>

class GameState
{
public:
	int gameMatrix[3][3];
	GameState* parent = nullptr;
	int Gcost = 0;
	int Hcost = 0;
	int Fcost = 0;
	bool duplicate = false;

	GameState();
	GameState(int(&gameMatrix)[3][3],int Gcost);
	//~GameState();
	void ToString();

private:
	//number of tiles out of place
	void evaluateMatrix();
	void setGameMatrix(int(&gameMatrix)[3][3]);
};

GameState::GameState()
{
}

GameState::GameState(int(&gameMatrix)[3][3], int Gcost)
{
	setGameMatrix(gameMatrix);
	//evaluate the matrix
	evaluateMatrix();
	this->Gcost = Gcost;
	Fcost = this->Gcost + Hcost;
}

//GameState::~GameState()
//{
//}

void GameState::ToString() {
	std::cout << "\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << gameMatrix[i][j] << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void GameState::setGameMatrix(int(&gameMatrix)[3][3]) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			this->gameMatrix[i][j]= gameMatrix[i][j];
};

void GameState::evaluateMatrix() {
	int tmp = 0;

	if (gameMatrix[0][0] != 1)
		tmp++;
	if (gameMatrix[0][1] != 2)
		tmp++;
	if (gameMatrix[0][2] != 3)
		tmp++;
	if (gameMatrix[1][2] != 4)
		tmp++;
	if (gameMatrix[2][2] != 5)
		tmp++;
	if (gameMatrix[2][1] != 6)
		tmp++;
	if (gameMatrix[2][0] != 7)
		tmp++;
	if (gameMatrix[1][0] != 8)
		tmp++;

	Hcost = tmp;

};
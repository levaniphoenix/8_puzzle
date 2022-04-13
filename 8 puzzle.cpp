// 8 puzzle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GameState.hpp"
#include <vector>
#include <algorithm>
#include <random>

int startMaterix[3][3] = {
    {8,3,5},
    {4,1,6},
    {2,7,0}
};

/*  easy example
    {0,2,3},
    {1,4,5},
    {8,7,6}
*/

/*  easy example
    {8,3,5},
    {4,1,6},
    {2,7,0}
*/

/*  hard example
    {6,2,8},
    {5,0,7},
    {1,3,4}
*/

int finalMaterix[3][3] = {
    {1,2,3},
    {8,0,4},
    {7,6,5}
};

int helperMaterix[3][3];

std::vector<GameState*> open;
std::vector<GameState*> closed;
std::vector<GameState*> helperVector;

void randomiseStartMatrix() {
    std::vector<int> num = { 0,1,2,3,4,5,6,7,8 };
    srand(time(NULL));
    std::random_shuffle(num.begin(), num.end());
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            startMaterix[i][j] = num.back();
            num.pop_back();
        }
}

void copyMatrix(int(&gameMatrix_)[3][3], int(&helperMatrix_)[3][3]) {
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            helperMatrix_[i][j] = gameMatrix_[i][j];
}

bool compareMatrix(int(&x)[3][3], int(&y)[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (x[i][j] != y[i][j])
                return false;
    return true;
}

void doAllMoves(int (&gameMatrix)[3][3],int Gcost, GameState* parent) {

    helperVector.clear();

    for (int column = 0; column < 3; column++) {
        for (int row = 0; row < 3; row++) {

            //move left
            if (column - 1 >= 0)
                if (gameMatrix[row][column - 1] == 0 ) {
                    //std::cout << "can move left " << gameMatrix[row][column] << std::endl;
                    
                   
                    copyMatrix(gameMatrix,helperMaterix);
                    helperMaterix[row][column - 1] = helperMaterix[row][column];
                    helperMaterix[row][column] = 0;
                    GameState* tmp =new GameState(helperMaterix,Gcost+1);
                    tmp->parent = parent;

                    //tmp.ToString();
                    //add it to list open
                    helperVector.push_back(tmp);
                }
            
            //move right
            if (column + 1 < 3)
                if (gameMatrix[row][column + 1] == 0 ) {
                    //std::cout << "can move right " << gameMatrix[row][column] << std::endl;


                    copyMatrix(gameMatrix, helperMaterix);
                    helperMaterix[row][column + 1] = helperMaterix[row][column];
                    helperMaterix[row][column] = 0;
                    GameState* tmp = new GameState(helperMaterix, Gcost + 1);
                    tmp->parent = parent;

                    //tmp.ToString();
                    //add it to list open
                    helperVector.push_back(tmp);
                }

            //move up
            if (row - 1 >= 0)
                if (gameMatrix[row-1][column] == 0 ) {
                    //std::cout << "can move up " << gameMatrix[row][column] << std::endl;


                    copyMatrix(gameMatrix, helperMaterix);
                    helperMaterix[row-1][column] = helperMaterix[row][column];
                    helperMaterix[row][column] = 0;
                    GameState* tmp = new GameState(helperMaterix, Gcost + 1);
                    tmp->parent = parent;
                    
                    //tmp.ToString();
                    //add it to list open
                    helperVector.push_back(tmp);
                }

            //move down
            if (row + 1 < 3)
                if (gameMatrix[row + 1][column] == 0 ) {
                    //std::cout << "can down up " << gameMatrix[row][column] << std::endl;

                    copyMatrix(gameMatrix, helperMaterix);
                    helperMaterix[row + 1][column] = helperMaterix[row][column];
                    helperMaterix[row][column] = 0;
                    GameState* tmp = new GameState(helperMaterix, Gcost + 1);
                    tmp->parent = parent;

                    //tmp.ToString();
                    //add it to list open
                    helperVector.push_back(tmp);
                }
        }
    }
}

void A_star();
void width_search();

int main()
{
    //randomiseStartMatrix();
    GameState* gm =new GameState(startMaterix,0);
    gm->ToString();
    open.push_back(gm);
    A_star();

    gm = new GameState(startMaterix, 0);
    open.push_back(gm);
    width_search();
}

void A_star() {

    int iter = 0;
    int maxiter = 20000;
    //decide to add gamestates from helper to open
    //check if gameState in closed,if so mark to erase from closed 

    while (iter < maxiter) {
        for (int i = 0; i < helperVector.size(); i++) {
            for (int j = 0; j < closed.size(); j++) {
                if (compareMatrix(helperVector[i]->gameMatrix, closed[j]->gameMatrix))
                    helperVector[i]->duplicate = true;
            }
        }

        //check to see there is a better way to a specific gamestate
        for (int i = 0; i < helperVector.size(); i++) {
            for (int j = 0; j < open.size(); j++) {
                if (compareMatrix(helperVector[i]->gameMatrix, open[j]->gameMatrix) && helperVector[i]->Gcost < open[j]->Gcost) {

                    open[j]->Gcost = helperVector[i]->Gcost;
                    open[j]->Fcost = helperVector[i]->Fcost;
                    open[j]->parent = helperVector[i]->parent;

                    //mark as dup
                    helperVector[i]->duplicate = true;

                }
                //mark duplicate matrix
                if (compareMatrix(helperVector[i]->gameMatrix, open[j]->gameMatrix))
                    //helperVector.erase(helperVector.begin() + i);
                    helperVector[i]->duplicate = true;
            }
        }

        //delete duplicates, push other to open
        for (int i = 0; i < helperVector.size(); i++)
            if(!helperVector[i]->duplicate)
                open.push_back(helperVector[i]);
            else
                delete helperVector[i];

        //sort open, decending
        if(!open.empty())
            std::sort(open.begin(), open.end(), [](GameState* x, GameState* y) {
                return x->Fcost > y->Fcost;
                });


        if (open.empty()) {
            std::cout << "open list is empty, quiting\n";
            break;
        }

        //get last one from open
        GameState* current =open.back();
        open.pop_back();

        //check if current == goalState
        if (compareMatrix(current->gameMatrix, finalMaterix)) {
            std::cout << "problem solved in " << iter << " iterations\n";
            
            std::cout <<"Depth\\(number of moves to solve): " << current->Gcost << std::endl;

            std::cout << "Open list: " << open.size() << '\n';

            std::cout << "closed list: " << closed.size() << '\n';
            
            current->ToString();
            
            GameState* prev = current->parent;

            while (prev!=nullptr)
            {
                prev->ToString();
                prev = prev->parent;
            }
            break;
        }

        //discover new nodes
        doAllMoves(current->gameMatrix, current->Gcost, current);

        //add current to closed
        closed.push_back(current);

        //current.ToString();
        iter++;
    }
    if (iter == maxiter)
        std::cout << "maximum number of iterations reached, quiting...\n";
    
    for (auto i : open)
        delete i;
    
    for (auto i : closed)
        delete i;

    open.clear();
    closed.clear();
    helperVector.clear();
    
    std::cout << "done\n";
}

void width_search() {

    int iter = 0;
    int maxiter = 50000;
    //decide to add gamestates from helper to open
    //check if gameState in closed,if so mark to erase from closed 

    while (iter < maxiter) {
        for (int i = 0; i < helperVector.size(); i++) {
            for (int j = 0; j < closed.size(); j++) {
                if (compareMatrix(helperVector[i]->gameMatrix, closed[j]->gameMatrix))
                    helperVector[i]->duplicate = true;
            }
        }

        for (int i = 0; i < helperVector.size(); i++) {
            for (int j = 0; j < open.size(); j++) {
                if (compareMatrix(helperVector[i]->gameMatrix, open[j]->gameMatrix))
                    //helperVector.erase(helperVector.begin() + i);
                    helperVector[i]->duplicate = true;
            }
        }

        //delete duplicates, push other to open
        for (int i = 0; i < helperVector.size(); i++)
            if (!helperVector[i]->duplicate)
                open.push_back(helperVector[i]);
            else
                delete helperVector[i];

        if (open.empty()) {
            std::cout << "open list is empty, quiting\n";
            break;
        }
        //get last one from open
        GameState* current = open.front();
        open.erase(open.begin());

        //check if current == goalState
        if (compareMatrix(current->gameMatrix, finalMaterix)) {
            std::cout << "problem solved in " << iter << " iterations\n";

            std::cout << "Depth\\(number of moves to solve): " << current->Gcost << std::endl;
            
            std::cout << "Open list: " << open.size() << '\n';

            std::cout << "closed list: " << closed.size() << '\n';
            
            current->ToString();

            GameState* prev = current->parent;

            while (prev != nullptr)
            {
                prev->ToString();
                prev = prev->parent;
            }
            break;
        }

        //discover new nodes
        doAllMoves(current->gameMatrix, current->Gcost, current);

        //add current to closed
        closed.push_back(current);

        //current.ToString();
        iter++;
    }
    if (iter == maxiter)
        std::cout << "maximum number of iterations reached, quiting...\n";

    //delete gamestate from memory
    for (auto i : open)
        delete i;

    for (auto i : closed)
        delete i;

    //clear vectors
    open.clear();
    closed.clear();
    helperVector.clear();

    std::cout << "done\n";
}
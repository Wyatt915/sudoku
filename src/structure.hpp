#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#define TXTRED     "\x1b[31;1m"
#define TXTDEF     "\x1b[0m"

struct coord
{
	coord();
    coord(int, int);
	void set(int, int);
	int row;
	int col;
};

coord::coord(){
	row = 0;
	col = 0;
}

coord::coord(int r, int c){
	row = r;
	col = c;
}
void coord::set(int r, int c){
	row = r;
	col = c;
}

void resetList(bool*in){
	for (int i = 0; i < 10; i++){
		in[i] = true;
	}
	in[0] = false;
}

struct cell
{
	std::vector<int> get_dits();	//potentially valid values
    int get_dit(); //gets the first dit
    bool dits[10];
	int numDits();		//number of potentially valid values
	int value;			//solution to a cell
	bool given;			//Is the value one given by the initial puzzle?
	bool isValid;		//Is the value valid?
	void init();		//constructor
    void zeroDits();
	void setDits(bool*);
};

std::vector<int> cell::get_dits(){
    std::vector<int> out;
    for(int i = 0; i < 10; i++){
        if(dits[i]){
            out.push_back(i);
        }
    }
    return out;
}

void cell::setDits(bool* in){
	for (int i = 0; i < 10; i++){
        dits[i] = in[i];
	}
}

void cell::init(){
	value = 0;
	given = false;
	isValid = true;
    resetList(dits);
}

int cell::numDits(){
    int count = 0;
    for(int i = 0; i < 10; i++){
        if(dits[i]) { count++; }
    }
    return count;
}

int cell::get_dit(){
    int i = 1;
    while(!dits[i] && i < 10){ i++; }
    return i;
}

void cell::zeroDits(){
    for(int i = 0; i < 10; i++){
        dits[i] = false;
    }
}

class Sudoku{
public:
    Sudoku();
	Sudoku(int[9][9]);
	cell operator()(int, int);	//Subscripting
    cell at(coord);
	void print();				//Print to console
	//string tex();				//LaTeX representation of the Sudoku
	void populate(int[9][9]);	//converts an int[][] to a Sudoku
	void searchRow(coord, bool*);
	void searchCol(coord, bool*);
	void searchBlock(coord, bool*);
	void setAllDits();	//populate dit array for each cell
	bool setSingles();	//if there is exactly 1 dit in a cell, fill it in. return true if successful
	bool setUniques();	//if there is a unique dit in a row, cell, or block, fill it in. return true if successful
    coord findFirstEmpty(); //return coordinates of the first empty cell.
    bool setGuess(coord, int);
    bool valid();
    bool invalid();
    bool complete();
    std::vector<int> get_dits(coord);
private:
	cell data[9][9];
};

//--------------------------[Constructors and access]---------------------------

Sudoku::Sudoku(){
	for (int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j){
			data[i][j].init();
		}
	}
}

Sudoku::Sudoku(int ary[9][9]){
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            data[i][j].value = ary[i][j];
            data[i][j].isValid = true;
            if (data[i][j].value != 0){
                data[i][j].given = true;
            }
            else{
                data[i][j].given = false;
            }
        }
    }
}

cell Sudoku::operator()(int row, int col){
	return data[row][col];
}

cell Sudoku::at(coord in){
    return data[in.row][in.col];
}

void Sudoku::populate(int ary[9][9]){
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			data[i][j].value = ary[i][j];
			data[i][j].isValid = true;
			if (data[i][j].value != 0){
				data[i][j].given = true;
			}
            else{
                data[i][j].given = false;
            }
		}
	}
}

void Sudoku::print(){
	std::cout << '\n';
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if(j%3==0 && j!=0) { std::cout<< "| "; }
            if(data[i][j].value == 0){ std::cout << "  "; }
			else{
                if(data[i][j].given){
                    std::cout << "\x1b[31;1m" << data[i][j].value << "\x1b[0m ";
                }
                else{
                    std::cout << data[i][j].value << ' ';
                }
            }
		}
		if(i%3==2 && i!=8){
			std::cout << "\n- - - + - - - + - - -";
		}
		std::cout <<'\n';
	}
	std::cout << "\n";
}

//---------------------------------[Searches]-----------------------------------

std::vector<int> Sudoku::get_dits(coord c){
    return data[c.row][c.col].get_dits();
}

coord getBlock(coord in){
	coord out;
	out.row = ((in.row) / 3);
	out.col = ((in.col) / 3);
	return out;
}

void Sudoku::searchRow(coord in, bool* list){
	int col = in.col;
	for (int i = 0; i < 9; i++){
		if (data[i][col].value != 0){
			list[data[i][col].value] = false;
		}
	}
}

void Sudoku::searchCol(coord in, bool* list){
	int row = in.row;
	for (int i = 0; i < 9; i++){
		if (data[row][i].value != 0){
			list[data[row][i].value] = false;
		}
	}
}

void Sudoku::searchBlock(coord in, bool* list){
	coord blockPos = getBlock(in);
	int rowOffset, colOffset;
	rowOffset = blockPos.row * 3;
	colOffset = blockPos.col * 3;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (data[i + rowOffset][j + colOffset].value != 0){
				list[data[i + rowOffset][j + colOffset].value] = false;
			}
		}
	}
}

coord Sudoku::findFirstEmpty(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(data[i][j].value == 0){
                return coord(i,j);
            }
        }
    }
    return coord(-1, -1);
}

//--------------------------------[set values]----------------------------------

bool Sudoku::setGuess(coord c, int guess){
    data[c.row][c.col].value = guess;
    data[c.row][c.col].isValid = true;
    return true;
}

void Sudoku::setAllDits(){
	bool list[10];	//Used to determine the dits.  Each index set to false as
	//possibilities are ruled out.
	resetList(list);
	coord idx;
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			idx.set(i, j);
			searchRow(idx, list);
			searchCol(idx, list);
			searchBlock(idx, list);
			data[i][j].setDits(list);
			resetList(list);
		}
	}
}

bool Sudoku::setSingles(){
	bool out = false;
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (data[i][j].numDits() == 1 && data[i][j].value == 0){
				out = true;
				data[i][j].value = data[i][j].get_dit();
				data[i][j].isValid = true;
			}
			data[i][j].zeroDits();
		}
	}
	return out;
}


bool Sudoku::setUniques(){
    bool out = false;
    coord pos;
    for(int digit = 1; digit <= 9; digit++){
        int count = 0;
        //rows
        for(int r = 0; r < 9; r++){
            for(int c = 0; c < 9; c++){
                if(data[r][c].dits[digit]){
                    count++;
                    pos.set(r,c);
                }
            }
            if(count == 1) {
                data[pos.row][pos.col].value = digit;
                out = true;
            }
            count = 0;
        }
        //columns
        for(int c = 0; c < 9; c++){
            for(int r = 0; r < 9; r++){
                if(data[r][c].dits[digit]){
                    count++;
                    pos.set(r,c);
                }
            }
            if(count == 1) {
                data[pos.row][pos.col].value = digit;
                out = true;
            }
            count = 0;
        }
        //blocks
        for(int br = 0; br < 3; br++){
            for(int bc = 0; bc < 3; bc++){
            	int rowOffset, colOffset;
            	rowOffset = br * 3;
            	colOffset = bc * 3;
            	for (int i = 0; i < 3; i++){
            		for (int j = 0; j < 3; j++){
            			if (data[i + rowOffset][j + colOffset].dits[digit]){
                            count++;
                            pos.set(i+rowOffset, j+colOffset);
                        }
            		}
            	}
                if(count == 1) {
                    data[pos.row][pos.col].value = digit;
                    out = true;
                }
                count = 0;
            }
        }
    }
    return out;
}

//----------------------------[boolean properties]------------------------------

bool Sudoku::valid(){
    setAllDits();
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(data[i][j].value == 0 && data[i][j].numDits() == 0){ return false; }
        }
    }

    for(int digit = 1; digit <= 9; digit++){
        int count = 0;
        //rows
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(data[i][j].value == digit){ count++; }
            }
            if(count > 1) { return false; }
            count = 0;
        }
        //columns
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(data[j][i].value == digit){ count++; }
            }
            if(count > 1) { return false; }
            count = 0;
        }
        //blocks
        for(int br = 0; br < 3; br++){
            for(int bc = 0; bc < 3; bc++){
            	int rowOffset, colOffset;
            	rowOffset = br * 3;
            	colOffset = bc * 3;
            	for (int i = 0; i < 3; i++){
            		for (int j = 0; j < 3; j++){
            			if (data[i + rowOffset][j + colOffset].value == digit){ count++; }
            		}
            	}
                if(count > 1) { return false; }
                count = 0;
            }
        }
    }
    return true;
}

bool Sudoku::invalid(){
    return !valid();
}

bool Sudoku::complete(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(data[j][i].value == 0){ return false; }
        }
    }
    //at this point, we know the sudoku is filled, but is it a valid fill?
    return valid();
}

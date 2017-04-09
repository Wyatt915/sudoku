#include "structure.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void load_sudoku(std::string fname, int ary[9][9]){
	std::string line;
	std::ifstream infile(fname.c_str());
	int row = 0;
    int col = 0;
    bool lineHasData = false;
	while (std::getline(infile, line))
	{
        if (line.length() > 1){
            col = 0;
			for (char c : line){
                if(c >= '0' && c <= '9'){
                    ary[row][col] = c - '0';
                    col++;
                    lineHasData = true;
                }
			}
			if(lineHasData) row++;
            lineHasData = false;
	    }
    }
    infile.close();
}

void load_sudoku(int ary[9][9]){
    std::string line;
	int row = 0;
    int col = 0;
    bool lineHasData = false;
	while (std::getline(std::cin, line))
	{
        if (line.length() > 1){
            col = 0;
			for (char c : line){
                if(c >= '0' && c <= '9'){
                    ary[row][col] = c - '0';
                    col++;
                    lineHasData = true;
                }
			}
            std::cout << std::endl;
			if(lineHasData) row++;
            lineHasData = false;
	    }
	}
}

long int numcalls = 0;

bool solve(Sudoku& puzzle){
    numcalls++;
    if(puzzle.invalid()) { return false; }
    while(!puzzle.complete()){
        puzzle.setAllDits();
    	while (puzzle.setSingles()){
    		puzzle.setAllDits();
            //puzzle.setUniques();
            //puzzle.setAllDits();
    	}

        if(puzzle.complete()){ return true; }
        if(puzzle.invalid()) { return false; }

        coord poe = puzzle.findFirstEmpty(); //point of entry
        std::vector<int> dits = puzzle.get_dits(poe);
        Sudoku temp;
        for(int i = 0; i < dits.size(); i++){
            temp = puzzle;
            temp.setGuess(poe, dits[i]);
            if(solve(temp)) break;
        }
        puzzle = temp;
        if(puzzle.invalid()) { return false; }
    }
    return true;
}

int main(int argc, char* argv[]){
    int input[9][9];
	if(argc > 1){ load_sudoku(argv[1], input); }
    else{ load_sudoku(input); }

	Sudoku board(input);
	board.print();

    if(solve(board)){
        std::cout << "The puzzle is valid" << std::endl;
        std::cout << "Number of calls: " << numcalls << std::endl;
        board.print();
    }
    else{
        std::cout << "The puzzle is invalid" << std::endl;
        std::cout << "Number of calls: " << numcalls << std::endl;
        board.print();
    }

	return 0;
}

#include <vector>
#include <iostream>
#include "structure.hpp"
#include "fileops.h"


char c;

long int numcalls = 0;
long int stackdepth = 0;

bool solve(Sudoku& puzzle){
    numcalls++; stackdepth++;
    if (numcalls % 1000 == 0){
        std::cout << "Number of calls: " << numcalls << std::endl;
        std::cout << "Stack Depth:     " << stackdepth << std::endl;
        puzzle.print();
    }
    if(puzzle.invalid()) { stackdepth--; return false; }
    while(!puzzle.complete()){
        puzzle.setAllDits();
    	while (puzzle.setSingles()){
    		puzzle.setAllDits();
    	}

        if(puzzle.complete() && puzzle.valid()){ stackdepth--; return true; }
        if(puzzle.invalid()) { stackdepth--; return false; }

        puzzle.setAllDits();

        coord poe = puzzle.findFirstEmpty(); //point of entry
        std::vector<int> dits = puzzle.get_dits(poe);
        Sudoku temp;
        for(int i = 0; i < dits.size(); i++){
            temp = puzzle;
            temp.setGuess(poe, dits[i]);
            if(solve(temp)) break;
        }
        puzzle = temp;
        if(puzzle.invalid()) { stackdepth--; return false; }
    }
    return true;
}

int main(int argc, char* argv[]){
    int input[9][9];
	if(argc > 1){ load_sudoku(argv[1], input); }
    else{ load_sudoku("challenge.sudoku", input); }

	Sudoku board(input);
	board.print();
    //coord poe = board.findFirstEmpty(); //point of entry

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
    // board.setAllDits();

	return 0;
}

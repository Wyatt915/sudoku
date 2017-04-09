#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

void load_sudoku(std::string fname, int ary[9][9]){
	std::string line;
	std::ifstream infile(fname.c_str());
	int row = 0;
	while (std::getline(infile, line))
	{
		if (line.length() > 1){
			std::istringstream iss(line);
			for (int col = 0; col < 9; col++){
				iss >> ary[row][col];
			}
			row++;
		}
	}
	infile.close();
}

void load_sudoku(int ary[9][9]){
    std::string line;
	int row = 0;
	while (std::getline(std::cin, line))
	{
		if (line.length() > 1){
			std::istringstream iss(line);
			for (int col = 0; col < 9; col++){
				iss >> ary[row][col];
			}
			row++;
		}
	}
}

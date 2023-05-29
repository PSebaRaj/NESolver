#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Nash_2Player_NF.h"

std::vector<std::vector<double> > get_payoffs_from_file(std::string fname) {

	std::vector<std::vector<double> > player_payoffs;
	std::vector<double> row;
	std::string line, word;

	std::fstream file (fname, std::ios::in);

	if(file.is_open()) {
		while(getline(file, line)) {
			row.clear();

			std::stringstream r_str(line);

			while(getline(r_str, word, ',')) {
				row.push_back(std::stof(word));
			}
			player_payoffs.push_back(row);
		}
	}
	else {
		std::cerr << "Could not open the payoff file" << std::endl;;
	}

	return player_payoffs;
}

std::vector<std::vector<double> > get_zero_sum_payoffs(std::vector<std::vector<double> > given_payoffs) {
	// given A, return -A

	std::vector<std::vector<double> > player_payoffs;

	for (auto &orig_row : given_payoffs) {
		std::vector<double> row;
		for (auto &elem : orig_row) {
			if (elem == 0) {
				row.push_back(0.);
			} else {
				row.push_back(-elem);
			}
		}
		player_payoffs.push_back(row);
	}

	return player_payoffs;
}

std::vector<std::vector<double> > get_symmetric_payoffs(std::vector<std::vector<double> > given_payoffs) {
	// given A, return A^T (transpose)

	std::vector<std::vector<double> > player_payoffs (given_payoffs[0].size(), std::vector<double>());

	for (int i = 0; i < given_payoffs.size(); i++) {
        for (int j = 0; j < given_payoffs[i].size(); j++) {
            player_payoffs[j].push_back(given_payoffs[i][j]);
        }
    }

	return player_payoffs;
}

int main(int argc, char *argv[]) {
	if (argc == 1) { // ./NESOLVER, assume bimatrix game
		// If the user wants to entire both payoff matrices NOW
		int num_rows, num_cols;
		std::cout << "How many pure strategies does the row player have?" << std::endl;
		std::cin  >> num_rows;
		std::cout << "How many pure strategies does the column player have" << std::endl;
		std::cin  >> num_cols;


		// ROW PLAYER
		std::cout << "Enter values of the row player's payoff matrix: " << std::endl;

		std::vector<std::vector<double> > row_player_payoffs( num_rows , std::vector<double> (num_cols, 0));

		for (int i = 0; i < num_rows; i++) {
			for (int j = 0; j < num_cols; j++) {
					std::cout << "row_player_payoffs[" << i << "][" << j << "]" << std::endl;
					std::cin >> row_player_payoffs[i][j];
			}
		}

		// COLUMN PLAYER
		std::cout << "Enter values of the column player's payoff matrix: " << std::endl;

		std::vector<std::vector<double> > column_player_payoffs( num_rows , std::vector<double> (num_cols, 0));
		for (int i = 0; i < num_rows; i++) {
			for (int j = 0; j < num_cols; j++) {
					std::cout << "column_player_payoffs[" << i << "][" << j << "]" << std::endl;
					std::cin >> column_player_payoffs[i][j];
			}
		}

		// now find the nash equilibria
        Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
        NG.print_ne_support();

		return 0;

	} else if (argc == 2) {
		if (!strcmp(argv[1], "-b")) {

			int num_rows, num_cols;
			std::cout << "How many pure strategies does the row player have?" << std::endl;
			std::cin  >> num_rows;
			std::cout << "How many pure strategies does the column player have" << std::endl;
			std::cin  >> num_cols;


			// ROW PLAYER
			std::cout << "Enter values of the row player's payoff matrix: " << std::endl;

			std::vector<std::vector<double> > row_player_payoffs( num_rows , std::vector<double> (num_cols, 0));

			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < num_cols; j++) {
						std::cout << "row_player_payoffs[" << i << "][" << j << "]" << std::endl;
						std::cin >> row_player_payoffs[i][j];
				}
			}

			// COLUMN PLAYER
			std::cout << "Enter values of the column player's payoff matrix: " << std::endl;

			std::vector<std::vector<double> > column_player_payoffs( num_rows , std::vector<double> (num_cols, 0));
			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < num_cols; j++) {
						std::cout << "column_player_payoffs[" << i << "][" << j << "]" << std::endl;
						std::cin >> column_player_payoffs[i][j];
				}
			}

			// now find the nash equilibria
			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
			NG.print_ne_support();

			return 0;

		} else if (!strcmp(argv[1], "-z")) {
			int num_rows, num_cols;
			std::cout << "How many pure strategies does the row player have?" << std::endl;
			std::cin  >> num_rows;
			std::cout << "How many pure strategies does the column player have" << std::endl;
			std::cin  >> num_cols;

			// ROW PLAYER
			std::cout << "Enter values of the row player's payoff matrix: " << std::endl;

			std::vector<std::vector<double> > row_player_payoffs( num_rows , std::vector<double> (num_cols, 0));

			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < num_cols; j++) {
						std::cout << "row_player_payoffs[" << i << "][" << j << "]" << std::endl;
						std::cin >> row_player_payoffs[i][j];
				}
			}

			std::vector<std::vector<double> > column_player_payoffs = get_zero_sum_payoffs(row_player_payoffs);

			// now find the nash equilibria
			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
			NG.print_ne_support();

			return 0;

		} else if (!strcmp(argv[1], "-s")) {
			int num_rows;
			std::cout << "How many pure strategies does the row player have?" << std::endl;
			std::cin  >> num_rows;

			// ROW PLAYER
			std::cout << "Enter values of the row player's payoff matrix: " << std::endl;

			std::vector<std::vector<double> > row_player_payoffs( num_rows , std::vector<double> (num_rows, 0));

			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < num_rows; j++) {
						std::cout << "row_player_payoffs[" << i << "][" << j << "]" << std::endl;
						std::cin >> row_player_payoffs[i][j];
				}
			}

			std::vector<std::vector<double> > column_player_payoffs = get_symmetric_payoffs(row_player_payoffs);

			// now find the nash equilibria
			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
			NG.print_ne_support();

			return 0;

		} else {
			std::cerr << "Invalid flag provided" << std::endl;
			return 1;

		}

	} else if (argc == 3) {

		if (!strcmp(argv[1], "-b")) { // .NESOLVER -b file.csv
			std::cerr << "NOT ENOUGH PAYOFF MATRICES PROVIDED FOR BI-MATRIX GAME" << std::endl;
			std::cerr << "SOLVING ASSUMING BOTH PLAYERS HAVE IDENTICAL PAYOFF MATRICES" << std::endl;

			std::string r_fname = argv[2];
			std::vector<std::vector<double> > row_player_payoffs = get_payoffs_from_file(r_fname);

			// now find the nash equilibria
            Nash_2Player_NF NG(row_player_payoffs, row_player_payoffs);
            NG.print_ne_support();

			return 0;

		} else if (!strcmp(argv[1], "-z")) {
			// ROW PLAYER
			std::string r_fname = argv[2];
			std::vector<std::vector<double> > row_player_payoffs = get_payoffs_from_file(r_fname);

			// COLUMN PLAYER
			std::vector<std::vector<double> > column_player_payoffs = get_zero_sum_payoffs(row_player_payoffs);

			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
            NG.print_ne_support();

			return 0;

		} else if (!strcmp(argv[1], "-s")) {
			// ROW PLAYER
			std::string r_fname = argv[2];
			std::vector<std::vector<double> > row_player_payoffs = get_payoffs_from_file(r_fname);

			// COLUMN PLAYER
			std::vector<std::vector<double> > column_player_payoffs = get_symmetric_payoffs(row_player_payoffs);

			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
            NG.print_ne_support();

			return 0;

		} else {
			std::cerr << "Invalid flag provided" << std::endl;
			return 1;

		}
	} else if (argc == 4) {

		if (!strcmp(argv[1], "-b")) { // .NESOLVER -b file.csv file.csv
			// ran with CSVs of row and column player payoffs

			// ROW PLAYER
			std::string r_fname = argv[2];
			std::vector<std::vector<double> > row_player_payoffs = get_payoffs_from_file(r_fname);

			// COLUMN PLAYER
			std::string c_fname = argv[3];
			std::vector<std::vector<double> > column_player_payoffs = get_payoffs_from_file(c_fname);

			// now find the nash equilibria
            Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
            NG.print_ne_support();

			return 0;

		} else if (!strcmp(argv[1], "-z")) {
			std::cerr << "IGNORING 2ND / COLUMN PLAYER'S PROVIDED MATRIX" << std::endl;
			std::cerr << "Calculating column player's payoff matrix, '-A', from the row player's matrix 'A'" << std::endl;

			// ROW PLAYER
			std::string r_fname = argv[2];
			std::vector<std::vector<double> > row_player_payoffs = get_payoffs_from_file(r_fname);

			// COLUMN PLAYER
			std::vector<std::vector<double> > column_player_payoffs = get_zero_sum_payoffs(row_player_payoffs);

			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
            NG.print_ne_support();

			return 0;

		} else if (!strcmp(argv[1], "-s")) {
			std::cerr << "IGNORING 2ND / COLUMN PLAYER'S PROVIDED MATRIX" << std::endl;
			std::cerr << "Calculating column player's payoff matrix, 'A^T', from the row player's matrix 'A'" << std::endl;

			// ROW PLAYER
			std::string r_fname = argv[2];
			std::vector<std::vector<double> > row_player_payoffs = get_payoffs_from_file(r_fname);

			// COLUMN PLAYER
			std::vector<std::vector<double> > column_player_payoffs = get_symmetric_payoffs(row_player_payoffs);

			Nash_2Player_NF NG(row_player_payoffs, column_player_payoffs);
            NG.print_ne_support();

			return 0;

		} else {
			std::cerr << "Invalid flag provided" << std::endl;
			return 1;

		}
	} else {
		// ERROR
		return 1;
	}

}

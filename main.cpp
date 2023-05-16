#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char *argv[]) {
	if (argc ==  3) {
		// ran with CSVs of row and column player payoffs
		// first, let us read them in

		// ROW PLAYER
		std::string r_fname = argv[1];

		std::vector<std::vector<float> > row_player_payoffs;
		std::vector<float> r_row;
		std::string r_line, r_word;

		std::fstream r_file (r_fname, std::ios::in);

		if(r_file.is_open()) {
			while(getline(r_file, r_line)) {
				r_row.clear();

				std::stringstream r_str(r_line);

				while(getline(r_str, r_word, ',')) {
					r_row.push_back(std::stof(r_word));
				}
				row_player_payoffs.push_back(r_row);
			}
		}
		else {
			std::cerr << "Could not open the row payoff file" << std::endl;;
		}

		// print contents of payoff matrix
		std::cout << "Payoff matrix of ROW player:" << std::endl;;
		for(int i=0;i < row_player_payoffs.size(); i++) {
			for(int j=0; j < row_player_payoffs[i].size(); j++) {
				std::cout << row_player_payoffs[i][j] << " ";
			}

			std::cout << "\n";
		}

		// COLUMN PLAYER
		std::string c_fname = argv[2];

		std::vector<std::vector<float> > column_player_payoffs;
		std::vector<float> c_row;
		std::string c_line, c_word;

		std::fstream c_file (c_fname, std::ios::in);

		if(c_file.is_open()) {
			while(getline(c_file, c_line)) {
				c_row.clear();

				std::stringstream c_str(c_line);

				while(getline(c_str, c_word, ',')) {
					c_row.push_back(std::stof(c_word));
				}
				column_player_payoffs.push_back(c_row);
			}
		}
		else {
			std::cerr << "Could not open the column payoff file" << std::endl;;
		}

		// print contents of payoff matrix
		std::cout << "Payoff matrix of COLUNN player:"<< std::endl;;
		for(int i=0;i < column_player_payoffs.size(); i++) {
			for(int j=0; j < column_player_payoffs[i].size(); j++) {
				std::cout << column_player_payoffs[i][j] << " ";
			}

			std::cout << "\n";
		}

		// now find the nash equilibria

		return 0;

	} else if (argc == 1) {
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

		// print contents of payoff matrix
		std::cout << "Payoff matrix of ROW player:" << std::endl;;
		for(int i=0;i < row_player_payoffs.size(); i++) {
			for(int j=0; j < row_player_payoffs[i].size(); j++) {
				std::cout << row_player_payoffs[i][j] << " ";
			}

			std::cout << "\n";
		}
		std::cout << "Payoff matrix of COLUNN player:"<< std::endl;;
		for(int i=0;i < column_player_payoffs.size(); i++) {
			for(int j=0; j < column_player_payoffs[i].size(); j++) {
				std::cout << column_player_payoffs[i][j] << " ";
			}

			std::cout << "\n";
		}

		// now find the nash equilibria

		return 0;

	} else {
		// ERROR
		return 0;
	}

}

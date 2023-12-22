#ifndef NASH_2PLAYER_NF_H_
#define NASH_2PLAYER_NF_H_

#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>
#include <fstream>
#include <exception>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>

typedef Eigen::MatrixXd PayoffMatrix;

typedef std::vector<std::pair<std::vector<double>, std::vector<double>>> SupportPairs;

typedef std::tuple<Eigen::VectorXd, Eigen::VectorXd, std::vector<double>, std::vector<double>> ProbabilityVector;
typedef std::vector<ProbabilityVector> ProbabilityVectors;

class Nash_2Player_NF
{
public:
	PayoffMatrix RowPlayer;
	PayoffMatrix ColumnPlayer;

	Nash_2Player_NF();
	Nash_2Player_NF(std::vector<std::vector<double>> row_player_payoff, std::vector<std::vector<double>> col_player_payoff);

	virtual ~Nash_2Player_NF();

	// Convert vector of vector of doubles (vvd) to type PayoffMatrix
	Eigen::MatrixXd convert_to_matrix(std::vector<std::vector<double>> vvd);

	Eigen::MatrixXd make_nf_table(Eigen::MatrixXd &M);

	Eigen::Index find_pivot_row(const Eigen::MatrixXd &nf_table, int column_index);

	std::vector<int> non_basic_variables(const Eigen::MatrixXd &nf_table);

	std::vector<int> pivot_nf_table(Eigen::MatrixXd &nf_table, int column_index);

	Eigen::MatrixXd shift_nf_table(Eigen::MatrixXd nf_table, int num_rows, int num_cols);

	// Find set of combinations of n objects taken r
	void combination(int n, int r, std::vector<std::vector<double>> &powerset);

	// Find all possible combinations
	std::vector<std::vector<double>> powerset(int n);

	// Compute the indifference equations
	bool solve_indifference_cond(const PayoffMatrix &A, Eigen::VectorXd &prob, const std::vector<double> &rows, const std::vector<double> &columns);

	// Find possible support pairs
	SupportPairs potential_support_pairs(bool non_degenerate = false);

	// Does strategy consist of the given support
	bool obey_support(Eigen::VectorXd strategy_prob, std::vector<double> support_vec);

	// Produces probability vectors
	ProbabilityVectors indifference_strategies();

	// Determines if probability vector (proposed equilibrium) is actually an NE
	bool is_ne(ProbabilityVector pv);

	// Enumerates / Prints the support(s) found to be NE(s)
	void print_ne_support();

private:
	// Tolerance
	double tol = 1e-16;
};

#endif

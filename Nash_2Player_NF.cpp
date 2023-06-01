#include "Nash_2Player_NF.h"

Nash_2Player_NF::Nash_2Player_NF(std::vector<std::vector<double> > row_player_payoff, std::vector<std::vector<double> > col_player_payoff) {

        const Eigen::MatrixXd payoff_matrix_1 = this->convert_to_matrix(row_player_payoff);
        const Eigen::MatrixXd payoff_matrix_2 = this->convert_to_matrix(col_player_payoff);

        this->RowPlayer    = payoff_matrix_1;
        this->ColumnPlayer = payoff_matrix_2;

		// https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
        Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

		std::cout << "Payoff matrix of ROW player:" << std::endl;;
        std::cout << payoff_matrix_1.format(CleanFmt) << std::endl;

		std::cout << "Payoff matrix of COLUMN player:" << std::endl;;
        std::cout << payoff_matrix_2.format(CleanFmt) << std::endl;

}


Nash_2Player_NF::~Nash_2Player_NF() {
        // Default destructor
}


Eigen::MatrixXd Nash_2Player_NF::convert_to_matrix(std::vector<std::vector<double> > vvd) {

    std::size_t n_rows = vvd.size();
    std::size_t n_cols = vvd.at(0).size();

    Eigen::MatrixXd result(n_rows, n_cols);
    result.row(0) = Eigen::VectorXd::Map(&vvd[0][0], n_cols);

    for (std::size_t i = 1; i < n_rows; i++) {
        result.row(i) = Eigen::VectorXd::Map(&vvd[i][0], n_cols);
    }

    return result;
}


Eigen::MatrixXd Nash_2Player_NF::make_nf_table(Eigen::MatrixXd &M) {

    int m = M.rows();
    int n = M.cols();

    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(m,m); // m x m identity matrix
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(m, 1); // m x 1 one's vector
    Eigen::MatrixXd C(m, m+n+1); // holds M, I, and ones matrices concat. on one another

    C << M, I, ones;

    return C;

}


Eigen::Index Nash_2Player_NF::find_pivot_row(const Eigen::MatrixXd& nf_table, int column_index) {

    Eigen::Index id;
    Eigen::VectorXd ratios = nf_table.col(column_index).array() / nf_table.col(nf_table.cols() - 1).array();

	// pivot on max number... max num if most beneficial w/ both (+) and (-) payoffs
	// remember, we are not using minimax. so this works for strictly positive/negative games AND zero sum games
    ratios.array().maxCoeff(&id);
    return id;
}


std::vector<int> Nash_2Player_NF::non_basic_variables(const Eigen::MatrixXd &nf_table) {

    Eigen::MatrixXd columns = nf_table.block(0, 0, nf_table.rows(), nf_table.cols() - 1).transpose();
    std::vector<int> non_basic_vars;

    for (int i = 0; i < columns.rows(); i++) {
        Eigen::VectorXd vec_row = columns.row(i).array();
        Eigen::SparseMatrix<double> sparse_vec(vec_row.sparseView());

        if (sparse_vec.nonZeros() != 1) {
            non_basic_vars.push_back(i);
        }
    }

    return non_basic_vars;
}


std::vector<int> Nash_2Player_NF::pivot_nf_table(Eigen::MatrixXd &nf_table, int column_index) {

    std::vector<int> original_labels = non_basic_variables(nf_table);
    const int pivot_row_index = find_pivot_row(nf_table, column_index);
    double pivot_element = nf_table(pivot_row_index, column_index);

    for (int i = 0; i < nf_table.rows(); i++) {
        if (i != pivot_row_index) {
            nf_table.row(i) = nf_table.row(i) * pivot_element - nf_table.row(pivot_row_index) * nf_table(i, column_index);
        }
    }

    std::vector<int> non_basic_vars = this->non_basic_variables(nf_table);


    for(auto& org_var: original_labels) {
        std::vector<int>::iterator position = std::find(non_basic_vars.begin(), non_basic_vars.end(), org_var);
        if (position != non_basic_vars.end()) {
            non_basic_vars.erase(position);
		}
    }

    return non_basic_vars;
}


Eigen::MatrixXd Nash_2Player_NF::shift_nf_table(Eigen::MatrixXd nf_table, int num_rows, int num_cols) {

    Eigen::MatrixXd shifted_nf_table(num_rows, num_cols);

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols-1; j++) {
            shifted_nf_table(i, j) = nf_table(i, j % num_cols);
        }
        shifted_nf_table(i, num_cols-1) = 1.0;
    }

    return shifted_nf_table;
}


void Nash_2Player_NF::combination(int n, int r, std::vector<std::vector<double>> &powerset) {

    std::vector<double> v(n);
    std::fill(v.begin(), v.begin() + r, true);

    do {
        std::vector<double> elements;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                elements.push_back(i);
            }
        }

        if(elements.size() != 0) { // ignore the empty set
            powerset.push_back(elements);
		}
    } while (std::prev_permutation(v.begin(), v.end()));

}


std::vector<std::vector<double>> Nash_2Player_NF::powerset(int n) {

    std::vector<std::vector<double>> powerset;

	// OpenMP: https://learn.microsoft.com/en-us/cpp/parallel/openmp/reference/openmp-directives?view=msvc-170#for-openmp
    #pragma omp parallel for
    for(int i=0; i <= n; i++) {
        this->combination(n, i, powerset);
	}

    return powerset;

}


bool Nash_2Player_NF::solve_indifference_cond(const PayoffMatrix& A, Eigen::VectorXd &prob, const std::vector<double> &rows, const std::vector<double> &columns) {

    const int m = A.rows();
    const int n = A.cols();
    const int rows_m = rows.size();

    Eigen::MatrixXd M(m, n);
    std::vector<double> rows_rotated(rows);

    if (m != 0) {
		// shift vector forward by 1
        std::rotate(rows_rotated.begin(), rows_rotated.begin() + (rows_rotated.size()- 1), rows_rotated.end());
        for (int i = 0 ; i < m - 1; ++i) {
			M.row(i) = A.row(rows[i]) - A.row(rows_rotated[i]);
        }
    }

    Eigen::MatrixXd Z(1, n);
    Z.row(0).setOnes();

    for (int j : columns) {
        Z(0, j) = 0;
	}
    M.row(m-1) = Z;

    //	Set the last row to ones
    Z.row(0).setOnes();
    Eigen::MatrixXd M_new(M.rows()+1, M.cols());
    M_new << M, Z;

    Eigen::VectorXd b(m+1);
    b.setZero();
    b(m) = 1;

    Eigen::FullPivLU<Eigen::MatrixXd> lu(M_new);
    prob = lu.solve(b);

    if((prob.array() >= 0).all()){
        return true;
    }

    return false;
}


SupportPairs Nash_2Player_NF::potential_support_pairs(bool non_degenerate) {

    int p1_num_strategies = this->RowPlayer.rows();
    int p2_num_strategies = this->RowPlayer.cols();
    auto p1_supports = this->powerset(p1_num_strategies);

    std::vector<std::pair<std::vector<double>, std::vector<double>>> result;

	// OpenMP: https://learn.microsoft.com/en-us/cpp/parallel/openmp/reference/openmp-directives?view=msvc-170#for-openmp
    #pragma omp parallel for
    for (const auto& support1 : p1_supports) {
        if (support1.empty()) continue;

        auto p2_supports = powerset(p2_num_strategies);
        for (const auto& support2 : p2_supports) {
            if (support2.empty()) {
                continue;
            }

            result.emplace_back(support1, support2);
        }
    }

    return result;
}


bool Nash_2Player_NF::obey_support(Eigen::VectorXd strategy_prob, std::vector<double> support_vec) {

    double *ptr_support = &support_vec[0];
    Eigen::Map<Eigen::VectorXd> support(ptr_support, support_vec.size());

    if (strategy_prob.size() == 0) {
        return false;
    }

    for (int i = 0; i < strategy_prob.size(); ++i) {
        if ((support.array() == i).any() && strategy_prob(i) <= this->tol) {
            return false;
        }

        if ((support.array() != i).all() && strategy_prob(i) > this->tol) {
            return false;
        }

    }

    return true;
}


ProbabilityVectors Nash_2Player_NF::indifference_strategies() {

    ProbabilityVectors probVector;

    int tolerance = std::min(this->tol, 0.);

    SupportPairs pairs = this->potential_support_pairs(false);

    for(auto& pair: pairs){

        Eigen::VectorXd prob1, prob2;

        bool res1 = this->solve_indifference_cond(this->ColumnPlayer.transpose(), prob1, pair.second, pair.first);
        bool res2 = this->solve_indifference_cond(this->RowPlayer, prob2, pair.first, pair.second);

        if(res1 & res2){
			bool os_1 = this->obey_support(prob1, pair.first);
            bool os_2 = this->obey_support(prob2, pair.second);

			if(os_1 & os_2){
				probVector.push_back({prob1, prob2, pair.first, pair.second});
            }
        }
	}

    return probVector;
}


bool Nash_2Player_NF::is_ne(ProbabilityVector pv) {

    std::vector<double> row_support_indices, column_support_indices;

    for (const auto& double_element : std::get<2>(pv)) {
        row_support_indices.push_back(double_element);
    }

    for (const auto& double_element : std::get<3>(pv)) {
        column_support_indices.push_back(double_element);
    }

    auto u = std::get<1>(pv);
    Eigen::VectorXd row_payoffs = this->RowPlayer * u;

    auto v = std::get<0>(pv);
    Eigen::VectorXd column_payoffs = this->ColumnPlayer.transpose() * v;

    double* ptr_row_ids = &row_support_indices[0];
    Eigen::Map<Eigen::ArrayXd> row_sups(ptr_row_ids, row_support_indices.size());

    double* ptr_col_ids = &column_support_indices[0];
    Eigen::Map<Eigen::ArrayXd> col_sups(ptr_col_ids, column_support_indices.size());


    Eigen::VectorXd row_support_payoffs = row_payoffs(row_sups);
    Eigen::VectorXd column_support_payoffs = column_payoffs(col_sups);

    return (
        row_payoffs.maxCoeff() == row_support_payoffs.maxCoeff()
        && column_payoffs.maxCoeff() == column_support_payoffs.maxCoeff()
    );
}


void Nash_2Player_NF::print_ne_support() {

    int num_eq = 0;
    std::vector<std::pair<Eigen::VectorXd,Eigen::VectorXd>> nash_equilibrias;

    ProbabilityVectors probability_vectors = this->indifference_strategies();

    for(auto& prob_elmnt : probability_vectors){
        bool is_nash = this->is_ne(prob_elmnt);

		if(is_nash){
            nash_equilibrias.push_back({std::get<0>(prob_elmnt),std::get<1>(prob_elmnt)});
            num_eq++;
        }
    }

	Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

    if(num_eq == 0){
        std::cerr << "==========================================" << std::endl;
        std::cerr << "= NO NASH EQUILIBRIA EXIST FOR THIS GAME =" << std::endl;
        std::cerr << "==========================================" << std::endl;

    } else if(num_eq % 2 == 0){
        std::cerr << "==========================================" << std::endl;
        std::cerr << "======== ERROR : DEGENERATE GAME =========" << std::endl;
        std::cerr << "==========================================" << std::endl;

		std::cout << "FOUND " << nash_equilibrias.size() << " EQUILIBRIA FOR THIS GAME" << std::endl;

        for(auto& eq : nash_equilibrias){
			std::cout << std::endl;
            std::cout << "======== (NOT NASH!!!) EQUILIBRIUM =======" << std::endl;
			std::cout << "================ STRATEGIES ==============" << std::endl;
            std::cout << "Row Player    (Top to Bottom) : [" << eq.first.transpose().format(CleanFmt) << std::endl;
            std::cout << "Column Player (Left to Right) : [" << eq.second.transpose().format(CleanFmt) << std::endl;
            std::cout << "==========================================" << std::endl;
		}

    } else{
		std::cout << std::endl;
		std::cout << "FOUND " << nash_equilibrias.size() << " NASH EQUILIBRIA FOR THIS GAME" << std::endl;

        for(auto& eq : nash_equilibrias){
			std::cout << std::endl;
            std::cout << "============= NASH EQUILIBRIUM ===========" << std::endl;
			std::cout << "================ STRATEGIES ==============" << std::endl;
            std::cout << "Row Player    (Top to Bottom) : " << eq.first.transpose().format(CleanFmt) << std::endl;
            std::cout << "Column Player (Left to Right) : " << eq.second.transpose().format(CleanFmt) << std::endl;
            std::cout << "==========================================" << std::endl;
        }

    }

}







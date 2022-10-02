#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>

typedef std::vector<std::pair<int, int>> pair_vect;
typedef std::pair<int, int>              int_pair;

template<int X, int Max_Solutions = 1>
class ChessBoard {
public:
    ChessBoard(int x, int y)
            : m_num_of_set(1),
              solutions_count(0),
              solved_move_list(),
              solved_boards() {

        // Initialize board
        for (auto &arr: board) {
            arr.fill(0);
        }

        // Set starting position
        board[x][y] = 1;
        move_list.emplace_back(x, y);
    };

    /**
     * Sets a position on given coordinates. All positions are set to an integer
     * number according to the order they are set in
     *
     * If a solution is found, solved board is automatically saved in the class
     * instance.
     *
     * @warning x,y coordinates must be in bound, the function does not check
     * for out of bounds coordinates.
     *
     * @param x coordinate
     * @param y coordinate
     * @return true if move is made | false if field was already set
     */
    bool setPos(const std::pair<int, int> &coords) {
        // Check if field is empty
        if (board[coords.first][coords.second] > 0)
            return false;

        // Make move
        move_list.template emplace_back(coords.first, coords.second);
        board[coords.first][coords.second] = ++m_num_of_set;

        // Check if board is filled out
        if (m_num_of_set >= max) {
            ++solutions_count;
            // Save finished state
            solved_boards.push_back(board);
            solved_move_list.push_back(move_list);
        }
        return true;
    }

    /**
     * Resets the last move.
     *
     * Last set field is reset to 0 and the last move is removed from the list
     * of moves.
     *
     * Function does nothing if only first chess field is set.
     */
    void resetPos() {
        // Check if there are moves to remove
        if (m_num_of_set <= 1)
            return;

        auto &pos = move_list.back();
        board[pos.first][pos.second] = 0;
        --m_num_of_set;
        move_list.pop_back();
    }

    /**
     * Outputs found solutions to output stream.
     *
     * @param os ostream
     * @param chessBoard
     * @return reference to original ostream
     */
    friend std::ostream &
    operator<<(std::ostream &os, ChessBoard<X, Max_Solutions> &chessBoard) {
        os << "Solved boards of size " << X << " x " << X << "\nStarting at ("
           << chessBoard.get_start_pos().first << ", "
           << chessBoard.get_start_pos().second << ")\nFound "
           << chessBoard.solutions_count << " boards\n\n";

        int sol_num = 1;

        for (auto &brd: chessBoard.solved_boards) {
            os << "Solution " << sol_num++ << ":\n";
            for (int i = 0; i < X; ++i) {
                for (int j = 0; j < X; ++j) {
                    os << std::setw(3) << brd[i][j] << " ";
                }
                os << "\n";
            }
            os << "\n";
        }
        return os;
    }

    /**
     * Constant time function that returns the last set tile
     *
     * @return coordinates of last set tile
     */
    const int_pair &get_last_pos() const {
        return move_list.back();
    }

    /**
     * Constant time function that returns the first set tile
     *
     * @return coordinates of the starting tile
     */
    int_pair get_start_pos() const {
        return move_list.front();
    }

    /**
     * Function checks if set number of solutions has been found.
     *
     * @return true if number of found solutions >= required solutions
     */
    bool Finished() {
        return solutions_count >= Max_Solutions;
    }

private:
    //! 2D array containing tile board (chess board)
    std::array<std::array<int, X>, X> board;
    //! Number of set tiles
    int                               m_num_of_set;

    //! List of moves from start
    pair_vect                                      move_list;
    //! Number of tiles / max number of moves
    static const int                               max = X * X;
    //!
    int                                            solutions_count;
    std::vector<pair_vect>                         solved_move_list;
    std::vector<std::array<std::array<int, X>, X>> solved_boards;
};

/**
 * Class, for managing possible moves given board size and tile
 */
class MoveSet {
public:

    /**
     * Constructor
     *
     * @param start_pos 2D coordinates of tile (starting at 0,0)
     * @param max dimensions of the board (max x max)
     */
    MoveSet(const int_pair &start_pos, int max)
            : m_pos(0) {
        // Unpack coordinates
        int pos_x = start_pos.first;
        int pos_y = start_pos.second;

        // Check each possible move and add it to move array if in bounds
        if (pos_x + 1 < max && pos_y + 2 < max)
            m_move_array[m_pos++] = std::make_pair(pos_x + 1, pos_y + 2);
        if (pos_x + 2 < max && pos_y + 1 < max)
            m_move_array[m_pos++] = std::make_pair(pos_x + 2, pos_y + 1);
        if (pos_x - 1 >= 0 && pos_y - 2 >= 0)
            m_move_array[m_pos++] = std::make_pair(pos_x - 1, pos_y - 2);
        if (pos_x - 2 >= 0 && pos_y - 1 >= 0)
            m_move_array[m_pos++] = std::make_pair(pos_x - 2, pos_y - 1);
        if (pos_x + 1 < max && pos_y - 2 >= 0)
            m_move_array[m_pos++] = std::make_pair(pos_x + 1, pos_y - 2);
        if (pos_x + 2 < max && pos_y - 1 >= 0)
            m_move_array[m_pos++] = std::make_pair(pos_x + 2, pos_y - 1);
        if (pos_x - 1 >= 0 && pos_y + 2 < max)
            m_move_array[m_pos++] = std::make_pair(pos_x - 1, pos_y + 2);
        if (pos_x - 2 >= 0 && pos_y + 1 < max)
            m_move_array[m_pos++] = std::make_pair(pos_x - 2, pos_y + 1);

        // Set max number of possible moves and number of requested moves
        m_max = m_pos;
        m_pos = 0;
    }

    /**
     * Returns a unique possible move, until set of all possible moves is used
     *
     * @return tile coordinates or <-1,-1> if no possible moves remain
     */
    std::pair<int, int> getMove() {
        if (m_pos < m_max)
            return m_move_array[m_pos++];
        return std::make_pair(-1, -1);
    }

private:
    int                     m_pos;
    int                     m_max;
    std::array<int_pair, 8> m_move_array;
};

/**
 * Recursive algorithm for finding a solution to Knights tour problem
 *
 * Naive depth first search is used
 *
 * @tparam X dimension of chessboard
 * @tparam Max_Solutions number of solutions searched (could be less, if fewer
 * solutions exist, or if they are not found in set state limit)
 * @tparam Max_states maximal number of states searched
 * @param board initialized ChessBoard used in algorithm
 * @param state_count number of searched states
 * @return true if algorithm has finished (found solutions or reached state
 * limit), false otherwise.
 */
template<int X, int Max_Solutions = 1, int Max_states = 100000000>
bool recursive(ChessBoard<X, Max_Solutions> &board, int & state_count) {

    // Check if state limit has been reached
    if (state_count >= Max_states)
        return true;

    // Create set of possible moves
    MoveSet moves(board.get_last_pos(), X);

    // Iterate over moves
    // Could be refactored to use iterators
    while (true) {

        auto move = moves.getMove();

        // Check if moves are available
        if (move.first == -1) {
            // No possible moves remain, end loop
            break;
        }

        // Attempt to make a move, else continue
        if (board.setPos(move)) {

            // Check if board has finished, or if further recursion leads to
            // required number of solutions.
            if (board.Finished() || recursive(board, ++state_count)) {
                return true;
            }

            // Undo move
            board.resetPos();
        }
    }
    return false;
}

int main() {
    // Initialize generator
    std::random_device rd;
    std::mt19937       gen(rd());

    // Uniform distributions for random starting coordinates
    std::uniform_int_distribution<> distrib_5(0, 4);
    std::uniform_int_distribution<> distrib_6(0, 5);



    // Construct all chessboards
    ChessBoard<5, 5> board5_0_0(0, 0);
    ChessBoard<5, 5> board5_rand_1(distrib_5(gen), distrib_5(gen));
    ChessBoard<5, 5> board5_rand_2(distrib_5(gen), distrib_5(gen));
    ChessBoard<5, 5> board5_rand_3(distrib_5(gen), distrib_5(gen));
    ChessBoard<5, 5> board5_rand_4(distrib_5(gen), distrib_5(gen));
    ChessBoard<6, 5> board6_0_0(0, 0);
    ChessBoard<6, 5> board6_rand_1(distrib_6(gen), distrib_6(gen));
    ChessBoard<6, 5> board6_rand_2(distrib_6(gen), distrib_6(gen));
    ChessBoard<6, 5> board6_rand_3(distrib_6(gen), distrib_6(gen));
    ChessBoard<6, 5> board6_rand_4(distrib_6(gen), distrib_6(gen));



    // Calculate solutions and measure time for each
    const auto time_5_0_0_start = std::chrono::high_resolution_clock::now();
    int x = 0;
    recursive(board5_0_0,x);
    const auto time_5_0_0_end      = std::chrono::high_resolution_clock::now();
    const auto time_5_rand_1_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board5_rand_1,x);
    const auto time_5_rand_1_end   = std::chrono::high_resolution_clock::now();
    const auto time_5_rand_2_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board5_rand_2,x);
    const auto time_5_rand_2_end   = std::chrono::high_resolution_clock::now();
    const auto time_5_rand_3_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board5_rand_3,x);
    const auto time_5_rand_3_end   = std::chrono::high_resolution_clock::now();
    const auto time_5_rand_4_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board5_rand_4,x);
    const auto time_5_rand_4_end = std::chrono::high_resolution_clock::now();
    const auto time_6_0_0_start  = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board6_0_0,x);
    const auto time_6_0_0_end      = std::chrono::high_resolution_clock::now();
    const auto time_6_rand_1_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board6_rand_1,x);
    const auto time_6_rand_1_end   = std::chrono::high_resolution_clock::now();
    const auto time_6_rand_2_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board6_rand_2,x);
    const auto time_6_rand_2_end   = std::chrono::high_resolution_clock::now();
    const auto time_6_rand_3_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board6_rand_3,x);
    const auto time_6_rand_3_end   = std::chrono::high_resolution_clock::now();
    const auto time_6_rand_4_start = std::chrono::high_resolution_clock::now();
    x = 0;
    recursive(board6_rand_4,x);
    const auto time_6_rand_4_end = std::chrono::high_resolution_clock::now();


    // Calculate durations from time points
    std::chrono::duration<double> duration_5_0_0    = time_5_0_0_end -
                                                      time_5_0_0_start;
    std::chrono::duration<double> duration_5_rand_1 = time_5_rand_1_end -
                                                      time_5_rand_1_start;
    std::chrono::duration<double> duration_5_rand_2 = time_5_rand_2_end -
                                                      time_5_rand_2_start;
    std::chrono::duration<double> duration_5_rand_3 = time_5_rand_3_end -
                                                      time_5_rand_3_start;
    std::chrono::duration<double> duration_5_rand_4 = time_5_rand_4_end -
                                                      time_5_rand_4_start;
    std::chrono::duration<double> duration_6_0_0    = time_6_0_0_end -
                                                      time_6_0_0_start;
    std::chrono::duration<double> duration_6_rand_1 = time_6_rand_1_end -
                                                      time_6_rand_1_start;
    std::chrono::duration<double> duration_6_rand_2 = time_6_rand_2_end -
                                                      time_6_rand_2_start;
    std::chrono::duration<double> duration_6_rand_3 = time_6_rand_3_end -
                                                      time_6_rand_3_start;
    std::chrono::duration<double> duration_6_rand_4 = time_6_rand_4_end -
                                                      time_6_rand_4_start;



    // Print solutions to stdout
    std::cout << "Time: " << std::setw(9) << duration_5_0_0.count() << "s\n" << board5_0_0
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_5_rand_2.count() << "s\n" << board5_rand_2
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_5_rand_3.count() << "s\n" << board5_rand_3
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_5_rand_4.count() << "s\n" << board5_rand_4
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_5_rand_1.count() << "s\n" << board5_rand_1
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_6_0_0.count() << "s\n" << board6_0_0
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_6_rand_2.count() << "s\n" << board6_rand_1
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_6_rand_3.count() << "s\n" << board6_rand_2
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_6_rand_4.count() << "s\n" << board6_rand_3
              << std::endl;
    std::cout << "Time: " << std::setw(9) << duration_6_rand_1.count() << "s\n" << board6_rand_4
              << std::endl;

    return 0;
}

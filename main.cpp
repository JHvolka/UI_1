#include <iostream>
#include <array>
#include <vector>
#include <iomanip>

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
        move_list.template emplace_back(x, y);
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
            std::cout << "FOUND" << std::endl;
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
     * Prints out found solutions
     *
     * TODO
     *
     * @param os
     * @param Chboard
     * @return
     */
    friend std::ostream &operator<<(std::ostream &os, ChessBoard<X> &Chboard) {
        for (auto &list: Chboard.solved_move_list) {
            for (auto &itm: list)
                os << "(" << itm.first << ", " << itm.second << ")\n";
            os << "\n\n";
        }
        os << "\n\n";
        for (auto &brd: Chboard.solved_boards) {
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

    /**
     * FIXME DEBUG ONLY METHOD
     */
    void printDebug() {
        for (auto &tmp: board) {
            for (int num: tmp)
                std::cout << std::setw(3) << num << " ";
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "\n";
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

        // FIXME remove
//        std::cout << pos_x << " " << pos_y << ":\t";
//        for (int i = 0; i < m_max; ++i) {
//
//            std::cout << "(" << m_move_array[i].first << ","
//                      << m_move_array[i].second << ") ";
//        }
//        std::cout << "\n";
        // FIXME \remove
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


// FIXME remove
uint64_t move_count = 0;


template<int X>
bool recursive(ChessBoard<X> &board, int depth) {
//    std::cout << "Recurse " << depth << std::endl; // FIXME remove
//    board.printDebug(); // FIXME remove

    // Create set of possible moves
    MoveSet moves(board.get_last_pos(), X);

    // Iterate over moves
    // Could be refactored to use iterators
    while (true) {

        auto move = moves.getMove();

        // Check if moves are available
        if (move.first == -1) {
            // No possible moves remain, end loop
//            std::cout << "NO MOVES!!!" << std::endl; // FIXME remove
            break;
        }

//        std::cout << "move " << move_count++ << ": " << move.first
//                  << " " // FIXME remove
//                  << move.second << "\n";

        // Attempt to make a move, else continue
        if (board.setPos(move)) {
//            std::cout << "Pos set" << std::endl; // FIXME remove

            // Check if board has finished, or if further recursion leads to
            // required number of solutions.
            if (board.Finished() || recursive(board, depth + 1)) {
//                std::cout << "Finished" << std::endl; // FIXME remove
                return true;
            }

            // Undo move
            board.resetPos();
        }
    }
    return false;
}

int main() {

    ChessBoard<5> board(0, 0);
    recursive(board, 1);

    std::cout << board;


    return 0;
}

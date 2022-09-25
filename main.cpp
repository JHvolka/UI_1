#include <iostream>
#include <array>
#include <vector>

typedef std::vector <std::pair<int, int>> pair_vect;
typedef std::pair<int, int>               int_pair;

template<int X, bool end_before_all = true, int Max_Solutions = 1>
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
     * @warning x,y coordinates must be in bound, the function does not check
     * for out of bounds coordinates
     *
     * @param x
     * @param y
     * @return
     */
    bool setPos(const int x, const int y) {
        // Check if field is empty
        if (board[x][y] > 0)
            return false;

        // Make move
        move_list.template emplace_back(x, y);
        board[x][y] = ++m_num_of_set;

        // Check if board is filled out
        if (m_num_of_set > max) {
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
     * Last set field is set to 0 and the last move is removed from the list.
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
     * TODO
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
                    os << brd[i][j] << " ";
                }
                os << "\n";
            }
            os << "\n\n";
        }
        return os;
    }

    const int_pair &get_last_pos() const {
        return move_list.back();
    }

    int_pair get_start_pos() const {
        return move_list.front();
    }

    bool Finished() {
        return solutions_count >= Max_Solutions;
    }

private:
    std::array <std::array<int, X>, X> board;
    int                                m_num_of_set;
    pair_vect                          move_list;
    static const int                   max = X * X;
    int                                solutions_count;
    std::vector <pair_vect>            solved_move_list;
    std::vector <std::array<std::array < int, X>, X>> solved_boards;
};

/**
 * TODO
 */
class MoveSet {
public:
    MoveSet(const int_pair &start_pos, int max)
            : m_pos(0) {
        int pos_x = start_pos.first;
        int pos_y = start_pos.second;
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

        m_max = m_pos;
        m_pos = 0;

        std::cout << pos_x << " " << pos_y << ":\t";
        for (int i = 0; i < m_max; ++i) {

            std::cout << "(" << m_move_array[i].first << ","
                      << m_move_array[i].second << ") ";
        }
        std::cout << "\n";
    }

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

uint64_t move_count = 0;

template<int X>
bool recursive(ChessBoard<X> &board, int depth) {
    std::cout << "Recurse " << depth << std::endl;

    MoveSet moves(board.get_last_pos(), X);

    while (true) {

        auto move = moves.getMove();
        if (move.first == -1) {
            std::cout << "NO MOVES!!!" << std::endl;
            break;
        }

        std::cout << "move " << move_count++ << ": " << move.first << " "
                  << move.second << "\n";

        if (board.setPos(move.first, move.second)) {
            std::cout << "Pos set" << std::endl;


            if (board.Finished() || recursive(board, depth+1)) {
                std::cout << "Finished" << std::endl;
                return true;
            }
        }
        board.resetPos();
    }
    return false;
}

int main() {

    ChessBoard<6> board(0, 0);
    recursive(board,1);

    std::cout << board;


    return 0;
}

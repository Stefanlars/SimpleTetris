#ifndef SIMPLETETRIS_SCENERENDERER_H
#define SIMPLETETRIS_SCENERENDERER_H
#include <chrono>
#include <thread>
#include <vector>
#include <curses.h>  // PDCurses or ncurses

#include "Blocks/Block.h"

class SceneRenderer {
    std::vector<std::vector<char>> grid{20, std::vector<char>(10, '.')};
    bool gameRunning = false;

public:
    SceneRenderer() = default;

    std::vector<std::vector<char>>* getGrid() {
        return &grid;
    }

    // Initialize curses; call before startGame or in startGame
    void initCurses() {
        initscr();            // Start curses mode
        cbreak();             // Disable line buffering
        noecho();             // Don't echo typed characters
        keypad(stdscr, TRUE); // Enable special keys
        curs_set(0);          // Hide cursor
        // Optional: make getch non-blocking if you plan to poll input
        // nodelay(stdscr, TRUE);
    }

    void shutdownCurses() {
        endwin();             // Restore terminal
    }

    void startGame() {
        initCurses();
        gameRunning = true;

        int i = 0;
        while (gameRunning) {
            update(i);
            render(); // curses-based render
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            i++;
            if (i > 20) {
                endGame();
            }
        }

        shutdownCurses();
    }

    void endGame() {
        gameRunning = false;
    }

    void update(int offset) {
        resetGrid();

        constexpr Point initPosition = {4, 2};
        Block block(initPosition, BlockType::L);

        auto currentPositions = block.getCurrentPosition();

        if (offset < 18) {
            for (int i = 0; i < 4; i++) {
                grid[currentPositions[i].y + offset][currentPositions[i].x] = 'O';
            }
        }
    }

    void render() const {
        // Clear the curses screen
        clear();

        // Draw the grid starting at some top-left offset, e.g., (0,0)
        // Use mvaddch(row, col, ch) to place characters
        for (int r = 0; r < (int)grid.size(); ++r) {
            for (int c = 0; c < (int)grid[r].size(); ++c) {
                // Add spacing similar to " std::cout << ' ' << cell << ' '; "
                // One common approach is to draw each cell and maybe a space
                mvaddch(r, c * 2, grid[r][c]);
                mvaddch(r, c * 2 + 1, ' ');
            }
        }

        // Optional: draw a status line below the grid
        mvprintw((int)grid.size() + 1, 0, "Press q to quit");

        // Flush changes to the terminal
        refresh();
    }

    void resetGrid() {
        grid = std::vector<std::vector<char>>(20, std::vector<char>(10, '.'));
    }
};

#endif // SIMPLETETRIS_SCENERENDERER_H
#ifndef SIMPLETETRIS_SCENERENDERER_H
#define SIMPLETETRIS_SCENERENDERER_H
#include <chrono>
#include <thread>
#include <vector>
#include <curses.h>  // PDCurses or ncurses
#include <iostream>
#include <mutex>

#include "GameGrid.h"
#include "Blocks/Block.h"

class SceneRenderer {

    std::atomic<bool> gameRunning{false};

    std::mutex nCursesMutex;

    std::mutex blockMutex;

    int updateCounter = 0;
    int renderCounter = 0;

    // Configurable drop speed (milliseconds between auto-drops)
    int dropInterval = 300;  // 1 second by default
    int dropTimer = 0;

    GameGrid grid;

    std::optional<Block> activeBlock;

    // Returns true if spawn was successful, false if game over
    bool spawnNewBlock() {
        BlockType types[] = {BlockType::I, BlockType::O, BlockType::T,
                            BlockType::Z, BlockType::J, BlockType::L};
        BlockType randomType = types[rand() % 6];

        BlockColor colors[] = {BlockColor:: CYAN, BlockColor:: YELLOW,
                              BlockColor:: PURPLE, BlockColor::GREEN,
                              BlockColor::RED, BlockColor::BLUE, BlockColor::ORANGE};
        BlockColor randomColor = colors[rand() % 7];

        Point spawnPos = {5, 2};

        // Create the new block
        Block newBlock(spawnPos, randomType, randomColor, &grid);

        // Check if the spawn position is valid
        auto spawnPositions = newBlock.getCurrentPosition();
        for (const auto& pos : spawnPositions) {
            if (! grid.isValidPosition(pos)) {
                // Game over - can't spawn new block
                return false;
            }
        }

        // Valid spawn - replace the active block
        activeBlock = newBlock;
        dropTimer = 0;  // Reset drop timer

        return true;
    }

public:
    SceneRenderer() = default;

    // Initialize curses; call before startGame or in startGame
    void initCurses() {
        std::lock_guard<std::mutex> lock(nCursesMutex);

        initscr();            // Start curses mode
        // Check if terminal supports colors
        if (! has_colors()) {
            endwin();
            std::cerr << "Your terminal does not support colors!" << std::endl;
            exit(1);
        }

        start_color();        // Enable color support

        // Check if we can change colors
        if (! can_change_color()) {
            // This is normal for most terminals
        }

        cbreak();             // Disable line buffering
        noecho();             // Don't echo typed characters
        keypad(stdscr, TRUE); // Enable special keys
        curs_set(0);          // Hide cursor
        // Optional: make getch non-blocking if you plan to poll input
        nodelay(stdscr, TRUE);


        // Initialize color pairs (matching BlockColor enum values)
        init_pair(1, COLOR_CYAN, COLOR_BLACK);      // BlockColor::CYAN
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);    // BlockColor::YELLOW
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK);   // BlockColor::PURPLE
        init_pair(4, COLOR_GREEN, COLOR_BLACK);     // BlockColor::GREEN
        init_pair(5, COLOR_RED, COLOR_BLACK);       // BlockColor::RED
        init_pair(6, COLOR_BLUE, COLOR_BLACK);      // BlockColor::BLUE
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);
    }

    void shutdownCurses() {
        std::lock_guard<std::mutex> lock(nCursesMutex);

        endwin();             // Restore terminal
    }

    void startGame() {
        initCurses();
        gameRunning.store(true);

        // grid.createDummyData();

        // Use emplace to construct the Block in-place
        activeBlock.emplace(Point{5, 5}, BlockType::Z, BlockColor::BLUE, &grid);

        std::thread updateThread(&SceneRenderer::updateThreadTest, this);
        std::thread renderThread(&SceneRenderer::renderThreadTest, this);
        std::thread inputThread(&SceneRenderer::inputThread, this);


        updateThread.join();
        renderThread.join();
        inputThread.join();

        shutdownCurses();
    }

    void endGame() {
        gameRunning.store(false);
    }

    void inputThread() {

        while (gameRunning. load()) {
            int ch;
            {
                std::lock_guard<std::mutex> lock(nCursesMutex);
                ch = getch();
            }

            if (ch != ERR) {

                if (ch == 'q' || ch == 'Q') {
                    endGame();
                }

                if (activeBlock.has_value()) {
                    // Lock block access
                    std::lock_guard<std::mutex> blockLock(blockMutex);

                    MoveResult moveResult = MoveResult::BLOCKED;


                    // Handle other keys
                    if (ch == KEY_LEFT) {
                        moveResult = activeBlock.value().moveBlock(BlockMove::LEFT);
                    }
                    else if (ch == KEY_RIGHT) {
                        // Move right

                        moveResult = activeBlock.value().moveBlock(BlockMove::RIGHT);

                    }
                    else if (ch == KEY_DOWN) {
                        // Move down faster

                        moveResult = activeBlock.value().moveBlock(BlockMove::DOWN);

                    }
                    else if (ch == KEY_UP || ch == ' ') {
                        // Rotate

                        moveResult = activeBlock.value().moveBlock(BlockMove::ROTATE);

                    }

                    if (moveResult == MoveResult::LOCKED) {
                        auto spawnResult = spawnNewBlock();

                        if (spawnResult == false) {
                            gameRunning.store(false);
                        }
                    }
                }

            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }


    void updateThreadTest() {
        const int updateIntervalMs = 100;  // Update every 100ms

        while (gameRunning.load()) {
            updateCounter++;
            dropTimer += updateIntervalMs;

            // Auto-drop the block when timer reaches the interval
            if (dropTimer >= dropInterval) {
                std::lock_guard<std::mutex> blockLock(blockMutex);

                if (activeBlock.has_value()) {
                    MoveResult result = activeBlock->moveBlock(BlockMove::DOWN);

                    if (result == MoveResult::LOCKED) {
                        bool spawnResult = spawnNewBlock();
                        if (spawnResult == false) {
                            gameRunning.store(false);
                        }
                    }
                }

                dropTimer = 0;  // Reset the timer
            }

            std:: this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
        }
    }

    void renderThreadTest() {

        do {
            {

                //skip first 4 rows

                int rowsToSkip = 4;

                std::lock_guard<std::mutex> lock(nCursesMutex);
                // Clear the curses screen
                auto currentGrid = grid.getGrid();
                auto coloredBlocks = grid.getColorGrid();

                clear();

                // First pass: Draw the base grid (all '.' or '#')
                for (int r = 0; r < static_cast<int>(currentGrid. size()); ++r) {
                    if (r <= rowsToSkip - 1) continue;

                    for (int c = 0; c < static_cast<int>(currentGrid[r].size()); ++c) {
                        char cell = currentGrid[r][c];
                        mvaddch(r, c * 2, cell);
                        mvaddch(r, c * 2 + 1, ' ');
                    }
                }

                // Second pass: Draw colored blocks on top
                for (const auto& colorPos : coloredBlocks) {
                    int x = colorPos.position.x;
                    int y = colorPos.position.y;
                    BlockColor color = colorPos.color;

                    // Validate position
                    if (x >= 0 && x < 10 && y >= rowsToSkip && y < currentGrid.size()) {
                        if (color != BlockColor::NONE) {
                            // Turn on the color
                            attron(COLOR_PAIR(static_cast<int>(color)));

                            // Draw the colored block
                            mvaddch(y, x * 2, '#');
                            mvaddch(y, x * 2 + 1, ' ');

                            // Turn off the color
                            attroff(COLOR_PAIR(static_cast<int>(color)));
                        }
                    }
                }

                // Third pass:  Draw active block (if it exists)
                if (activeBlock.has_value()) {
                    BlockColor blockColor = activeBlock->getColor();
                    auto blockPositions = activeBlock->getCurrentPosition();

                    attron(COLOR_PAIR(static_cast<int>(blockColor)));

                    for (const auto& pos : blockPositions) {
                        if (pos.x >= 0 && pos.x < 10 && pos.y >= rowsToSkip && pos.y < currentGrid.size()) {
                            mvaddch(pos.y, pos.x * 2, '#');
                            mvaddch(pos.y, pos.x * 2 + 1, ' ');
                        }
                    }

                    attroff(COLOR_PAIR(static_cast<int>(blockColor)));
                }



                // Optional: draw a status line below the grid
                mvprintw(static_cast<int>(currentGrid.size()) + 1, 0, "Press q to quit, %d", renderCounter);

                // Flush changes to the terminal
                refresh();

            }

            renderCounter++;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));

        } while (gameRunning.load());


        {
            std::lock_guard<std::mutex> blockLock(nCursesMutex);

            auto currentGrid = grid.getGrid();
            mvprintw(static_cast<int>(currentGrid.size()) + 3, 0, "GAME OVER");

            refresh();
        }

        // wait 10 seconds to show user has lost
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));


    }
};

#endif // SIMPLETETRIS_SCENERENDERER_H
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

    GameGrid grid;

    std::optional<Block> activeBlock;

    void spawnNewBlock() {
        BlockType types[] = {BlockType::I, BlockType::O, BlockType::T,
                            BlockType::Z, BlockType::J, BlockType::L};
        BlockType randomType = types[rand() % 6];

        BlockColor colors[] = {BlockColor::CYAN, BlockColor::YELLOW,
                              BlockColor::PURPLE, BlockColor::GREEN,
                              BlockColor::RED, BlockColor:: BLUE, BlockColor::ORANGE};
        BlockColor randomColor = colors[rand() % 7];

        Point spawnPos = {5, 2};

        activeBlock.emplace(spawnPos, randomType, randomColor, &grid);
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
                        spawnNewBlock();
                    }
                }

            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }


    void updateThreadTest(){

        do {

            updateCounter++;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        } while (gameRunning.load());

    }

    void renderThreadTest() {

        do {
            {

                std::lock_guard<std::mutex> lock(nCursesMutex);
                // Clear the curses screen
                auto currentGrid = grid.getGrid();
                auto coloredBlocks = grid.getColorGrid();

                clear();

                // First pass: Draw the base grid (all '.' or '#')
                for (int r = 0; r < static_cast<int>(currentGrid. size()); ++r) {
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
                    if (x >= 0 && x < 10 && y >= 0 && y < 20) {
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
                        if (pos.x >= 0 && pos.x < 10 && pos.y >= 0 && pos.y < 20) {
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
    }
};

#endif // SIMPLETETRIS_SCENERENDERER_H
//
// Created by sdalp on 12/13/2025.
//

#ifndef SIMPLETETRIS_SCENERENDERER_H
#define SIMPLETETRIS_SCENERENDERER_H
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "Blocks/Block.h"


class SceneRenderer {

    // 20 rows, 10 columns
    std::vector<std::vector<char>> grid{20, std::vector<char>(10, '.')};

    bool gameRunning = false;

    public:
        SceneRenderer() = default;


        // returns a pointer for the grid
        std::vector<std::vector<char>>* getGrid() {
            return &grid;
        }

        void startGame() {
            gameRunning = true;

            int i = 0;
            while (gameRunning) {


                update(i);

                render();

                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                i++;
                if (i > 20) {
                    endGame();
                }
            }
        }

        void endGame() {
            gameRunning = false;
        }

        // test running the rendering
        void update(int offset) {

            resetGrid();

            constexpr Point initPosition = {4, 2};
            Block block(initPosition, BlockType::L);

            auto currentPositions = block.getCurrentPosition();
            // Mark block positions:

            if (offset < 18) {
                for (int i = 0; i < 4; i++) {
                    grid[currentPositions[i].y + offset][currentPositions[i].x] = 'O';
                }
            }

        }

        void render() const {

            // Print your screen
            for (const auto& row : grid) {
                for (const char cell : row) {
                    std::cout << " " << cell << " ";
                }
                std::cout << "\n";
            }

            std::cout << std::flush;  // Force output

        }


        void resetGrid() {
            grid = std::vector<std::vector<char>>(20, std::vector<char>(10, '.'));
        }

};


#endif //SIMPLETETRIS_SCENERENDERER_H
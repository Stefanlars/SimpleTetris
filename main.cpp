//
// Created by sdalp on 12/7/2025.
//


#include <iostream>

#include "Blocks/Block.h"

#include <vector>

int main() {
    constexpr Point initPosition = {4, 5};
    Block testBlock(initPosition, BlockType::L);

    auto currentPositions = testBlock.getCurrentPosition();

    // Use vector of vectors:
    std::vector<std::vector<char>> screen(20, std::vector<char>(10, '.'));

    // Mark block positions:
    for (int i = 0; i < 4; i++) {
        screen[currentPositions[i][0]][currentPositions[i][1]] = 'O';
    }

    // Print the screen:
    for (auto & i : screen) {
        for (const char j : i) {
            std::cout << " " << j << " ";
        }
        std::cout << std::endl;
    }
}

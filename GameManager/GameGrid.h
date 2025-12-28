//
// Created by sdalp on 12/16/2025.
//

#ifndef SIMPLETETRIS_GAMEGRID_H
#define SIMPLETETRIS_GAMEGRID_H
#include <vector>
#include <enums.h>

struct ColorPosition {
    Point position;
    BlockColor color;
};

class GameGrid {

    std::vector<std::vector<char>> grid{24, std::vector<char>(10, '.')};

    // empty until game starts populating it
    std::vector<ColorPosition> colorGrid{

    };

    // Helper function to check if a position has a block
    bool hasBlockAt(int x, int y) const {
        for (const auto& colorPos : colorGrid) {
            if (colorPos.position.x == x && colorPos.position.y == y) {
                return true;
            }
        }
        return false;
    }

    // Helper function to check if a row is completely filled
    bool isRowFilled(int row) const {
        int width = grid[0].size();  // Should be 10

        for (int x = 0; x < width; x++) {
            if (!hasBlockAt(x, row)) {
                return false;  // Found an empty spot
            }
        }

        return true;  // All positions filled
    }

    // Remove all blocks from a specific row
    void removeRow(int row) {
        // Use erase-remove idiom to remove all blocks in this row
        colorGrid.erase(
            std::remove_if(colorGrid. begin(), colorGrid.end(),
                [row](const ColorPosition& cp) {
                    return cp.position. y == row;
                }),
            colorGrid.end()
        );
    }

    // Move all blocks above a certain row down by one
    void moveRowsDown(int aboveRow) {
        for (auto& colorPos : colorGrid) {
            if (colorPos.position.y < aboveRow) {
                colorPos.position.y++;
            }
        }
    }

    // some kind of dynamic array of blocks with their positions

    public:

        int deleteFilledRows() {
            int rowsCleared = 0;

            // Check from bottom to top
            for (int row = grid.size() - 1; row >= 0; row--) {
                if (isRowFilled(row)) {
                    removeRow(row);
                    moveRowsDown(row);
                    rowsCleared++;

                    // Check the same row again since blocks moved down
                    row++;
                }
            }

            return rowsCleared;
            // Optional: return number of rows cleared for scoring
            // return rowsCleared;
        }


        std::vector<std::vector<char>> getGrid() {
            return grid;
        }

        std::vector<ColorPosition> getColorGrid() {
            return colorGrid;
        }


        // Create some dummy data for testing
        void createDummyData() {
            // Clear any existing data
            colorGrid.clear();

            // Bottom row (row 19) - mix of colors
            colorGrid.push_back({{0, 19}, BlockColor::CYAN});
            colorGrid.push_back({{1, 19}, BlockColor::YELLOW});
            colorGrid.push_back({{2, 19}, BlockColor::PURPLE});
            colorGrid.push_back({{3, 19}, BlockColor::GREEN});
            colorGrid.push_back({{4, 19}, BlockColor::RED});
            colorGrid.push_back({{5, 19}, BlockColor::BLUE});
            colorGrid.push_back({{6, 19}, BlockColor:: ORANGE});
            colorGrid.push_back({{7, 19}, BlockColor:: CYAN});
            colorGrid.push_back({{8, 19}, BlockColor::YELLOW});
            colorGrid.push_back({{9, 19}, BlockColor:: PURPLE});

            // Row 18 - alternating cyan blocks
            colorGrid.push_back({{0, 18}, BlockColor:: CYAN});
            colorGrid.push_back({{2, 18}, BlockColor:: CYAN});
            colorGrid.push_back({{4, 18}, BlockColor::CYAN});
            colorGrid.push_back({{6, 18}, BlockColor::CYAN});
            colorGrid.push_back({{8, 18}, BlockColor::CYAN});

            // Row 17 - yellow horizontal line
            colorGrid.push_back({{2, 17}, BlockColor:: YELLOW});
            colorGrid.push_back({{3, 17}, BlockColor:: YELLOW});
            colorGrid.push_back({{4, 17}, BlockColor::YELLOW});
            colorGrid.push_back({{5, 17}, BlockColor:: YELLOW});
            colorGrid.push_back({{6, 17}, BlockColor::YELLOW});
            colorGrid.push_back({{7, 17}, BlockColor:: YELLOW});

            // Row 16 - red L-shape
            colorGrid.push_back({{3, 16}, BlockColor::RED});
            colorGrid.push_back({{4, 16}, BlockColor::RED});
            colorGrid.push_back({{5, 16}, BlockColor::RED});

            // Row 15 - green blocks
            colorGrid.push_back({{1, 15}, BlockColor::GREEN});
            colorGrid.push_back({{2, 15}, BlockColor::GREEN});
            colorGrid.push_back({{7, 15}, BlockColor::GREEN});
            colorGrid.push_back({{8, 15}, BlockColor::GREEN});

            // Row 14 - blue blocks
            colorGrid.push_back({{7, 14}, BlockColor:: BLUE});
            colorGrid.push_back({{8, 14}, BlockColor::BLUE});
            colorGrid.push_back({{9, 14}, BlockColor::BLUE});

            // Row 13 - purple T-shape
            colorGrid.push_back({{4, 13}, BlockColor:: PURPLE});
            colorGrid.push_back({{5, 13}, BlockColor::PURPLE});
            colorGrid.push_back({{6, 13}, BlockColor::PURPLE});

            // Row 10 - floating orange blocks
            colorGrid.push_back({{2, 10}, BlockColor:: ORANGE});
            colorGrid.push_back({{3, 10}, BlockColor::ORANGE});

            // Row 8 - scattered blocks
            colorGrid.push_back({{5, 8}, BlockColor:: CYAN});
            colorGrid.push_back({{6, 8}, BlockColor::CYAN});

            // Row 5 - single blocks
            colorGrid.push_back({{1, 5}, BlockColor::RED});
            colorGrid.push_back({{8, 5}, BlockColor:: BLUE});

        }

        bool isValidPosition(Point position) {
            // first check if in bounds
            if (position.y < 0 || position.y >= grid.size()) {
                return false;
            }

            if (position.x < 0 || position.x >= grid[0].size()) {
                return false;
            }

            // then check if touching another block
            for (const auto colorPosition : colorGrid) {
                if (position.x == colorPosition.position.x && position.y == colorPosition.position.y) {
                    return false;
                }
            }

            return true;

        }

        void addColorBlocks(BlockData block) {

            for (const auto position: block.positions) {
                colorGrid.push_back({position, block.color});
            }

            deleteFilledRows();

        }




};


#endif //SIMPLETETRIS_GAMEGRID_H
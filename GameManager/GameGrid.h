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

    std::vector<std::vector<char>> grid{20, std::vector<char>(10, '.')};

    // empty until game starts populating it
    std::vector<ColorPosition> colorGrid{

    };


    // some kind of dynamic array of blocks with their positions

    public:


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




};


#endif //SIMPLETETRIS_GAMEGRID_H
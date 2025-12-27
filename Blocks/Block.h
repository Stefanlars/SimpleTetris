//
// Created by sdalp on 12/7/2025.
//

#ifndef SIMPLETETRIS_BLOCK_H
#define SIMPLETETRIS_BLOCK_H
#include <array>

#include "enums.h"
#include "GameManager/GameGrid.h"

// I (straight line),
// O (square),
// T (T-shape),
// S (skew),
// Z (reverse skew),
// J (L-shape),
// and L (reverse L-shape)






class Block {
    Point centerPosition;
    std::array<Point, 4> blockPositions{};
    BlockColor color;
    GameGrid* gameGrid;


    // Helper method to check if all positions of the block are valid
    [[nodiscard]] bool areAllPositionsValid(const Point& testCenterPos) const {
        for (int i = 0; i < 4; i++) {
            Point testPos;
            testPos.x = testCenterPos.x + blockPositions[i].x;
            testPos.y = testCenterPos.y + blockPositions[i].y;

            if (!gameGrid->isValidPosition(testPos)) {
                return false;
            }
        }
        return true;
    }


public:
    explicit Block(
        const Point initPosition,
        const BlockType blockOption,
        const BlockColor blockColor,
        GameGrid* gameGrid
        ):
    centerPosition(initPosition),
    color(blockColor),
    gameGrid(gameGrid)
    {
        // center position is always 2

        this->blockPositions[2].y = 0;
        this->blockPositions[2].x = 0;

        switch (blockOption) {
            // line block
            case BlockType::I:
                // Left side
                blockPositions[0].y = 0;
                blockPositions[0].x = -2;


                blockPositions[1].y = 0;
                blockPositions[1].x = -1;


                blockPositions[3].y = 0;
                blockPositions[3].x = 1;
                break;
            // Square
            case BlockType::O:
                // up 1
                blockPositions[0].y = -1;
                blockPositions[0].x = 0;

                // up & right 1
                blockPositions[1].y = 1;
                blockPositions[1].x = 1;

                // right 1
                blockPositions[3].y = 0;
                blockPositions[3].x = 1;
                break;
            // T shaped
            case BlockType::T:
                // up 1
                blockPositions[0].y = -1;
                blockPositions[0].x = 0;

                blockPositions[1].y = 0;
                blockPositions[1].x = -1;

                blockPositions[3].y = 0;
                blockPositions[3].x = 1;
                break;

            case BlockType::Z:
                blockPositions[0].y = -1;
                blockPositions[0].x = -1;

                blockPositions[1].y = -1;
                blockPositions[1].x = 0;

                blockPositions[3].y = 0;
                blockPositions[3].x = 1;
                break;

            case BlockType::J:
                blockPositions[0].y = -2;
                blockPositions[0].x = 0;

                blockPositions[1].y = -1;
                blockPositions[1].x = 0;

                blockPositions[3].y = 0;
                blockPositions[3].x = -1;
                break;

            case BlockType::L:
                blockPositions[0].y = -2;
                blockPositions[0].x = 0;

                blockPositions[1].y = -1;
                blockPositions[1]. x = 0;

                blockPositions[3].y = 0;
                blockPositions[3].x = 1;
                break;

            default: ;
        }
    }

    void setCenterPosition(const Point position) {
        this->centerPosition = position;
    };

    [[nodiscard]] std::array<Point, 4> getCurrentPosition() const {

        std::array<Point, 4> returnArray{};
        for (int i = 0; i < 4; i++) {
            returnArray[i].x = centerPosition.x + this->blockPositions[i].x;
            returnArray[i].y = centerPosition.y + this->blockPositions[i].y;
        }

        return returnArray;

    }

    [[nodiscard]] BlockColor getColor() const {
        return this->color;
    }


    MoveResult moveBlock(const BlockMove move) {
        // create a copy of the current block. perform the movement.
        // check if it conflicts with a current block.
        //
        // if down or QUICK_DOWN lock block and add to color grid in gamegrid.

        // any other movement just don't allow the user to perform it

        Point testPos = centerPosition;

        switch (move) {
            case BlockMove::ROTATE:
                // up for now
                testPos.y -= 1;
                break;

            case BlockMove::LEFT:

                testPos.x -= 1;
                break;

            case BlockMove::RIGHT:
                testPos.x += 1;
                break;

            case BlockMove::DOWN:
                testPos.y += 1;
                break;

            case BlockMove::QUICK_DOWN:
                break;

            default:
                break;

        }



        if (areAllPositionsValid(testPos)) {
            centerPosition = testPos;
            return MoveResult::MOVED;
        }
        if (move == BlockMove:: DOWN) {
            // Get current positions and add them to the grid
            auto positions = getCurrentPosition();

            gameGrid->addColorBlocks({positions, color});
            return MoveResult::LOCKED;
        }

        return MoveResult::BLOCKED;

    }



};


#endif //SIMPLETETRIS_BLOCK_H
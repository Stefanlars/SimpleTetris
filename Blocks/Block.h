//
// Created by sdalp on 12/7/2025.
//

#ifndef SIMPLETETRIS_BLOCK_H
#define SIMPLETETRIS_BLOCK_H
#include <array>

#include "enums.h"
#include "GameManager/GameGrid.h"


class Block {
    Point centerPosition;
    std::array<Point, 4> blockPositions{};
    BlockColor color;
    GameGrid* gameGrid;

    int rotation = 0;


    // Helper method to check if all positions of the block are valid
    [[nodiscard]] bool areAllPositionsValid(const Point& testCenterPos) const {
        for (int i = 0; i < 4; i++) {
            Point testPos{};
            testPos.x = testCenterPos.x + blockPositions[i].x;
            testPos.y = testCenterPos.y + blockPositions[i].y;

            if (!gameGrid->isValidPosition(testPos)) {
                return false;
            }
        }
        return true;
    }

    // Helper to validate with custom positions (for rotation testing)
    [[nodiscard]] bool areAllPositionsValidWithCustom(
        const Point& testCenterPos,
        const std::array<Point, 4>& testPositions
    ) const {
        for (int i = 0; i < 4; i++) {
            Point testPos{};
            testPos.x = testCenterPos.x + testPositions[i].x;
            testPos.y = testCenterPos.y + testPositions[i].y;

            if (! gameGrid->isValidPosition(testPos)) {
                return false;
            }
        }
        return true;
    }

    void rotateBlock() {
        // 90-degree clockwise rotation formula:
        // new_x = old_y
        // new_y = -old_x

        for (auto& pos : blockPositions) {
            const int oldX = pos.x;
            const int oldY = pos.y;

            pos.x = oldY;
            pos.y = -oldX;
        }
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
        Point testPos = centerPosition;
        auto testPositions = blockPositions;  // Copy current positions
        bool isRotation = false;

        switch (move) {
            case BlockMove::ROTATE:
                // Rotate the test positions
                for (auto& pos : testPositions) {
                    int oldX = pos.x;
                    int oldY = pos.y;
                    pos.x = oldY;
                    pos.y = -oldX;
                }
                isRotation = true;
                break;

            case BlockMove::LEFT:
                testPos.x -= 1;
                break;

            case BlockMove::RIGHT:
                testPos.x += 1;
                break;

            case BlockMove:: DOWN:
                testPos.y += 1;
                break;

            case BlockMove::QUICK_DOWN:
                // Drop all the way down
                while (areAllPositionsValid(Point{testPos.x, testPos.y + 1})) {
                    testPos.y += 1;
                }
                // Lock immediately
                centerPosition = testPos;
                auto positions = getCurrentPosition();
                gameGrid->addColorBlocks({positions, color});
                return MoveResult::LOCKED;
                break;
        }

        // For rotation, check with modified positions
        if (isRotation) {
            if (areAllPositionsValidWithCustom(testPos, testPositions)) {
                // Valid rotation - commit it
                blockPositions = testPositions;
                centerPosition = testPos;
                return MoveResult::MOVED;
            } else {
                // Try wall kicks
                Point kicks[] = {{-1, 0}, {1, 0}, {0, -1}, {-2, 0}, {2, 0}};

                for (const auto& kick :  kicks) {
                    Point kickPos = {testPos.x + kick.x, testPos.y + kick. y};
                    if (areAllPositionsValidWithCustom(kickPos, testPositions)) {
                        blockPositions = testPositions;
                        centerPosition = kickPos;
                        return MoveResult::MOVED;
                    }
                }

                // Rotation failed
                return MoveResult::BLOCKED;
            }
        }

        // For movement, check with current positions
        if (areAllPositionsValid(testPos)) {
            centerPosition = testPos;
            return MoveResult::MOVED;
        }

        // If move is DOWN and blocked, lock the block
        if (move == BlockMove::DOWN) {
            auto positions = getCurrentPosition();
            gameGrid->addColorBlocks({positions, color});
            return MoveResult::LOCKED;
        }

        return MoveResult:: BLOCKED;
    }

};


#endif //SIMPLETETRIS_BLOCK_H
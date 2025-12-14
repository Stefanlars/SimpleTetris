//
// Created by sdalp on 12/7/2025.
//

#ifndef SIMPLETETRIS_BLOCK_H
#define SIMPLETETRIS_BLOCK_H
#include <array>
#include <string>

// I (straight line),
// O (square),
// T (T-shape),
// S (skew),
// Z (reverse skew),
// J (L-shape),
// and L (reverse L-shape)


enum class BlockType { I, O, T, S, Z, J, L };


struct Point {
    int x;
    int y;
};

class Block {
    Point centerPosition;
    std::array<Point, 4> blockPositions{};


public:
    explicit Block(const Point initPosition, const BlockType blockOption): centerPosition(initPosition) {
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

    std::array<Point, 4> getCurrentPosition() const {

        std::array<Point, 4> returnArray{};
        for (int i = 0; i < 4; i++) {
            returnArray[i].x = centerPosition.x + this->blockPositions[i].x;
            returnArray[i].y = centerPosition.y + this->blockPositions[i].y;
        }

        return returnArray;

    }





};


#endif //SIMPLETETRIS_BLOCK_H
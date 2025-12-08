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
    int blockPositions[4][2]{};


public:
    explicit Block(const Point initPosition, const BlockType blockOption): centerPosition(initPosition) {
        // center position is always 2

        this->blockPositions[2][0] = 0;
        this->blockPositions[2][1] = 0;

        switch (blockOption) {
            // line block
            case BlockType::I:
                // Left side
                blockPositions[0][0] = 0;
                blockPositions[0][1] = -2;


                blockPositions[1][0] = 0;
                blockPositions[1][1] = -1;


                blockPositions[3][0] = 0;
                blockPositions[3][1] = 1;
                break;
            // Square
            case BlockType::O:
                // up 1
                blockPositions[0][0] = -1;
                blockPositions[0][1] = 0;

                // up & right 1
                blockPositions[1][0] = 1;
                blockPositions[1][1] = 1;

                // right 1
                blockPositions[3][0] = 0;
                blockPositions[3][1] = 1;
                break;
            // T shaped
            case BlockType::T:
                // up 1
                blockPositions[0][0] = -1;
                blockPositions[0][1] = 0;

                blockPositions[1][0] = 0;
                blockPositions[1][1] = -1;

                blockPositions[3][0] = 0;
                blockPositions[3][1] = 1;
                break;

            case BlockType::S:
                // down 1, left 1
                blockPositions[0][0] = 1;
                blockPositions[0][1] = -1;

                blockPositions[1][0] = 1;
                blockPositions[1][1] = 0;

                blockPositions[3][0] = 0;
                blockPositions[3][1] = 1;
                break;

            case BlockType::Z:
                blockPositions[0][0] = -1;
                blockPositions[0][1] = -1;

                blockPositions[1][0] = -1;
                blockPositions[1][1] = 0;

                blockPositions[3][0] = 0;
                blockPositions[3][1] = 1;
                break;

            case BlockType::J:
                blockPositions[0][0] = -2;
                blockPositions[0][1] = 0;

                blockPositions[1][0] = -1;
                blockPositions[1][1] = 0;

                blockPositions[3][0] = 0;
                blockPositions[3][1] = -1;
                break;

            case BlockType::L:
                blockPositions[0][0] = -2;
                blockPositions[0][1] = 0;

                blockPositions[1][0] = -1;
                blockPositions[1][1] = 0;

                blockPositions[3][0] = 0;
                blockPositions[3][1] = 1;
                break;

            default: ;
        }
    }

    void setCenterPosition(const Point position) {
        this->centerPosition = position;
    };

    std::array<std::array<int, 2>, 4> getCurrentPosition() const {

        std::array<std::array<int, 2>, 4> returnArray{};
        for (int i = 0; i < 4; i++) {
            returnArray[i][0] = centerPosition.y + this->blockPositions[i][0];
            returnArray[i][1] = centerPosition.x + this->blockPositions[i][1];
        }

        return returnArray;

    }





};


#endif //SIMPLETETRIS_BLOCK_H
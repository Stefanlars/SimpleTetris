//
// Created by sdalp on 12/26/2025.
//

#ifndef SIMPLETETRIS_ENUMS_H
#define SIMPLETETRIS_ENUMS_H


enum class BlockType { I, O, T, S, Z, J, L };

enum class BlockMove {
    ROTATE,
    LEFT,
    RIGHT,
    DOWN,
    QUICK_DOWN
};

enum class BlockColor {
    NONE = 0,
    CYAN,
    YELLOW,
    PURPLE,
    GREEN,
    RED,
    BLUE,
    ORANGE
};


struct Point {
    int x;
    int y;
};

#endif //SIMPLETETRIS_ENUMS_H
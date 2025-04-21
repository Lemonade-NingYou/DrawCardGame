#ifndef CARD_H
#define CARD_H

#define MAX_NAME_LENGTH 50

// 稀有度枚举
typedef enum {
    RARITY_B = 0,
    RARITY_A = 1,
    RARITY_S = 2,
    RARITY_COUNT
} Rarity;

// 卡牌结构体
typedef struct {
    char name[MAX_NAME_LENGTH];
    Rarity rarity;
} Card;

const char* rarity_to_string(Rarity rarity);

#endif
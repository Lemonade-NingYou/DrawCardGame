#ifndef CARD_POOL_H
#define CARD_POOL_H

#include "card.h"

// 卡池结构体
typedef struct {
    Card* cards;
    int base_probabilities[RARITY_COUNT];  // 基础概率（百分比）
    int current_probabilities[RARITY_COUNT]; // 当前概率（考虑保底机制）
    int rarity_counts[RARITY_COUNT];  // 各稀有度的卡牌数量
    int total_cards;
    
    // 保底机制相关变量
    int pity_threshold_S;   // S级保底阈值
    int pity_threshold_A;   // A级保底阈值
    int pity_increase_S;    // S级保底概率增量
    int pity_increase_A;    // A级保底概率增量
} CardPool;

CardPool* init_card_pool();
void free_card_pool(CardPool* pool);
int add_card_to_pool(CardPool* pool, const char* name, Rarity rarity);
void apply_pity(CardPool* pool, int pity_counter_S, int pity_counter_A);

#endif
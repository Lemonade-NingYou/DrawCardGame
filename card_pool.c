#include "card_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CardPool* init_card_pool() {
    CardPool* pool = (CardPool*)malloc(sizeof(CardPool));
    if (!pool) return NULL;
    
    pool->cards = NULL;
    pool->total_cards = 0;
    memset(pool->rarity_counts, 0, sizeof(pool->rarity_counts));
    
    // 设置基础概率 (B:89%, A:10%, S:1%)
    pool->base_probabilities[RARITY_B] = 89;
    pool->base_probabilities[RARITY_A] = 10;
    pool->base_probabilities[RARITY_S] = 1;
    
    // 初始化当前概率
    memcpy(pool->current_probabilities, pool->base_probabilities, sizeof(pool->base_probabilities));
    
    // 设置保底参数
    pool->pity_threshold_S = 80;  // S级80抽保底
    pool->pity_threshold_A = 10;  // A级10抽保底
    pool->pity_increase_S = 10;   // S级保底概率增加10%
    pool->pity_increase_A = 20;   // A级保底概率增加20%
    
    return pool;
}

void free_card_pool(CardPool* pool) {
    if (pool) {
        free(pool->cards);
        free(pool);
    }
}

int add_card_to_pool(CardPool* pool, const char* name, Rarity rarity) {
    if (!pool || rarity < 0 || rarity >= RARITY_COUNT) return 0;
    
    pool->total_cards++;
    pool->rarity_counts[rarity]++;
    
    Card* new_cards = (Card*)realloc(pool->cards, pool->total_cards * sizeof(Card));
    if (!new_cards) return 0;
    
    pool->cards = new_cards;
    strncpy(pool->cards[pool->total_cards-1].name, name, MAX_NAME_LENGTH-1);
    pool->cards[pool->total_cards-1].name[MAX_NAME_LENGTH-1] = '\0';
    pool->cards[pool->total_cards-1].rarity = rarity;
    return 1;
}

void apply_pity(CardPool* pool, int pity_counter_S, int pity_counter_A) {
    // 重置为基本概率
    memcpy(pool->current_probabilities, pool->base_probabilities, sizeof(pool->base_probabilities));
    
    // 检查S级保底
    if (pity_counter_S >= pool->pity_threshold_S) {
        printf("触发S级保底! S级概率提升%d%%\n", pool->pity_increase_S);
        pool->current_probabilities[RARITY_S] += pool->pity_increase_S;
        pool->current_probabilities[RARITY_A] -= pool->pity_increase_S / 2;
        pool->current_probabilities[RARITY_B] -= pool->pity_increase_S / 2;
    }
    // 检查A级保底
    else if (pity_counter_A >= pool->pity_threshold_A) {
        printf("触发A级保底! A级概率提升%d%%\n", pool->pity_increase_A);
        pool->current_probabilities[RARITY_A] += pool->pity_increase_A;
        pool->current_probabilities[RARITY_B] -= pool->pity_increase_A;
    }
    
    // 确保概率不为负且总和为100%
    int sum = 0;
    for (int i = 0; i < RARITY_COUNT; i++) {
        if (pool->current_probabilities[i] < 0) pool->current_probabilities[i] = 0;
        sum += pool->current_probabilities[i];
    }
    
    // 归一化处理
    if (sum != 100) {
        int diff = 100 - sum;
        pool->current_probabilities[RARITY_B] += diff;
    }
}
#include "gacha.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void show_coins(const UserData* user) {
    if (user) {
        printf("当前金币: %d\n", user->coins);
    }
}

Card draw_card(CardPool* pool, UserData* user) {
    if (!pool || !user) {
        Card empty = {"", RARITY_B};
        return empty;
    }
    
    // 应用当前保底状态
    apply_pity(pool, user->pity_counter_S, user->pity_counter_A);
    
    // 根据当前概率决定抽到什么稀有度的卡
    int rand_num = rand() % 100;
    int current_prob = 0;
    Rarity selected_rarity = RARITY_B;
    
    for (int i = 0; i < RARITY_COUNT; i++) {
        current_prob += pool->current_probabilities[i];
        if (rand_num < current_prob) {
            selected_rarity = i;
            break;
        }
    }
    
    // 更新保底计数器
    user->total_pulls++;
    user->pity_counter_S++;
    user->pity_counter_A++;
    
    // 根据抽到的卡牌重置计数器
    if (selected_rarity >= RARITY_S) {
        user->pity_counter_S = 0;
        user->pity_counter_A = 0;
    } else if (selected_rarity >= RARITY_A) {
        user->pity_counter_A = 0;
    }
    
    // 在选定稀有度的卡牌中随机选择一张
    int start_index = 0;
    for (int i = 0; i < selected_rarity; i++) {
        start_index += pool->rarity_counts[i];
    }
    
    int selected_card = start_index + (rand() % pool->rarity_counts[selected_rarity]);
    
    // 记录抽到的卡牌
    add_pulled_card(user, pool->cards[selected_card]);
    
    return pool->cards[selected_card];
}

void show_menu() {
    printf("\n=== 抽卡系统 ===\n");
    printf("1. 单抽 (1金币)\n");
    printf("2. 十连抽 (10金币)\n");
    printf("3. 查看抽卡记录\n");
    printf("4. 退出\n");
    printf("请选择: ");
}
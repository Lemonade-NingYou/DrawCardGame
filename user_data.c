#include "user_data.h"
#include <stdlib.h>
#include <stdio.h>

UserData* init_user_data() {
    UserData* user = (UserData*)malloc(sizeof(UserData));
    if (!user) return NULL;
    
    user->total_pulls = 0;
    user->pity_counter_S = 0;
    user->pity_counter_A = 0;
    user->pulled_cards = NULL;
    user->pulled_count = 0;
    user->coins = 100;  // 初始金币
    
    return user;
}

void free_user_data(UserData* user) {
    if (user) {
        free(user->pulled_cards);
        free(user);
    }
}

int can_afford_pull(const UserData* user, int count) {
    if (!user) return 0;
    return user->coins >= count;
}

void deduct_coins(UserData* user, int amount) {
    if (user) {
        user->coins -= amount;
        if (user->coins < 0) user->coins = 0;
    }
}

void add_pulled_card(UserData* user, Card card) {
    if (!user) return;
    
    user->pulled_count++;
    Card* new_pulled = (Card*)realloc(user->pulled_cards, user->pulled_count * sizeof(Card));
    if (new_pulled) {
        user->pulled_cards = new_pulled;
        user->pulled_cards[user->pulled_count-1] = card;
    }
}

void show_user_history(const UserData* user) {
    if (!user) return;
    
    printf("\n=== 抽卡记录 ===\n");
    printf("总抽卡次数: %d\n", user->total_pulls);
    printf("当前S级保底计数: %d/%d\n", user->pity_counter_S, 80);
    printf("当前A级保底计数: %d/%d\n", user->pity_counter_A, 10);
    
    if (user->pulled_count > 0) {
        printf("\n最近10次抽卡结果:\n");
        int start = (user->pulled_count > 10) ? user->pulled_count - 10 : 0;
        for (int i = start; i < user->pulled_count; i++) {
            printf("%3d. %-20s (%s级)\n", 
                  i+1, 
                  user->pulled_cards[i].name, 
                  rarity_to_string(user->pulled_cards[i].rarity));
        }
    }
}
void add_coins(UserData* user, int amount) {
    if (user && amount > 0) {
        user->coins += amount;
    }
}
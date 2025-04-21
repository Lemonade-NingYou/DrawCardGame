#ifndef USER_DATA_H
#define USER_DATA_H

#include "card.h"

// 用户数据文件版本
#define USER_DATA_VERSION 2

// 用户抽卡记录
typedef struct {
    int total_pulls;
    int pity_counter_S;
    int pity_counter_A;
    Card* pulled_cards;
    int pulled_count;
    int coins;          // 金币数量
} UserData;

UserData* init_user_data();
void free_user_data(UserData* user);
void add_pulled_card(UserData* user, Card card);
void show_user_history(const UserData* user);
int can_afford_pull(const UserData* user, int count);
void deduct_coins(UserData* user, int amount);
void add_coins(UserData* user, int amount);  // 新增金币增加函数

#endif
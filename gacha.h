#ifndef GACHA_H
#define GACHA_H

#include "card.h"
#include "card_pool.h"
#include "user_data.h"

Card draw_card(CardPool* pool, UserData* user);
void show_menu();
void show_coins(const UserData* user);  // 新增显示金币函数

#endif
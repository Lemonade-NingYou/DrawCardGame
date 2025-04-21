#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int save_user_data(const UserData* user) {
    if (!user) return 0;
    
    FILE* file = fopen(USERDATA_FILE, "wb");
    if (!file) return 0;
    
    // 写入文件头
    UserFileHeader header = {
        .version = USER_DATA_VERSION,
        .data_size = sizeof(UserData) - sizeof(Card*)  // 不包含指针本身
    };
    if (fwrite(&header, sizeof(UserFileHeader), 1, file) != 1) goto error;
    
    // 写入用户数据（不包括pulled_cards指针）
    if (fwrite(&user->total_pulls, sizeof(int), 1, file) != 1) goto error;
    if (fwrite(&user->pity_counter_S, sizeof(int), 1, file) != 1) goto error;
    if (fwrite(&user->pity_counter_A, sizeof(int), 1, file) != 1) goto error;
    if (fwrite(&user->pulled_count, sizeof(int), 1, file) != 1) goto error;
    if (fwrite(&user->coins, sizeof(int), 1, file) != 1) goto error;
    
    // 写入抽到的卡牌
    if (user->pulled_count > 0) {
        if (fwrite(user->pulled_cards, sizeof(Card), user->pulled_count, file) != user->pulled_count) goto error;
    }
    
    fclose(file);
    return 1;
    
error:
    fclose(file);
    return 0;
}

UserData* load_user_data() {
    FILE* file = fopen(USERDATA_FILE, "rb");
    if (!file) return NULL;
    
    // 读取文件头
    UserFileHeader header;
    if (fread(&header, sizeof(UserFileHeader), 1, file) != 1) goto error;
    
    UserData* user = init_user_data();
    if (!user) {
        fclose(file);
        return NULL;
    }
    
    // 读取用户数据
    if (fread(&user->total_pulls, sizeof(int), 1, file) != 1) goto error;
    if (fread(&user->pity_counter_S, sizeof(int), 1, file) != 1) goto error;
    if (fread(&user->pity_counter_A, sizeof(int), 1, file) != 1) goto error;
    if (fread(&user->pulled_count, sizeof(int), 1, file) != 1) goto error;
    
    // 版本1没有coins字段，版本2有
    if (header.version >= 2) {
        if (fread(&user->coins, sizeof(int), 1, file) != 1) goto error;
    } else {
        user->coins = 100;  // 旧版本默认给100金币
    }
    
    // 读取抽到的卡牌
    if (user->pulled_count > 0) {
        user->pulled_cards = (Card*)malloc(user->pulled_count * sizeof(Card));
        if (!user->pulled_cards) goto error;
        if (fread(user->pulled_cards, sizeof(Card), user->pulled_count, file) != user->pulled_count) goto error;
    }
    
    fclose(file);
    return user;
    
error:
    fclose(file);
    free_user_data(user);
    return NULL;
}

int load_card_config(CardPool* pool, const char* filename) {
    if (!pool) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件: %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        // 去除行尾的换行符
        line[strcspn(line, "\n")] = '\0';
        
        // 跳过空行和注释
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        // 处理卡牌行（格式: 卡牌名称,稀有度(B/A/S)）
        char* comma = strchr(line, ',');
        if (!comma) {
            printf("警告: 第%d行: 无效格式，应为'卡牌名称,稀有度(B/A/S)'\n", line_num);
            continue;
        }
        
        *comma = '\0';
        char rarity_char = toupper(*(comma + 1));
        Rarity rarity;
        
        switch (rarity_char) {
            case 'B': rarity = RARITY_B; break;
            case 'A': rarity = RARITY_A; break;
            case 'S': rarity = RARITY_S; break;
            default:
                printf("警告: 第%d行: 无效稀有度 '%c'，应为B/A/S\n", line_num, rarity_char);
                continue;
        }
        
        if (!add_card_to_pool(pool, line, rarity)) {
            printf("警告: 第%d行: 添加卡牌失败\n", line_num);
            continue;
        }
    }
    
    fclose(file);
    
    if (pool->total_cards == 0) {
        printf("错误: 配置文件中没有有效的卡牌\n");
        return 0;
    }
    
    return 1;
}
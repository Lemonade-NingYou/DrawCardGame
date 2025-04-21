#ifndef FILE_IO_H
#define FILE_IO_H

#include "card.h"
#include "user_data.h"
#include "card_pool.h"

#define USERDATA_FILE "userdata.bin"
#define MAX_LINE_LENGTH 256

// 用户数据文件头
typedef struct {
    int version;        // 文件版本
    int data_size;      // 数据部分大小
} UserFileHeader;

int save_user_data(const UserData* user);
UserData* load_user_data();
int load_card_config(CardPool* pool, const char* filename);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"
#include "card_pool.h"
#include "user_data.h"
#include "gacha.h"
#include "file_io.h"

// 显示程序标题和信息
void show_program_header(const CardPool* pool) {
    printf("\n============ 抽卡模拟系统 ============\n");
    printf("卡牌总数: %d\n", pool->total_cards);
    printf("稀有度分布:\n");
    printf("  B级: %2d张 (概率: %2d%%)\n", 
          pool->rarity_counts[RARITY_B], pool->base_probabilities[RARITY_B]);
    printf("  A级: %2d张 (概率: %2d%%)\n", 
          pool->rarity_counts[RARITY_A], pool->base_probabilities[RARITY_A]);
    printf("  S级: %2d张 (概率: %2d%%)\n", 
          pool->rarity_counts[RARITY_S], pool->base_probabilities[RARITY_S]);
    printf("保底机制:\n");
    printf("  S级保底: %d抽\n", pool->pity_threshold_S);
    printf("  A级保底: %d抽\n", pool->pity_threshold_A);
    printf("======================================\n");
}

// 处理单抽逻辑
void handle_single_pull(CardPool* pool, UserData* user) {
    if (!can_afford_pull(user, 1)) {
        printf("❌ 金币不足! 需要1金币 (当前: %d)\n", user->coins);
        return;
    }
    
    Card drawn = draw_card(pool, user);
    if (strlen(drawn.name) == 0) {
        printf("⚠️ 抽卡失败! 请重试\n");
        return;
    }
    
    deduct_coins(user, 1);
    printf("\n🎉 抽到: %-20s (%s级)\n", 
          drawn.name, rarity_to_string(drawn.rarity));
    
    if (!save_user_data(user)) {
        printf("⚠️ 警告: 保存数据失败!\n");
    }
}

// 处理十连抽逻辑
void handle_multi_pull(CardPool* pool, UserData* user) {
    if (!can_afford_pull(user, 10)) {
        printf("❌ 金币不足! 需要10金币 (当前: %d)\n", user->coins);
        return;
    }
    
    printf("\n✨ 十连抽结果:\n");
    for (int i = 0; i < 10; i++) {
        Card drawn = draw_card(pool, user);
        printf("%2d. %-20s (%s级)\n", 
              i+1, drawn.name, rarity_to_string(drawn.rarity));
    }
    
    deduct_coins(user, 10);
    if (!save_user_data(user)) {
        printf("⚠️ 警告: 保存数据失败!\n");
    }
}

int main() {
    // 初始化随机数种子
    srand(time(NULL));
    
    // 初始化卡池
    CardPool* pool = init_card_pool();
    if (!pool) {
        fprintf(stderr, "❌ 错误: 初始化卡池失败\n");
        return EXIT_FAILURE;
    }
    
    // 加载卡牌配置
    if (!load_card_config(pool, "card_config.txt")) {
        fprintf(stderr, "❌ 错误: 加载卡牌配置失败\n");
        free_card_pool(pool);
        return EXIT_FAILURE;
    }
    
    // 加载用户数据
    UserData* user = load_user_data();
    if (!user) {
        printf("ℹ️ 加载用户数据\n");
        user = init_user_data();
        if (!user) {
            fprintf(stderr, "❌ 错误: 初始化用户数据失败\n");
            free_card_pool(pool);
            return EXIT_FAILURE;
        }
    }
    

    
    // 主循环
    int choice = 0;
    while (1) {
        show_coins(user);
        show_menu();
        
        // 获取用户输入
        if (scanf("%d", &choice) != 1) {
            printf("⚠️ 请输入有效数字\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }
        
        // 处理用户选择
        switch (choice) {
            case 1:  // 单抽
                handle_single_pull(pool, user);
                break;
                
            case 2:  // 十连抽
                handle_multi_pull(pool, user);
                break;
                
            case 3:  // 查看记录
                show_user_history(user);
                break;
                
            case 4:  // 退出
                printf("\n💾 保存数据并退出...\n");
                goto exit_program;
                
            case 999:  // 开发者模式
                add_coins(user, 1000);
                printf("\n🔓 开发者模式: 获得1000金币!\n");
                // 显示系统信息
    show_program_header(pool);
                break;
                
            default:
                printf("⚠️ 无效选项! 请选择1-4\n");
        }
        
        printf("\n");
    }
    
exit_program:
    // 清理资源
    if (!save_user_data(user)) {
        fprintf(stderr, "⚠️ 警告: 保存用户数据失败\n");
    }
    
    free_user_data(user);
    free_card_pool(pool);
    
    return EXIT_SUCCESS;
}
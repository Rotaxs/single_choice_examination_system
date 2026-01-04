#ifndef UTILS_H
#define UTILS_H

// 表格
#define TABLE_UL "\u250C" // ┌
#define TABLE_U "\u2500"  // ─
#define TABLE_UR "\u2510" // ┐
#define TABLE_L "\u2502"  // │
#define TABLE_R "\u2502"  // │
#define TABLE_BL "\u2514" // └
#define TABLE_B "\u2500"  // ─
#define TABLE_BR "\u2518" // ┘

// 颜色和光标
#define CLS "\033[2J\033[H"
#define RESET "\033[0m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define YELLOW "\033[33m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define BLACK "\033[30m"
#define BG_WHITE "\033[47m"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"
#define CURSOR_RESET printf("\033[H")
#define CURSOR_MOVETO(r, c) printf("\033[r;cH")
#define CURSOR_SHIFT_UP printf("\033[1A")

// 键盘输入
#define KEY_GUIDE 224
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_DEL 83
#define KEY_HOME 71
#define KEY_END 79
#define KEY_PGUP 73
#define KEY_PGDN 81
#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_BACK 8

#include "common.h"
#include <stdbool.h>

// 打印
#define print_space(x)          \
    for (int i = 0; i < x; i++) \
        printf(" ");
#define print_table(y, x)       \
    for (int i = 0; i < x; i++) \
        printf(MENU_BORDER_COLOR BOLD "%s" RESET, y);
#define print_enter printf("\n")

#define INFO(s)                            \
    do                                     \
    {                                      \
        printf(HIDE_CURSOR);               \
        printf(GREEN "INFO: %s" RESET, s); \
        print_enter;                       \
        printf(SHOW_CURSOR);               \
    } while (0)
#define ERR(s)                            \
    do                                    \
    {                                     \
        printf(HIDE_CURSOR);              \
        printf(RED "ERROR: %s" RESET, s); \
        print_enter;                      \
        printf(SHOW_CURSOR);              \
    } while (0)
#define WARNING(s)                             \
    do                                         \
    {                                          \
        printf(HIDE_CURSOR);                   \
        printf(YELLOW "WARNING: %s" RESET, s); \
        print_enter;                           \
        printf(SHOW_CURSOR);                   \
    } while (0)
#define WAITING(S)                     \
    do                                 \
    {                                  \
        printf(HIDE_CURSOR);           \
        printf(CYAN);                  \
        spin_loading(S, LOADING_TIME); \
        printf(RESET);                 \
        print_enter;                   \
        printf(SHOW_CURSOR);           \
    } while (0)

int get_keyboard_input();
bool get_account_input(char *account, int account_size);
bool get_password_input(char *pwd, int pwd_size);
bool get_question_input(char *question, int question_size);
bool get_option_input(char *option, int option_size);
bool get_answer_input(char *answer);
bool get_category_input(char *category, int category_size);
bool get_id_input(int *id);
bool get_y_or_n_input();

void spin_loading(char *content, int loading_time);
int get_digit_count(int num);
void cipher(char *pwd);
void flush_buffer();

// 用户数据 链表操作
UserNode *list_user_create();
bool list_user_is_reduplicate(UserNode *head, char *account);
bool list_user_add(UserNode *head, int id, char *account, char *password);
void list_user_print(UserNode *head);
bool list_user_modify(UserNode *head, int target_id, char *account, char *password);
bool list_user_delete(UserNode *head, int target_id);
UserNode *list_user_search(UserNode *head, char *account);
void list_user_destroy(UserNode *head);
int list_user_get_len(UserNode *head);

// 试题链表操作
QuestionNode *list_question_create();
void list_question_add(QuestionNode *head,
                       int id, char *question, char option[4][OPTIONLEN + 1], char answer, char category[CATEGORYLEN + 1]);
bool list_question_delete(QuestionNode *head, int target_id);
bool list_question_modify(QuestionNode *head,
                          int target_id, char *question, char option[4][OPTIONLEN + 1], char answer, char category[CATEGORYLEN + 1]);
QuestionNode *list_question_search(QuestionNode *head, int id);
int list_question_search_by_category(QuestionNode *head, char *category, int *ids);
void list_question_destroy(QuestionNode *head);
int list_question_get_len(QuestionNode *head);

// 试卷链表操作
PaperNode *list_paper_create();
void list_paper_add(PaperNode *head, int id, char *title,
                    int *question_ids, int *question_scores,
                    int total_question, int paper_score,
                    char *start_time, char *end_time);
bool list_paper_delete(PaperNode *head, int target_id);
bool list_paper_modify(PaperNode *head, int target_id, char *title,
                       int *question_ids, int *question_scores,
                       int total_question, int paper_score,
                       char *start_time, char *end_time);
void list_paper_destroy(PaperNode *head);

#endif

#include "utils.h"
#include "common.h"
#include "file_io.h"
#include <conio.h>
#include <time.h>

/**
 * @brief 工具函数：加密与解密
 * @param pwd 密码
 */
void cipher(char *pwd)
{
    for (int i = 0; pwd[i] != '\0'; i++)
        pwd[i] ^= ENCRYPT_KEY;
}

/**
 * @brief 工具函数：刷新缓冲区
 */
void flush_buffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

/**
 * @brief 工具函数：非阻塞输入，并将输入的值返回
 * @return 键盘输入的值
 */
int get_keyboard_input()
{
    int ch = getch();
    if (ch == KEY_GUIDE)
    {
        return getch();
    }
    return ch;
}

/**
 * @brief 工具函数：在输入密码时，判断输入的字符是否合法
 * @param ch 要判断的字符
 */
bool is_valid_char(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '@' || ch == '#' || ch == '_';
}

/**
 * @brief 工具函数：获取账号和密码的输入
 * @param str 输入的值会保存在 str 中
 * @param str_size str 字符串的大小
 * @param show_str 输入的时候是否明文显示输入的字符
 * @return 成功返回 true，否则返回 false
 */
bool get_input(char *str, int str_size, bool show_str)
{
    printf(SHOW_CURSOR);
    int len = 0;
    int ch;
    while (1)
    {
        ch = get_keyboard_input();
        if (ch == KEY_ENTER)
        {
            if (!show_str && len < MIN_PWD_LEN)
                return false;
            if (show_str && len < 1)
                return false;
            break;
        }
        if (ch == KEY_BACK && len > 0)
        {
            // 回退，打印空格覆盖最后一个字符，再回退
            printf("\b \b");
            len--;
        }
        else if (ch == KEY_RIGHT || ch == KEY_LEFT || ch == KEY_UP || ch == KEY_DOWN || ch == KEY_END || ch == KEY_HOME || ch == KEY_DEL || ch == KEY_PGDN || ch == KEY_PGUP)
            continue;
        else if (is_valid_char(ch))
        {
            if (show_str)
            {
                if (len < MAX_ACCOUNT_LEN && len < str_size - 1)
                {
                    printf("%c", ch);
                    str[len] = ch;
                    len++;
                }
            }
            else
            {
                if (len < MAX_PWD_LEN && len < str_size - 1)
                {
                    printf(YELLOW "*" RESET);
                    str[len] = ch;
                    len++;
                }
            }
        }
    }
    str[len] = '\0';
    return true;
}

/**
 * @brief 工具函数：获取键盘密码输入的工具函数
 * @param pwd 将键盘的合法输入存入 pwd 中
 * @param pwd_size 在传入 pwd 时给 pwd 数组开的大小
 * @return 如果密码过短，返回 false，否则返回 true
 */
bool get_password_input(char *pwd, int pwd_size)
{
    return get_input(pwd, pwd_size, false);
}

/**
 * @brief 工具函数：获取账号输入
 * @param account 账号
 * @param account_size 账号字符串的长度
 * @return 如果输入成功，返回 true；否则返回 false
 */
bool get_account_input(char *account, int account_size)
{
    return get_input(account, account_size, true);
}

/**
 * @brief 工具函数：获取试题的输入
 * @param question 试题
 * @param question_size 试题字符串的大小
 * @return 如果输入成功，返回 true，否则返回 false
 */
bool get_question_input(char *question, int question_size)
{
    char line[QLEN + 1];
    question[0] = '\0';
    // 循环读取每一行
    while (fgets(line, sizeof(line), stdin))
    {
        if (line[0] == END_FLAG && line[1] == '\n')
            break;
        if (strlen(question) + strlen(line) < question_size - 1)
            strcat(question, line);
        else
            return false;
    }
    // 处理多余的换行符
    int len = strlen(question);
    while (len > 0 && (question[len - 1] == '\n' || question[len - 1] == '\r'))
    {
        question[len - 1] = '\0';
        len--;
    }
    return true;
}

/**
 * @brief 工具函数：获取试题选项的输入
 * @param option 选项字符串
 * @param option_size 字符串的大小
 * @return 如果输入过长，返回 false，正确输入返回 true
 */
bool get_option_input(char *option, int option_size)
{
    // 1. 直接读取到目标空间（或临时空间）
    if (fgets(option, option_size, stdin) == NULL)
        return false;

    // 检查是否读取到了换行符
    char *nl = strchr(option, '\n');
    if (nl)
        *nl = '\0';
    else
    {
        flush_buffer();
        return false;
    }

    // Windows/Linux 换行时可能存在 \r
    char *rl = strchr(option, '\r');
    if (rl)
        *rl = '\0';
    return true;
}

/**
 * @brief 工具函数：获取试题答案的输入
 * @param answer 答案字符指针
 * @return 如果答案输入合法，返回 true，否则返回 false
 */
bool get_answer_input(char *answer)
{
    char temp;
    if (scanf(" %c", &temp) == 1)
    {
        flush_buffer();
        if (temp >= 'a' && temp <= 'd')
            temp -= 32;
        if (temp >= 'A' && temp <= 'D')
        {
            *answer = temp;
            return true;
        }
    }
    return false;
}

/**
 * @brief 工具函数：获取试题类别的输入
 * @param category 类别字符串
 * @param category_size 类别字符串的大小
 * @return 如果类别不超过长度限制，返回 true，否则返回 false
 */
bool get_category_input(char *category, int category_size)
{
    return get_option_input(category, category_size);
}

/**
 * @brief 工具函数：获取 id 输入
 * @param id id 指针
 * @return 读取成功返回 true，否则返回 false
 */
bool get_id_input(int *id)
{
    if (scanf(" %d", id) == 1)
    {
        flush_buffer();
        return true;
    }
    flush_buffer();
    return false;
}

/**
 * @brief 工具函数：输入 y 还是 n
 * @return 如果是 y，返回 true，其他字符，返回 false
 */
bool get_y_or_n_input()
{
    char ch;
    if (scanf(" %c", &ch) != 1)
        return false;
    flush_buffer();
    if (ch == 'y' || ch == 'Y')
        return true;
    return false;
}

/**
 * @brief 工具函数：获取标题的输入
 * @param title 标题字符串
 * @param title_size 标题字符串的大小
 * @return 如果长度不超过限制，返回 true，否则返回 false
 */
bool get_title_input(char *title, int title_size)
{
    return get_option_input(title, title_size);
}

/**
 * @brief 工具函数：获取分数的输入
 * @param score 分数指针
 * @return 读取成功返回 true，否则返回 false
 */
bool get_score_input(int *score)
{
    return get_id_input(score);
}

/**
 * @brief 工具函数：获取日期的输入
 * @param date 字符串格式的日期
 * @return 如果日期格式正确，返回 true，否则返回 false
 */
bool get_date_input(char *date)
{
    if (fgets(date, 20, stdin) != NULL)
    {
        date[strcspn(date, "\n")] = '\0';
        return is_valid_date(date);
    }
    return false;
}

/**
 * @brief 工具函数：加载动画
 * @param content 加载动画显示的内容
 * @param loading_time 加载动画持续的时间，单位为微秒（μs）
 */
void spin_loading(char *content, int loading_time)
{
    const char *spin = "|/-\\";
    for (int i = 0; i < 20; i++)
    {
        printf("\rWAITING: %s %c", content, spin[i % 4]);
        fflush(stdout);
        usleep(loading_time);
    }
}

/**
 * @brief 工具函数：获取数字的位数
 * @param num 数字
 * @return 返回数字的位数
 */
int get_digit_count(int num)
{
    int res = 0;
    while (num)
    {
        num /= 10;
        res++;
    }
    return res;
}

/**
 * @brief 交换两个数的值
 * @param a 第一个数的指针
 * @param b 第二个数的指针
 */
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief 删除元素
 * @param arr 要删除元素的数组，数组元素不能重复
 * @param len 数组长度的指针
 * @return 删除成功返回 true，否则返回 false
 */
bool delete_item_from_array(int *arr, int *len, int item)
{
    int target_index;
    for (target_index = 0; target_index < *len; target_index++)
    {
        if (arr[target_index] == item)
            break;
    }
    if (target_index == 0 && arr[0] != item)
        return false;
    (*len)--;
    for (int i = target_index; i < *len; i++)
        arr[i] = arr[i + 1];
}

// =============================== 日期相关工具 ====================================

/**
 * @brief 工具函数：获取当前时间
 * @return 返回 Date 结构体
 */
Date get_date()
{
    Date date;
    struct tm *info;
    time_t rawtime;

    time(&rawtime);
    info = localtime(&rawtime);

    date.year = info->tm_year + 1900;
    date.month = info->tm_mon + 1;
    date.day = info->tm_mday;
    date.hour = info->tm_hour;
    date.minute = info->tm_min;

    return date;
}

/**
 * @brief 工具函数：判断一个字符是否是数字字符
 * @param d 要判断的字符
 */
bool is_digit(char d)
{
    return d >= '0' && d <= '9';
}

/**
 * @brief 检查字符串是否为规定的时间格式 YYYY/MM/DD HH:MM
 * @param str 要检查的字符串
 * @return 如果字符串符合要求，返回 true，否则返回 false
 */
bool is_valid_date(char *str)
{
    if (strlen(str) != 16)
        return false;

    // 检查年份
    for (int i = 0; i < 4; i++)
    {
        if (!is_digit(str[i]))
            return false;
    }
    int year = (str[0] - '0') * 1000 + (str[1] - '0') * 100 + (str[2] - '0') * 10 + str[3] - '0';

    // 检查分隔符
    if (str[4] != '/' || str[7] != '/' || str[10] != ' ' || str[13] != ':')
        return false;

    // 检查月
    if (!is_digit(str[5]) || !is_digit(str[6]))
        return false;
    int month = (str[5] - '0') * 10 + str[6] - '0';
    if (month <= 0 || month > 12)
        return false;

    // 检查日
    if (!is_digit(str[8]) || !is_digit(str[9]))
        return false;
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && year % 100 != 0)
        days_per_month[1] = 29;
    else if (year % 400 == 0)
        days_per_month[1] = 29;
    int day = (str[8] - '0') * 10 + str[9] - '0';
    if (day <= 0 || day > days_per_month[month - 1])
        return false;

    // 检查时
    if (!is_digit(str[11]) || !is_digit(str[12]))
        return false;
    int hour = (str[11] - '0') * 10 + str[12] - '0';
    if (hour < 0 || hour >= 24)
        return false;

    // 检查分
    if (!is_digit(str[14]) || !is_digit(str[15]))
        return false;
    int minute = (str[14] - '0') * 10 + str[15] - '0';
    if (minute < 0 || minute >= 60)
        return false;

    return true;
}

/**
 * @brief 将字符串格式的日期转化为结构体
 * @param str 字符串格式的日期，格式为 YYYY/MM/DD HH:MM
 * @return 返回结构体格式的日期
 */
Date str_to_date(char *str)
{
    Date date;
    date.year = (str[0] - '0') * 1000 + (str[1] - '0') * 100 + (str[2] - '0') * 10 + str[3] - '0';
    date.month = (str[5] - '0') * 10 + str[6] - '0';
    date.day = (str[8] - '0') * 10 + str[9] - '0';
    date.hour = (str[11] - '0') * 10 + str[12] - '0';
    date.minute = (str[14] - '0') * 10 + str[15] - '0';
    return date;
}

// =================== 用户数据的链表操作 ====================

UserNode *list_user_create()
{
    UserNode *head = (UserNode *)malloc(sizeof(UserNode));
    head->next = NULL;
    return head;
}

bool list_user_is_reduplicate(UserNode *head, char *account)
{
    UserNode *cur_node = head->next;
    for (; cur_node != NULL; cur_node = cur_node->next)
    {
        if (strcmp(cur_node->account, account) == 0)
            return true;
    }
    return false;
}

bool list_user_add(UserNode *head, int id, char *account, char *password)
{
    if (list_user_is_reduplicate(head, account))
        return false;
    UserNode *user_node = (UserNode *)malloc(sizeof(UserNode));
    user_node->id = id;
    strcpy(user_node->account, account);
    strcpy(user_node->password, password);
    user_node->next = head->next;
    head->next = user_node;
    return true;
}

bool list_user_modify(UserNode *head, int target_id, char *account, char *password)
{
    UserNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->id == target_id)
        {
            strcpy(cur_node->account, account);
            strcpy(cur_node->password, password);
            return true;
        }
    }
    return false;
}

bool list_user_delete(UserNode *head, int target_id)
{
    UserNode *cur_node = head;
    for (; cur_node->next; cur_node = cur_node->next)
    {
        if (cur_node->next->id == target_id)
        {
            UserNode *temp = cur_node->next;
            cur_node->next = temp->next;
            free(temp);
            return true;
        }
    }
    return false;
}

void list_user_destroy(UserNode *head)
{
    UserNode *cur = head;
    while (cur != NULL)
    {
        UserNode *next = cur->next;
        free(cur);
        cur = next;
    }
}

UserNode *list_user_search(UserNode *head, char *account)
{
    UserNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        if (strcmp(cur_node->account, account) == 0)
            return cur_node;
    }
    return NULL;
}

void list_user_print(UserNode *head)
{
    UserNode *ne = head->next;
    print_enter;
    for (; ne; ne = ne->next)
    {
        printf("id: %d\n", ne->id);
        printf("account: %s\n", ne->account);
        printf("password: %s\n", ne->password);
    }
}

int list_user_get_len(UserNode *head)
{
    int res = 0;
    UserNode *node = head->next;
    for (; node; node = node->next)
        res++;
    return res;
}

// =================== 试题数据的链表操作 ====================

QuestionNode *list_question_create()
{
    QuestionNode *head = (QuestionNode *)malloc(sizeof(QuestionNode));
    head->next = NULL;
    return head;
}

void list_question_add(QuestionNode *head,
                       int id, char *question, char option[4][OPTIONLEN + 1], char answer, char category[CATEGORYLEN + 1])
{
    QuestionNode *new_node = (QuestionNode *)malloc(sizeof(QuestionNode));
    new_node->id = id;
    strcpy(new_node->question, question);
    for (int i = 0; i < 4; i++)
        strcpy(new_node->option[i], option[i]);
    new_node->answer = answer;
    strcpy(new_node->category, category);
    new_node->next = head->next;
    head->next = new_node;
}

bool list_question_delete(QuestionNode *head, int target_id)
{
    QuestionNode *cur_node = head;
    for (; cur_node->next; cur_node = cur_node->next)
    {
        if (cur_node->next->id == target_id)
        {
            QuestionNode *temp = cur_node->next;
            cur_node->next = temp->next;
            free(temp);
            return true;
        }
    }
    return false;
}

bool list_question_modify(QuestionNode *head,
                          int target_id, char *question, char option[4][OPTIONLEN + 1], char answer, char category[CATEGORYLEN + 1])
{
    QuestionNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->id == target_id)
        {
            strcpy(cur_node->question, question);
            for (int i = 0; i < 4; i++)
                strcpy(cur_node->option[i], option[i]);
            cur_node->answer = answer;
            strcpy(cur_node->category, category);
            return true;
        }
    }
    return false;
}

int list_question_search_by_category(QuestionNode *head, char *category, int *ids)
{
    int size = 0;
    QuestionNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        if (strcmp(cur_node->category, category) == 0)
        {
            ids[size++] = cur_node->id;
        }
    }
    return size;
}

QuestionNode *list_question_search(QuestionNode *head, int id)
{
    QuestionNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->id == id)
            return cur_node;
    }
    return NULL;
}

void list_question_destroy(QuestionNode *head)
{
    QuestionNode *cur = head;
    while (cur != NULL)
    {
        QuestionNode *next = cur->next;
        free(cur);
        cur = next;
    }
}

int list_question_get_len(QuestionNode *head)
{
    int total = 0;
    QuestionNode *node = head->next;
    for (; node; node = node->next)
        total++;
    return total;
}

int list_question_get_ids(QuestionNode *head, int *ids)
{
    int len = 0;
    QuestionNode *node = head->next;
    for (; node; node = node->next)
        ids[len++] = node->id;
    return len;
}

// =================== 试卷数据的链表操作 ====================

PaperNode *list_paper_create()
{
    PaperNode *head = (PaperNode *)malloc(sizeof(PaperNode));
    head->next = NULL;
    return head;
}

void list_paper_add(PaperNode *head, int id, char *title,
                    int *question_ids, int *question_scores,
                    int total_question, int paper_score,
                    char *start_time, char *end_time, bool published)
{
    PaperNode *new_node = (PaperNode *)malloc(sizeof(PaperNode));

    new_node->id = id;
    strcpy(new_node->title, title);
    for (int i = 0; i < total_question; i++)
    {
        new_node->question_ids[i] = question_ids[i];
        new_node->question_scores[i] = question_scores[i];
    }
    new_node->total_questions = total_question;
    new_node->paper_score = paper_score;
    strcpy(new_node->start_time, start_time);
    strcpy(new_node->end_time, end_time);
    new_node->published = published;

    new_node->next = head->next;
    head->next = new_node;
}

bool list_paper_delete(PaperNode *head, int target_id)
{
    PaperNode *cur_node = head;
    for (; cur_node->next; cur_node = cur_node->next)
    {
        if (cur_node->next->id == target_id)
        {
            PaperNode *temp = cur_node->next;
            cur_node->next = temp->next;
            free(temp);
            return true;
        }
    }
    return false;
}

bool list_paper_modify(PaperNode *head, int target_id, char *title,
                       int *question_ids, int *question_scores,
                       int total_question, int paper_score,
                       char *start_time, char *end_time)
{
    PaperNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->id == target_id)
        {
            strcpy(cur_node->title, title);
            memset(cur_node->question_ids, 0, sizeof(int) * MAX_PAPER_QUESTIONS);
            memset(cur_node->question_scores, 0, sizeof(int) * MAX_PAPER_QUESTIONS);
            for (int i = 0; i < total_question; i++)
            {
                cur_node->question_ids[i] = question_ids[i];
                cur_node->question_scores[i] = question_scores[i];
            }
            cur_node->total_questions = total_question;
            cur_node->paper_score = paper_score;
            strcpy(cur_node->start_time, start_time);
            strcpy(cur_node->end_time, end_time);
            return true;
        }
    }
    return false;
}

void list_paper_destroy(PaperNode *head)
{
    PaperNode *cur = head;
    while (cur != NULL)
    {
        PaperNode *next = cur->next;
        free(cur);
        cur = next;
    }
}

int list_paper_get_len(PaperNode *head)
{
    PaperNode *node = head->next;
    int len = 0;
    for (; node; node = node->next)
        len++;
    return len;
}
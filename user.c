#include "user.h"
#include "common.h"
#include "file_io.h"
#include "utils.h"

/**
 * @brief 菜单功能函数：用户登录
 * @param head 用户数据链表
 * @return 登录成功返回当前用户数据结点，否则返回空指针
 */
UserNode *user_login(UserNode *head)
{
    bool is_valid;
    char input_account[MAX_ACCOUNT_LEN + 1];
    char input_pwd[MAX_PWD_LEN + 1];
    int attempted = 0;
    UserNode *cur_user;

    // 账号检测
    while (1)
    {
        printf(CLS SHOW_CURSOR);
        printf("请输入用户名：");
        get_account_input(input_account, MAX_ACCOUNT_LEN + 1);
        print_enter;
        if (strcmp(input_account, "q") == 0)
        {
            WAITING("正在返回到用户操作界面...");
            return NULL;
        }
        cur_user = list_user_search(head, input_account);
        if (cur_user == NULL)
        {
            ERR("没有找到该用户的信息");
            usleep(WAITING_TIME);
            continue;
        }
        else
            break;
    }

    // 密码检测
    while (1)
    {
        // 保留用户名在屏幕上
        printf(CLS);
        printf("请输入用户名：%s", input_account);
        print_enter;

        printf("请输入用户密码：");
        is_valid = get_password_input(input_pwd, MAX_PWD_LEN);
        print_enter;
        if (!is_valid)
        {
            WARNING("密码过短，请重新输入");
            attempted++;
            if (attempted >= PWD_ATTEMP_COUNT)
            {
                WAITING("尝试次数过多，即将返回到主菜单...");
                return NULL;
            }
            if (attempted >= PWD_ATTEMP_COUNT - 2)
            {
                char buffer[35];
                sprintf(buffer, "你还有 %d 次的尝试机会", PWD_ATTEMP_COUNT - attempted);
                WARNING(buffer);
                usleep(500000);
            }
            usleep(WAITING_TIME);
            continue;
        }

        WAITING("正在进行登录检测...");

        // 密码正确
        if (strcmp(input_pwd, cur_user->password) == 0)
        {
            CURSOR_SHIFT_UP;
            INFO("密码正确                    ");
            WAITING("即将进入学生操作界面...");
            return cur_user;
        }
        else
        {
            CURSOR_SHIFT_UP;
            ERR("密码错误，请重新输入          ");
            attempted++;

            if (attempted >= PWD_ATTEMP_COUNT)
            {
                WAITING("尝试次数过多，即将返回到主菜单...");
                return NULL;
            }
            if (attempted >= PWD_ATTEMP_COUNT - 2)
            {
                char buffer[35];
                sprintf(buffer, "你还有 %d 次的尝试机会", PWD_ATTEMP_COUNT - attempted);
                WARNING(buffer);
                usleep(500000);
            }
            usleep(WAITING_TIME);
        }
    }
    return NULL;
}

void modify_password(UserNode *cur_stu)
{
    char input_previous_password[MAX_PWD_LEN + 1];
    char first_changed_password[MAX_PWD_LEN + 1];
    char second_changed_password[MAX_PWD_LEN + 1];
    bool is_valid;

    while (1)
    {
        printf(CLS);
        printf("请输入原密码：");
        get_password_input(input_previous_password, MAX_PWD_LEN + 1);
        print_enter;
        if (strcmp(input_previous_password, "q") == 0)
        {
            WAITING("正在返回到用户操作界面...");
            return;
        }
        if (strcmp(input_previous_password, cur_stu->password) != 0)
        {
            ERR("密码错误");
            usleep(WAITING_TIME);
            continue;
        }
        while (1)
        {
            printf(CLS);
            printf("请输入新密码：");
            is_valid = get_password_input(first_changed_password, MAX_PWD_LEN + 1);
            print_enter;
            if (!is_valid)
            {
                WARNING("密码过短，请重新输入");
                usleep(WAITING_TIME);
                continue;
            }
            printf("再次输入新密码：");
            get_password_input(second_changed_password, MAX_PWD_LEN + 1);
            print_enter;
            if (strcmp(first_changed_password, second_changed_password) == 0)
            {
                strcpy(cur_stu->password, first_changed_password);
                INFO("修改成功");
                WAITING("正在返回到学生操作界面");
                return;
            }
            else
            {
                ERR("两次密码不一致，请重新输入");
                usleep(WAITING_TIME);
            }
        }
    }
}

// void print_exercise_question(QuestionNode *question_head, int id, int selection, int choice)
// {
//     printf(CLS);
//     QuestionNode *cur_question = list_question_search(question_head, id);

//     print_table(TABLE_U, MENU_WIDTH);
//     print_enter;
//     print_enter;

//     // 打印题目
//     printf("%s", cur_question->question);
//     print_enter;
//     // 打印选项
//     for (int i = 0; i < 4; i++)
//     {
//         if (choice == -1)
//         {
//             if (selection == i)
//                 printf(BG_WHITE BLACK "> %c. %s <\n" RESET, 'A' + i, cur_question->option[i]);
//             else
//                 printf("%c. %s\n", 'A' + i, cur_question->option[i]);
//         }
//         else
//         {
//             if (i == choice)
//             {
//                 if (cur_question->answer - 'A' == i)
//                     printf(BG_WHITE GREEN "> %c. %s <\n" RESET, 'A' + i, cur_question->option[i]);
//                 else
//                     printf(BG_WHITE RED "> %c. %s <\n" RESET, 'A' + i, cur_question->option[i]);
//             }
//             else
//                 printf("%c. %s\n", 'A' + i, cur_question->option[i]);
//         }
//     }

//     print_enter;
//     print_table(TABLE_B, MENU_WIDTH);
//     print_enter
// }

// void exercise(UserNode *user_head, QuestionNode *question_head)
// {
//     int num;
//     int question_ids[MAXQ_COUNT];
//     int *choosed_questions, *choices;
//     int total_questions = list_question_get_ids(question_head, question_ids);
//     while (1)
//     {
//         printf("请输入要做的题数");
//         if (!get_num_input(&num))
//         {
//             ERR("请输入正确的数字");
//             usleep(WAITING_TIME);
//             continue;
//         }
//         if (num > total_questions)
//         {
//             ERR("题库中还没有这么多题");
//             usleep(WAITING_TIME);
//             continue;
//         }
//     }
//     // 初始化练习题目和各题的用户的选择
//     choosed_questions = (int *)malloc(sizeof(int) * num);
//     choices = (int *)malloc(sizeof(int) * num);
//     memset(choices, -1, sizeof(choices));
//     get_n_random_from_arr(question_ids, total_questions, choosed_questions, num);
// }
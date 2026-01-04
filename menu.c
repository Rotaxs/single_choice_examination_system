#include "menu.h"
#include "common.h"
#include "admin.h"
#include "user.h"
#include "file_io.h"
#include "utils.h"

/**
 * @brief 获取若干选项中最短的选项，用于对齐
 * @param options 选项数组
 * @param len 选项的个数
 */
int get_shortest_option(const Text *options, const int len)
{
    int min_id = 0;
    for (int i = 1; i < len; i++)
    {
        if (options[i].length < options[min_id].length)
            min_id = i;
    }
    return min_id;
}

/**
 * @brief 绘制菜单
 * @param title 菜单的标题
 * @param prompt 菜单下的提示，如操作提示等
 * @param selection 选项 （0 - 2）
 * @param options Text 类型的数组，包含所有选项
 * @param len 选项的个数
 */
void draw_menu(const Text title, const Text prompt,
               const int selection, const Text *options, const int len)
{
    const int total_length = MENU_WIDTH;
    const int title_length = title.length;
    const int left_interval_length = (MENU_WIDTH - 2 - title_length) / 2;
    const int right_interval_length = total_length - left_interval_length - title_length - 2;

    CURSOR_RESET;

    // 标题部分（框 + 标题）
    print_table(TABLE_UL, 1);
    print_table(TABLE_U, total_length - 2);
    print_table(TABLE_UR, 1);
    print_enter;
    print_table(TABLE_L, 1);
    print_space(left_interval_length);
    printf(BOLD MENU_TITLE_COLOR "%s" RESET, title.content);
    print_space(right_interval_length);
    print_table(TABLE_R, 1);
    print_enter;
    print_table(TABLE_BL, 1);
    print_table(TABLE_B, total_length - 2);
    print_table(TABLE_BR, 1);

    print_enter;
    print_enter;

    int shortest_option_id = get_shortest_option(options, len);
    int shortest_len = options[shortest_option_id].length;
    // 选项部分
    for (int i = 0; i < len; i++)
    {
        char *text = options[i].content;
        printf(BOLD);

        if (selection == i)
        {
            print_space((total_length - shortest_len) / 2 - 4);
            printf(GREEN BG_WHITE);
            if (selection == len - 1)
                printf(RED);
            printf(">>> %s", text);
        }
        else
        {
            print_space((total_length - shortest_len) / 2);
            printf("%s", text);
        }
        printf(RESET);
        print_enter;
    }

    // 提示部分
    print_enter;
    print_space((total_length - prompt.length) / 2);
    printf(BOLD YELLOW "%s" RESET, prompt.content);
    print_enter;
}

void main_menu(UserNode *user_head, PaperNode *paper_head, QuestionNode *question_head,
               int *user_id, int *paper_id, int *question_id)
{
    const int option_count = 3; // 选项的个数
    const int admin_option = 0, stu_option = 1, exit_option = 2;
    const Text title = {"标准化选择题考试系统 v1.0", 25};
    const Text prompt = {"使用 [↑/↓] 移动，[Enter] 确认", 29};
    Text options[] = {
        {"1. 管理员登录", 13},
        {"2. 学生登录", 11},
        {"0. 退出系统", 11}};

    printf(CLS HIDE_CURSOR);
    int selection = 0;
    while (1)
    {
        draw_menu(title, prompt, selection, options, option_count);
        int key = get_keyboard_input();

        if (key == KEY_DOWN)
            selection = (selection + 1) % option_count;
        else if (key == KEY_UP)
            selection = (selection - 1 + option_count) % option_count;
        else if (key == KEY_ENTER)
        {
            if (selection == exit_option)
                break;
            else if (selection == admin_option)
            {
                if (admin_login())
                {
                    admin_menu(user_head, paper_head, question_head, user_id, paper_id, question_id);
                    printf(CLS HIDE_CURSOR);
                }
                else
                {
                    printf(CLS HIDE_CURSOR);
                    continue;
                }
            }
            else if (selection == stu_option)
            {
                if (user_login(user_head))
                {
                    stu_menu();
                    printf(CLS HIDE_CURSOR);
                }
                else
                {
                    printf(CLS HIDE_CURSOR);
                    continue;
                }
            }
        }
    }

    printf(SHOW_CURSOR CLS);
    printf("您已经安全退出标准化选择题考试系统\n");
    printf("欢迎下次使用\n");
}

void admin_menu(UserNode *user_head, PaperNode *paper_head, QuestionNode *question_head,
                int *user_id, int *paper_id, int *question_id)
{
    const int option_count = 4;
    const int question_option = 0, user_option = 1, paper_option = 2, exit_option = 3;
    const Text title = {"管理员操作界面", 14};
    // TODO 这里的提示可以打印试题数，试卷数，学生数
    const Text prompt = {"使用 [↑/↓] 移动，[Enter] 确认", 29};
    Text options[] = {
        {"1. 试题管理", 11},
        {"2. 用户管理", 11},
        {"3. 试卷管理", 11},
        {"0. 返回主菜单", 13}};

    printf(HIDE_CURSOR CLS);
    int selection = 0;

    while (1)
    {
        draw_menu(title, prompt, selection, options, option_count);
        int key = get_keyboard_input();

        if (key == KEY_DOWN)
            selection = (selection + 1) % option_count;
        else if (key == KEY_UP)
            selection = (selection - 1 + option_count) % option_count;
        else if (key == KEY_ENTER)
        {
            if (selection == exit_option)
                break;
            else if (selection == user_option)
            {
                admin_manage_stu_menu(user_head, user_id, paper_id, question_id);
                printf(CLS HIDE_CURSOR);
            }
            else if (selection == paper_option)
            {
                admin_manage_paper_menu(paper_head, user_id, paper_id, question_id);
                printf(CLS HIDE_CURSOR);
            }
            else if (selection == question_option)
            {
                admin_manage_question_menu(question_head, user_id, paper_id, question_id);
                printf(CLS HIDE_CURSOR);
            }
        }
    }
}

void admin_manage_stu_menu(UserNode *head, int *cur_stu_id, int *cur_paper_id, int *cur_question_id)
{
    const int option_count = 5;
    const int add_option = 0, del_option = 1, modify_option = 2, search_option = 3, exit_option = 4;
    const Text title = {"学生信息管理", 12};
    char buffer[40];
    Text prompt;
    int user_count;

    Text options[] = {
        {"1. 添加用户", 11},
        {"2. 删除用户", 11},
        {"3. 修改用户信息", 15},
        {"4. 查询用户做题信息", 19},
        {"0. 返回上一级", 13}};

    printf(HIDE_CURSOR CLS);
    int selection = 0;

    while (1)
    {
        user_count = list_user_get_len(head);
        sprintf(buffer, "系统中有 %d 位学生的信息", user_count);
        prompt.content = buffer;
        prompt.length = 22 + get_digit_count(user_count);
        draw_menu(title, prompt, selection, options, option_count);
        int key = get_keyboard_input();

        if (key == KEY_DOWN)
            selection = (selection + 1) % option_count;
        else if (key == KEY_UP)
            selection = (selection - 1 + option_count) % option_count;
        else if (key == KEY_ENTER)
        {
            if (selection == exit_option)
                break;
            if (selection == add_option)
            {
                add_stu(head, cur_stu_id);
                save_user_data(head);
                save_ids(*cur_stu_id, *cur_paper_id, *cur_question_id);
            }
            else if (selection == del_option)
            {
                del_stu(head);
                save_user_data(head);
            }
            else if (selection == modify_option)
            {
                modify_stu(head);
                save_user_data(head);
            }
            else if (selection == search_option)
            {
            }
            printf(CLS HIDE_CURSOR);
        }
    }
}

void admin_manage_paper_menu(PaperNode *head, int *cur_stu_id, int *cur_paper_id, int *cur_question_id)
{
    const int option_count = 5;
    const int _option = 0, see_option = 1, hand_option = 2, export_option = 3, exit_option = 4;
    const Text title = {"组卷系统", 8};
    // TODO 这里的提示当前的试卷数
    const Text prompt = {"使用 [↑/↓] 移动，[Enter] 确认", 29};
    Text options[] = {
        {"1. 开始组卷", 11},
        {"2. 浏览试卷", 11},
        {"3. 发布试卷", 11},
        {"4. 导出试卷", 11},
        {"0. 返回上一级", 13}};

    printf(HIDE_CURSOR CLS);
    int selection = 0;

    while (1)
    {
        draw_menu(title, prompt, selection, options, option_count);
        int key = get_keyboard_input();

        if (key == KEY_DOWN)
            selection = (selection + 1) % option_count;
        else if (key == KEY_UP)
            selection = (selection - 1 + option_count) % option_count;
        else if (key == KEY_ENTER)
        {
            if (selection == exit_option)
                break;
        }
    }
}

void admin_manage_question_menu(QuestionNode *head, int *cur_stu_id, int *cur_paper_id, int *cur_question_id)
{
    const int option_count = 6;
    const int add_option = 0, del_option = 1, modify_option = 2, search_option = 3, browse_option = 4, exit_option = 5;
    int question_count;
    char buffer[35];
    const Text title = {"试题管理", 8};
    Text prompt;
    Text options[] = {
        {"1. 添加试题", 11},
        {"2. 删除试题", 11},
        {"3. 修改试题", 11},
        {"4. 搜索试题", 11},
        {"5. 浏览试题", 11},
        {"0. 返回主菜单", 13}};

    printf(HIDE_CURSOR CLS);
    int selection = 0;

    while (1)
    {
        question_count = list_question_get_len(head);
        sprintf(buffer, "题库目前共有 %d 道试题", question_count);
        prompt.content = buffer;
        prompt.length = 20 + get_digit_count(question_count);
        draw_menu(title, prompt, selection, options, option_count);
        int key = get_keyboard_input();

        if (key == KEY_DOWN)
            selection = (selection + 1) % option_count;
        else if (key == KEY_UP)
            selection = (selection - 1 + option_count) % option_count;
        else if (key == KEY_ENTER)
        {
            if (selection == exit_option)
                break;
            else if (selection == add_option)
            {
                printf(SHOW_CURSOR);
                add_question(head, cur_question_id);
                save_question_data(head);
                save_ids(*cur_stu_id, *cur_paper_id, *cur_question_id);
                printf(HIDE_CURSOR);
            }
            else if (selection == del_option)
            {
                printf(SHOW_CURSOR);
                del_question(head);
                save_question_data(head);
                printf(HIDE_CURSOR);
            }
            else if (selection == modify_option)
            {
                printf(SHOW_CURSOR);
                modify_question(head);
                save_question_data(head);
                printf(HIDE_CURSOR);
            }
            else if (selection == search_option)
            {
                printf(SHOW_CURSOR);
                search_question(head);
                printf(HIDE_CURSOR);
            }
            else if (selection == browse_option)
            {
                printf(HIDE_CURSOR);
                browse_question(head);
            }
            printf(CLS HIDE_CURSOR);
        }
    }
}

void stu_menu()
{
    const int option_count = 3;
    const int exercise_option = 0, exam_option = 1, exit_option = 2;
    const Text title = {"学生操作界面", 12};
    // TODO 这里的信息可以打印当前学生的信息
    const Text prompt = {"使用 [↑/↓] 移动，[Enter] 确认", 29};
    Text options[] = {
        {"1. 练习", 7},
        {"2. 考试", 7},
        {"0. 返回主菜单", 13}};

    printf(HIDE_CURSOR CLS);
    int selection = 0;

    while (1)
    {
        draw_menu(title, prompt, selection, options, option_count);
        int key = get_keyboard_input();

        if (key == KEY_DOWN)
            selection = (selection + 1) % option_count;
        else if (key == KEY_UP)
            selection = (selection - 1 + option_count) % option_count;
        else if (key == KEY_ENTER)
        {
            if (selection == exit_option)
                break;
        }
    }
}

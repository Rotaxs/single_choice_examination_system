#include "admin.h"
#include "common.h"
#include "utils.h"

/**
 * @brief 菜单功能函数：管理员登录
 * @return 登录成功，返回 true，否则返回 false
 */
bool admin_login()
{
    bool is_valid;
    char input_pwd[MAX_PWD_LEN + 1];
    int attempted = 0;

    while (1)
    {
        printf(CLS);
        printf("请输入管理员密码：");

        // 判断密码是否过短
        is_valid = get_password_input(input_pwd, MAX_PWD_LEN + 1);
        print_enter;
        if (!is_valid)
        {
            WARNING("密码过短，请重新输入");
            attempted++;
            if (attempted >= PWD_ATTEMP_COUNT)
            {
                WAITING("尝试次数过多，即将返回到主菜单...");
                return false;
            }
            if (attempted >= PWD_ATTEMP_COUNT - 2)
            {
                char buffer[35];
                sprintf(buffer, "你还有 %d 次的尝试机会", PWD_ATTEMP_COUNT - attempted);
                WARNING(buffer);
                usleep(500000);
            }
            sleep(1);
            continue;
        }

        WAITING("正在进行登录检测...");

        // 密码正确
        if (strcmp(input_pwd, ADMIN_PWD) == 0)
        {
            CURSOR_SHIFT_UP;
            INFO("密码正确                    ");
            WAITING("即将跳转至管理员操作界面...");
            return true;
        }
        else
        {
            CURSOR_SHIFT_UP;
            ERR("密码错误，请重新输入          ");
            attempted++;

            if (attempted >= PWD_ATTEMP_COUNT)
            {
                WAITING("尝试次数过多，即将返回到主菜单...");
                return false;
            }
            if (attempted >= PWD_ATTEMP_COUNT - 2)
            {
                char buffer[35];
                sprintf(buffer, "你还有 %d 次的尝试机会", PWD_ATTEMP_COUNT - attempted);
                WARNING(buffer);
                usleep(500000);
            }
            sleep(1);
        }
    }
    return false;
}

/**
 * @brief 菜单功能函数：添加学生
 * @param head 学生数据链表
 * @param cur_id 最新 id 的指针
 */
void add_stu(UserNode *head, int *cur_id)
{
    bool is_valid;
    char account[MAX_ACCOUNT_LEN + 1];
    char password[MAX_PWD_LEN + 1];
    printf(CLS);
    while (1)
    {
        printf("请输入学生用户名：");
        get_account_input(account, MAX_ACCOUNT_LEN + 1);
        print_enter;
        if (strcmp(account, "q") == 0)
            break;
        if (list_user_is_reduplicate(head, account))
        {
            ERR("该用户名已存在");
            usleep(WAITING_TIME);
            continue;
        }
        printf("请输入密码：");
        is_valid = get_password_input(password, MAX_PWD_LEN + 1);
        print_enter;
        if (!is_valid)
        {
            WARNING("密码过短，请重新输入");
            usleep(WAITING_TIME);
            continue;
        }
        bool ok = list_user_add(head, *cur_id, account, password);
        if (!ok)
            ERR("添加学生失败");
        else
        {
            char buffer[45];
            sprintf(buffer, "成功添加学生信息，学生 ID 为 %d", *cur_id);
            INFO(buffer);
            (*cur_id)++;
        }
        printf("要继续添加学生信息吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }
    WAITING("正在返回到学生信息管理界面...");
}

/**
 * @brief 菜单功能函数：删除学生
 * @param head 用户数据链表
 */
void del_stu(UserNode *head)
{
    UserNode *user_node = NULL;
    bool ok;
    char input_account[MAX_ACCOUNT_LEN + 1];
    printf(CLS);
    while (1)
    {
        printf("请输入学生的用户名：");
        get_account_input(input_account, MAX_ACCOUNT_LEN + 1);
        print_enter;
        if (strcmp(input_account, "q") == 0)
            break;
        user_node = list_user_search(head, input_account);
        if (user_node == NULL)
        {
            ERR("没有找到该学生");
            usleep(WAITING_TIME);
            continue;
        }
        ok = list_user_delete(head, user_node->id);
        if (!ok)
            ERR("删除失败");
        else
            INFO("删除成功");
        printf("要继续删除学生信息吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }
    WAITING("正在返回到学生信息管理界面...");
}

/**
 * @brief 菜单功能函数：修改学生信息
 * @param head 用户数据链表
 */
void modify_stu(UserNode *head)
{
    UserNode *user_node = NULL;
    char input_account[MAX_ACCOUNT_LEN + 1];
    char changed_account[MAX_ACCOUNT_LEN + 1];
    char changed_password[MAX_PWD_LEN + 1];
    bool ok, is_valid;

    printf(CLS);
    while (1)
    {
        printf("请输入学生用户名：");
        get_account_input(input_account, MAX_ACCOUNT_LEN + 1);
        print_enter;
        if (strcmp(input_account, "q") == 0)
            break;
        user_node = list_user_search(head, input_account);
        if (user_node == NULL)
        {
            ERR("没有找到该学生");
            usleep(WAITING_TIME);
            continue;
        }

        while (1)
        {
            printf("要将用户名修改为：");
            get_account_input(changed_account, MAX_ACCOUNT_LEN + 1);
            print_enter;
            if (strcmp(input_account, changed_account) != 0 && list_user_is_reduplicate(head, changed_account))
            {
                ERR("该用户名已存在");
                usleep(WAITING_TIME);
                continue;
            }
            printf("要将密码修改为：");
            is_valid = get_password_input(changed_password, MAX_PWD_LEN + 1);
            print_enter;
            if (!is_valid)
            {
                WARNING("密码过短，请重新输入");
                usleep(WAITING_TIME);
                continue;
            }
            break;
        }

        ok = list_user_modify(head, user_node->id, changed_account, changed_password);
        if (!ok)
            ERR("修改失败");
        else
            INFO("修改成功");
        printf("要继续修改学生信息吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }
    WAITING("正在返回到学生信息管理界面...");
}

/**
 * @brief 菜单功能函数：添加试题
 * @param head 试题数据链表
 * @param cur_id 最新试题 id 的指针
 */
void add_question(QuestionNode *head, int *cur_id)
{
    bool ok;
    char question[QLEN + 1];
    char options[4][OPTIONLEN + 1];
    char answer;
    char category[CATEGORYLEN + 1];
    printf(SHOW_CURSOR);
    while (1)
    {
        while (1)
        {
            printf(CLS);
            printf("请输入试题题干（另起一行用 #\\n 作为结束）：\n");
            if (!get_question_input(question, QLEN + 1))
            {
                WARNING("选项长度过长，请重新输入");
                usleep(WAITING_TIME);
                continue;
            }
            break;
        }
        for (int i = 0; i < 4; i++)
        {
            printf("请输入选项 %c: ", 'A' + i);
            while (!get_option_input(options[i], OPTIONLEN + 1))
            {
                WARNING("选项长度过长，请重新输入");
                printf("请输入选项 %c: ", 'A' + i);
            }
        }
        printf("这道题的答案是：");
        while (!get_answer_input(&answer))
        {
            WARNING("答案格式输入错误，请重新输入");
            printf("这道题的答案是：");
        }
        printf("请输入这道题的分类：");
        while (!get_category_input(category, CATEGORYLEN + 1))
        {
            WARNING("分类长度过长，请重新输入");
            printf("请输入这道题的分类：");
        }

        list_question_add(head, *cur_id, question, options, answer, category);
        INFO("添加成功");
        char buffer[30];
        sprintf(buffer, "这道题目的 ID 为 %d", *cur_id);
        INFO(buffer);
        (*cur_id)++;
        printf("要继续添加试题吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }

    WAITING("正在返回到试题管理界面...");
}

/**
 * @brief 菜单功能函数：删除试题
 */
void del_question(QuestionNode *head)
{
    int id;
    printf(CLS);
    while (1)
    {
        printf("请输入要删除的试题的 ID：");
        while (!get_id_input(&id))
        {
            WARNING("Warning: 请输入正确的 ID");
            printf("请输入要删除的试题的 ID：");
        }
        if (list_question_delete(head, id))
            INFO("删除成功");
        else
            ERR("该试题不存在");
        printf("要继续删除试题吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }
    WAITING("正在返回到试题管理界面...");
}

/**
 * @brief 菜单功能辅助函数：打印试题
 * @param node 要打印的试题的结点
 */
void print_question(QuestionNode *node)
{
    printf("ID：%d\n", node->id);
    printf("%s\n", node->question);
    for (int i = 0; i < 4; i++)
    {
        printf("%c. ", 'A' + i);
        printf("%s", node->option[i]);
        print_enter;
    }
    print_enter;
    printf("答案：%c\n", node->answer);
    printf("类别：%s\n", node->category);
}

/**
 * @brief 菜单功能辅助函数：打印试题
 * @param head 试题数据链表
 * @param id 要打印的试题的 id
 */
void print_question_by_id(QuestionNode *head, int id)
{
    QuestionNode *cur_node = list_question_search(head, id);
    print_question(cur_node);
}

/**
 * @brief 菜单功能函数：修改试题
 */
void modify_question(QuestionNode *head)
{
    int id;
    while (1)
    {
        printf(CLS);
        QuestionNode *node;
        while (1)
        {
            printf(CLS);
            printf("请输入要修改的试题的 ID：");
            while (!get_id_input(&id))
            {
                WARNING("Warning: 请输入正确的 ID");
                printf("请输入要修改的试题的 ID：");
            }
            node = list_question_search(head, id);
            if (node != NULL)
                break;
            char buffer[45];
            sprintf(buffer, "ID 为 %d 的试题不存在或已被删除", id);
            ERR(buffer);
        }
        char changed_question[QLEN + 1];
        char changed_option[4][OPTIONLEN + 1];
        char changed_answer;
        char changed_category[CATEGORYLEN + 1];

        printf("搜索到该题如下\n");
        print_table(TABLE_U, MENU_WIDTH);
        print_enter;
        print_question_by_id(head, id);
        print_table(TABLE_B, MENU_WIDTH);
        print_enter;

        printf("要修改这道题的题干吗？（y/n）");
        if (get_y_or_n_input())
        {
            while (1)
            {
                printf("要将这题的题干修改为：\n");
                if (!get_question_input(changed_question, QLEN + 1))
                {
                    WARNING("选项长度过长，请重新输入");
                    usleep(WAITING_TIME);
                    continue;
                }
                break;
            }
        }
        else
            strcpy(changed_question, node->question);

        printf("要修改这道题的选项吗？（y/n）");
        if (get_y_or_n_input())
        {
            for (int i = 0; i < 4; i++)
            {
                printf("请输入选项 %c: ", 'A' + i);
                while (!get_option_input(changed_option[i], OPTIONLEN + 1))
                {
                    WARNING("选项长度过长，请重新输入");
                    printf("请输入选项 %c: ", 'A' + i);
                }
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
                strcpy(changed_option[i], node->option[i]);
        }

        printf("要修改这道题的答案吗？（y/n）");
        if (get_y_or_n_input())
        {
            printf("请输入答案：");
            while (!get_answer_input(&changed_answer))
            {
                WARNING("答案格式输入错误，请重新输入");
                printf("请输入答案：");
            }
        }
        else
            changed_answer = node->answer;

        printf("要修改这道题的类别吗？（y/n）");
        if (get_y_or_n_input())
        {
            printf("请输入这道题的分类：");
            while (!get_category_input(changed_category, CATEGORYLEN + 1))
            {
                WARNING("分类长度过长，请重新输入");
                printf("请输入这道题的分类：");
            }
        }
        else
            strcpy(changed_category, node->category);

        list_question_modify(head, id, changed_question, changed_option, changed_answer, changed_category);

        INFO("修改成功");

        printf("要继续修改题目吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }
    WAITING("正在返回到试题管理界面...");
}

/**
 * @brief 菜单功能辅助函数：显示 id 在 ids 中的试题
 * @param head 试题数据链表
 * @param ids 要打印的试题的 id
 * @param size ids 的大小
 */
void display_questions(QuestionNode *head, int *ids, int size)
{
    if (size <= 0)
        return;

    int i = 0;
    int key;

    while (i < size)
    {
        printf(CLS);
        print_table(TABLE_U, MENU_WIDTH);

        // 计算当前页结束的位置
        int page_end = (i + ONCE_COUNT > size) ? size : i + ONCE_COUNT;

        // 显示当前页的题目
        for (int j = i; j < page_end; j++)
        {
            print_enter;
            print_question(list_question_search(head, ids[j]));
            print_enter;
            print_table(TABLE_B, MENU_WIDTH);
        }

        // 交互逻辑
        printf(YELLOW "\n[第 %d-%d 题 / 共 %d 题]  方向键←/→：翻页  q：返回\n" RESET, i + 1, page_end, size);

        while (1)
        {
            key = get_keyboard_input();
            if (key == 'q')
                return;
            if (key == KEY_RIGHT)
            {
                if (page_end < size)
                {
                    i += ONCE_COUNT;
                    break;
                }
            }
            else if (key == KEY_LEFT)
            {
                if (i >= ONCE_COUNT)
                {
                    i -= ONCE_COUNT;
                    break;
                }
            }
        }
    }
}

/**
 * @brief 菜单功能函数：浏览试题
 */
void browse_question(QuestionNode *head)
{
    printf(CLS);
    QuestionNode *node = head->next;
    int ids[MAXQ_COUNT];
    int size = 0;
    for (; node; node = node->next)
        ids[size++] = node->id;
    display_questions(head, ids, size);
}

/**
 * @brief 菜单功能函数：搜索试题
 */
void search_question(QuestionNode *head)
{
    int ids[MAXQ_COUNT], size;
    char key;
    QuestionNode *node;
    char category[CATEGORYLEN + 1];
    while (1)
    {
        printf(CLS);
        printf("请输入要搜索的试题的类别：");
        while (!get_category_input(category, CATEGORYLEN + 1))
        {
            WARNING("分类长度过长，请重新输入");
            printf("请输入要搜索的试题的类别：");
        }
        size = list_question_search_by_category(head, category, ids);
        if (size == 0)
        {
            ERR("没有该类别的题目");
            usleep(WAITING_TIME);
            continue;
        }
        // printf("size = %d\n", size);
        printf(HIDE_CURSOR);
        display_questions(head, ids, size);
        printf(SHOW_CURSOR);
        printf("要继续搜索题目吗？（y/n）");
        if (!get_y_or_n_input())
            break;
    }
}

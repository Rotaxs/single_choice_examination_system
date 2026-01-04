#include "user.h"
#include "common.h"
#include "file_io.h"
#include "utils.h"

/**
 * @brief 菜单功能函数：用户登录
 * @param head 用户数据链表
 * @return 登录成功返回 true，否则返回 false
 */
bool user_login(UserNode *head)
{
    bool is_valid;
    char input_account[MAX_ACCOUNT_LEN + 1];
    char input_pwd[MAX_PWD_LEN + 1];
    int attempted = 0;
    UserNode *user_data;

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
            return false;
        }
        user_data = list_user_search(head, input_account);
        if (user_data == NULL)
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
                return false;
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
        if (strcmp(input_pwd, user_data->password) == 0)
        {
            CURSOR_SHIFT_UP;
            INFO("密码正确                    ");
            WAITING("即将进入学生操作界面...");
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
            usleep(WAITING_TIME);
        }
    }
    return false;
}

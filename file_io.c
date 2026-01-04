#include "file_io.h"
#include "common.h"
#include "utils.h"

/**
 * @brief 文件 I/O 函数：保存用户链表中的数据到文件
 * @param head 用户链表的头指针
 * @return 如果保存成功，返回 true，否则返回 false
 */
bool save_user_data(UserNode *head)
{
    FILE *fp = fopen(USERDATAPATH, "wb");
    if (fp == NULL)
    {
        printf(RED BOLD "Error: 打开用户数据文件失败\n" RESET);
        return false;
    }

    UserNode *cur_node = head->next;
    for (; cur_node; cur_node = cur_node->next)
    {
        cipher(cur_node->password);
        int ok = fwrite(cur_node, sizeof(UserNode) - sizeof(UserNode *), 1, fp);
        cipher(cur_node->password);
        if (ok != 1)
        {
            printf(RED BOLD "Error: 写入用户数据文件时错误" RESET);
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

/**
 * @brief 文件 I/O 函数：读取文件中的用户数据
 * @param head 用户链表的头指针
 * @return 读取成功返回 true，否则返回 false
 */
bool load_user_data(UserNode *head)
{
    FILE *fp = fopen(USERDATAPATH, "rb");
    if (fp == NULL)
        return save_user_data(head);

    UserNode user_data;

    while (fread(&user_data, sizeof(UserNode) - sizeof(UserNode *), 1, fp))
    {
        cipher(user_data.password);
        list_user_add(head, user_data.id, user_data.account, user_data.password);
    }

    fclose(fp);
    return true;
}

/**
 * @brief 文件 I/O 函数：保存试题链表中的数据到文件
 * @param head 试题数据链表
 * @return 读取成功返回 true，否则返回 false
 */
bool save_question_data(QuestionNode *head)
{
    FILE *fp = fopen(QDATAPATH, "wb");
    if (fp == NULL)
    {
        printf(RED "Error: 打开试题数据文件时出错" RESET);
        return false;
    }
    int data_size = sizeof(QuestionNode) - sizeof(QuestionNode *);
    QuestionNode *cur_node = head->next;

    for (; cur_node; cur_node = cur_node->next)
    {
        int ok = fwrite(cur_node, data_size, 1, fp);
        if (ok != 1)
        {
            printf(RED BOLD "Error: 写入试题数据文件时错误" RESET);
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

/**
 * @brief 文件 I/O 函数：读取试题文件中的数据
 * @param head 试题数据链表
 * @return 读取成功返回 true，否则返回 false
 */
bool load_question_data(QuestionNode *head)
{
    FILE *fp = fopen(QDATAPATH, "rb");
    if (fp == NULL)
        return save_question_data(head);

    QuestionNode question_data;
    int data_size = sizeof(QuestionNode) - sizeof(QuestionNode *);

    while (fread(&question_data, data_size, 1, fp))
    {
        list_question_add(head, question_data.id, question_data.question,
                          question_data.option, question_data.answer, question_data.category);
    }

    fclose(fp);
    return true;
}

/**
 * @brief 文件 I/O 函数：保存试卷链表中的数据到文件
 * @param head 试卷数据链表
 * @return 读取成功返回 true，否则返回 false
 */
bool save_paper_data(PaperNode *head)
{
    FILE *fp = fopen(PAPERDATAPATH, "wb");
    if (fp == NULL)
    {
        printf(RED "Error: 打开试卷数据文件时出错" RESET);
        return false;
    }

    int data_size = sizeof(PaperNode) - sizeof(PaperNode *);
    PaperNode *cur_node = head->next;

    for (; cur_node; cur_node = cur_node->next)
    {
        int ok = fwrite(cur_node, data_size, 1, fp);
        if (ok != 1)
        {
            printf(RED BOLD "Error: 写入试卷数据文件时错误" RESET);
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

/**
 * @brief 文件 I/O 函数：读取试卷文件中的数据
 * @param head 试卷数据链表
 * @return 读取成功返回 true，否则返回 false
 */
bool load_paper_data(PaperNode *head)
{
    FILE *fp = fopen(PAPERDATAPATH, "rb");
    if (fp == NULL)
        return save_paper_data(head);

    PaperNode paper_data;
    int data_size = sizeof(PaperNode) - sizeof(PaperNode *);

    while (fread(&paper_data, data_size, 1, fp))
    {
        list_paper_add(head, paper_data.id, paper_data.title,
                       paper_data.question_ids, paper_data.question_scores, paper_data.total_questions,
                       paper_data.paper_score, paper_data.start_time, paper_data.end_time);
    }

    fclose(fp);
    return true;
}

/**
 * @brief 读取最新用户/试卷/试题 id
 */
void load_ids(int *user_id, int *paper_id, int *question_id)
{
    FILE *fp = fopen(IDS_PATH, "rb");
    if (fp == NULL)
    {
        *user_id = 1;
        *paper_id = 1;
        *question_id = 1;
        save_ids(*user_id, *paper_id, *question_id);
        return;
    }

    if (fread(user_id, sizeof(int), 1, fp) != 1)
        *user_id = 1;
    if (fread(paper_id, sizeof(int), 1, fp) != 1)
        *paper_id = 1;
    if (fread(question_id, sizeof(int), 1, fp) != 1)
        *question_id = 1;

    fclose(fp);
}

/**
 * @brief 保存用户/试卷/试题 id 到文件
 * @return 保存成功返回 true，否则返回 false
 */
bool save_ids(int user_id, int paper_id, int question_id)
{
    FILE *fp = fopen(IDS_PATH, "wb");
    if (fp == NULL)
    {
        printf(RED BOLD "Error: 打开ID文件时发生错误" RESET);
        return false;
    }

    fwrite(&user_id, sizeof(user_id), 1, fp);
    fwrite(&paper_id, sizeof(paper_id), 1, fp);
    fwrite(&question_id, sizeof(question_id), 1, fp);

    fclose(fp);
    return true;
}

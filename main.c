#include "utils.h"
#include "common.h"
#include "file_io.h"
#include "admin.h"
#include "menu.h"
#include <time.h>

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand(time(NULL));

    UserNode *user_head = list_user_create();
    PaperNode *paper_head = list_paper_create();
    QuestionNode *question_head = list_question_create();
    load_user_data(user_head, paper_head);
    load_question_data(question_head);
    load_paper_data(paper_head);

    int user_id, paper_id, question_id;
    load_ids(&user_id, &paper_id, &question_id);
    // save_ids(1, 5, 31);
    main_menu(user_head, paper_head, question_head, &user_id, &paper_id, &question_id);
    // admin_menu(user_head, paper_head, question_head, &user_id, &paper_id, &question_id);
    // stu_menu(user_head, question_head, paper_head);

    list_user_destroy(user_head);
    list_paper_destroy(paper_head);
    list_question_destroy(question_head);

    return 0;
}

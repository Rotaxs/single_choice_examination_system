#ifndef ADMIN_H
#define ADMIN_H

#include <stdbool.h>
#include "common.h"

bool admin_login();
void add_stu(UserNode *head, int *cur_id);
void del_stu(UserNode *head);
void modify_stu(UserNode *head);

void add_question(QuestionNode *head, int *cur_id);
void del_question(QuestionNode *head);
void modify_question(QuestionNode *head);
void browse_question(QuestionNode *head);
void search_question(QuestionNode *head);

#endif

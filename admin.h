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

PaperNode *init_cur_paper(char *title, int size, int *cur_id);
void add_paper(PaperNode *paper_head, PaperNode *cur_paper);
void choose_question(QuestionNode *q_head, PaperNode *cur_paper);
void modify_title(PaperNode *cur_paper);
void set_scores(QuestionNode *q_head, PaperNode *cur_paper);
void set_time(PaperNode *cur_paper);
void publish_paper(PaperNode *cur_paper);

#endif

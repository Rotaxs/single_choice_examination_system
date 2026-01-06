#ifndef USER_H
#define USER_H

#include "common.h"

UserNode *user_login(UserNode *head);
void modify_password(UserNode *cur_stu);

#endif
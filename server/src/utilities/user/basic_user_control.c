/*
 * Author           ： Junfeng Zhang <564691478@qq.com>
 * Last-Change      ： April 6, 2014
 */

#include "linkc_types.h"
#include "linkc_user.h"
#include "linkc_db.h"
#include "def.h"
#include <string.h>

int user_logout(struct user_data* user)
{
    CHECK_FAILED(status_set (user,STATUS_OFFLINE),ERROR_SET_STATE);
    return LINKC_SUCCESS;
}

int user_login(struct user_data* user)
{
    user->UID = check_pass(user->login);
    if(user->UID == LINKC_FAILURE)
        return LINKC_FAILURE;
    if(get_info(user->UID,STATUS_GET) != STATUS_OFFLINE)
        return LINKC_FAILURE;
    status_set(user,STATUS_ONLINE);
    return LINKC_SUCCESS;
}

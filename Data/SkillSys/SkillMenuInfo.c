#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct MenuItemDef gSkillMenuInfos[MAX_SKILL_NUM + 1] = {
    [SID_Dance] = {
        .name = "　踊る",
        .nameMsgId = 0x67E,
        .helpMsgId = 0x6C2,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = DanceCommandUsabilityRework,
        .onDraw = NULL,
        .onSelected = PlayCommandEffect,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },

    [SID_Steal] = {
        .name = "　盗む",
        .nameMsgId = 0x67F,
        .helpMsgId = 0x6C4,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = StealCommandUsabilityRework,
        .onDraw = NULL,
        .onSelected = StealCommandEffect,
        .onIdle = 0,
        .onSwitchIn = 0,
        .onSwitchOut = 0
    },

    [SID_Summon] = {
        .name = "　召喚",
        .nameMsgId = 0x693,
        .helpMsgId = 0x6DD,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = SummonCommandUsabilityRework,
        .onDraw = 0,
        .onSelected = SummonCommandEffect,
        .onIdle = 0,
        .onSwitchIn = 0,
        .onSwitchOut = 0
    },
};

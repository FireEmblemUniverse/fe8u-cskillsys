#pragma once

#include "common-chax.h"

enum KTUTORIAL_flags {
    KTUTORIAL_COMBO_ATK = 1,
    KTUTORIAL_BATTLE_SURROUNDER,

    KTUTORIAL_MAX = 96,
};

void ClearKtutorialFlags(void);
void EMS_SaveKTutorialFlags(u8 * dst, const u32 size); /* need 0x18 Bytes */
void EMS_LoadKTutorialFlags(u8 * src, const u32 size);

void TriggerKtutorial(int flag);

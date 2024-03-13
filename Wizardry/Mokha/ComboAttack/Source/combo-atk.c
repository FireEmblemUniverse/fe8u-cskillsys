#include "common-chax.h"
#include "combo-attack.h"
#include "weapon-range.h"
#include "kernel-tutorial.h"

void ResetComboAtkList(void)
{
    u32 i;
    CpuFill16(0, gComboAtkList, sizeof(gComboAtkList));

    for (i = 0; i < ARRAY_COUNT(gComboAtkList); i++)
    {
        gComboAtkList[i].uid = COMBO_ATK_UID_INVALID;
    }
}

/* This is only valid in after battle unit inited */
void BattleGenerateComboAtkList(void)
{
    struct Unit * unit;
    u16 item;
    int range;

    int i, cnt = 0;

    ResetComboAtkList();

    for (i = 1; i < 0x100; i++)
    {
        if (!(cnt < COMBO_ATK_MAX))
            break;

        if ((i & 0xC0) != UNIT_FACTION(&gBattleActor.unit))
            continue;

        /* If not valid unit */
        unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->index == gBattleActor.unit.index)
            continue;

        /* If no weapon */
        item = GetUnitEquippedWeapon(unit);
        if (!item)
            continue;

        range = RECT_DISTANCE(
            unit->xPos, unit->yPos,
            gBattleTarget.unit.xPos, gBattleTarget.unit.yPos);

        if (!IsItemCoveringRangeRework(item, range, unit))
            continue;

        gComboAtkList[cnt].uid = i;
        gComboAtkList[cnt].weapon = ITEM_INDEX(item);
        cnt++;
    }

    if (cnt > 0)
        TriggerKtutorial(KTUTORIAL_COMBO_ATK);
}

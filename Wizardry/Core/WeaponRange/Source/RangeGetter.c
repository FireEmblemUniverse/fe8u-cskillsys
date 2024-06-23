#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmmap.h"
#include "bmidoten.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "weapon-range.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

typedef int (* WeaponRangeGetterFunc_t)(int old, struct Unit * unit, u16 item);
// extern const WeaponRangeGetterFunc_t gWeaponRangeGetters[];
extern WeaponRangeGetterFunc_t const * const gpWeaponRangeGetters;

int GetItemMinRangeRework(u16 item, struct Unit * unit)
{
    int status = GetItemMinRange(item);

    if (IS_UNIT_PTR(unit))
    {

    }
    return status;
}

int GetItemMaxRangeRework(u16 item, struct Unit * unit)
{
    const WeaponRangeGetterFunc_t * it;
    int status = GetItemMaxRange(item);

    if (status == 0)
        status = GetUnitMagBy2Range(unit);

    if (IS_UNIT_PTR(unit))
    {
        for (it = gpWeaponRangeGetters; *it; it++)
            status = (*it)(status, unit, item);
    }
    return status;
}

int WeaponRangeGetterSkills(int range, struct Unit * unit, u16 item)
{
    switch (GetItemType(item)) {
    case ITYPE_BOW:
#if defined(SID_RangeBonusBow1) && (COMMON_SKILL_VALID(SID_RangeBonusBow1))
        if (SkillTester(unit, SID_RangeBonusBow1))
            range = range + 1;
#endif

#if defined(SID_RangeBonusBow2) && (COMMON_SKILL_VALID(SID_RangeBonusBow2))
        if (SkillTester(unit, SID_RangeBonusBow2))
            range = range + 2;
#endif

#if defined(SID_RunningStart) && (COMMON_SKILL_VALID(SID_RunningStart))
        if (SkillTester(unit, SID_RunningStart))
            range = range + gActionData.moveCount/2;
#endif
        break;

    case ITYPE_ANIMA:
    case ITYPE_LIGHT:
    case ITYPE_DARK:
#if defined(SID_RangeBonusBMag1) && (COMMON_SKILL_VALID(SID_RangeBonusBMag1))
        if (SkillTester(unit, SID_RangeBonusBMag1))
            range = range + 1;
#endif

#if defined(SID_RangeBonusBMag2) && (COMMON_SKILL_VALID(SID_RangeBonusBMag2))
        if (SkillTester(unit, SID_RangeBonusBMag2))
            range = range + 2;
#endif
        break;
    }
    return range;
}

int GetUnitMinRange(struct Unit * unit)
{
    int i, item, _ret, ret = 1;
     for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        item = unit->items[i];

        if (ITEM_INDEX(item) > 0 && CanUnitUseWeapon(unit, item))
        {
            _ret = GetItemMinRangeRework(item, unit);

            LTRACEF("item=%#x, rng=%d, min=%d", item, _ret, ret);

            if (_ret < ret)
                ret = _ret;
        }
    }
    return ret;
}

int GetUnitMaxRange(struct Unit * unit)
{
    int i, item, _ret, ret = 1;
     for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        item = unit->items[i];

        if (ITEM_INDEX(item) > 0 && CanUnitUseWeapon(unit, item))
        {
            _ret = GetItemMaxRangeRework(item, unit);

            LTRACEF("item=%#x, rng=%d, max=%d", item, _ret, ret);

            if (_ret > ret)
                ret = _ret;
        }
    }
    return ret;
}

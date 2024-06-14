#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitPower(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->pow;

    for (it = gpPowGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int PowGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemPowBonus(weapon);
    return status;
}

int PowGetterSkills(int status, struct Unit * unit)
{
#if defined(SID_StrBonus) && (SID_StrBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_StrBonus))
        status += 2;
#endif

#if defined(SID_DefiantStr) && (SID_DefiantStr < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantStr))
        if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
            status += 7;
#endif

#if defined(SID_Fury) && (SID_Fury < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_Fury))
        status += 3;
#endif

#if defined(SID_FuryPlus) && (SID_FuryPlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FuryPlus))
        status += 4;
#endif

#if defined(SID_FortressDef) && (SID_FortressDef < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FortressDef))
        status -= 3;
#endif

#if defined(SID_FortressRes) && (SID_FortressRes < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FortressRes))
        status -= 3;
#endif

#if defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_RuinedBlade))
        status -= 5;
#endif

#if defined(SID_HeavyBladePlus) && (SID_HeavyBladePlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_HeavyBladePlus))
        status += 5;
#endif

#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_POW)
        status += 7;
#endif

    if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
    {
#if defined(SID_PushStrength) && (SID_PushStrength < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushStrength))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }
    
#if (defined(SID_Resolve) && (SID_Resolve < MAX_SKILL_NUM)) 
        if (SkillTester(unit, SID_Resolve))
        {
            if (((GetUnitMaxHp(unit) % 2 == 0) && (GetUnitCurrentHp(unit) < GetUnitMaxHp(unit)/2)) ||
                ((GetUnitMaxHp(unit) % 2 != 0) && (GetUnitCurrentHp(unit) -1 < GetUnitMaxHp(unit)/2)))
                {
                    status += status/2;
                }
        }
#endif

    return status;
}

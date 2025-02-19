#include "common-chax.h"
#include "combat-art.h"
#include "weapon-range.h"
#include "kernel-tutorial.h"
#include "constants/texts.h"

u8 CombatArtActionCommandUsability(const struct MenuItemDef *def, int number)
{
	int i;

	if (gActiveUnit->state & US_HAS_MOVED)
		return MENU_NOTSHOWN;

	if (gActiveUnit->state & US_IN_BALLISTA)
		return MENU_NOTSHOWN;

	/* If vanilla "Attack" button is enabled, here not show */
	if (AttackCommandUsabilityFix(def, number) == MENU_ENABLED)
		return MENU_NOTSHOWN;

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		int item = gActiveUnit->items[i];

		if (item == 0)
			break;

		if (!(GetItemAttributes(item) & IA_WEAPON))
			continue;

		if (!CanUnitUseWeaponNow(gActiveUnit, item))
			continue;

		if (!CanUnitPlayCombatArt(gActiveUnit, item))
			continue;

		RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, item));
		MakeTargetListForWeapon(gActiveUnit, item);
		if (GetSelectTargetCount() == 0)
			continue;

		return MENU_ENABLED;
	}
	return MENU_NOTSHOWN;
}

int CombatArtActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item)
{
	int color = TEXT_COLOR_SYSTEM_WHITE;

	if (!CheckKtutFlagTriggered(KTUTORIAL_COMBATART_MENU))
		color = TEXT_COLOR_SYSTEM_GREEN;

	Text_SetColor(&item->text, color);
	Text_DrawString(&item->text, GetStringFromIndex(MSG_COMBATART_UM_NAME));

	 PutText(
		&item->text,
		TILEMAP_LOCATED(BG_GetMapBuffer(menu->frontBg), item->xTile, item->yTile));

	return 0;
}

u8 CombatArtActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct MenuProc *sub_menu;

	if (menuItem->availability == MENU_DISABLED)
		return MENU_ACT_SND6B;

	ResetIconGraphics();
	LoadIconPalettes(4);

	sub_menu = StartOrphanMenu(&gCombatArtItemMenuDef);

	if (gActiveUnit->pClassData->number != CLASS_PHANTOM) {
		StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
		SetFaceBlinkControlById(0, 5);
	}

	ForceMenuItemPanel(sub_menu, gActiveUnit, 0xF, 0xB);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

int CombatArtActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	int i, mask = 0;

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		int item = gActiveUnit->items[i];

		if (item == 0)
			break;

		if (!(GetItemAttributes(item) & IA_WEAPON))
			continue;

		if (!CanUnitUseWeaponNow(gActiveUnit, item))
			continue;

		if (!CanUnitPlayCombatArt(gActiveUnit, item))
			continue;

		RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, item));
		mask |= GetItemReachBitsRework(item, gActiveUnit);
	}

	GenerateUnitStandingReachRange(gActiveUnit, mask);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED);
	return 0;
}

int CombatArtActionCommandUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	ResetCombatArtList();
	ResetCombatArtStatus();
	HideMoveRangeGraphics();
	return 0;
}

/**
 * Attack menu fix
 */
u8 AttackCommandUsabilityFix(const struct MenuItemDef *def, int number)
{
	ResetCombatArtList();
	ResetCombatArtStatus();

	return AttackCommandUsability(def, number);
}

u8 AttackBallistaCommandUsabilityFix(const struct MenuItemDef *def, int number)
{
	ResetCombatArtList();
	ResetCombatArtStatus();

	return AttackBallistaCommandUsability(def, number);
}

int AttackActionCommandHoverFix(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	ResetCombatArtList();
	ResetCombatArtStatus();
	return DisplayUnitStandingAttackRange(menu, menuItem);
}

int AttackActionCommandUnhoverFix(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	ResetCombatArtList();
	ResetCombatArtStatus();
	return HideMoveRangeGraphicsWrapper(menu, menuItem);
}

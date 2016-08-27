Action Entity_Timer(Handle Timer)
{
	char ClassName[32];
	for (int Entity = 0; Entity < 2048; Entity++)
	{
		if (!IsValidEntity(Entity))
		{
			continue;
		}
		GetEntityClassname(Entity, ClassName, sizeof(ClassName));
		if ((StrContains(ClassName, "weapon_", false) || StrContains(ClassName, "item_", false)) && GetEntDataEnt2(Entity, FindSendPropInfo("CBaseCombatWeapon", "m_hOwnerEntity")) && GetEntProp(Entity, Prop_Data, "m_iHammerID") == 0)
		{
			AcceptEntityInput(Entity, "Kill");
		}
	}
	return Plugin_Handled;
}
void Entity_OnPluginStart()
{
	CreateTimer(30.0, Entity_Timer, _, TIMER_REPEAT);
}
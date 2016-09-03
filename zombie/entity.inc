void Entity_OnPluginStart()
{
	HookEntityOutput("func_button", "OnPressed", Entity_OnPressed);
}
void Entity_OnPressed(const char[] Output, int Caller, int Activer, float Delay)
{
	if (CheckAlive(Activer))
	{
		CPrintToChatAll("{green}[ZA][实体]{blue} %N 使用了按钮/道具", Activer);
	}
}
Action Entity_Timer(Handle Timer)
{
	if (Navbar_Time < 30)
	{
		return Plugin_Handled;
	}
	char ClassName[32];
	for (int Entity = 0; Entity < 2048; Entity++)
	{
		if (!IsValidEntity(Entity))
		{
			continue;
		}
		if (!GetEntityClassname(Entity, ClassName, sizeof(ClassName)))
		{
			continue;
		}
		if ((StrContains(ClassName, "weapon_", false) != -1 || StrContains(ClassName, "item_", false) != -1) && GetOwner(Entity) == -1 && GetHammerId(Entity) == 0)
		{
			AcceptEntityInput(Entity, "Kill");
		}
	}
	return Plugin_Handled;
}
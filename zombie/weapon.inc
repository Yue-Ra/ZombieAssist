char Weapon_Name[32][32];
int Weapon_Type[32];
int Weapon_Cash[32];
char Weapon_Alias[32][32];

bool Weapon_Allow = true;
int Weapon_Max = 0;

void Weapon_Load()
{
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM weapon ORDER BY type ASC");
	SQL_TQuery(MySQL, Weapon_Load_Query, Query);
}
void Weapon_Load_Query(Handle Owner, Handle Child, const char[] Error, any Data)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Weapon Handle Failure");
		return;
	}
	int Num = 0;
	while (SQL_FetchRow(Child))
	{
		SQL_FetchString(Child, 0, Weapon_Name[Num], sizeof(Weapon_Name[]));
		Weapon_Type[Num] = SQL_FetchInt(Child, 1);
		Weapon_Cash[Num] = SQL_FetchInt(Child, 2);
		SQL_FetchString(Child, 3, Weapon_Alias[Num], sizeof(Weapon_Alias[]));
		LogMessage("Weapon %d\tName %s\tType %d\tCash %d\tAlias %s", Num, Weapon_Name[Num], Weapon_Type[Num], Weapon_Cash[Num], Weapon_Alias[Num]);
		Num++;
	}
	Weapon_Max = Num;
}
Action Weapon_Command(int Client, int Args)
{
	if (CheckHuman(Client))
	{
		if (Args == 0)
		{
			Weapon_Menu(Client);
			return Plugin_Handled;
		}
		char ItemName[32];
		GetCmdArgString(ItemName, sizeof(ItemName));
		if (StrContains(ItemName, "weapon_", false) == -1)
		{
			Format(ItemName, sizeof(ItemName), "weapon_%s", ItemName);
		}
		for (int Num = 0; Num < Weapon_Max; Num++)
		{
			if (StrContains(ItemName, Weapon_Name[Num], false) != -1 || StrContains(ItemName, Weapon_Alias[Num], false) != -1)
			{
				Weapon_Buy(Client, Num);
				return Plugin_Handled;
			}
		}
		PrintToChat(Client, "[ZA]武器名称不存在");
	}
	return Plugin_Handled;
}
Action Weapon_Reload_Command(int Client, int Args)
{
	Weapon_Load();
	return Plugin_Handled;
}
void Weapon_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Weapon_Menu_Handle);
	SetMenuTitle(MenuHandle, "武器菜单\n当前金钱 %d", GetCash(Client));
	char ItemName[32];
	for (int Num = 0; Num < Weapon_Max; Num++)
	{
		Format(ItemName, sizeof(ItemName), "%s - %d", Weapon_Name[Num], Weapon_Cash[Num]);
		AddMenuItem(MenuHandle, "", ItemName);
	}
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Weapon_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		Weapon_Buy(Client, Item);
	}
}
void Weapon_Buy(int Client, int Item)
{
	if (!CheckHuman(Client))
	{
		return;
	}
	if (!Weapon_Allow)
	{
		PrintToChat(Client, "[ZA]武器功能已被禁用");
		return;
	}
	int Cash = GetCash(Client);
	if (Cash < Weapon_Cash[Item])
	{
		PrintToChat(Client, "[ZA]金钱不足，还需要 %d", Weapon_Cash[Item] - Cash);
		return;
	}
	SetCash(Client, Cash - Weapon_Cash[Item]);
	int Slot = GetPlayerWeaponSlot(Client, Weapon_Type[Item]);
	if (Slot != -1)
	{
		CS_DropWeapon(Client, Slot, false, false);
	}
	GivePlayerItem(Client, Weapon_Name[Item]);
}
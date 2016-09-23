char Weapon_Name[24][32];
int Weapon_Type[24];
int Weapon_Cash[24];
char Weapon_Alias[24][32];
int Weapon_Skill[24];
int Weapon_Ammo[24];
int Weapon_Addon[24][10];
int Weapon_Speed[24];
int Weapon_Accel[24];

int Weapon_Limit = 0;
int Weapon_Client[MAXPLAYERS + 1];
int Weapon_Time[MAXPLAYERS + 1];
int Weapon_MaxAmmo[MAXPLAYERS + 1];

bool Weapon_Allow = true;
Handle Weapon_Handle[MAXPLAYERS + 1];
int Weapon_Max = 0;

void Weapon_OnMapStart()
{
	Weapon_Limit = 0;
}
void Weapon_OnRoundStart()
{
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Weapon_Time[Client] = 0;
		Weapon_Client[Client] = 0;
		ClearTimer(Weapon_Handle[Client]);
	}
}
void Weapon_OnClientPutInServer(int Client)
{
	Weapon_Time[Client] = 0;
	Weapon_Client[Client] = 0;
	SDKHook(Client, SDKHook_WeaponEquipPost, Weapon_WeaponEquipPost);
	SDKHook(Client, SDKHook_WeaponSwitchPost, Weapon_WeaponSwitchPost);
}
void Weapon_WeaponEquipPost(int Client, int Weapon)
{
	char Name[32];
	GetEntityClassname(Weapon, Name, sizeof(Name));
	int Skill = Skill_13_OnWeapon(Client);
	if (Skill == 0)
	{
		return;
	}
	int Item = Weapon_GetByEntity(Weapon);
	if (Item == -1)
	{
		return;
	}
	SDKHook(Weapon, SDKHook_Reload, Weapon_Reload);
	SDKHook(Weapon, SDKHook_ReloadPost, Weapon_ReloadPost);
}
void Weapon_WeaponSwitchPost(int Client, int Weapon)
{
	char Name[32];
	GetEntityClassname(Weapon, Name, sizeof(Name));
	ClearTimer(Weapon_Handle[Client]);
	int Item = Weapon_GetByEntity(Weapon);
	if (Item == -1)
	{
		return;
	}
	int Skill = Skill_13_OnWeapon(Client);
	if (Skill > 0)
	{
		Weapon_MaxAmmo[Client] = Weapon_Ammo[Item] + Weapon_Addon_Count(Item, Skill);
		CreateTimer(0.01, Weapon_Addon_Timer, Weapon);
	}
	if (Skill_15_OnWeapon(Client))
	{
		SetSpeed(Client, float(Weapon_Accel[Item]) / float(Weapon_Speed[Item]));
	}
}
Action Weapon_Addon_Timer(Handle Timer, any Weapon)
{
	int Item = Weapon_GetByEntity(Weapon);
	if (Item == -1)
	{
		return Plugin_Handled;
	}
	int Client = GetOwner(Weapon);
	if (Client == -1)
	{
		return Plugin_Handled;
	}
	if (GetClip(Weapon) == Weapon_Ammo[Item])
	{
		SetClip(Weapon, Weapon_MaxAmmo[Client]);
	}
	return Plugin_Handled;
}
Action Weapon_Reload(int Weapon)
{
	int Item = Weapon_GetByEntity(Weapon);
	if (Item == -1)
	{
		return Plugin_Continue;
	}
	int Client = GetOwner(Weapon);
	if (Client == -1)
	{
		return Plugin_Continue;
	}
	if (Skill_13_OnWeapon(Client) == 0)
	{
		return Plugin_Continue;
	}
	int Ammo = GetClip(Weapon);
	if (Ammo == Weapon_Ammo[Item] || Ammo == Weapon_MaxAmmo[Client])
	{
		return Plugin_Handled;
	}
	return Plugin_Continue;
}
void Weapon_ReloadPost(int Weapon, bool Success)
{
	int Client = GetOwner(Weapon);
	if (Skill_13_OnWeapon(Client) == 0)
	{
		return;
	}
	ClearTimer(Weapon_Handle[Client]);
	float ReloadTime = GetEntPropFloat(Client, Prop_Send, "m_flNextAttack") - GetGameTime();
	if (ReloadTime <= 0.0)
	{
		return;
	}
	Weapon_Handle[Client] = CreateTimer(ReloadTime + 0.1, Weapon_Reload_Timer, Weapon);
}
Action Weapon_Reload_Timer(Handle Timer, any Weapon)
{
	int Client = GetOwner(Weapon);
	if (Client == -1)
	{
		ClearTimer(Weapon_Handle[Client]);
		return Plugin_Handled;
	}
	int Skill = Skill_13_OnWeapon(Client);
	if (Skill == 0)
	{
		ClearTimer(Weapon_Handle[Client]);
		return Plugin_Handled;
	}
	int Item = Weapon_GetByEntity(Weapon);
	if (Item == -1)
	{
		ClearTimer(Weapon_Handle[Client]);
		return Plugin_Handled;
	}
	SetClip(Weapon, Weapon_MaxAmmo[Client]);
	ClearTimer(Weapon_Handle[Client]);
	return Plugin_Handled;
}
void Weapon_Load()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
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
		Weapon_Skill[Num] = SQL_FetchInt(Child, 4);
		Weapon_Ammo[Num] = SQL_FetchInt(Child, 5);
		for (int Add = 0; Add < 10; Add++)
		{
			Weapon_Addon[Num][Add] = SQL_FetchInt(Child, 6 + Add);
		}
		Weapon_Speed[Num] = SQL_FetchInt(Child, 16);
		Weapon_Accel[Num] = SQL_FetchInt(Child, 17);
		LogMessage("Weapon %d\tName %s\tType %d\tCash %d\tAlias %s\tSkill %d\tAmmo %d", Num, Weapon_Name[Num], Weapon_Type[Num], Weapon_Cash[Num], Weapon_Alias[Num], Weapon_Skill[Num], Weapon_Ammo[Num]);
		Num++;
	}
	Weapon_Max = Num;
}
int Weapon_Addon_Count(int Item, int Level)
{
	int Count = 0;
	for (int Num = 0; Num < Level; Num++)
	{
		Count += Weapon_Addon[Item][Num];
	}
	return Count;
}
int Weapon_GetByEntity(int Entity)
{
	char Name[32];
	GetEntityClassname(Entity, Name, sizeof(Name));
	for (int Num = 0; Num < Weapon_Max; Num++)
	{
		if (StrEqual(Name, Weapon_Name[Num], false))
		{
			if (Weapon_Type[Num] > 1)
			{
				return -1;
			}
			return Num;
		}
	}
	return -1;
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
		CPrintToChat(Client, "{green}[ZA][武器] {red}武器名称不存在");
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
	char ItemName[64];
	float Discount = Skill_11_OnWeapon(Client);
	for (int Num = 0; Num < Weapon_Max; Num++)
	{
		Format(ItemName, sizeof(ItemName), "%s(%s) - %d", Weapon_Name[Num], Weapon_Alias[Num], RoundToCeil(Weapon_Cash[Num] * Discount));
		ReplaceString(ItemName, sizeof(ItemName), "weapon_", "", false);
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
	if (!Weapon_Allow)
	{
		CPrintToChat(Client, "{green}[ZA][武器] {red}武器功能已被禁用");
		return;
	}
	if (!CheckHuman(Client))
	{
		CPrintToChat(Client, "{green}[ZA][武器] {red}只有人类才能打手枪");
		return;
	}
	if (Weapon_Skill[Item] > Skill_10_OnWeapon(Client))
	{
		CPrintToChat(Client, "{green}[ZA][技能] {red}需要 {gold}%s{red} 技能 {gold}%d{red} 级", Skill_Name[9], Weapon_Skill[Item]);
		return;
	}
	int Cash = GetCash(Client);
	int Price = RoundToCeil(Weapon_Cash[Item] * Skill_11_OnWeapon(Client));
	if (Cash < Price)
	{
		CPrintToChat(Client, "{green}[ZA][武器] {red}金钱不足,还需要 {gold}%d", Price - Cash);
		return;
	}
	if (Weapon_Limit > 0 && StrEqual(Weapon_Name[Item], "weapon_hegrenade", false))
	{
		if (Weapon_Client[Client] >= Weapon_Limit)
		{
			CPrintToChat(Client, "{green}[ZA][武器] {red}手雷购买超过上限 {gold}%d", Weapon_Limit);
			return;
		}
		else
		{
			Weapon_Client[Client]++;
		}
	}
	SetCash(Client, Cash - Price);
	int Slot = GetPlayerWeaponSlot(Client, Weapon_Type[Item]);
	if (Slot != -1)
	{
		CS_DropWeapon(Client, Slot, false, false);
	}
	GivePlayerItem(Client, Weapon_Name[Item]);
}
void Weapon_Unlimited(int Client)
{
	Weapon_Time[Client] = GetTime() + 5;
}
void Weapon_Credit(int Client)
{
	if (Weapon_Time[Client] < GetTime())
	{
		return;
	}
	int Weapon = GetWeapon(Client);
	SetClip(Weapon, GetClip(Weapon) + 1);
}
int Skill_Client[MAXPLAYERS + 1][16];
int Skill_Alive[MAXPLAYERS + 1];
char Skill_Team[16][8];
char Skill_Name[16][32];
char Skill_Info[16][64];
int Skill_Cost[16][10];
int Skill_MaxLv[16];
int Skill_Max = 0;
int Skill_Select[MAXPLAYERS + 1];

Handle Skill_2_Handle[MAXPLAYERS + 1];
Handle Skill_3_Handle[MAXPLAYERS + 1];

bool Skill_Allow[16];
void Skill_Allow_Set(int Skills[16])
{
	for (int Num = 0; Num < sizeof(Skills); Num++)
	{
		Skill_Allow[Num] = Skills[Num] == 0 ? false : true;
	}
}
void Skill_Init(int Client)
{
	for (int Num = 0; Num < 16; Num++)
	{
		Skill_Client[Client][Num] = 0;
	}
	Skill_Alive[Client] = 0;
	ClearTimer(Skill_2_Handle[Client]);
	ClearTimer(Skill_3_Handle[Client]);
}
void Skill_OnLevelUp(int Client)
{
	int Count = Skill_Count(Client);
	int MaxSp = Skill_MaxSP(Client);
	if (Count > MaxSp)
	{
		Skill_Empty(Client);
		Count = 0;
	}
	Skill_Alive[Client] = MaxSp - Count;
}
Action Skill_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Skill_Menu(Client);
	}
	return Plugin_Handled;
}
Action Skill_Reload_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Skill_Load();
	}
	return Plugin_Handled;
}
void Skill_Load()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM skill ORDER BY id ASC");
	SQL_TQuery(MySQL, Skill_Load_Query, Query);
}
void Skill_Load_Query(Handle Owner, Handle Child, const char[] Error, any Data)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Skill Handle Failure");
		return;
	}
	int Count = 0;
	while (SQL_FetchRow(Child))
	{
		SQL_FetchString(Child, 1, Skill_Team[Count], sizeof(Skill_Team[]));
		SQL_FetchString(Child, 2, Skill_Name[Count], sizeof(Skill_Name[]));
		SQL_FetchString(Child, 3, Skill_Info[Count], sizeof(Skill_Info[]));
		Skill_MaxLv[Count] = SQL_FetchInt(Child, 4);
		for (int Num = 0; Num < 10; Num++)
		{
			Skill_Cost[Count][Num] = SQL_FetchInt(Child, 5 + Num);
		}
		Count++;
	}
	Skill_Max = Count;
}
void Skill_Set(int Client, int Skills[16])
{
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Skill_Client[Client][Num] = Skill_MaxLv[Num];
	}
	Skill_Alive[Client] = 0;
	/*
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Skill_Client[Client][Num] = Skills[Num];
		if (Skill_Client[Client][Num] > Skill_MaxLv[Num])
		{
			Skill_Client[Client][Num] = 0;
		}
	}
	int Count = Skill_Count(Client);
	int MaxSp = Skill_MaxSP(Client);
	if (Count > MaxSp)
	{
		Skill_Empty(Client);
		Count = 0;
	}
	Skill_Alive[Client] = MaxSp - Count;
	*/
}
void Skill_Empty(int Client)
{
	char Query[192];
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Skill_Client[Client][Num] = 0;
	}
	Format(Query, sizeof(Query), "UPDATE user SET sk1=0,sk2=0,sk3=0,sk4=0,sk5=0,sk6=0,sk7=0,sk8=0,sk9=0,sk10=0,sk11=0,sk12=0,sk13,sk14=0,sk15=0,sk16=0 WHERE id=%d", Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	CPrintToChat(Client, "{green}[ZA][技能] {red}你的技能配点已被重置");
}
void Skill_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Skill_Menu_Handle);
	SetMenuTitle(MenuHandle, "技能菜单\n技能点 %d", Skill_Alive[Client]);
	char Item[192];
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Format(Item, sizeof(Item), "[%s]%s[%d/%d]", Skill_Team[Num], Skill_Name[Num], Skill_Client[Client][Num], Skill_MaxLv[Num]);
		AddMenuItem(MenuHandle, "", Item);
	}
	// SetMenuPagination(MenuHandle, 10);
	SetMenuExitButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Skill_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		Skill_Item_Menu(Client, Item);
		/*
		Skill_Update(Client, Item);
		Skill_Menu(Client);
		*/
	}
}
void Skill_Item_Menu(int Client, int Num)
{
	Skill_Select[Client] = Num;
	Menu MenuHandle = CreateMenu(Skill_Menu_Item_Handle);
	SetMenuTitle(MenuHandle, "详细信息");
	char Item[192];
	Format(Item, sizeof(Item), "【类型】%s", Skill_Team[Num]);
	AddMenuItem(MenuHandle, "", Item, ITEMDRAW_DISABLED);
	Format(Item, sizeof(Item), "【名称】%s", Skill_Name[Num]);
	AddMenuItem(MenuHandle, "", Item, ITEMDRAW_DISABLED);
	Format(Item, sizeof(Item), "【简介】%s", Skill_Info[Num]);
	AddMenuItem(MenuHandle, "", Item, ITEMDRAW_DISABLED);
	char Cost[192], Level[192];
	for (int Count = 1; Count <= Skill_MaxLv[Num]; Count++)
	{
		if (Skill_Client[Client][Num] == Count)
		{
			Format(Cost, sizeof(Cost), "%s[%2d] ", Cost, Skill_Cost[Num][Count - 1]);
			Format(Level, sizeof(Level), "%s[%2d] ", Level, Count);
		}
		else
		{
			Format(Cost, sizeof(Cost), "%s%2d ", Cost, Skill_Cost[Num][Count - 1]);
			Format(Level, sizeof(Level), "%s%2d ", Level, Count);
		}
	}
	Format(Item, sizeof(Item), "【消耗】 %s", Cost);
	AddMenuItem(MenuHandle, "", Item, ITEMDRAW_DISABLED);
	Format(Item, sizeof(Item), "【等级】 %s", Level);
	AddMenuItem(MenuHandle, "", Item, ITEMDRAW_DISABLED);
	AddMenuItem(MenuHandle, "", "确认升级");
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Skill_Menu_Item_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		int Num = Skill_Select[Client];
		Skill_Update(Client, Num);
		Skill_Item_Menu(Client, Num);
	}
	else if (ActionHandle == MenuAction_Cancel)
	{
		Skill_Menu(Client);
	}
}
void Skill_Update(int Client, int Num)
{
	if (Skill_Client[Client][Num] == Skill_MaxLv[Num])
	{
		CPrintToChat(Client, "{green}[ZA][技能] {red}技能等级已到达上限");
		return;
	}
	if (Skill_Alive[Client] < Skill_Cost[Num][Skill_Client[Client][Num] - 1])
	{
		CPrintToChat(Client, "{green}[ZA][技能] {red}技能点不足无法升级");
		return;
	}
	Skill_Alive[Client] -= Skill_Cost[Num][Skill_Client[Client][Num]];
	Skill_Client[Client][Num]++;
	char Query[192];
	Format(Query, sizeof(Query), "UPDATE user SET sk%d=sk%d+ 1 WHERE id=%d", Num + 1, Num + 1, Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	CPrintToChat(Client, "{green}[ZA][技能] {gold}%s{blue} 当前等级为 {gold}%d", Skill_Name[Num], Skill_Client[Client][Num]);
}
int Skill_MaxSP(int Client)
{
	return Client_Birth[Client] * 10 + Client_Level[Client];
}
int Skill_Count(int Client)
{
	int Count = 0;
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		for (int Skill = 0; Skill < Skill_Client[Client][Num]; Skill++)
		{
			Count += Skill_Cost[Num][Skill];
		}
	}
	return Count;
}
int Skill_1(int Client)
{
	if (!Skill_Allow[0] || Skill_Client[Client][0] == 0)
	{
		return 0;
	}
	return Skill_Client[Client][0];
}
void Skill_2_OnPlayerSpawn(int Client)
{
	ClearTimer(Skill_2_Handle[Client]);
	if (!Skill_Allow[1] || Skill_Client[Client][1] == 0)
	{
		return;
	}
	Skill_2_Handle[Client] = CreateTimer(900.0 / Skill_Client[Client][1], Skill_2_Timer, Client, TIMER_REPEAT);
}
Action Skill_2_Timer(Handle Timer, any Client)
{
	if (!Skill_Allow[1] || !CheckAlive(Client))
	{
		ClearTimer(Skill_2_Handle[Client]);
		return Plugin_Handled;
	}
	Credit_Give(Client, 1, "积分补给");
	return Plugin_Handled;
}
void Skill_2_OnPlayerDeath(int Client)
{
	if (!Skill_Allow[1])
	{
		return;
	}
	ClearTimer(Skill_2_Handle[Client]);
}
void Skill_3_OnPlayerSpawn(int Client)
{
	if (!Skill_Allow[2] || !CheckAlive(Client))
	{
		return;
	}
	if (ZR_IsClientHuman(Client))
	{
		CreateTimer(1.0, Skill_3_Human_Timer, Client, TIMER_FLAG_NO_MAPCHANGE);
		ClearTimer(Skill_3_Handle[Client]);
		Skill_3_Handle[Client] = CreateTimer(11.0 - float(Skill_Client[Client][2]), Skill_3_Timer, Client, TIMER_REPEAT);
	}
	else
	{
		CreateTimer(1.0, Skill_3_Zombie_Timer, Client, TIMER_FLAG_NO_MAPCHANGE);
	}
}
Action Skill_3_Human_Timer(Handle Timer, any Client)
{
	if (!Skill_Allow[2] || !CheckHuman(Client))
	{
		return Plugin_Handled;
	}
	AddHealth(Client, Skill_Client[Client][2] * 10);
	return Plugin_Handled;
}
Action Skill_3_Zombie_Timer(Handle Timer, any Client)
{
	if (!Skill_Allow[2] || !CheckZombie(Client))
	{
		return Plugin_Handled;
	}
	AddHealth(Client, Skill_Client[Client][2] * 200);
	return Plugin_Handled;
}
Action Skill_3_Timer(Handle Timer, any Client)
{
	if (!Skill_Allow[2] || !CheckHuman(Client))
	{
		ClearTimer(Skill_3_Handle[Client]);
		return Plugin_Handled;
	}
	int Armour = GetArmour(Client);
	if (Armour >= 100)
	{
		return Plugin_Handled;
	}
	SetArmour(Client, Armour + 1);
	return Plugin_Handled;
}
void Skill_3_OnPlayerDeath(int Client)
{
	if (!Skill_Allow[2])
	{
		return;
	}
	ClearTimer(Skill_3_Handle[Client]);
}
float Skill_3_OnTakeDamage(int Client)
{
	if (!Skill_Allow[2] || Skill_Client[Client][2] == 0)
	{
		return 1.0;
	}
	return (100 - float(Skill_Client[Client][2])) / 100;
}
void Skill_4_OnTakeDamage(int Client, int Cash)
{
	if (!Skill_Allow[3] || Skill_Client[Client][3] == 0)
	{
		return;
	}
	SetCash(Client, GetCash(Client) + RoundToCeil(Cash * (1 + float(Skill_Client[Client][3]) / 10) - Cash));
}
void Skill_4_OnClientInfect(int Client)
{
	if (!Skill_Allow[3] || Skill_Client[Client][3] == 0)
	{
		return;
	}
	SetCash(Client, GetCash(Client) + 200 * Skill_Client[Client][3]);
}
float Skill_5_OnTakeDamage(int Client)
{
	if (!Skill_Allow[4] || Skill_Client[Client][4] == 0)
	{
		return 0.0;
	}
	return 0.2 * Skill_Client[Client][4];
}
bool Skill_6_OnTakeDamage(int Client)
{
	if (!Skill_Allow[5] || Skill_Client[Client][5] == 0)
	{
		return false;
	}
	return GetRandomInt(1, 10 * (11 - Skill_Client[Client][5])) == 1;
}
int Skill_7_OnTakeDamage(int Client)
{
	if (!Skill_Allow[6] || Skill_Client[Client][6] == 0)
	{
		return 0;
	}
	return Skill_Client[Client][6] * 50;
}
bool Skill_8_OnTakeDamage(int Client)
{
	if (!Skill_Allow[7] || Skill_Client[Client][7] == 0)
	{
		return false;
	}
	return GetRandomInt(1, 10 * (11 - Skill_Client[Client][7])) == 1;
}
bool Skill_9_OnCredit(int Client)
{
	if (!Skill_Allow[8] || Skill_Client[Client][8] == 0)
	{
		return false;
	}
	return true;
}
int Skill_10_OnWeapon(int Client)
{
	if (!Skill_Allow[9] || Skill_Client[Client][9] == 0)
	{
		return 0;
	}
	return Skill_Client[Client][9];
}
void Skill_11_OnPlayerSpawn(int Client)
{
	if (!Skill_Allow[10] || Skill_Client[Client][10] == 0)
	{
		return;
	}
	SetCash(Client, GetCash(Client) + 400 * Skill_Client[Client][10]);
}
float Skill_11_OnWeapon(int Client)
{
	if (!Skill_Allow[10] || Skill_Client[Client][10] == 0)
	{
		return 1.0;
	}
	return float(100 - Skill_Client[Client][10]) / 100;
}
void Skill_12_OnClientInfect(int Client)
{
	if (!Skill_Allow[11] || Skill_Client[Client][11] == 0)
	{
		return;
	}
	SetSpeed(Client, GetSpeed(Client) * 1.01);
}
int Skill_13_OnWeapon(int Client)
{
	if (!Skill_Allow[12] || Skill_Client[Client][12] == 0)
	{
		return 0;
	}
	return Skill_Client[Client][12];
}
float Skill_14_OnDamage(int Client)
{
	if (!Skill_Allow[13] || Skill_Client[Client][13] == 0)
	{
		return 1.0;
	}
	if (HumanCount() > 3)
	{
		return 1.0;
	}
	return 1.0 + float(Skill_Client[Client][13]) / 10;
}
bool Skill_15_OnWeapon(int Client)
{
	if (!Skill_Allow[14] || Skill_Client[Client][14] == 0)
	{
		return false;
	}
	return true;
}
bool Skill_16_OnDamage(int Client)
{
	if (!Skill_Allow[15] || Skill_Client[Client][15] == 0)
	{
		return false;
	}
	return true;
}
Action Skill_16_Timer(Handle Timer, any Client)
{
	Clr_Damage_Knife(Client);
	if (!CheckZombie(Client))
	{
		return Plugin_Handled;
	}
	float ClientPos[3];
	float ModelPos[3];
	ModelPos[2] = 1.0;
	GetClientAbsOrigin(Client, ClientPos);
	TE_SetupExplosion(ClientPos, PrecacheModel("sprites/blueglow2.vmt"), 5.0, 1, 0, 50, 40, ModelPos);
	TE_SendToAll();
	TE_SetupSmoke(ClientPos, PrecacheModel("sprites/steam1.vmt"), 10.0, 3);
	TE_SendToAll();
	EmitAmbientSound("ambient/explosions/explode_8.wav", ClientPos, Client, _, _, 1.0);
	ForcePlayerSuicide(Client);
	return Plugin_Handled;
}
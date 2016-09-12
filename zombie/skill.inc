int Skill_Client[MAXPLAYERS + 1][12];
int Skill_Alive[MAXPLAYERS + 1];
char Skill_Team[12][8];
char Skill_Name[12][32];
char Skill_Info[12][64];
int Skill_Cost[12];
int Skill_MaxLv[12];
int Skill_Max = 0;

Handle Skill_2_Handle[MAXPLAYERS + 1];
Handle Skill_3_Handle[MAXPLAYERS + 1];
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
		Skill_Cost[Count] = SQL_FetchInt(Child, 4);
		Skill_MaxLv[Count] = SQL_FetchInt(Child, 5);
		LogMessage("Skill %d\tTeam %s\tName %s\tInfo %s\tCost %d\tCount %d", Count, Skill_Team[Count], Skill_Name[Count], Skill_Info[Count], Skill_Cost[Count], Skill_MaxLv[Count]);
		Count++;
	}
	Skill_Max = Count;
}
void Skill_Init(int Client, int[] Skills)
{
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Skill_Client[Client][Num] = Skills[Num];
	}
	int Count = Skill_Count(Client);
	int MaxSp = Skill_MaxSP(Client);
	if (Count > MaxSp)
	{
		Skill_Empty(Client);
		Count = 0;
	}
	Skill_Alive[Client] = MaxSp - Count;
}
void Skill_Empty(int Client)
{
	char Query[192];
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Skill_Client[Client][Num] = 0;
	}
	Format(Query, sizeof(Query), "UPDATE user SET sk1=0,sk2=0,sk3=0,sk4=0,sk5=0,sk6=0,sk7=0,sk8=0,sk9=0,sk10=0,sk11=0,sk12=0 WHERE id=%d", Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	CPrintToChat(Client, "{green}[ZA][技能]{blue} 你的技能点已被重置");
}
void Skill_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Skill_Menu_Handle);
	SetMenuTitle(MenuHandle, "技能菜单\n技能点 %d", Skill_Alive[Client]);
	char Item[192];
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Format(Item, sizeof(Item), "[%s]%s[%d/%d]\n[消耗]%d [介绍]%s", Skill_Team[Num], Skill_Name[Num], Skill_Client[Client][Num], Skill_MaxLv[Num], Skill_Cost[Num], Skill_Info[Num]);
		AddMenuItem(MenuHandle, "", Item);
	}
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Skill_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		Skill_Update(Client, Item);
	}
}
void Skill_Update(int Client, int Num)
{
	if (Skill_Alive[Client] < Skill_Cost[Num])
	{
		CPrintToChat(Client, "{green}[ZA][技能]{red} 技能点不足无法升级");
		return;
	}
	if (Skill_Client[Client][Num] == Skill_MaxLv[Num])
	{
		CPrintToChat(Client, "{green}[ZA][技能]{red} 技能等级已到达上限");
		return;
	}
	Skill_Alive[Client]--;
	Skill_Client[Client][Num]++;
	char Query[192];
	Format(Query, sizeof(Query), "UPDATE user SET sk%d=sk%d+1 WHERE id=%d", Num, Num, Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	CPrintToChat(Client, "{green}[ZA][技能]{blue} 当前技能等级为 {gold}%d", Skill_Client[Client][Num]);
}
int Skill_MaxSP(int Client)
{
	return Client_Birth[Client] * 12 + Client_Level[Client];
}
int Skill_Count(int Client)
{
	int Count = 0;
	for (int Num = 0; Num < Skill_Max; Num++)
	{
		Count += Skill_Client[Client][Num];
	}
	return Count;
}
int Skill_1(int Client)
{
	if (Skill_Client[Client][0] == 0)
	{
		return 0;
	}
	return Skill_Client[Client][0];
}
void Skill_2_OnPlayerSpawn(int Client)
{
	ClearTimer(Skill_2_Handle[Client]);
	if (Skill_Client[Client][1] == 0)
	{
		return;
	}
	Skill_2_Handle[Client] = CreateTimer(600.0 / Skill_Client[Client][1], Skill_2_Timer, Client, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}
Action Skill_2_Timer(Handle Timer, any Client)
{
	if (CheckAlive(Client))
	{
		Credit_Give(Client, 1, "积分补给");
		return Plugin_Handled;
	}
	ClearTimer(Skill_2_Handle[Client]);
	return Plugin_Handled;
}
void Skill_2_OnPlayerDeath(int Client)
{
	ClearTimer(Skill_2_Handle[Client]);
}
void Skill_3_OnPlayerSpawn(int Client)
{
	if (ZR_IsClientHuman(Client))
	{
		ClearTimer(Skill_3_Handle[Client]);
		AddHealth(Client, Skill_Client[Client][2] * 10);
		Skill_3_Handle[Client] = CreateTimer(11.0 - float(Skill_Client[Client][2]), Skill_3_Timer, Client, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
	}
	else
	{
		AddHealth(Client, Skill_Client[Client][2] * 100);
	}
}
Action Skill_3_Timer(Handle Timer, any Client)
{
	if (CheckHuman(Client))
	{
		if (GetClientHealth(Client) < 100 + Skill_Client[Client][3] * 10)
		{
			AddHealth(Client, 1);
		}
		return Plugin_Handled;
	}
	ClearTimer(Skill_3_Handle[Client]);
	return Plugin_Handled;
}
void Skill_3_OnPlayerDeath(int Client)
{
	ClearTimer(Skill_3_Handle[Client]);
}
float Skill_3_OnTakeDamage(int Client)
{
	if (Skill_Client[Client][2] == 0)
	{
		return 1.0;
	}
	return (100 - float(Skill_Client[Client][2])) / 100;
}
void Skill_4_OnTakeDamage(int Client, int Cash)
{
	if (Skill_Client[Client][3] == 0)
	{
		return;
	}
	SetCash(Client, GetCash(Client) + RoundToCeil(Cash * (1 + float(Skill_Client[Client][3]) / 10) - Cash));
}
void Skill_4_OnClientInfect(int Client)
{
	if (Skill_Client[Client][3] == 0)
	{
		return;
	}
	SetCash(Client, GetCash(Client) + 200 * Skill_Client[Client][3]);
}
float Skill_5_OnTakeDamage(int Client)
{
	if (Skill_Client[Client][4] == 0)
	{
		return 0.0;
	}
	return 0.2 * Skill_Client[Client][4];
}
bool Skill_6_OnTakeDamage(int Client)
{
	if (Skill_Client[Client][5] == 0)
	{
		return false;
	}
	return GetRandomInt(1, 10 * (11 - Skill_Client[Client][5])) == 1;
}
int Skill_7_OnTakeDamage(int Client)
{
	if (Skill_Client[Client][6] == 0)
	{
		return 0;
	}
	return Skill_Client[Client][6] * 50;
}
bool Skill_8_OnTakeDamage(int Client)
{
	if (Skill_Client[Client][7] == 0)
	{
		return false;
	}
	return GetRandomInt(1, 10 * (11 - Skill_Client[Client][7])) == 1;
}
bool Skill_9_OnCredit(int Client)
{
	if (Skill_Client[Client][8] == 0)
	{
		return false;
	}
	return true;
}
bool Skill_10_OnWeapon(int Client)
{
	if (Skill_Client[Client][9] == 0)
	{
		return false;
	}
	return true;
}
void Skill_11_OnPlayerSpawn(int Client)
{
	if (Skill_Client[Client][10] == 0)
	{
		return;
	}
	SetCash(Client, GetCash(Client) + 400 * Skill_Client[Client][10]);
}
float Skill_11_OnWeapon(int Client)
{
	if (Skill_Client[Client][10] == 0)
	{
		return 1.0;
	}
	return float(100 - Skill_Client[Client][10]) / 100;
}
void Skill_12_OnClientInfect(int Client)
{
	SetEntPropFloat(Client, Prop_Data, "m_flLaggedMovementValue", 1.01);
}
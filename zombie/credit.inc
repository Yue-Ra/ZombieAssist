int Credit_Client[MAXPLAYERS + 1];

int Credit_Human_Max = 0;
int Credit_Zombie_Max = 0;

char Credit_Human_Name[6][16];
char Credit_Human_Info[6][64];
char Credit_Human_Alias[6][16];
int Credit_Human_Cost[6];
int Credit_Human_Cool[6];
int Credit_Human_Time[MAXPLAYERS + 1][6];

char Credit_Zombie_Name[6][16];
char Credit_Zombie_Info[6][64];
char Credit_Zombie_Alias[6][16];
int Credit_Zombie_Cost[6];
int Credit_Zombie_Cool[6];
int Credit_Zombie_Time[MAXPLAYERS + 1][6];

bool Credit_Human_Allow[6];
bool Credit_Zombie_Allow[6];

Handle Credit_Human_6_Handle[MAXPLAYERS + 1];

void Credit_Human_Allow_Set(int Credits[6])
{
	for (int Num = 0; Num < sizeof(Credits); Num++)
	{
		Credit_Human_Allow[Num] = Credits[Num] == 0 ? false : true;
	}
}
void Credit_Zombie_Allow_Set(int Credits[6])
{
	for (int Num = 0; Num < sizeof(Credits); Num++)
	{
		Credit_Zombie_Allow[Num] = Credits[Num] == 0 ? false : true;
	}
}
void Credit_Give(int Client, int Value, const char[] Reason, bool ChatAll = false)
{
	if (!ChatAll)
	{
		CPrintToChat(Client, "{green}[ZA][积分]{blue} 你通过 {gold}%s{blue} 获得了 {gold}%d{blue} 积分", Reason, Value);
	}
	else
	{
		CPrintToChatAll("{green}[ZA][积分]{blue} 玩家 {gold}%N{blue} 通过 {gold}%s{blue} 获得了 {gold}%d{blue} 积分", Client, Reason, Value);
	}
	Credit_Client[Client] += Value;
	int Skill = Skill_1(Client);
	if (Credit_Client[Client] > 20 + Skill)
	{
		Credit_Client[Client] = 20 + Skill;
	}
}
bool Credit_Down(int Client, int Value)
{
	if (Credit_Client[Client] < Value)
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 积分不足,还需要 {gold}%d", Value - Credit_Client[Client]);
		return false;
	}
	Credit_Client[Client] -= Value;
	return true;
}
void Credit_Load()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM credit WHERE type=0 ORDER BY id ASC");
	LogMessage(Query);
	SQL_TQuery(MySQL, Credit_Load_Query, Query, CS_TEAM_CT);
	Format(Query, sizeof(Query), "SELECT * FROM credit WHERE type=1 ORDER BY id ASC");
	LogMessage(Query);
	SQL_TQuery(MySQL, Credit_Load_Query, Query, CS_TEAM_T);
}
void Credit_Load_Query(Handle Owner, Handle Child, const char[] Error, any Team)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Credit Handle Failure");
		return;
	}
	int Num = 0;
	while (SQL_FetchRow(Child))
	{
		if (Team == CS_TEAM_CT)
		{
			SQL_FetchString(Child, 2, Credit_Human_Name[Num], sizeof(Credit_Human_Name[]));
			SQL_FetchString(Child, 3, Credit_Human_Info[Num], sizeof(Credit_Human_Info[]));
			SQL_FetchString(Child, 4, Credit_Human_Alias[Num], sizeof(Credit_Human_Alias[]));
			Credit_Human_Cost[Num] = SQL_FetchInt(Child, 5);
			Credit_Human_Cool[Num] = SQL_FetchInt(Child, 6);
			LogMessage("Credit Human %d\tName %s\tInfo %s\tAlias %s\tCost %d\tCool %d", Num, Credit_Human_Name[Num], Credit_Human_Info[Num], Credit_Human_Alias[Num], Credit_Human_Cost[Num], Credit_Human_Cool[Num]);
		}
		else
		{
			SQL_FetchString(Child, 2, Credit_Zombie_Name[Num], sizeof(Credit_Zombie_Name[]));
			SQL_FetchString(Child, 3, Credit_Zombie_Info[Num], sizeof(Credit_Zombie_Info[]));
			SQL_FetchString(Child, 4, Credit_Zombie_Alias[Num], sizeof(Credit_Zombie_Alias[]));
			Credit_Zombie_Cost[Num] = SQL_FetchInt(Child, 5);
			Credit_Zombie_Cool[Num] = SQL_FetchInt(Child, 6);
			LogMessage("Credit Zombie %d\tName %s\tInfo %s\tAlias %s\tCost %d\tCool %d", Num, Credit_Zombie_Name[Num], Credit_Zombie_Info[Num], Credit_Zombie_Alias[Num], Credit_Zombie_Cost[Num], Credit_Zombie_Cool[Num]);
		}
		Num++;
	}
	if (Team == CS_TEAM_CT)
	{
		Credit_Human_Max = Num;
	}
	else
	{
		Credit_Zombie_Max = Num;
	}
}
Action Credit_Reload_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Credit_Load();
	}
	return Plugin_Handled;
}
void Credit_OnMapStart()
{
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Credit_Client[Client] = 0;
		for (int Item = 0; Item < 6; Item++)
		{
			Credit_Human_Time[Client][Item] = 0;
			Credit_Zombie_Time[Client][Item] = 0;
		}
	}
}
void Credit_OnClientPutInServer(int Client)
{
	Credit_Client[Client] = 0;
	for (int Item = 0; Item < 6; Item++)
	{
		Credit_Human_Time[Client][Item] = 0;
		Credit_Zombie_Time[Client][Item] = 0;
	}
}
void Credit_OnRoundEnd(int Team)
{
	if (Team == CS_TEAM_CT)
	{
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (CheckAlive(Client))
			{
				Credit_Give(Client, 3, "存活人类");
			}
		}
	}
	else
	{
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (CheckGame(Client) && GetClientTeam(Client) > CS_TEAM_SPECTATOR)
			{
				Credit_Give(Client, 1, "本局游戏");
			}
		}
	}
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		for (int Item = 0; Item < 6; Item++)
		{
			Credit_Human_Time[Client][Item] = 0;
			Credit_Zombie_Time[Client][Item] = 0;
		}
	}
}
Action Credit_Command(int Client, int Args)
{
	if (CheckAlive(Client))
	{
		if (Args == 0)
		{
			if (CheckHuman(Client))
			{
				Credit_Human_Menu(Client);
			}
			else
			{
				Credit_Zombie_Menu(Client);
			}
			return Plugin_Handled;
		}
		char Alias[32];
		GetCmdArgString(Alias, sizeof(Alias));
		int Time = GetTime();
		if (CheckHuman(Client))
		{
			for (int Item = 0; Item < 6; Item++)
			{
				if (StrContains(Alias, Credit_Human_Alias[Item], false) != -1)
				{
					if (Credit_Human_Time[Client][Item] > Time)
					{
						CPrintToChat(Client, "{green}[ZA][积分]{red} %s 冷却时间还有 {gold}%d{red} 秒", Credit_Human_Name[Item], Credit_Human_Time[Client][Item] - Time);
						return Plugin_Handled;
					}
					switch (Item)
					{
					case 0: Credit_Human_1(Client);
					case 1: Credit_Human_2(Client);
					case 2: Credit_Human_3(Client);
					case 3: Credit_Human_4(Client);
					case 4: Credit_Human_5(Client);
					case 5: Credit_Human_6(Client);
					}
					return Plugin_Handled;
				}
			}
		}
		else
		{
			for (int Item = 0; Item < 6; Item++)
			{
				if (StrContains(Alias, Credit_Zombie_Alias[Item], false))
				{
					if (Credit_Zombie_Time[Client][Item] > Time)
					{
						CPrintToChat(Client, "{green}[ZA][积分]{red} %s 冷却时间还有 {gold}%d{red} 秒", Credit_Zombie_Name[Item], Credit_Zombie_Time[Client][Item] - Time);
						return Plugin_Handled;
					}
					switch (Item)
					{
					case 0: Credit_Zombie_1(Client);
					case 1: Credit_Zombie_2(Client);
					case 2: Credit_Zombie_3(Client);
					case 3: Credit_Zombie_4(Client);
					case 4: Credit_Zombie_5(Client);
					}
					return Plugin_Handled;
				}
			}
		}
	}
	return Plugin_Handled;
}
void Credit_Human_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Credit_Human_Menu_Handle);
	SetMenuTitle(MenuHandle, "[人类]积分菜单\n当前积分 %d", Credit_Client[Client]);
	for (int Num = 0; Num < Credit_Human_Max; Num++)
	{
		char Line[192];
		Format(Line, sizeof(Line), "[人类]%s - %d积分\n[快捷]%s - %s", Credit_Human_Name[Num], Credit_Human_Cost[Num], Credit_Human_Alias[Num], Credit_Human_Info[Num]);
		AddMenuItem(MenuHandle, "", Line);
	}
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
void Credit_Zombie_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Credit_Zombie_Menu_Handle);
	SetMenuTitle(MenuHandle, "[僵尸]积分菜单\n当前积分 %d", Credit_Client[Client]);
	for (int Num = 0; Num < Credit_Zombie_Max; Num++)
	{
		char Line[192];
		Format(Line, sizeof(Line), "[僵尸]%s - %d积分\n[快捷]%s - %s", Credit_Zombie_Name[Num], Credit_Zombie_Cost[Num], Credit_Zombie_Alias[Num] , Credit_Zombie_Info[Num]);
		AddMenuItem(MenuHandle, "", Line);
	}
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Credit_Human_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		Credit_Human_Buy(Client, Item);
	}
}
int Credit_Zombie_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		Creidt_Zombie_Buy(Client, Item);
	}
}
void Credit_Human_Buy(int Client, int Item)
{
	if (!IsPlayerAlive(Client))
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 没活还想玩积分?");
		return;
	}
	if (ZR_IsClientZombie(Client))
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 僵尸还想玩人类积分?");
		return;
	}
	if (!Credit_Human_Allow[Item])
	{
		CPrintToChat(Client, "{green}[ZA][积分]{gold} %s {red}已被禁用", Credit_Human_Name[Item]);
		return;
	}
	int Time = GetTime();
	if (Credit_Human_Time[Client][Item] > Time)
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} %s 冷却时间还有 {gold}%d{red} 秒", Credit_Human_Name[Item], Credit_Human_Time[Client][Item] - Time);
		return;
	}
	switch (Item)
	{
	case 0: Credit_Human_1(Client);
	case 1: Credit_Human_2(Client);
	case 2: Credit_Human_3(Client);
	case 3: Credit_Human_4(Client);
	case 4: Credit_Human_5(Client);
	case 5: Credit_Human_6(Client);
	}
}
void Creidt_Zombie_Buy(int Client, int Item)
{
	if (!IsPlayerAlive(Client))
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 没活还想玩积分?");
		return;
	}
	if (ZR_IsClientHuman(Client))
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 人类还想玩僵尸积分?");
		return;
	}
	if (!Credit_Zombie_Allow[Item])
	{
		CPrintToChat(Client, "{green}[ZA][积分]{gold} %s {red}已被禁用", Credit_Zombie_Name[Item]);
		return;
	}
	int Time = GetTime();
	if (Credit_Zombie_Time[Client][Item] > Time)
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} %s 冷却时间还有 {gold}%d{red} 秒", Credit_Zombie_Name[Item], Credit_Zombie_Time[Client][Item] - Time);
		return;
	}
	switch (Item)
	{
	case 0: Credit_Zombie_1(Client);
	case 1: Credit_Zombie_2(Client);
	case 2: Credit_Zombie_3(Client);
	case 3: Credit_Zombie_4(Client);
	case 4: Credit_Zombie_5(Client);
	}
}
void Credit_Human_1(int Client)
{
	if (Skill_10_OnWeapon(Client) < 2)
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 请先学习 {gold}%s{red} 技能", Skill_Name[9]);
		return;
	}
	if (!Credit_Down(Client, Credit_Human_Cost[0]))
	{
		return;
	}
	Credit_Human_Time[Client][0] = GetTime() + Credit_Human_Cool[0];
	GivePlayerItem(Client, "weapon_smokegrenade");
}
void Credit_Human_2(int Client)
{
	if (!Credit_Down(Client, Credit_Human_Cost[1]))
	{
		return;
	}
	Credit_Human_Time[Client][1] = GetTime() + Credit_Human_Cool[1];
	AddHealth(Client, 20);
}
void Credit_Human_3(int Client)
{
	if (!Credit_Down(Client, Credit_Human_Cost[2]))
	{
		return;
	}
	Credit_Human_Time[Client][2] = GetTime() + Credit_Human_Cool[2];
	Weapon_Unlimited(Client);
}
void Credit_Human_4(int Client)
{
	if (!Skill_9_OnCredit(Client))
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 请先学习 {gold}%s{red} 技能", Skill_Name[8]);
		return;
	}
	if (!Credit_Down(Client, Credit_Human_Cost[3]))
	{
		return;
	}
	Credit_Human_Time[Client][3] = GetTime() + Credit_Human_Cool[3];
	Infect_NoMother(Client);
}
void Credit_Human_5(int Client)
{
	if (Skill_10_OnWeapon(Client) < 3)
	{
		CPrintToChat(Client, "{green}[ZA][积分]{red} 请先学习 {gold}%s{red} 技能", Skill_Name[9]);
		return;
	}
	if (!Credit_Down(Client, Credit_Human_Cost[4]))
	{
		return;
	}
	Credit_Human_Time[Client][4] = GetTime() + Credit_Human_Cool[4];
	Infect_NoInfect(Client);
}
void Credit_Human_6(int Client)
{
	if (!Credit_Down(Client, Credit_Human_Cost[5]))
	{
		return;
	}
	ClearTimer(Credit_Human_6_Handle[Client]);
	Credit_Human_6_Handle[Client] = CreateTimer(0.1, Credit_Human_6_Timer, Client, TIMER_REPEAT);
	CreateTimer(5.0, Credit_Human_6_Close, Client);
}
Action Credit_Human_6_Timer(Handle Timer, any Client)
{
	if (CheckHuman(Client))
	{
		SetSpeed(Client, float(277) / float(250));
		return Plugin_Handled;
	}
	ClearTimer(Credit_Human_6_Handle[Client]);
	return Plugin_Handled;
}
Action Credit_Human_6_Close(Handle Timer, any Client)
{
	ClearTimer(Credit_Human_6_Handle[Client]);
	return Plugin_Handled;
}
void Credit_Zombie_1(int Client)
{
	if (!Credit_Down(Client, Credit_Zombie_Cost[0]))
	{
		return;
	}
	Credit_Zombie_Time[Client][0] = GetTime() + Credit_Zombie_Cool[0];
	AddHealth(Client, 2000);
}
void Credit_Zombie_2(int Client)
{
	if (!Credit_Down(Client, Credit_Zombie_Cost[1]))
	{
		return;
	}
	Credit_Zombie_Time[Client][1] = GetTime() + Credit_Zombie_Cool[1];
	Damage_Zombie_Credit(Client);
}
void Credit_Zombie_3(int Client)
{
	if (!Credit_Down(Client, Credit_Zombie_Cost[2]))
	{
		return;
	}
	Credit_Zombie_Time[Client][2] = GetTime() + Credit_Zombie_Cool[2];
	Smoke_Client(Client);
}
void Credit_Zombie_4(int Client)
{
	if (!Credit_Down(Client, Credit_Zombie_Cost[3]))
	{
		return;
	}
	Credit_Zombie_Time[Client][3] = GetTime() + Credit_Zombie_Cool[3];
	Infect_Respawn(Client);
}
void Credit_Zombie_5(int Client)
{
	if (!Credit_Down(Client, Credit_Zombie_Cost[4]))
	{
		return;
	}
	Credit_Zombie_Time[Client][4] = GetTime() + Credit_Zombie_Cool[4];
	Alpha_Zombie_Credit(Client);
}
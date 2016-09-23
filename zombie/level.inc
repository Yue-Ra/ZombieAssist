Handle Client_Point_Handle[MAXPLAYERS + 1];
Action Level_Timer(Handle Timer)
{
	char Msg[192];
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckAlive(Client))
		{
			Format(Msg, sizeof(Msg), "积分 %d\n等级 %d\n经验 %d/%d", Credit_Client[Client], Client_Level[Client], Client_Point[Client], Client_LevelUp[Client]);
			SendHudTextMsg(Client, 0.9, 0.7, 1.0, 255, 200, 0, Msg);
		}
	}
	return Plugin_Handled;
}
void Client_Point_Give(int Client, int Value, const char[] Reason, bool ChatAll = false)
{
	if (!ChatAll)
	{
		CPrintToChat(Client, "{green}[ZA][等级] {blue}你通过 {gold}%s{blue} 获得了 {gold}%d{blue} 经验", Reason, Value);
	}
	else
	{
		CPrintToChatAll("{green}[ZA][等级] {blue}玩家 {gold}%N{blue} 通过 {gold}%s{blue} 获得了 {gold}%d{blue} 经验", Client, Reason, Value);
	}
	Client_Point[Client] += Value;
	if (Client_Level[Client] == 50)
	{
		if (Client_Point[Client] > Client_LevelUp[Client])
		{
			Client_Point[Client] = Client_LevelUp[Client];
		}
		return;
	}
	char Query[192];
	if (Client_Point[Client] < Client_LevelUp[Client])
	{
		Format(Query, sizeof(Query), "UPDATE user SET point=point+%d WHERE id=%d", Value, Client_ID[Client]);
		SQL_TQuery(MySQL, MySQL_Query, Query);
		return;
	}
	SetOverlays(Client, "overlays/ub/levelup", 5.0);
	if (!IsSoundPrecached("ub/levelup.mp3"))
	{
		PrecacheSound("ub/levelup.mp3");
	}
	EmitSoundToAll("ub/levelup.mp3", Client, _, _, _, 1.0, _, _, _, _, _, _);
	Client_Point[Client] = 0;
	Client_Level[Client] += 1;
	Format(Query, sizeof(Query), "UPDATE user SET level=level+1,point=0 WHERE id=%d", Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	Client_LevelUp[Client] = Client_LevelUp_Count(Client);
	Skill_OnLevelUp(Client);
	CPrintToChat(Client, "{green}[ZA][等级] {blue}当前等级 {gold}%d{blue},下一级需要 {gold}%d{blue}点经验", Client_Level[Client], Client_LevelUp_Count(Client));
	CPrintToChatAll("{green}[ZA][等级] {blue}玩家 {gold}%N{blue} 升级了,当前等级为 {gold}%d", Client, Client_Level[Client]);
}
void Client_Point_OnClientPutInServer(int Client)
{
	ClearTimer(Client_Point_Handle[Client]);
}
void Client_Point_OnPlayerSpawn(int Client)
{
	ClearTimer(Client_Point_Handle[Client]);
	Client_Point_Handle[Client] = CreateTimer(60.0, Client_Point_Timer, Client, TIMER_REPEAT);
}
void Client_Point_OnPlayerDeath(int Client)
{
	ClearTimer(Client_Point_Handle[Client]);
}
Action Client_Point_Timer(Handle Timer, any Client)
{
	if (!CheckHuman(Client))
	{
		ClearTimer(Client_Point_Handle[Client]);
		return Plugin_Handled;
	}
	Client_Point_Give(Client, 5, "存活人类");
	return Plugin_Handled;
}
int Client_LevelUp_Count(int Client)
{
	if (Client_Level[Client] > 40)
	{
		return 9600 + (Client_Level[Client] - 41) * 500;
	}
	else if (Client_Level[Client] > 30)
	{
		return 6000 + (Client_Level[Client] - 31) * 400;
	}
	else if (Client_Level[Client] > 20)
	{
		return 3300 + (Client_Level[Client] - 21) * 300;
	}
	else if (Client_Level[Client] > 10)
	{
		return 1500 + (Client_Level[Client] - 11) * 200;
	}
	else
	{
		return 600 + (Client_Level[Client] - 1) * 100;
	}
}
void Level_OnRoundEnd(int Team)
{
	if (Team == CS_TEAM_CT)
	{
		int Point = Get_WinPoint();
		CPrintToChatAll("{green}[ZA][等级] {blue}存活人类获得 {gold}%d{blue} 经验", Point);
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (CheckHuman(Client))
			{
				Client_Point_Give(Client, Point, "人类胜利");
			}
			else if (CheckGame(Client) && GetClientTeam(Client) > CS_TEAM_SPECTATOR)
			{
				Client_Point_Give(Client, Point / 3, "本局游戏");
			}
		}
	}
	else
	{
		int Point = Get_LosePoint();
		CPrintToChatAll("{green}[ZA][等级] {blue}在场玩家获得 {gold}%d{blue} 经验", Point);
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (CheckGame(Client) && GetClientTeam(Client) > CS_TEAM_SPECTATOR)
			{
				Client_Point_Give(Client, Point, "本局游戏");
			}
		}
	}
}
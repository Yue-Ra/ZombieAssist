int Level_Client[MAXPLAYERS + 1];
int Level_Points[MAXPLAYERS + 1];
int Level_Points_Client[MAXPLAYERS + 1];

Action Level_Timer(Handle Timer)
{
	char Msg[192];
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckAlive(Client))
		{
			Format(Msg, sizeof(Msg), "积分 %d\n等级 %d\n经验 %d/%d", Credit_Client[Client], Level_Client[Client], Level_Points[Client], Level_Points_Client[Client]);
			SendHudTextMsg(Client, 0.9, 0.7, 1.0, 255, 200, 0, Msg);
		}
	}
}
void Level_Init(int Client, int Value, int Points)
{
	Level_Client[Client] = Value;
	Level_Points[Client] = Points;
	Level_Points_Client[Client] = Level_Points_Need(Client);
}
void Level_Points_Give(int Client, int Value)
{
	Level_Points[Client] += Value;
	if (Level_Client[Client] == 50)
	{
		if (Level_Points[Client] > Level_Points_Client[Client])
		{
			Level_Points[Client] = Level_Points_Client[Client];
		}
		return;
	}
	if (Level_Points[Client] < Level_Points_Client[Client])
	{
		return;
	}
	Level_Points[Client] = 0;
	Level_Client[Client] += 1;
	Level_Points_Client[Client] = Level_Points_Need(Client);
	CPrintToChat(Client, "{green}[ZA][等级]{blue} 当前等级 {gold}%d{blue},下一级需要 {gold}%d{blue} 点经验", Level_Client[Client], Level_Points_Need(Client));
}
int Level_Points_Need(int Client)
{
	if (Level_Client[Client] > 40)
	{
		return 9600 + (Level_Client[Client] - 41) * 500;
	}
	else if (Level_Client[Client] > 30)
	{
		return 6000 + (Level_Client[Client] - 31) * 400;
	}
	else if (Level_Client[Client] > 20)
	{
		return 3300 + (Level_Client[Client] - 21) * 300;
	}
	else if (Level_Client[Client] > 10)
	{
		return 1500 + (Level_Client[Client] - 11) * 200;
	}
	else
	{
		return 600 + (Level_Client[Client] - 1) * 100;
	}
}
void Level_OnRoundEnd(int Team)
{
	int Value = Team == CS_TEAM_CT ? Map_Win_Points : Map_Lose_Points;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (GetClientTeam(Client) == CS_TEAM_CT || GetClientTeam == CS_TEAM_TT)
		{
			Level_Points_Give(Client, Value);
		}
	}
}
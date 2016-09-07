bool Alpha_Client[MAXPLAYERS + 1];
int Alpha_Zombie[MAXPLAYERS + 1];

bool Alpha_Allow;

Action Alpha_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Alpha_Client[Client] = !Alpha_Client[Client];
		CPrintToChat(Client, "{green}[ZA][透明]{blue} 当前模型透明为 %s", Alpha_Client[Client] ? "开启" : "关闭");
	}
	return Plugin_Handled;
}
void Alpha_OnMapStart()
{
	Alpha_Allow = true;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Alpha_Client[Client] = true;
		Alpha_Zombie[Client] = 0;
	}
}
void Alpha_OnClientPutInServer(int Client)
{
	Alpha_Client[Client] = true;
	Alpha_Zombie[Client] = 0;
}
void Alpha_OnRoundEnd()
{
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Alpha_Zombie[Client] = 0;
	}
}
void Alpha_OnGameFrame()
{
	int Time = GetTime();
	if (!Alpha_Allow)
	{
		return;
	}
	if (GetTeamClientCount(CS_TEAM_CT) + GetTeamClientCount(CS_TEAM_T) < 2)
	{
		return;
	}
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (!Alpha_Client[Client])
		{
			continue;
		}
		if (CheckHuman(Client))
		{
			int Distance = GetCloseClientDistance(Client);
			if (Distance < 100)
			{
				Distance = 100;
			}
			SetAlpha(Client, Distance);
		}
		else if (CheckZombie(Client))
		{
			if (Alpha_Zombie[Client] >= Time)
			{
				SetAlpha(Client, 75);
			}
		}
	}
}
int GetCloseClientDistance(int Client)
{
	int	Last = 255;
	float ClientVec[3];
	float TargetVec[3];
	GetClientAbsOrigin(Client, ClientVec);
	for (int Target = 1; Target <= MaxClients; Target++)
	{
		if (Client == Target)
		{
			continue;
		}
		if (CheckHuman(Target))
		{
			GetClientAbsOrigin(Target, TargetVec);
			int Temp = RoundToCeil(GetVectorDistance(ClientVec, TargetVec, false));
			if (Temp < Last)
			{
				Last = Temp;
			}
		}
	}
	return Last;
}
void Alpha_Zombie_Credit(int Client)
{
	Alpha_Zombie[Client] = GetTime() + 4;
}
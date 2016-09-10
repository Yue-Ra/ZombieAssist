bool Infect_Human_NoInfect[MAXPLAYERS + 1];
bool Infect_Human_NoMother[MAXPLAYERS + 1];
bool Infect_Zombie_Respawn[MAXPLAYERS + 1];
int Infect_Zombie_Time[MAXPLAYERS + 1];
float Infect_Time;

void Infect_Init()
{
	Infect_Time = GetConVarFloat(FindConVar("zr_infect_spawntime_max"));
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Infect_Human_NoInfect[Client] = false;
		Infect_Human_NoMother[Client] = false;
		Infect_Zombie_Respawn[Client] = false;
		Infect_Zombie_Time[Client] = 0;
	}
}
void Infect_NoInfect(int Client)
{
	Infect_Human_NoInfect[Client] = true;
}
void Infect_NoMother(int Cilent)
{
	Infect_Human_NoMother[Cilent] = true;
}
void Infect_Respawn(int Client)
{
	Infect_Zombie_Respawn[Client] = true;
	Infect_Respawn_Credit(Client);
}
void Infect_Respawn_Check(int Client)
{
	if (!Infect_Zombie_Respawn[Client])
	{
		Infect_Zombie_Time[Client] = GetTime() + 5;
		CPrintToChat(Client, "{green}[ZA][感染]{blue} {gold}5{blue}秒内使用爆菊馒头可以变成人类");
		return;
	}
	CPrintToChatAll("{green}[ZA][感染]{blue} 僵尸 %N 拥有爆菊馒头效果,{gold}5{blue}秒后将变成人类", Client);
	CreateTimer(5.0, Infect_Respawn_Client_Timer, Client);
}
void Infect_Respawn_Credit(int Client)
{
	if (Infect_Zombie_Time[Client] == 0)
	{
		return;
	}
	if (GetTime() > Infect_Zombie_Time[Client])
	{
		CPrintToChat(Client, "{green}[ZA][感染]{red} 爆菊馒头用的太晚啦,已经无效了");
		return;
	}
	CPrintToChatAll("{green}[ZA][感染]{blue} 僵尸 %N 拥有爆菊馒头效果,{gold}5{blue}秒后将变成人类", Client);
	CreateTimer(5.0, Infect_Respawn_Client_Timer, Client);
}
Action Infect_Respawn_Client_Timer(Handle Timer, any Client)
{
	if (CheckZombie(Client))
	{
		ZR_HumanClient(Client);
	}
	return Plugin_Handled;
}
public Action ZR_OnClientInfect(&Client, &Attack, &bool: motherInfect, &bool: respawnOverride, &bool: Respawn)
{
	if (Attack <= 0 && Infect_Time >= Time_Time && Infect_Human_NoMother[Client])
	{
		CPrintToChat(Client, "{green}[ZA]{blue} 你免疫了开局尸变");
		Infect_Human_NoMother[Client] = false;
		return Plugin_Handled;
	}
	if (CheckZombie(Attack))
	{
		if (Infect_Human_NoInfect[Client])
		{
			CPrintToChat(Client, "{green}[ZA]{blue} 你免疫了一次感染");
			Infect_Human_NoInfect[Client] = false;
			return Plugin_Handled;
		}
		Credit_Give(Attack, 1, "感染人类");
	}
	return Plugin_Continue;
}
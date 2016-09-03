int Damage_Attack[MAXPLAYERS + 1];
int Damage_Infect[MAXPLAYERS + 1];
int Damage_Attack_Rank[5];
int Damage_Infect_Rank[5];

bool Damage_Allow = true;
bool Damage_Fall = true;

void Damage_Init(int Client)
{
	Damage_Attack[Client] = 0;
	Damage_Infect[Client] = 0;
}
void Damage_OnRoundStart()
{
	Damage_Show();
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Damage_Attack[Client] = 0;
		Damage_Infect[Client] = 0;
	}
}
void Damage_Show()
{
	if (GetTime() - Navbar_Time < 120 || GetTeamClientCount(CS_TEAM_CT) + GetTeamClientCount(CS_TEAM_T) < 15)
	{
		return;
	}
	int Temp = 0;
	for (int Num = 0; Num < 5; Num++)
	{
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (Damage_Attack_Rank[0] == Client || Damage_Attack_Rank[1] == Client || Damage_Attack_Rank[2] == Client || Damage_Attack_Rank[3] == Client || Damage_Attack_Rank[4] == Client)
			{
				continue;
			}
			if (Damage_Attack[Client] > Temp)
			{
				Temp = Damage_Attack[Client];
				Damage_Attack_Rank[Num] = Client;
			}
		}
		Temp = 0;
	}
	for (int Num = 0; Num < 5; Num++)
	{
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (Damage_Infect_Rank[0] == Client || Damage_Infect_Rank[1] == Client || Damage_Infect_Rank[2] == Client || Damage_Infect_Rank[3] == Client || Damage_Infect_Rank[4] == Client)
			{
				continue;
			}
			if (Damage_Infect[Client] > Temp)
			{
				Temp = Damage_Infect[Client];
				Damage_Infect_Rank[Num] = Client;
			}
		}
		Temp = 0;
	}
	CreateTimer(0.0, Damage_Attack_Timer);
	CreateTimer(3.0, Damage_Infect_Timer);
}
Action Damage_Attack_Timer(Handle Timer)
{
	char Msg[192] = "伤害排行榜";
	for (int Num = 0; Num < 5; Num++)
	{
		if (CheckGame(Damage_Attack_Rank[Num]))
		{
			Format(Msg, sizeof(Msg), "%s\n%d - %N", Msg, Num + 1, Damage_Attack_Rank[Num]);
		}
		Damage_Attack_Rank[Num] = 0;
	}
	SendHudTextMsg(0, 0.0, 0.65, 3.0, 0, 255, 0, Msg);
	return Plugin_Handled;
}
Action Damage_Infect_Timer(Handle Timer)
{
	char Msg[192] = "感染排行榜";
	for (int Num = 0; Num < 5; Num++)
	{
		if (CheckGame(Damage_Infect_Rank[Num]))
		{
			Format(Msg, sizeof(Msg), "%s\n%d - %N", Msg, Num + 1, Damage_Infect_Rank[Num]);
		}
		Damage_Infect_Rank[Num] = 0;
	}
	SendHudTextMsg(0, 0.0, 0.65, 3.0, 0, 255, 0, Msg);
	return Plugin_Handled;
}
float Damage_Improve()
{
	if (Mode_Start)
	{
		return 100.0;
	}
	if (!Damage_Allow)
	{
		return 1.0;
	}
	int Human = HumanCount();
	if (Human == 0)
	{
		return 1.0;
	}
	int Zombie = ZombieCount();
	if (Zombie == 0)
	{
		return 1.0;
	}
	if (Zombie < Human)
	{
		return 1.0;
	}
	float Value = float(Zombie) / float(Human);
	return Value < 6 ? Value : 6.0;
}
void Damage_Hook(int Client)
{
	SDKHook(Client, SDKHook_OnTakeDamage, Damage_TakeDamage);
}
void Damage_Unhook(int Client)
{
	SDKUnhook(Client, SDKHook_OnTakeDamage, Damage_TakeDamage);
}
Action Damage_TakeDamage(int Client, int &Attack, int &Inflictor, float &Damage, int &DamageType, int &Weapon, float damageForce[3], float damagePosition[3])
{
	if (DamageType == 32 && !Damage_Fall)
	{
		return Plugin_Handled;
	}
	if (DamageType == 64 && CheckZombie(Client))
	{
		IgniteEntity(Client, 5.0);
		return Plugin_Handled;
	}
	char ClassName[32];
	GetEntityClassname(Attack, ClassName, sizeof(ClassName));
	if (StrEqual(ClassName, "entityflame", false) && CheckHuman(Client))
	{
		return Plugin_Handled;
	}
	if (!CheckAlive(Attack))
	{
		return Plugin_Continue;
	}
	if (CheckHuman(Client) && CheckZombie(Attack))
	{
		if (Damage == 195)
		{
			Damage_Infect[Attack]++;
			CPrintToChatAll("{green}[ZA][伤害]{red} 人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			PrintCenterTextAll("人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			ForcePlayerSuicide(Client);
			Sound_Play();
			return Plugin_Handled;
		}
		if (Mode_True)
		{
			if (Mode_Start)
			{
				return Plugin_Handled;
			}
			char WeaponName[32];
			GetClientWeapon(Client, WeaponName, sizeof(WeaponName));
			if (StrEqual(WeaponName, "weapon_knife", false))
			{
				Damage_Infect[Attack]++;
				ForcePlayerSuicide(Client);
				return Plugin_Handled;
			}
		}
		return Plugin_Continue;
	}
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
		Damage_Attack[Attack] += Damage;
		if (Damage == 15 || Damage == 20 || Damage == 55 || Damage == 65 || Damage == 195)
		{
			CPrintToChatAll("{green}[ZA][伤害]{red} 人类 %N 刀了僵尸 %N", Attack, Client);
		}
		if (Mode_True)
		{
			if (Damage == 15 || Damage == 20)
			{
				Damage = 4000.0;
			}
			else if (Damage == 55 || Damage == 65)
			{
				Damage = 8000.0;
			}
			else if (Damage == 195)
			{
				Damage = 15000.0;
			}
		}
		Damage *= Damage_Improve();
		PrintCenterText(Attack, "[伤害]对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}
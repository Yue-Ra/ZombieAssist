int Damage_Attack[MAXPLAYERS + 1];
int Damage_Attack_Temp[MAXPLAYERS + 1];
int Damage_Infect[MAXPLAYERS + 1];
int Damage_Attack_Rank[5];
int Damage_Infect_Rank[5];
int Damage_Attack_Count[5];
int Damage_Infect_Count[5];

int Damage_Zombie[MAXPLAYERS + 1];

bool Damage_Allow = true;
bool Damage_Fall = true;

void Damage_OnMapStart()
{
	Damage_Allow = true;
	Damage_Fall = true;
}
void Damage_Init(int Client)
{
	Damage_Zombie[Client] = 0;
	Damage_Attack[Client] = 0;
	Damage_Attack_Temp[Client] = 0;
	Damage_Infect[Client] = 0;
}
void Damage_OnRoundStart()
{
	Damage_Show();
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Damage_Init(Client);
	}
}
void Damage_Attack_Up(int Client, int Value)
{
	Damage_Attack[Client] += Value;
	Damage_Attack_Temp[Client] += Value;
	if (Damage_Attack_Temp[Client] < 4000)
	{
		return;
	}
	Damage_Attack_Temp[Client] -= 4000;
	CPrintToChat(Client, "{green}[ZA][伤害]{blue} 通过 4000 伤害获得 {gold}1{blue} 积分");
	Credit_Give(Client, 1, "4000点伤害");
}
void Damage_Show()
{
	if (!CheckRound(15) || Time_Time < 120)
	{
		return;
	}
	int Temp = 0;
	for (int Num = 0; Num < 5; Num++)
	{
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (Damage_Attack[Client] < 2500)
			{
				continue;
			}
			if (Damage_Attack_Rank[0] == Client || Damage_Attack_Rank[1] == Client || Damage_Attack_Rank[2] == Client || Damage_Attack_Rank[3] == Client || Damage_Attack_Rank[4] == Client)
			{
				continue;
			}
			if (Damage_Attack[Client] > Temp)
			{
				Temp = Damage_Attack[Client];
				Damage_Attack_Rank[Num] = Client;
				Damage_Attack_Count[Num] = Damage_Attack[Client];
			}
		}
		Temp = 0;
	}
	for (int Num = 0; Num < 5; Num++)
	{
		for (int Client = 1; Client <= MaxClients; Client++)
		{
			if (Damage_Infect[Client] < 2)
			{
				continue;
			}
			if (Damage_Infect_Rank[0] == Client || Damage_Infect_Rank[1] == Client || Damage_Infect_Rank[2] == Client || Damage_Infect_Rank[3] == Client || Damage_Infect_Rank[4] == Client)
			{
				continue;
			}
			if (Damage_Infect[Client] > Temp)
			{
				Temp = Damage_Infect[Client];
				Damage_Infect_Rank[Num] = Client;
				Damage_Infect_Count[Num] = Damage_Infect[Client];
			}
		}
		Temp = 0;
	}
	CreateTimer(1.0, Damage_Attack_Timer);
	CreateTimer(4.0, Damage_Infect_Timer);
}
Action Damage_Attack_Timer(Handle Timer)
{
	char Msg[192] = "伤害排行榜";
	for (int Num = 0; Num < 5; Num++)
	{
		if (CheckGame(Damage_Attack_Rank[Num]))
		{
			int Credit = 5 - Num;
			char Rank[32];
			Format(Rank, sizeof(Rank), "伤害排行榜第 %d 名", Num + 1);
			Credit_Give(Damage_Attack_Rank[Num], Credit, Rank, true);
			Format(Msg, sizeof(Msg), "%s\n%d. %N - %d - %d积分", Msg, Num + 1, Damage_Attack_Rank[Num], Damage_Attack_Count[Num], Credit);
		}
		Damage_Attack_Rank[Num] = 0;
		Damage_Attack_Count[Num] = 0;
	}
	SendHudTextMsg(0, 0.02, 0.28, 3.0, 0, 255, 0, Msg);
	return Plugin_Handled;
}
Action Damage_Infect_Timer(Handle Timer)
{
	char Msg[192] = "感染排行榜";
	for (int Num = 0; Num < 5; Num++)
	{
		if (CheckGame(Damage_Infect_Rank[Num]))
		{
			int Credit = (6 - Num) / 2;
			char Rank[32];
			Format(Rank, sizeof(Rank), "感染排行榜第 %d 名", Num + 1);
			Credit_Give(Damage_Infect_Rank[Num], Credit, Rank, true);
			Format(Msg, sizeof(Msg), "%s\n%d. %N - %d - %d积分", Msg, Num + 1, Damage_Infect_Rank[Num], Damage_Infect_Count[Num], Credit);
		}
		Damage_Infect_Rank[Num] = 0;
		Damage_Infect_Count[Num] = 0;
	}
	SendHudTextMsg(0, 0.02, 0.28, 3.0, 0, 255, 0, Msg);
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
void Damage_OnClientDisconnect(int Client)
{
	SDKUnhook(Client, SDKHook_OnTakeDamage, Damage_TakeDamage);
}
Action Damage_TakeDamage(int Client, int &Attack, int &Inflictor, float &Damage, int &DamageType, int &Weapon, float damageForce[3], float damagePosition[3])
{
	if (DamageType == 32 && !Damage_Fall)
	{
		return Plugin_Handled;
	}
	if (DamageType == 64 && CheckZombie(Client) && Damage < 100)
	{
		ExtinguishEntity(Client);
		IgniteEntity(Client, 5.0);
		PrintCenterText(Attack, "[伤害]对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		Damage_Attack_Up(Attack, RoundToCeil(Damage));
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
			Credit_Give(Attack, 1, "爆菊人类");
			Infect_Respawn_Check(Attack);
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
		Damage_Infect[Attack]++;
		return Plugin_Continue;
	}
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
		Damage_Attack_Up(Attack, RoundToCeil(Damage));
		if (Damage == 15 || Damage == 20 || Damage == 55 || Damage == 65 || Damage == 195)
		{
			CPrintToChatAll("{green}[ZA][伤害]{red} 人类 %N 刀了僵尸 %N", Attack, Client);
		}
		if (Mode_True)
		{
			if (!(GetEntityFlags(Attack) & FL_DUCKING))
			{
				if (Damage == 15 || Damage == 20)
				{
					Damage = 4000.0;
				}
				else if (Damage == 55 || Damage == 65)
				{
					Damage = 4000.0;
				}
				else if (Damage == 195)
				{
					Damage = 15000.0;
				}
			}
		}
		Damage *= Damage_Improve();
		if (Damage_Zombie[Client] >= GetTime())
		{
			Damage *= 0.4;
		}
		PrintCenterText(Attack, "[伤害]对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}
void Damage_Zombie_Credit(int Client)
{
	Damage_Zombie[Client] = GetTime() + 3;
}
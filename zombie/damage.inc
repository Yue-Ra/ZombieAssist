int Damage_Attack[MAXPLAYERS + 1];
int Damage_Attack_Temp[MAXPLAYERS + 1];
int Damage_Infect[MAXPLAYERS + 1];
int Damage_Attack_Rank[5];
int Damage_Infect_Rank[5];
int Damage_Attack_Count[5];
int Damage_Infect_Count[5];

int Damage_Zombie[MAXPLAYERS + 1];

bool Damage_Fall = true;

void Damage_OnMapStart()
{
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
	Credit_Give(Client, 1, "4000伤害");
	Client_Point_Give(Client, 15, "4000伤害");
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
		if (CheckHuman(Attack))
		{
			Damage_Attack_Up(Attack, RoundToCeil(Damage));
			IgniteEntity(Client, 5.0 + Skill_5_OnTakeDamage(Attack));
			Damage += Skill_7_OnTakeDamage(Attack);
			PrintCenterText(Attack, "[伤害]对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		}
		if (GetClientHealth(Client) > Damage)
		{
			SetEntityHealth(Client, GetClientHealth(Client) - RoundToCeil(Damage));
			return Plugin_Handled;
		}
		return Plugin_Changed;
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
			CPrintToChatAll("{green}[ZA][伤害] {red}人类 {gold}%N{red} 被僵尸 {gold}%N{red} 爆菊了", Client, Attack);
			PrintCenterTextAll("人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			ForcePlayerSuicide(Client);
			Sound_Play();
			Credit_Give(Attack, 1, "爆菊人类", true);
			Client_Point_Give(Attack, 25, "爆菊人类", true);
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
		Skill_4_OnTakeDamage(Attack, RoundToCeil(Damage));
		PrintCenterText(Attack, "[伤害]对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		if (Damage == 15 || Damage == 20 || Damage == 55 || Damage == 65 || Damage == 195)
		{
			CPrintToChatAll("{green}[ZA][伤害] {red}人类 {gold}%N{red} 刀了僵尸 {gold}%N", Attack, Client);
		}
		if (Mode_True)
		{
			GetEntityClassname(GetWeapon(Attack), ClassName, sizeof(ClassName));
			if (StrEqual(ClassName, "weapon_knife", false) && Skill_16_OnDamage(Attack))
			{
				CreateTimer(3.0, Skill_16_Timer, Client, TIMER_FLAG_NO_MAPCHANGE);
				return Plugin_Continue;
			}
			// if (!(GetEntityFlags(Attack) & FL_DUCKING))
			// {
			// 	if (Damage == 15 || Damage == 20)
			// 	{
			// 		Damage = 4000.0;
			// 	}
			// 	else if (Damage == 55 || Damage == 65)
			// 	{
			// 		Damage = 4000.0;
			// 	}
			// 	else if (Damage == 195)
			// 	{
			// 		Damage = 15000.0;
			// 	}
			// }
		}
		Damage *= Damage_Zombie_Check(Client);
		Damage *= Skill_3_OnTakeDamage(Client);
		Damage *= Skill_14_OnDamage(Attack);
		if (Skill_6_OnTakeDamage(Attack))
		{
			IgniteEntity(Client, 0.5);
			CPrintToChat(Attack, "{green}[ZA][技能] {blue}触发了 {gold}%s{blue} 效果", Skill_Name[5]);
		}
		if (Skill_8_OnTakeDamage(Attack))
		{
			Damage *= 3.0;
			CPrintToChat(Attack, "{green}[ZA][技能] {blue}触发了 {gold}%s{blue} 效果", Skill_Name[7]);
		}
		int Flags = GetEntityFlags(Client);
		if (Flags & FL_ONGROUND == 0 || Flags & FL_DUCKING == 2)
		{
			float ClientPos[3];
			GetClientAbsOrigin(Client, ClientPos);
			float AttackPos[3];
			GetClientEyePosition(Attack, AttackPos);
			float VectorPos[3];
			MakeVectorFromPoints(AttackPos, ClientPos, VectorPos);
			NormalizeVector(VectorPos, VectorPos);
			ScaleVector(VectorPos, 300.0);
			TeleportEntity(Client, NULL_VECTOR , NULL_VECTOR , VectorPos);
		}
		if (Mode_True && Mode_Start)
		{
			Damage *= 100;
		}
		PrintCenterText(Attack, "[伤害]对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}
float Damage_Zombie_Check(int Client)
{
	if (Damage_Zombie[Client] > GetTime())
	{
		return 0.4;
	}
	return 1.0;
}
void Damage_Zombie_Credit(int Client)
{
	Damage_Zombie[Client] = GetTime() + 3;
}
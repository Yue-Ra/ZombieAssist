int Damage_Client[MAXPLAYERS + 1] = {0, ...};
int Damage_First = 0;

bool Damage_Allow = true;

void Damage_OnRoundStart()
{
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Damage_Client[Client] = 0;
	}
}
void Damage_OnRoundEnd()
{
	Damage_First = 0;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (Damage_Client[Client] > Damage_Client[Damage_First])
		{
			Damage_First = Client;
		}
	}
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
		if (Leader_Block(Client))
		{
			return Plugin_Handled;
		}
		if (Damage == 195)
		{
			CPrintToChatAll("{green}[ZA]{default} 人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			PrintCenterTextAll("人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			ForcePlayerSuicide(Client);
			Sound_Play();
			return Plugin_Handled;
		}
		if (Mode_Start)
		{
			return Plugin_Handled;
		}
		if (Mode_True)
		{
			char WeaponName[32];
			GetClientWeapon(Client, WeaponName, sizeof(WeaponName));
			if (StrEqual(WeaponName, "weapon_knife", false))
			{
				ForcePlayerSuicide(Client);
				return Plugin_Handled;
			}
		}
		return Plugin_Continue;
	}
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
		if (Mode_True)
		{
			if (Damage == 15 || Damage == 20)
			{
				Damage = 4000.0;
			}
			else if (Damage == 55 || Damage == 65)
			{
				Damage = 10000.0;
			}
		}
		Damage *= Damage_Improve();
		if (Leader_Close(Attack))
		{
			Damage *= 1.5;
		}
		PrintCenterText(Attack, "对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}
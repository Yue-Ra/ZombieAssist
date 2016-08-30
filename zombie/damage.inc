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
	return float(Zombie) / float(Human);
}
void Damage_Hook(int Client)
{
	SDKHook(Client, SDKHook_OnTakeDamage, Damage_TakeDamage);
}
void Damage_Unhook(int Client)
{
	SDKUnhook(Client, SDKHook_OnTakeDamage, Damage_TakeDamage);
}
Action Damage_TakeDamage(int Client, int &Attack, int &Inflictor, float &Damage, int &damagetype, int &Weapon, float damageForce[3], float damagePosition[3])
{
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
			PrintToChatAll("[ZA]人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			PrintCenterTextAll("人类 %N 被僵尸 %N 爆菊了", Client, Attack);
			ForcePlayerSuicide(Client);
			return Plugin_Handled;
		}
		return Plugin_Continue;
	}
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
		float Improve = Damage_Improve();
		if (Improve > 6)
		{
			Improve = 6.0;
		}
		Damage *= Improve;
		if (Leader_Close(Attack))
		{
			Damage *= 1.5;
		}
		PrintCenterText(Attack, "对僵尸 %N 造成 %.2f 伤害", Client, Damage);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}
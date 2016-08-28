int Damage_Client[MAXPLAYERS + 1] = {0, ...};
int Damage_First = 0;

bool Damage_Allow = true;

void Damage_OnRoundStart()
{
	for (int Client = 1; Client <= MaxClinets; Client++)
	{
		Damage_Client[Client] = 0;
	}
}
void Damage_OnRoundEnd()
{
	Damage_First = 0;
	for (int Client = 1; Client <= MaxClinets; Client++)
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
Action Damage_TakeDamage(int Client, int &Attack, int &inflictor, float &Damage, int &damagetype, int &Weapon, float damageForce[3], float damagePosition[3])
{
	if (Client == Attack)
	{
		return Plugin_Handled;
	}
	if (!CheckAlive(Attack))
	{
		return Plugin_Continue;
	}
	if (GetClientTeam(Client) == GetClientTeam(Attack))
	{
		return Plugin_Handled;
	}
	if (CheckHuman(Client) && CheckZombie(Attack))
	{
		if (Leader_Block(Client))
		{
			return Plugin_Handled;
		}
		if(damage > 100)
		{
			//确保一些地图人类血多，僵尸背刀能致死
			damage = 2000.0;
			return Plugin_Changed;
		}
		return Plugin_Handled;
	}
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
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

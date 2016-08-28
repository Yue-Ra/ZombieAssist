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
		return;
	}
	int Zombie = ZombieCount();
	if (Zombie == 0)
	{
		return;
	}
	if (Zombie < Human)
	{
		return 1.0;
	}
	return Zombie / Human;
}
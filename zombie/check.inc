bool CheckGame(int Client)
{
	if (Client < 1 || Client > 64 || !IsClientConnected(Client) || !IsClientInGame(Client))
	{
		return false;
	}
	return true;
}
bool CheckAlive(int Client)
{
	if (!CheckGame(Client))
	{
		return false;
	}
	return IsPlayerAlive(Client);
}
bool CheckHuman(int Client)
{
	if (!CheckAlive(Client))
	{
		return false;
	}
	return ZR_IsClientHuman(Client);
}
bool CheckZombie(int Client)
{
	if (!CheckAlive(Client))
	{
		return false;
	}
	return ZR_IsClientZombie(Client);
}
int HumanCount()
{
	int Count = 0;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckHuman(Client))
		{
			Count++;
		}
	}
	return Count;
}
int ZombieCount()
{
	int Count = 0;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckZombie(Client))
		{
			Count++;
		}
	}
	return Count;
}
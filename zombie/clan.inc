void Clan_Check()
{
	char ClanName[32];
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckAlive(Client))
		{
			CS_GetClientClanTag(Client, ClanName, sizeof(ClanName));
			if (!StrEqual(ClanName, "UB游戏社区", false))
			{
				return;
			}
			Credit_Give(Client, 1, "官方队标");
		}
	}
}
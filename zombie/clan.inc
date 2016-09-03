void Clan_Init(int Client)
{
	char ClanName[32];
	CS_GetClientClanTag(Client, ClanName, sizeof(ClanName));
	if (StrEqual(ClanName, "UB游戏社区", false))
	{
		Clan_Give(Client);
	}
}
void Clan_Give(int Client)
{
	CPrintToChat(Client, "{green}[ZA][队标]{blue} 获得官方队标奖励");
}
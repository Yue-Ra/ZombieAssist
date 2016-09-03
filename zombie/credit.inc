int Credit_Client[MAXPLAYERS + 1];
int Credit_Count[MAXPLAYERS + 1] = {20, ...};

bool Credit_Allow = false;
bool Credit_Show = false;

void Credit_OnRoundStart()
{
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Credit_Client[Client] = Credit_Count[Client];
	}
	if (!Credit_Show)
	{
		return;
	}
	char Msg[192];
	Format(Msg, sizeof(Msg), "生化尖兵: %N\n团队精英: %N,%N\n僵尸克星: %N,%N\n感染精英: %N,%N");
	SendHudTextMsg(0, 0.0, 0.6, 5.0, 0, 255, Msg);
}
void Credit_Give(int Client, int Type)
{
	if (Type == 1)
	{
		Credit_Human_1 = Client;
	}
	else if (Type == 2)
	{

	}
}
void Credit_OnRoundEnd()
{
	if (Team != CS_TEAM_CT || GetTime() - Navbar_Time < 120 || GetTeamClientCount(CS_TEAM_CT) + GetTeamClientCount(CS_TEAM_T) < 15)
	{
		Credit_Show = false;
		return;
	}
	Credit_Show = true;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		Credit_Damage[Client] = false;
		if (CheckAlive(Client))
		{
			Credit_Count[Client]++;
		}
	}
}
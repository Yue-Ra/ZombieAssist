bool Leader_Allow = true;

void Leader_Create()
{
	int Last = 0;
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckHuman(Client))
		{
			if (Last == 0 || Damage_Client[Client] > Damage_Client[Last])
			{
				Last = Client;
			}
		}
	}
}
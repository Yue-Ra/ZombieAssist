bool ub_mantou[MAXPLAYERS + 1];

void MantouClean()
{
	for(int i = MAXPLAYERS; i <= MAXPLAYERS; i++)
	{
		ub_mantou[i] = false;
	}
}

void MantouGive(int userid)
{
	ub_mantou[userid] = true;
	CreateTimer(15.0, MantouRemove, userid);
}

void MantouRemove(Handle timer, any userid)
{
	ub_mantou[userid] = false;
}

void MantouBackToHuman(int client, int args)
{
	userid = GetClientUserId(client);
	if(ub_mantou[userid])
	{
		ub_mantou[userid] = false;
		InfectZombieToHuman(client);
	}
}

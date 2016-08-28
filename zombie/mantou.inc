bool ub_mantou[MAXPLAYERS + 1];

void MantouInit()
{
	HookEvent("player_death", Event_PlayerDeath);
}

Action Event_PlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
	int attacker = event.GetInt("attacker");
	int victim = event.GetInt("userid");
	char weapon[32];
	event.GetString("weapon", weapon, sizeof(weapon));

	if(CheckZombie(attacker) && CheckHuman(victim) && StrEqual(weapon, "knife", false))
	{
		//TODO sound, credit, etc.
		PrintToChatAll("[ZA]人类 %N 被僵尸 %N 爆菊了", victim, attacker);
		PrintCenterTextAll("[ZA]人类 %N 被僵尸 %N 爆菊了", victim, attacker);
		ub_mantou[attacker] = true;
		CreateTimer(15.0, MantouRemove, attacker);
	}
}

void MantouRemove(Handle timer, any client)
{
	ub_mantou[client] = false;
}

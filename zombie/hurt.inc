bool ub_mantou[MAXPLAYERS + 1];



Action Event_PlayerHurt(Event event, const char[] name, bool dontBroadcast)
{
	int attacker = event.GetInt("attacker");
	int victim = event.GetInt("userid");
	char weapon[32];
	event.GetString("weapon", weapon, sizeof(weapon));
	int damage = event.GetInt("dmg_health");

	if(CheckZombie(attacker) && CheckHuman(victim) && StrEqual(weapon, "knife", false))
	{
		PrintToChatAll("[ZA]人类 %N 被僵尸 %N 爆菊了", Client, Attack);
		PrintCenterTextAll("[ZA]人类 %N 被僵尸 %N 爆菊了", Client, Attack);
	}
}

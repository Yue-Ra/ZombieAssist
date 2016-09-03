void Event_OnPluginStart()
{
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("round_start", Event_RoundStart, EventHookMode_Post);
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
}
void Event_PlayerSpawn(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (CheckHuman(Client))
	{
		SetArmour(Client, 100);
	}
}
void Event_RoundStart(Event event, const char[] name, bool dontBroadcast)
{
	Damage_OnRoundStart();
	Weapon_OnRoundStart();
	Navbar_OnRoundStart();
}
void Event_RoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	Navbar_OnRoundEnd();
	Map_OnRoundEnd(GetEventInt(event, "winner"));
	Mode_OnRoundEnd();
	Alert_OnRoundEnd();
}
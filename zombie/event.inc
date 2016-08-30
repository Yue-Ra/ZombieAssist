void Event_Init()
{
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("round_start", Event_RoundStart, EventHookMode_Post);
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
}
void Event_PlayerSpawn(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (CheckAlive(Client))
	{
		SetArmour(Client, 100);
	}
}
void Event_RoundStart(Event event, const char[] name, bool dontBroadcast)
{
	Navbar_OnRoundStart();
	Damage_OnRoundStart();
}
void Event_RoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	Map_OnRoundEnd(GetEventInt(event, "winner"));
	Damage_OnRoundEnd();
	Mode_OnRoundEnd();
}
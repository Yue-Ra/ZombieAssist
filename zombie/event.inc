void Event_OnPluginStart()
{
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("player_death", Event_PlayerDeath, EventHookMode_Post);
	HookEvent("weapon_fire", Event_WeaponFire, EventHookMode_Post);
	HookEvent("round_start", Event_RoundStart, EventHookMode_Post);
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
	HookEvent("round_freeze_end", Event_RoundFreezeEnd, EventHookMode_Post);
}
void Event_PlayerSpawn(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	Skill_2_OnPlayerSpawn(Client);
	Skill_3_OnPlayerSpawn(Client);
	Skill_11_OnPlayerSpawn(Client);
	if (ZR_IsClientHuman(Client))
	{
		SetArmour(Client, 100);
	}
}
void Event_PlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	int Attack = GetClientOfUserId(GetEventInt(event, "attacker"));
	Skill_2_OnPlayerDeath(Client);
	Skill_3_OnPlayerDeath(Client);
	if (CheckHuman(Attack))
	{
		Credit_Give(Attack, Mode_True ? 1 : 3 , "击杀僵尸");
	}
}
void Event_WeaponFire(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	Weapon_Credit(Client);
}
void Event_RoundStart(Event event, const char[] name, bool dontBroadcast)
{
	Infect_Init();
	Clan_OnRoundStart();
	Damage_OnRoundStart();
	Weapon_OnRoundStart();
	Time_OnRoundStart();
	Model_OnRoundStart();
}
void Event_RoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	int Team = GetEventInt(event, "winner");
	Alert_Init();
	Alpha_OnRoundEnd();
	Map_OnRoundEnd(Team);
	Level_OnRoundEnd(Team);
	Credit_OnRoundEnd(Team);
	Mode_OnRoundEnd();
}
void Event_RoundFreezeEnd(Event event, const char[] name, bool dontBroadcast)
{
	Time_OnRoundFreezeEnd();
}
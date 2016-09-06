void Event_OnPluginStart()
{
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("player_death", Event_PlayerDeath, EventHookMode_Post);
	HookEvent("player_hurt", Event_PlayerHurt, EventHookMode_Post);
	HookEvent("bullet_impact", Event_BulletImpact, EventHookMode_Post);
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
void Event_PlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	int Attack = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
		Credit_Up(Attack, 1);
	}
}
void Event_PlayerHurt(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	int Attack = GetClientOfUserId(GetEventInt(event, "attacker"));
	int Damage = GetEventInt(event, "dmg_health") + GetEventInt(event, "dmg_armor");
	if (CheckZombie(Client) && CheckHuman(Attack))
	{
		Damage_Attack_Up(Attack, Damage);
	}
}
void Event_BulletImpact(Event event, const char[] name, bool dontBroadcast)
{
	int Client = GetClientOfUserId(GetEventInt(event, "userid"));
	Weapon_Credit(Client);
}
void Event_RoundStart(Event event, const char[] name, bool dontBroadcast)
{
	Infect_Init();
	Damage_OnRoundStart();
	Weapon_OnRoundStart();
	Navbar_OnRoundStart();
}
void Event_RoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	int Team = GetEventInt(event, "winner");
	Alert_Init();
	Navbar_OnRoundEnd();
	Map_OnRoundEnd(Team);
	Credit_OnRoundEnd(Team);
	Mode_OnRoundEnd();
}
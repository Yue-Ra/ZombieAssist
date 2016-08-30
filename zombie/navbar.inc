int Navbar_Time = 0;

bool Navbar_Allow = true;

void Navbar_OnRoundStart()
{
	Navbar_Time = GetTime() + GetConVarInt(FindConVar("mp_freezetime"));
}
Action Navbar_Timer(Handle Timer)
{
	if (!Navbar_Allow)
	{
		return Plugin_Stop;
	}
	char Info[256];
	Format(Info, sizeof(Info), "人类: %d\n僵尸: %d\n时间: %d\n\n伤害倍数: %.2f", HumanCount(), ZombieCount(), GetTime() - Navbar_Time, Damage_Improve());
	SendKeyHintTextMsg(0, Info);
	return Plugin_Handled;
}
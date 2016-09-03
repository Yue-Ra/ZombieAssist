int Navbar_Time = 0;

bool Navbar_Allow = true;
bool Navbar_Check = false;

void Navbar_OnRoundStart()
{
	Navbar_Time = GetTime() + GetConVarInt(FindConVar("mp_freezetime"));
}
void Navbar_OnRoundEnd()
{
	if (Navbar_Time < 120 || GetTeamClientCount(CS_TEAM_CT) + GetTeamClientCount(CS_TEAM_T) < 15)
	{
		Navbar_Check = false;
	}
	Navbar_Check = true;
}
Action Navbar_Timer(Handle Timer)
{
	int Time = GetTime() - Navbar_Time;
	if (Mode_True)
	{
		Mode_Check(Time);
	}
	if (!Navbar_Allow)
	{
		return Plugin_Handled;
	}
	char Info[256];
	Format(Info, sizeof(Info), "人类: %d\n僵尸: %d\n时间: %d\n伤害倍数: %.2f", HumanCount(), ZombieCount(), Time, Damage_Improve());
	SendKeyHintTextMsg(0, Info);
	return Plugin_Handled;
}
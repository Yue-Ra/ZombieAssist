bool Navbar_Allow = true;

Action Navbar_Timer(Handle Timer)
{
	if (Mode_True)
	{
		Mode_Check(Time_Time);
	}
	if (!Navbar_Allow)
	{
		return Plugin_Handled;
	}
	char Info[256];
	Format(Info, sizeof(Info), "人类: %d\n僵尸: %d\n时间: %d\n伤害倍数: %.2f", HumanCount(), ZombieCount(), Time_Time, Damage_Improve());
	SendKeyHintTextMsg(0, Info);
	return Plugin_Handled;
}
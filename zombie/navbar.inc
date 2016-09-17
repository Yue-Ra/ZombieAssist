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
	Format(Info, sizeof(Info), "人类: %d\n僵尸: %d\n时间: %d", HumanCount(), ZombieCount(), Time_Time);
	SendKeyHintTextMsg(0, Info);
	return Plugin_Handled;
}
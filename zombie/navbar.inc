int Navbar_Time = 0;

void Navbar_OnRoundStart()
{
	Navbar_Time = GetTime();
}
Action Navbar_Timer(Handle Timer)
{
	char Info[256];
	Format(Info, sizeof(Info), "人类: %d\n僵尸: %d\n时间: %d\n", HumanCount(), ZombieCount(), GetTime() - Navbar_Time);
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckAlive(Client))
		{

		}
	}
}
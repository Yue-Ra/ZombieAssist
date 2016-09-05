public void OnPluginStart()
{
	RegConsoleCmd("sm_a", Test);
}
public Action Test(int Client, int Args)
{
	char Cmd[192];
	char Cmds[12][16];
	GetCmdArgString(Cmd, sizeof(Cmd));
	ExplodeString(Cmd, " ", Cmds, sizeof(Cmds), sizeof(Cmds[]));
	for (int Num = 0; Num < sizeof(Cmds); Num++)
	{
		PrintToChatAll(Cmds[Num]);
	}
	SendHudTextMsg(Client, StringToFloat(Cmds[0]), StringToFloat(Cmds[1]), StringToFloat(Cmds[2]), StringToInt(Cmds[3]), StringToInt(Cmds[4]), StringToInt(Cmds[5]), StringToInt(Cmds[6]), StringToInt(Cmds[7]), StringToFloat(Cmds[8]), StringToFloat(Cmds[9]), StringToFloat(Cmds[10]), Cmds[11]);
	return Plugin_Handled;
}
public void SendHudTextMsg(int Client, float X, float Y, float Time, int Red, int Green, int Blue, int Alpha, int Effect, float fxTime, float In, float Out, char[] Msg)
{
	Handle Hud = CreateHudSynchronizer();
	SetHudTextParams(X, Y, Time, Red, Green, Blue, Alpha, Effect, fxTime, In, Out);
	ClearSyncHud(Client, Hud);
	ShowSyncHudText(Client, Hud, Msg);
	CloseHandle(Hud);
}
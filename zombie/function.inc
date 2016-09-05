stock void ClearTimer(& Handle: Timer)
{
	if (Timer == INVALID_HANDLE)
	{
		return;
	}
	KillTimer(Timer);
	Timer = INVALID_HANDLE;
}
stock int GetCash(int Client)
{
	return GetEntData(Client, FindSendPropInfo("CCSPlayer", "m_iAccount"));
}
stock void SetCash(int Client, int Value)
{
	SetEntData(Client, FindSendPropInfo("CCSPlayer", "m_iAccount"), Value);
}
stock int GetArmour(int Client)
{
	return GetEntProp(Client, Prop_Send, "m_ArmorValue");
}
stock void SetArmour(int Client, int Value)
{
	SetEntProp(Client, Prop_Send, "m_ArmorValue", Value);
	SetEntProp(Client, Prop_Send, "m_bHasHelmet", Value ? 1 : 0);
}
stock int GetOwner(int Entity)
{
	return GetEntDataEnt2(Entity, FindSendPropInfo("CBaseCombatWeapon", "m_hOwnerEntity"));
}
stock int GetHammerId(int Entity)
{
	return GetEntProp(Entity, Prop_Data, "m_iHammerID");
}
stock void SetConVarIntValue(const char[] Name, int Value, bool Replace)
{
	Handle Var = FindConVar(Name);
	if (Var == INVALID_HANDLE)
	{
		return;
	}
	SetConVarInt(Var, Value, Replace);
}
stock void SetConVarFloatValue(const char[] Name, float Value, bool Replace)
{
	Handle Var = FindConVar(Name);
	if (Var == INVALID_HANDLE)
	{
		return;
	}
	SetConVarFloat(Var, Value, Replace);
}
stock void SendKeyHintTextMsg(int Client, char[] Msg)
{
	Handle Hud = INVALID_HANDLE;
	if (Client == 0)
	{
		Hud = StartMessageAll("KeyHintText");
	}
	else
	{
		Hud = StartMessageOne("KeyHintText", Client);
	}
	char Info[256];
	VFormat(Info, sizeof(Info), Msg, 3);
	if (Hud == INVALID_HANDLE)
	{
		return;
	}
	BfWriteByte(Hud, 1);
	BfWriteString(Hud, Info);
	EndMessage();
}
stock void SendHudTextMsg(int Client, float X, float Y, float Time, int Red, int Green, int Blue, char[] Msg)
{
	Handle Hud = CreateHudSynchronizer();
	SetHudTextParams(X, Y, Time, Red, Green, Blue, 0, 1, 0.0, 0.0, 0.0);
	if (Client == 0)
	{
		for (int Target = 1; Target <= MaxClients; Target++)
		{
			if (CheckAlive(Target))
			{
				ShowSyncHudText(Target, Hud, Msg);
			}
		}
	}
	else
	{
		ShowSyncHudText(Client, Hud, Msg);
	}
	// CloseHandle(Hud);
}
int GetCash(int Client)
{
	return GetEntData(Client, FindSendPropInfo("CCSPlayer", "m_iAccount"));
}
void SetCash(int Client, int Value)
{
	SetEntData(Client, FindSendPropInfo("CCSPlayer", "m_iAccount"), Value);
}
int GetArmour(int Client)
{
	return GetEntProp(Client, Prop_Send, "m_ArmorValue");
}
void SetArmour(int Client, int Value)
{
	SetEntProp(Client, Prop_Send, "m_ArmorValue", Value);
	SetEntProp(Client, Prop_Send, "m_bHasHelmet", Value ? 1 : 0);
}
int GetOwner(int Entity)
{
	return GetEntDataEnt2(Entity, FindSendPropInfo("CBaseCombatWeapon", "m_hOwnerEntity"));
}
int GetHammerId(int Entity)
{
	return GetEntProp(Entity, Prop_Data, "m_iHammerID");
}
void SendKeyHintTextMsg(Client, char[] Msg)
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
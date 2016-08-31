int Leader_Client = 0;
int Leader_Count = 2;
bool Leader_Allow = true;
float Leader_Vector[3];

void Leader_OnRoundStart()
{
	if (!Leader_Allow)
	{
		return;
	}
	CreateTimer(30.0, Leader_Timer, Damage_First, TIMER_FLAG_NO_MAPCHANGE);
}
Action Leader_Timer(Handle Timer, any Client)
{
	if (!Leader_Allow)
	{
		return;
	}
	if (!CheckHuman(Client))
	{
		return;
	}
	SetEntityModel(Client, "");
	CreateTimer(1.0, Leader_Light, Client, TIEMR_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}
Action Leader_Light(Handle Timer, any Client)
{
	if (!CheckHuman(Client))
	{
		return Plugin_Stop;
	}
	GetClientAbsOrigin(Client, Leader_Vector);
	TE_SetupBeamRingPoint(Leader_Vector, 0.0, 400.0, PrecacheModel("materials/sprites/laser.vmt"), PrecacheModel("materials/sprites/halo01.vmt"), 0, 15, 0.5, 5.0, 0.0, {0, 0, 255, 255}, 10, 0);
	TE_SendToAll();
}
bool Leader_Block(int Client)
{
	if (Leader_Client != Client)
	{
		return false;
	}
	if (Leader_Count < 1)
	{
		return false;
	}
	Leader_Count--;
	CPrintToChat(Client, "{green}[ZA]{default} 队长免疫次数剩下 %d 次", Client);
	return true;
}
bool Leader_Close(int Client)
{
	if (!Leader_Allow)
	{
		return false;
	}
	if (!CheckHuman(Client))
	{
		return false;
	}
	float Vector[3];
	GetClientAbsOrigin(Client, Vector);
	if (GetVectorDistance(Leader_Vector, Vector, false) > 400)
	{
		return false;
	}
	return true;
}
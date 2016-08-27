bool Alpha_Client[MAXPLAYERS + 1] = {true, ...};

bool Alpha_Allow = true;

Action Alpha_Command(int Client, int Args)
{
	Alpha_Client = !Alpha_Client[Client];
	PrintToChat(Client, "[ZA]当前模型透明为 %s", Alpha_Client[Client] ? "开启" : "关闭");
}
void Alpha_OnGameFrame()
{
	if (!Alpha_Allow)
	{
		return;
	}
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (!Alpha_Client[Client])
		{
			continue;
		}
		if (CheckHuman(Client))
		{
			int Distance = GetCloseClientDistance(Client);
			if (Distance < 30)
			{
				Distance = 30;
			}
			if (GetEntityRenderMode(Client) != RENDER_TRANSALPHA)
			{
				SetEntityRenderMode(Client, RENDER_TRANSALPHA);
			}
			SetEntityRenderColor(Client, 255, 255, 255, Distance);
		}
	}
}
int GetCloseClientDistance(int Client)
{
	int	Last = 255;
	float ClientVec[3];
	float TargetVec[3];
	GetClientAbsOrigin(Client, ClientVec);
	for (int Target = 1; Target <= MaxClients; Target++)
	{
		if (Client == Target)
		{
			continue;
		}
		if (CheckHuman(Target))
		{
			GetClientAbsOrigin(Target, TargetVec);
			int Temp = RoundToCeil(GetVectorDistance(ClientVec, TargetVec, false));
			if (Temp < Last)
			{
				Last = Temp;
			}
		}
	}
	return Last;
}
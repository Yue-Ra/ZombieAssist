int Alert_Time = -1;
Handle Alert_Handle = INVALID_HANDLE;
Regex Alert_Regex;
Regex Alert_Number;

void Alert_Init()
{
	Alert_Regex = CompileRegex("([0-9]+s|[0-9]+ s|[0-9]+S|[0-9]+ S)");
	Alert_Number = CompileRegex("([0-9]+)");
}
void Alert_OnRoundEnd()
{
	if (Alert_Handle != INVALID_HANDLE)
	{
		KillTimer(Alert_Handle);
		Alert_Handle = INVALID_HANDLE;
	}
}
void Alert_Check(const char[] Msg)
{
	if (Alert_Number.Match(Msg) == -1 && Alert_Regex.Match(Msg) == -1)
	{
		return;
	}
	char Str[32];
	if (!Alert_Number.GetSubString(0, Str, sizeof(Str)) && !Alert_Regex.GetSubString(0, Str, sizeof(Str)))
	{
		return;
	}
	Alert_Time = StringToInt(Str);
	Alert_Create(Alert_Time);
	if (Alert_Handle != INVALID_HANDLE)
	{
		KillTimer(Alert_Handle);
		Alert_Handle = INVALID_HANDLE;
	}
	Alert_Handle = CreateTimer(1.0, Alert_Timer, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}
Action Alert_Timer(Handle Timer)
{
	if (Alert_Time < 0)
	{
		KillTimer(Alert_Handle);
		Alert_Handle = INVALID_HANDLE;
	}
	else
	{
		Alert_Time--;
		if (Alert_Time > -1)
		{
			Alert_Create(Alert_Time);
		}
	}
	return Plugin_Handled;
}
void Alert_Create(int Time)
{
	int Color[3];
	char Msg[32];
	if (Time == 0)
	{
		Color[0] = 0;
		Color[1] = 0;
		Color[2] = 255;
		Format(Msg, sizeof(Msg), "GO!");
	}
	else if (Time < 3)
	{
		Color[0] = 255;
		Color[1] = 0;
		Color[2] = 0;
		Format(Msg, sizeof(Msg), "%d", Time);
	}
	else
	{
		Color[0] = 0;
		Color[1] = 255;
		Color[2] = 0;
		Format(Msg, sizeof(Msg), "%d", Time);
	}
	SendHudTextMsg(0, -1.0, -0.35, 1.0, Color[0], Color[1], Color[2], Msg);
}
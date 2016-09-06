int Alert_Count[6];
char Alert_Found[6][32];
char Alert_String[6][192];
Handle Alert_Handle[6];
Regex Alert_Regex;
Regex Alert_Ignore;

void Alert_OnPluginStart()
{
	Alert_Regex = CompileRegex("([0-9]+)");
	Alert_Ignore = CompileRegex("(MAP|LVL|TYPE|LEVEL|STAGE|MINUTE|POINTS|等级|关)", PCRE_CASELESS);
}
void Alert_Init()
{
	for (int Num = 0; Num < 6; Num++)
	{
		Alert_Count[Num] = 0;
		ClearTimer(Alert_Handle[Num]);
	}
}
int Alert_Alive()
{
	for (int Num = 0; Num < 6; Num++)
	{
		if (Alert_Count[Num] == 0)
		{
			return Num;
		}
	}
	Alert_Count[0] = 0;
	ClearTimer(Alert_Handle[0]);
	return 0;
}
void Alert_Check(const char[] Msg)
{
	if (Alert_Ignore.Match(Msg) > 0)
	{
		return;
	}
	int Count = Alert_Regex.Match(Msg);
	if (Count == -1)
	{
		return;
	}
	int Num = Alert_Alive();
	strcopy(Alert_String[Num], sizeof(Alert_String[]), Msg);
	if (!Alert_Regex.GetSubString(Count - 1, Alert_Found[Num], sizeof(Alert_Found[])))
	{
		return;
	}
	ReplaceString(Alert_String[Num], sizeof(Alert_String[]), Alert_Found[Num], "{TIME}", false);
	for (int Line = 0; Line < 6; Line++)
	{
		if (Alert_Count[Line] == 0 || Num == Line)
		{
			continue;
		}
		if (StrEqual(Alert_String[Num], Alert_String[Line], false))
		{
			return;
		}
	}
	Alert_Count[Num] = StringToInt(Alert_Found[Num]);
	Alert_Create(Alert_Count[Num], Num);
	ClearTimer(Alert_Handle[Num]);
	Alert_Handle[Num] = CreateTimer(1.0, Alert_Timer, Num, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}
Action Alert_Timer(Handle Timer, any Num)
{
	Alert_Count[Num]--;
	if (Alert_Count[Num] <= 0)
	{
		ClearTimer(Alert_Handle[Num]);
		return Plugin_Handled;
	}
	Alert_Create(Alert_Count[Num], Num);
	return Plugin_Handled;
}
void Alert_Create(int Time, int Line)
{
	int Color[3];
	char Str[32];
	Format(Str, sizeof(Str), "%d", Time);
	char Msg[192];
	strcopy(Msg, sizeof(Msg), Alert_String[Line]);
	ReplaceString(Msg, sizeof(Msg), "{TIME}", Str, false);
	Color[0] = 0;
	Color[1] = 255;
	Color[2] = 0;
	if (Time < 6)
	{
		Color[0] = 255;
		Color[1] = 0;
	}
	SendHudTextMsg(0, -1.0, 0.72 + 0.03 * Line, 1.1, Color[0], Color[1], Color[2], Msg);
}
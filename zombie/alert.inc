int Alert_Count[6];
char Alert_String[6][192];
Handle Alert_Handle[6];
bool Alert_Chinese[6];
Regex Alert_Regex;
Regex Alert_Ignore;

void Alert_OnPluginStart()
{
	Alert_Regex = CompileRegex("([0-9]+)");
	Alert_Ignore = CompileRegex("(%|@|C4|BY|HP|AFK|ARC|MAP|TIP|GIL|MAP|LVL|AMMO|ONLY|SONG|TYPE|EIFEL|LEVEL|STAGE|BUTTON|SHOOTS|PEOPLE|PLAYER|MINUTE|POINTS|SERVER|CONTACT|COWBELL|COOLDOWN|关|分|级|挂机|提示|获得|服务器)", PCRE_CASELESS);
}
void Alert_OnRoundEnd()
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
void Alert_Check(const char[] Msg, bool Chinese = false)
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
	char Found[32];
	if (!Alert_Regex.GetSubString(Count - 1, Found, sizeof(Found)))
	{
		return;
	}
	int Num = StringToInt(Found);
	char RegexStr[192];
	strcopy(RegexStr, sizeof(RegexStr), Msg);
	ReplaceString(RegexStr, sizeof(RegexStr), Found, "{TIME}", false);
	for (int Line = 0; Line < 6; Line++)
	{
		if (StrEqual(RegexStr, Alert_String[Line], false))
		{
			return;
		}
	}
	int Time = Num;
	Num = Alert_Alive();
	Alert_Chinese[Num] = Chinese;
	Alert_Count[Num] = Time;
	strcopy(Alert_String[Num], sizeof(Alert_String[]), RegexStr);
	Alert_Create(Alert_Count[Num], Num);
	ClearTimer(Alert_Handle[Num]);
	Alert_Handle[Num] = CreateTimer(1.0, Alert_Timer, Num, TIMER_REPEAT);
}
Action Alert_Timer(Handle Timer, any Num)
{
	Alert_Count[Num]--;
	if (Alert_Count[Num] > 0)
	{
		Alert_Create(Alert_Count[Num], Num);
		return Plugin_Handled;
	}
	Alert_Count[Num] = 0;
	Format(Alert_String[Num], sizeof(Alert_String[]), "");
	ClearTimer(Alert_Handle[Num]);
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
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckAlive(Client))
		{
			if ((CheckChinese(Client) && !Alert_Chinese[Line]) || (!CheckChinese(Client) && Alert_Chinese[Line]))
			{
				continue;
			}
			SendHudTextMsg(Client, -1.0, 0.72 + 0.03 * Line, 1.1, Color[0], Color[1], Color[2], Msg);
		}
	}
}
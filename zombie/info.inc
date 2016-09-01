char Info_Msg[MAXPLAYERS + 1][192];

int Info_Max = 0;
int Info_Last = 0;

Action Info_Reload_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Info_Load();
	}
	return Plugin_Handled;
}
void Info_Load()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM info ORDER BY id ASC");
	SQL_TQuery(MySQL, Info_Load_Query, Query);
}
void Info_Load_Query(Handle Owner, Handle Child, const char[] Error, any Data)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Info Handle Failure");
		return;
	}
	int Num = 0;
	while (SQL_FetchRow(Child))
	{
		SQL_FetchString(Child, 1, Info_Msg[Num], sizeof(Info_Msg[]));
		LogMessage("Info %s\tMsg %s", Num, Info_Msg[Num]);
		Num++;
	}
	Info_Max = Num;
}
Action Info_Timer(Handle Timer)
{
	if (Info_Max == 0)
	{
		return Plugin_Handled;
	}
	CPrintToChatAll("{green}[ZA][提示]{default} %s", Info_Msg[Info_Last]);
	PrintHintTextToAll("[提示]%s", Info_Msg[Info_Last]);
	Info_Last++;
	if (strlen(Info_Msg[Info_Last]) == 0)
	{
		Info_Last = 0;
	}
	return Plugin_Handled;
}
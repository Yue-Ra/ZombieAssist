Action Say_Command(int Client, int Args)
{
	if (Client == 0)
	{
		char Message[192];
		GetCmdArgString(Message, sizeof(Message));
		Alert_Check(Message);
		char Escape[384];
		EscapeStringMySQL(Message, Escape, sizeof(Escape));
		char Query[384];
		Format(Query, sizeof(Query), "INSERT INTO translate(map,old) VALUES ('%s','%s')", Map_Name, Message);
		SQL_TQuery(MySQL, MySQL_Query, Query);
		Format(Query, sizeof(Query), "SELECT new FROM translate WHERE map='%s' AND old='%s'", Map_Name, Escape);
		SQL_TQuery(MySQL, Translate_Query, Query);
	}
	return Plugin_Continue;
}
void Translate_Query(Handle Owner, Handle Child, const char[] Error, any Message)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Translate Handle Failure");
		return;
	}
	if (SQL_GetRowCount(Child) == 0)
	{
		return;
	}
	if (!SQL_FetchRow(Child))
	{
		return;
	}
	char Translate[192];
	SQL_FetchString(Child, 0, Translate, sizeof(Translate));
	if (strlen(Translate) == 0)
	{
		return;
	}
	CPrintToChatAll("{green}[ZA][翻译]{lightgreen} %s", Translate);
	Alert_Check(Translate);
}

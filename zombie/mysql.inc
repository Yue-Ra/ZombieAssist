Handle MySQL = INVALID_HANDLE;
void MySQL_Start()
{
	if (MySQL == INVALID_HANDLE)
	{
		SQL_TConnect(MySQL_Connect, "moeub");
	}
}
void MySQL_Close()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
	if (!CloseHandle(MySQL))
	{
		return;
	}
	MySQL = INVALID_HANDLE;
}
void MySQL_Connect(Handle Owner, Handle Child, const char[] Error, any Data)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("MySQL Connect Failure");
		return;
	}
	SQL_SetCharset(Child, "utf8");
	MySQL = Child;
	LogMessage("MySQL Connect Success");
	Map_Load();
	Model_Load();
	Weapon_Load();
}
void MySQL_Query(Handle Owner, Handle Child, const char[] Error, any Data)
{
}
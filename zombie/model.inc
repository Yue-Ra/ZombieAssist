int Model_ID[128];
char Model_Name[128][32];
char Model_Path[128][128];
int Model_Type[128];

int Model_Max = 0;
void Model_Load()
{
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM model ORDER BY id ASC");
	SQL_TQuery(MySQL, Model_Query, Query);
}
void Model_Query(Handle Owner, Handle Child, const char[] Error, any Data)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Model Handel Failure");
		return;
	}
	int Num = 0;
	while (SQL_FetchRow(Child))
	{
		Model_ID[Num] = SQL_FetchInt(Child, 0);
		SQL_FetchString(Child, 1, Model_Name[Num], sizeof(Model_Name[]));
		SQL_FetchString(Child, 2, Model_Path[Num], sizeof(Model_Path[]));
		Model_Type[Num] = SQL_FetchInt(Child, 3);
		Num++;
	}
	Model_Max = Num;
}
Action Model_Menu(int Client, int Args)
{

}
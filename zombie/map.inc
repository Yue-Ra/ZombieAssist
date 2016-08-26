char Map_Name[32];
int Map_Win;
int Map_Lose;
float Map_Spawn;
void Map_Init()
{
	GetCurrentMap(Map_Name, sizeof(Map_Name));
}
void Map_Load()
{
	char Query[192];
	Format(Query, sizeof(Query), "SELECT * FROM map WHERE name='%s'", Map_Name);
	SQL_TQuery(MySQL, Map_Load_Query, Query);
}
void Map_Clean()
{
	Format(Map_Name, sizeof(Map_Name), "");
	Map_Win = 0;
	Map_Lose = 0;
	Map_Spawn = 10.0;
}
void Map_Load_Query(Handle Owner, Handle Child, const char[] Error, any Data)
{
	if (Owner == INVALID_HANDLE || Child == INVALID_HANDLE)
	{
		LogError("Map Handle Failure");
		return;
	}
	if (SQL_GetRowCount(Child) == 0)
	{
		Map_Create();
		return;
	}
	if (!SQL_FetchRow(Child))
	{
		LogError("Map FetchRow Failure");
		return;
	}
	Map_Win = SQL_FetchInt(Child, 1);
	Map_Lose = SQL_FetchInt(Child, 2);
	Map_Spawn = SQL_FetchFloat(Child, 3);
	Map_SetSpawnTime(Map_Spawn);
	LogMessage("Map %s Load Success", Map_Name);
}
void Map_Create()
{
	char Query[192];
	Format(Query, sizeof(Query), "INSERT INTO map(name) VALUES ('%s')", Map_Name);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	Map_SetSpawnTime(Map_Spawn);
	LogMessage("Map %s Create Success", Map_Name);
}
void Map_SetSpawnTime(float Value)
{
	SetConVarFloat(FindConVar("zr_infect_spawntime_min"), Value, true, true);
	SetConVarFloat(FindConVar("zr_infect_spawntime_max"), Value, true, true);
}
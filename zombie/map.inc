char Map_Name[32];
int Map_Win = 0;
int Map_Lose = 0;
float Map_Spawn = 10.0;
void Map_Init()
{
	GetCurrentMap(Map_Name, sizeof(Map_Name));
}
void Map_Load()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
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
void Map_OnRoundEnd(int Team)
{
	char Query[192];
	if (Team == CS_TEAM_CT)
	{
		Map_Win++;
		Format(Query, sizeof(Query), "UPDATE map SET win=win+1 WHERE name='%s'", Map_Name);
	}
	else
	{
		Map_Lose++;
		Format(Query, sizeof(Query), "UPDATE map SET lose=lose+1 WHERE name='%s'", Map_Name);
	}
	SQL_TQuery(MySQL, MySQL_Query, Query);
	PrintToChatAll("[ZA]地图累计胜利 %d,累计失败 %d", Map_Win, Map_Lose);
}
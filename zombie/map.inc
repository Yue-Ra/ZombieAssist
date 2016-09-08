char Map_Name[32];
int Map_Win = 0;
int Map_Lose = 0;
float Map_Spawn = 10.0;

void Map_Init()
{
	GetCurrentMap(Map_Name, sizeof(Map_Name));
	if (StrContains(Map_Name, "zm_", false) != -1)
	{
		Mode_True = true;
	}
}
Action Map_Reload_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Map_Load();
	}
	return Plugin_Handled;
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
void Map_OnMapEnd()
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
	if (SQL_FetchInt(Child, 4) == 1)
	{
		SetConVarBounds(FindConVar("mp_roundtime"), ConVarBound_Upper, true, 15.0);
	}
	Alpha_Allow = SQL_FetchInt(Child, 5) == 0 ? false : true;
	Model_Allow = SQL_FetchInt(Child, 6) == 0 ? false : true;
	Navbar_Allow = SQL_FetchInt(Child, 7) == 0 ? false : true;
	Damage_Allow = SQL_FetchInt(Child, 8) == 0 ? false : true;
	Weapon_Allow = SQL_FetchInt(Child, 9) == 0 ? false : true;
	Damage_Fall = SQL_FetchInt(Child, 11) == 0 ? false : true;
	SetConVarIntValue("sv_enablebunnyhopping", SQL_FetchInt(Child, 12), true);
	Weapon_Limit = SQL_FetchInt(Child, 13);
	Credit_Allow = SQL_FetchInt(Child, 14) == 0 ? false : true;
	LogMessage("Map %s Load Success", Map_Name);
}
Action Map_Command(int Client)
{
	if (CheckGame(Client))
	{
		Map_Menu(Client);
	}
	return Plugin_Handled;
}
void Map_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Map_Menu_Handle);
	SetMenuTitle(MenuHandle, "地图菜单");
	AddMenuItem(MenuHandle, "", "僵尸出现时间");
	AddMenuItem(MenuHandle, "", "延长回合时间");
	AddMenuItem(MenuHandle, "", "模型透明开关");
	AddMenuItem(MenuHandle, "", "人类模型开关");
	AddMenuItem(MenuHandle, "", "侧边信息开关");
	AddMenuItem(MenuHandle, "", "伤害加成开关");
	AddMenuItem(MenuHandle, "", "远程购买开关");
	AddMenuItem(MenuHandle, "", "人类队长开关");
	AddMenuItem(MenuHandle, "", "掉落伤害开关");
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Map_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{

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
	CPrintToChatAll("{green}[ZA][地图]{blue} 地图累计胜利 {gold}%d{blue} 失败 {gold}%d", Map_Win, Map_Lose);
}
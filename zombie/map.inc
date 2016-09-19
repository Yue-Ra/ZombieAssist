char Map_Name[32];
int Map_Win = 0;
int Map_Lose = 0;
float Map_Spawn = 10.0;
int Map_Win_Point = 100;
int Map_Lose_Point = 30;

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
	Map_Win_Point = 100;
	Map_Lose_Point = 30;
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
	Map_Win = SQL_FetchInt(Child, 4);
	Map_Lose = SQL_FetchInt(Child, 5);
	Map_Win_Point = SQL_FetchInt(Child, 6);
	Map_Lose_Point = SQL_FetchInt(Child, 7);
	Map_Spawn = SQL_FetchFloat(Child, 8);
	Map_SetSpawnTime(Map_Spawn);
	if (SQL_FetchInt(Child, 9) == 1)
	{
		SetConVarBounds(FindConVar("mp_roundtime"), ConVarBound_Upper, true, 15.0);
	}
	SetConVarIntValue("mp_timelimit", SQL_FetchInt(Child, 10), true);
	SetConVarFloatValue("mp_roundtime", SQL_FetchFloat(Child, 11), true);
	Damage_Fall = SQL_FetchInt(Child, 12) == 0 ? false : true;
	SetConVarIntValue("sv_enablebunnyhopping", SQL_FetchInt(Child, 13), true);
	Alpha_Allow = SQL_FetchInt(Child, 14) == 0 ? false : true;
	Model_Allow = SQL_FetchInt(Child, 15) == 0 ? false : true;
	Navbar_Allow = SQL_FetchInt(Child, 16) == 0 ? false : true;
	Weapon_Allow = SQL_FetchInt(Child, 17) == 0 ? false : true;
	Weapon_Limit = SQL_FetchInt(Child, 18);
	int Credits[6];
	for (int Num = 0; Num < sizeof(Credits); Num++)
	{
		Credits[Num] = SQL_FetchInt(Child, 19 + Num);
	}
	Credit_Human_Allow_Set(Credits);
	for (int Num = 0; Num < sizeof(Credits); Num++)
	{
		Credits[Num] = SQL_FetchInt(Child, 25 + Num);
	}
	Credit_Zombie_Allow_Set(Credits);
	int Skills[16];
	for (int Num = 0; Num < sizeof(Skills); Num++)
	{
		Skills[Num] = SQL_FetchInt(Child, 31 + Num);
	}
	Skill_Allow_Set(Skills);
	LogMessage("Map %s Load Success", Map_Name);
}
Action Map_Admin_Command(int Client, int Args)
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
	if (!Mode_True)
	{
		Format(Query, sizeof(Query), "INSERT INTO map(name) VALUES ('%s')", Map_Name);
		SetConVarIntValue("mp_timelimit", 30, true);
		SetConVarFloatValue("mp_roundtime", 10.0, true);
		Damage_Fall = true;
		SetConVarIntValue("sv_enablebunnyhopping", 0, true);
	}
	else
	{
		SetConVarIntValue("mp_timelimit", 15, true);
		SetConVarFloatValue("mp_roundtime", 3.5, true);
		Damage_Fall = false;
		SetConVarIntValue("sv_enablebunnyhopping", 1, true);
		Format(Query, sizeof(Query), "INSERT INTO map(name,timelimit,roundtime,fall,bhop) VALUES ('%s',15,3.5,0,1)", Map_Name);
	}
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
int Get_WinPoint()
{
	return Map_Win_Point;
}
int Get_LosePoint()
{
	return Map_Lose_Point;
}
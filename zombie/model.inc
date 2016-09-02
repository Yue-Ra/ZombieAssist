int Model_ID[128];
char Model_Name[128][32];
char Model_Path[128][128];
int Model_Type[128];
int Model_Client[MAXPLAYERS + 1];

bool Model_Allow = true;
int Model_Max = 0;

void Model_Load()
{
	if (MySQL == INVALID_HANDLE)
	{
		return;
	}
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
	LogMessage("Model %d Load Success", Model_Max);
}
Action Model_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Model_Menu(Client);
	}
	return Plugin_Handled;
}
Action Model_Reload_Command(int Client, int Args)
{
	Model_Load();
	return Plugin_Handled;
}
void Model_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Model_Menu_Handle);
	SetMenuTitle(MenuHandle, "模型菜单\n当前模型 %s", Model_Client[Client] == 0 ? "【普通】默认模型" : Model_Name[Model_Client[Client]]);
	AddMenuItem(MenuHandle, "", "【普通】默认模型");
	char ItemName[32];
	for (int Num = 0; Num < Model_Max; Num++)
	{
		if (Model_Type[Num] == 2)
		{
			Format(ItemName, sizeof(ItemName), "【特殊】%s", Model_Name[Num]);
		}
		else if (Model_Type[Num] == 1)
		{
			Format(ItemName, sizeof(ItemName), "【会员】%s", Model_Name[Num]);
		}
		else
		{
			Format(ItemName, sizeof(ItemName), "【普通】%s", Model_Name[Num]);
		}
		AddMenuItem(MenuHandle, "", ItemName);
	}
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Model_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{
		Model_Update(Client, Item);
	}
}
void Model_Update(int Client, int Value)
{
	char Query[192];
	Format(Query, sizeof(Query), "UPDATE user SET model=%d WHERE id=%d", Value, Client_ID[Client]);
	SQL_TQuery(MySQL, MySQL_Query, Query);
	Model_Set(Client);
}
void Model_Set(int Client)
{
	if (!Model_Allow)
	{
		CPrintToChat(Client, "{green}[ZA][模型]{red} 模型功能已被禁用");
		return;
	}
	if (Model_Client[Client] == 0)
	{
		return;
	}
	if (!FileExists(Model_Path[Model_Client[Client]]))
	{
		CPrintToChat(Client, "{green}[ZA][模型]{red} 模型文件 %s 丢失", Model_Path[Model_Client[Client]]);
		LogError("Model File %s Not Found", Model_Path[Model_Client[Client]]);
		return;
	}
	SetEntityModel(Client, Model_Path[Model_Client[Client]]);
}
void Model_OnRoundStart()
{
	if (!Model_Allow)
	{
		CPrintToChatAll("{green}[ZA][模型]{red} 模型功能已被禁用");
		return;
	}
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckHuman(Client))
		{
			Model_Set(Client);
		}
	}
}
void Admin_Init(int Client)
{
	if (GetUserAdmin(Client) == INVALID_ADMIN_ID)
	{
		return;
	}
	CS_SetClientClanTag(Client, "Admin");
}
Action Admin_Command(int Client, int Args)
{
	if (CheckGame(Client))
	{
		Admin_Menu(Client);
	}
	return Plugin_Handled;
}
void Admin_Menu(int Client)
{
	Menu MenuHandle = CreateMenu(Admin_Menu_Handle);
	SetMenuTitle(MenuHandle, "管理菜单");
	AddMenuItem(MenuHandle, "", "地图设置");
	SetMenuExitBackButton(MenuHandle, true);
	DisplayMenu(MenuHandle, Client, 30);
}
int Admin_Menu_Handle(Menu MenuHandle, MenuAction ActionHandle, int Client, int Item)
{
	if (ActionHandle == MenuAction_Select)
	{

	}
}
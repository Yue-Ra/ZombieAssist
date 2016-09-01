void Admin_Init(int Client)
{
	if (GetUserAdmin(Client) == INVALID_ADMIN_ID)
	{
		return;
	}
	CS_SetClientClanTag(Client, "Admin");
}
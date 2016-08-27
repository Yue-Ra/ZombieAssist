bool Alpha_Client[MAXPLAYERS + 1] = {true, ...};

bool Alpha_Allow = true;

void Alpha_Command(int Client, int Args)
{
	Alpha_Client = !Alpha_Client[Client];
	PrintToChat(Client, "[ZA]当前模型透明为 %s", Alpha_Client[Client] ? "开启" : "关闭");
}
void Alpha_OnGameFrame()
{
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckHuman(Client))
		{

		}
	}
}
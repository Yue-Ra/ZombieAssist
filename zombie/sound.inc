void Sound_Play()
{
	int Num = GetRandomInt(1, 5);
	for (int Client = 1; Client <= MaxClients; Client++)
	{
		if (CheckAlive(Client))
		{
			ClientCommand(Client, "play cola/%d.mp3", Num);
		}
	}
}
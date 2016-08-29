#include <sourcemod>
#include <sdktools_sound>
#include <clients>

bool firstblood;
int player[MAXPLAYERS + 1][3];
Handle player_timer[MAXPLAYERS + 1][2];

#define KILLING 0
#define TITLE 1
#define DEATH 2

#define KILL_INTERNAL 15.0

public OnPluginStart()
{
	Clean();
	firstblood = false;
	HookEvent("player_death", Event_PlayerDeath);
	HookEvent("round_freeze_end", Event_RoundFreeEnd);
}

Action Event_RoundFreeEnd(Event e, const char[] name, bool dontBroadcast)
{
	firstblood = true;
}

public OnMapStart()
{
	Clean();
}

void Clean()
{
	for(int i = 0; i < MAXPLAYERS + 1; i++)
	{
		player[i][0] = 0;
		player[i][1] = 0;
		player[i][2] = 0;
		player_timer[i][0] = null;
		player_timer[i][1] = null;
	}
}

Action Announcer_DecKilling(Handle timer, any userid)
{
	player[userid][KILLING] = 0;
	player_timer[userid][KILLING] = null;
	return Plugin_Handled;
}

public Action Event_PlayerDeath(Event e, const char[] name, bool dontBroadcast)
{
	int attacker = e.GetInt("attacker");
	int userid = e.GetInt("userid");
	int attacker_client = GetClientOfUserId(attacker);
	int victim_client = GetClientOfUserId(userid);

	player[userid][DEATH]++;

	if(attacker == userid)
	{
		Announcer_PostDeath(victim_client, player[userid][DEATH]);
		return Plugin_Continue;
	}

	player[attacker][KILLING]++;
	player[attacker][TITLE]++;
	player[attacker][DEATH] = 0;
	player[userid][TITLE] = 0;

	if(firstblood)
	{
		Announcer_PlaySound("announcer/firstblood.mp3");
		PrintToChatAll("%N拿下了第一滴血", attacker_client);
		PrintCenterTextAll("%N打下了第一滴血", attacker_client);
		firstblood = false;
	}

	switch(player[attacker][TITLE])
	{
		case 3:
		{
			Announcer_PlaySound("announcer/title_3_killingspree.mp3");
			PrintToChatAll("%N正在大杀特杀", attacker_client);
		}
		case 4:
		{
			Announcer_PlaySound("announcer/title_4_dominating.mp3");
			PrintToChatAll("%N已经主宰比赛", attacker_client);
		}
		case 5:
		{
			Announcer_PlaySound("announcer/title_5_megakill.mp3");
			PrintToChatAll("%N已经杀人如麻", attacker_client);
		}
		case 6:
		{
			Announcer_PlaySound("announcer/title_6_unstoppable.mp3");
			PrintToChatAll("%N无人能挡", attacker_client);
		}
		case 7:
		{
			Announcer_PlaySound("announcer/title_7_wickedsick.mp3");
			PrintToChatAll("%N已经变态杀戮", attacker_client);
		}
		case 8:
		{
			Announcer_PlaySound("announcer/title_8_monsterkill.mp3");
			PrintToChatAll("%N妖怪般的杀戮", attacker_client);
		}
		case 9:
		{
			Announcer_PlaySound("announcer/title_9_godlike.mp3");
			PrintToChatAll("%N如同神一般", attacker_client);
		}
		default:
		{
			if(player[attacker][TITLE] > 9)
			{
				Announcer_PlaySound("announcer/title_10_holyshit.mp3");
				PrintToChatAll("%N超越神一般的杀戮", attacker_client);
				PrintCenterTextAll("%N已经超越神一般的杀戮，拜托谁去爆了ta", attacker_client);
			}
			else
			{
				Announcer_PostKilling(player[attacker][KILLING], attacker_client);
				if(player_timer[attacker][KILLING] != null)
				{
					KillTimer(player_timer[attacker][KILLING]);
					player_timer[attacker][KILLING] = null;
				}
				player_timer[attacker][KILLING] = CreateTimer(KILL_INTERNAL, Announcer_DecKilling, attacker);
				return Plugin_Continue;
			}
		}

	}

	if(player_timer[attacker][TITLE] != null)
	{
		KillTimer(player_timer[attacker][TITLE]);
		player_timer[attacker][TITLE] = null;
	}
	DataPack pack;
	player_timer[attacker][TITLE] = CreateDataTimer(1.5, Announcer_DelayPostKilling, pack);
	pack.WriteCell(player[attacker][KILLING]);
	pack.WriteCell(attacker_client);

	if(player_timer[attacker][KILLING] != null)
	{
		KillTimer(player_timer[attacker][KILLING]);
		player_timer[attacker][KILLING] = null;
	}
	player_timer[attacker][KILLING] = CreateTimer(KILL_INTERNAL, Announcer_DecKilling, attacker);

	Announcer_PostDeath(victim_client, player[userid][DEATH]);
	return Plugin_Continue;
}

void Announcer_PlaySound(const char[] path)
{
	char command[255];

	Format(command, sizeof(command), "playgamesound %s", path);

	int target_playersConnected = GetMaxClients();

	for (int target_playerClient = 1; target_playerClient <= target_playersConnected; target_playerClient++)
	{
	    if (IsClientInGame(target_playerClient))
	    {
	        ClientCommand(target_playerClient, command);
	    }
	}
}

Action Announcer_DelayPostKilling(Handle timer, Handle pack)
{
	ResetPack(pack);
	int kill = ReadPackCell(pack);
	int attacker_client = ReadPackCell(pack);
	Announcer_PostKilling(kill, attacker_client);
	player_timer[GetClientUserId(attacker_client)][TITLE] = null;
	return Plugin_Handled;
}

void Announcer_PostKilling(int kill, int attacker_client)
{
	switch(kill)
	{
		case 2:
		{
			Announcer_PlaySound("announcer/kill_2.mp3");
			PrintToChatAll("%N完成一次双杀", attacker_client);
		}
		case 3:
		{
			Announcer_PlaySound("announcer/kill_3.mp3");
			PrintToChatAll("%N完成一次三杀", attacker_client);
		}
		case 4:
		{
			Announcer_PlaySound("announcer/kill_4.mp3");
			PrintToChatAll("%N正在疯狂杀戮", attacker_client);
		}
		default:
		{
			if(kill > 4)
			{
				Announcer_PlaySound("announcer/kill_5.mp3");
				PrintToChatAll("%N正在暴走", attacker_client);
			}
		}
	}
}

void Announcer_PostDeath(int client, int death)
{
	//PrintToChatAll("%N dead %d", client, death);
	if(death >= 10)
	{
		PrintCenterTextAll("%N已经超鬼了，谁可怜可怜给个菊花", client);
		PrintToChatAll("%N已经超鬼了，谁可怜可怜给个菊花", client);
	}
	else
	{
		switch(death)
		{
			case 3:
			{
				PrintToChatAll("%N正在大死特死", client);
			}
			case 4:
			{
				PrintToChatAll("%N已经被死亡主宰", client);
			}
			case 5:
			{
				PrintToChatAll("%N死地不计其数", client);
			}
			case 6:
			{
				PrintToChatAll("%N已经不可救药", client);
			}
			case 7:
			{
				PrintToChatAll("%N像个傻X", client);
			}
			case 8:
			{
				PrintToChatAll("%N送的根本停不下来", client);
			}
			case 9:
			{
				PrintToChatAll("%N猪一样的队友", client);
			}
		}
	}
}

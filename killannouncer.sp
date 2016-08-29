#include <sourcemod>
#include <sdktools>
#include <sdktools_sound>
#include <clients>
#include <sdktools_engine>

bool firstblood;
int player[MAXPLAYERS + 1][3];
Handle player_timer[MAXPLAYERS + 1][2];
char sound_firstblood[] = "announcer/firstblood.mp3";
char sound_kill[][32] = {
	"",
	"",
	"announcer/kill_2.mp3",
	"announcer/kill_3.mp3",
	"announcer/kill_4.mp3",
	"announcer/kill_5.mp3"
};
char sound_title[][32] = {
	"",
	"",
	"",
	"announcer/title_3.mp3",
	"announcer/title_4.mp3",
	"announcer/title_5.mp3",
	"announcer/title_6.mp3",
	"announcer/title_7.mp3",
	"announcer/title_8.mp3",
	"announcer/title_9.mp3",
	"announcer/title_10.mp3"
};
char string_firstblood[] = "%N拿下了第一滴血";
char string_kill[][32] = {
	"",
	"",
	"%N完成了一次双杀",
	"%N完成了一次三杀",
	"%N完成了一次疯狂杀戮",
	"%N正在暴走"
};
char string_title[][64] = {
	"",
	"",
	"",
	"%N正在大杀特杀",
	"%N已经主宰全场",
	"%N已经杀人如麻",
	"%N无人能挡",
	"%N正在变态杀戮",
	"%N像妖怪一般",
	"%N入神一般",
	"%N已经超越了神一般的存在，快去弄死"
};

#define KILLING 0
#define TITLE 1
#define DEATH 2

#define KILL_INTERNAL 15.0

public Plugin:myinfo =
{
	name = "Announcer",
	author = "まきちゃん~",
	description = "kill announcer",
	version = "1.1",
	url = "moeub.com"
};

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
	for(int i = 2; i < 6; i++)
	{
		PrintToServer("cached: %s", sound_kill[i]);
		char downloadFile[PLATFORM_MAX_PATH];
		PrecacheSound(sound_kill[i], true);
		Format(downloadFile, PLATFORM_MAX_PATH, "sound/%s", sound_kill[i]);
		AddFileToDownloadsTable(downloadFile);
	}

	for(int i = 3; i < 11; i++)
	{
		PrintToServer("cached: %s", sound_title[i]);
		char downloadFile[PLATFORM_MAX_PATH];
		PrecacheSound(sound_title[i], true);
		Format(downloadFile, PLATFORM_MAX_PATH, "sound/%s", sound_title[i]);
		AddFileToDownloadsTable(downloadFile);
	}

	char downloadFile[PLATFORM_MAX_PATH];
	PrecacheSound(sound_firstblood, true);
	Format(downloadFile, PLATFORM_MAX_PATH, "sound/%s", sound_firstblood);
	AddFileToDownloadsTable(downloadFile);

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
		PrintToChatAll(string_firstblood, attacker_client);
		PrintCenterTextAll(string_firstblood, attacker_client);
		firstblood = false;
	}

	if(player[attacker][TITLE] >= 3 && player[attacker][TITLE] <= 9)
	{
		Announcer_PlaySound(sound_title[player[attacker][TITLE]]);
		PrintToChatAll(string_title[player[attacker][TITLE]], attacker_client);
	}
	else if(player[attacker][TITLE] > 9)
	{
		Announcer_PlaySound(sound_title[10]);
		PrintToChatAll(string_title[10], attacker_client);
		PrintCenterTextAll(string_title[10], attacker_client);
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
			new Float:vec[3];
			GetClientEyePosition(target_playerClient, vec);
			EmitAmbientSound(path, vec, SOUND_FROM_WORLD);
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
	if(kill >=2 && kill <= 4)
	{
		Announcer_PlaySound(sound_kill[kill]);
		PrintToChatAll(string_kill[kill], attacker_client);
	}
	else if(kill >= 5)
	{
		Announcer_PlaySound(sound_kill[5]);
		PrintToChatAll(string_kill[5], attacker_client);
		PrintCenterTextAll(string_kill[5], attacker_client);
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

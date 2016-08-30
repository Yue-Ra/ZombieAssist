#pragma semicolon 1
#include <sourcemod>
#include <cstrike>
#include <sdkhooks>
#include <sdktools>
#include <zombiereloaded>
#include <stringescape>
#include "zombie/function"
#include "zombie/check"
#include "zombie/navbar"
#include "zombie/entity"
#include "zombie/mysql"
#include "zombie/client"
#include "zombie/map"
#include "zombie/alpha"
#include "zombie/model"
#include "zombie/weapon"
#include "zombie/damage"
#include "zombie/leader"
#include "zombie/event"

public Plugin myinfo =
{
	name = "Zombie Assist",
	author = "Yuera",
	description = "Zombiei Assist",
	version = "1.0",
	url = "http://www.moeub.com/"
};
public void OnPluginStart()
{
	MySQL_Start();
	Event_Init();
	RegConsoleCmd("sm_skin", Model_Command);
	RegConsoleCmd("sm_model", Model_Command);
	RegAdminCmd("sm_model_reload", Model_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_zbuy", Weapon_Command);
	RegConsoleCmd("sm_weapon", Weapon_Command);
	RegAdminCmd("sm_weapon_reload", Weapon_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_alpha", Alpha_Command);
	CreateTimer(1.0, Navbar_Timer, _, TIMER_REPEAT);
	CreateTimer(30.0, Entity_Timer, _, TIMER_REPEAT);
}
public void OnPluginEnd()
{
	MySQL_Close();
}
public void OnMapStart()
{
	Map_Init();
	Map_Load();
}
public void OnMapEnd()
{
	Map_Clean();
}
public void OnClientConnected(int Client)
{
	Client_Init(Client);
}
public void OnClientPutInServer(int Client)
{
	Client_Load(Client);
	Damage_Hook(Client);
}
public void OnClientDisconnect(int Client)
{
	Client_Clean(Client);
	Damage_Unhook(Client);
}
public void OnGameFrame()
{
	Alpha_OnGameFrame();
}
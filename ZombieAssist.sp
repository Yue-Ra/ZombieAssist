#pragma semicolon 1
#include <sourcemod>
#include <cstrike>
#include <sdkhooks>
#include <sdktools>
#include <zombiereloaded>
#include <morecolors>
#include <stringescape>
#include "zombie/function"
#include "zombie/time"
#include "zombie/alert"
#include "zombie/sound"
#include "zombie/check"
#include "zombie/mode"
#include "zombie/navbar"
#include "zombie/entity"
#include "zombie/mysql"
#include "zombie/client"
#include "zombie/alpha"
#include "zombie/info"
#include "zombie/model"
#include "zombie/weapon"
#include "zombie/damage"
#include "zombie/event"
#include "zombie/smoke"
#include "zombie/credit"
#include "zombie/level"
#include "zombie/infect"
#include "zombie/clan"
#include "zombie/map"
#include "zombie/translate"
#include "zombie/admin"

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
	MySQL_OnPluginStart();
	Alert_OnPluginStart();
	Event_OnPluginStart();
	Entity_OnPluginStart();
	RegConsoleCmd("sm_jifen", Credit_Command);
	RegAdminCmd("sm_test", Credit_Test, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_credit", Credit_Command);
	RegAdminCmd("sm_credit_reload", Credit_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("say", Say_Command);
	RegAdminCmd("sm_map_reload", Map_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_skin", Model_Command);
	RegAdminCmd("sm_info_reload", Info_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_model", Model_Command);
	RegAdminCmd("sm_model_reload", Model_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_zbuy", Weapon_Command);
	RegConsoleCmd("sm_weapon", Weapon_Command);
	RegAdminCmd("sm_weapon_reload", Weapon_Reload_Command, ADMFLAG_GENERIC);
	RegConsoleCmd("sm_alpha", Alpha_Command);
	RegAdminCmd("sm_zeadmin", Admin_Command, ADMFLAG_GENERIC);
	CreateTimer(1.0, Level_Timer, _, TIMER_REPEAT);
	CreateTimer(1.0, Navbar_Timer, _, TIMER_REPEAT);
	CreateTimer(20.0, Info_Timer, _, TIMER_REPEAT);
	CreateTimer(30.0, Entity_Timer, _, TIMER_REPEAT);
}
public void OnPluginEnd()
{
	MySQL_OnPluginEnd();
}
public void OnMapStart()
{
	Alpha_OnMapStart();
	Credit_OnMapStart();
	Weapon_OnMapStart();
	Damage_OnMapStart();
	Map_Init();
	Map_Load();
	Info_Load();
	Alert_Init();
	Model_Load();
	Credit_Load();
	Weapon_Load();
}
public void OnMapEnd()
{
	Map_OnMapEnd();
	Mode_OnMapEnd();
}
public void OnConfigsExecuted()
{
	Mode_OnConfigsExecuted();
}
public void OnClientConnected(int Client)
{
	Client_Init(Client);
}
public void OnClientPutInServer(int Client)
{
	Admin_Init(Client);
	Damage_Init(Client);
	Client_Load(Client);
	Damage_Hook(Client);
	Alpha_OnClientPutInServer(Client);
	Credit_OnClientPutInServer(Client);
	Weapon_OnClientPutInServer(Client);
}
public void OnClientDisconnect(int Client)
{
	Client_OnClientDisconnect(Client);
	Damage_OnClientDisconnect(Client);
}
public void OnGameFrame()
{
	Alpha_OnGameFrame();
}
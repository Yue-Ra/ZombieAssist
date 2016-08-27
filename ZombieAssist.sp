#include <sourcemod>
#include <cstrike>
#include <sdkhooks>
#include <sdktools>
#include <zombiereloaded>
#include "zombie/check"
#include "zombie/mysql"
#include "zombie/client"
#include "zombie/map"
#include "zombie/model"

public void OnPluginStart()
{
	MySQL_Start();
	RegConsoleCmd("sm_skin", Model_Command);
	RegConsoleCmd("sm_model", Model_Command);
	RegAdminCmd("sm_model_reload", Model_Reload_Command, ADMFLAG_GENERIC);
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
}
public void OnClientDisconnect(int Client)
{
	Client_Clean(Client);
}
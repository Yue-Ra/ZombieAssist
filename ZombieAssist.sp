#include <sourcemod>
#include <cstrike>
#include <sdkhooks>
#include <sdktools>
#include <zombiereloaded>
#include "zombie/check"
#include "zombie/mysql"
#include "zombie/client"

public void OnPluginStart()
{
	MySQL_Start();
}
public void OnPluginEnd()
{
	MySQL_Close();
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
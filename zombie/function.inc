stock void ClearTimer(& Handle: Timer)
{
	if (Timer == INVALID_HANDLE)
	{
		return;
	}
	KillTimer(Timer);
	Timer = INVALID_HANDLE;
}
stock void SetAlpha(int Client, int Value)
{
	if (!IsPlayerAlive(Client))
	{
		return;
	}
	if (GetEntityRenderMode(Client) != RENDER_TRANSALPHA)
	{
		SetEntityRenderMode(Client, RENDER_TRANSALPHA);
	}
	SetEntityRenderColor(Client, 255, 255, 255, Value);
}
stock void AddHealth(int Client, int Value)
{
	if (!IsPlayerAlive(Client))
	{
		return;
	}
	SetEntityHealth(Client, GetClientHealth(Client) + Value);
}
stock int GetWeapon(int Client)
{
	return GetEntPropEnt(Client, Prop_Send, "m_hActiveWeapon");
}
stock int GetAmmo(int Weapon)
{
	return GetEntProp(Weapon, Prop_Data, "m_iClip1");
}
stock void SetAmmo(int Weapon, int Value)
{
	SetEntProp(Weapon, Prop_Data, "m_iClip1", Value);
}
stock int GetCash(int Client)
{
	return GetEntData(Client, FindSendPropInfo("CCSPlayer", "m_iAccount"));
}
stock void SetCash(int Client, int Value)
{
	SetEntData(Client, FindSendPropInfo("CCSPlayer", "m_iAccount"), Value);
}
stock int GetArmour(int Client)
{
	return GetEntProp(Client, Prop_Send, "m_ArmorValue");
}
stock void SetArmour(int Client, int Value)
{
	SetEntProp(Client, Prop_Send, "m_ArmorValue", Value);
	SetEntProp(Client, Prop_Send, "m_bHasHelmet", Value ? 1 : 0);
}
stock int GetOwner(int Entity)
{
	return GetEntDataEnt2(Entity, FindSendPropInfo("CBaseCombatWeapon", "m_hOwnerEntity"));
}
stock int GetHammerId(int Entity)
{
	return GetEntProp(Entity, Prop_Data, "m_iHammerID");
}
stock void SetConVarIntValue(const char[] Name, int Value, bool Replace)
{
	Handle Var = FindConVar(Name);
	if (Var == INVALID_HANDLE)
	{
		return;
	}
	SetConVarInt(Var, Value, Replace);
}
stock void SetConVarFloatValue(const char[] Name, float Value, bool Replace)
{
	Handle Var = FindConVar(Name);
	if (Var == INVALID_HANDLE)
	{
		return;
	}
	SetConVarFloat(Var, Value, Replace);
}
stock void SendKeyHintTextMsg(int Client, char[] Msg)
{
	Handle Hud = INVALID_HANDLE;
	if (Client == 0)
	{
		Hud = StartMessageAll("KeyHintText");
	}
	else
	{
		Hud = StartMessageOne("KeyHintText", Client);
	}
	char Info[256];
	VFormat(Info, sizeof(Info), Msg, 3);
	if (Hud == INVALID_HANDLE)
	{
		return;
	}
	BfWriteByte(Hud, 1);
	BfWriteString(Hud, Info);
	EndMessage();
}
stock void SendHudTextMsg(int Client, float X, float Y, float Time, int Red, int Green, int Blue, char[] Msg)
{
	Handle Hud = CreateHudSynchronizer();
	SetHudTextParams(X, Y, Time, Red, Green, Blue, 0, 1, 0.0, 0.0, 0.0);
	if (Client == 0)
	{
		for (int Target = 1; Target <= MaxClients; Target++)
		{
			if (CheckAlive(Target))
			{
				ShowSyncHudText(Target, Hud, Msg);
			}
		}
	}
	else
	{
		ShowSyncHudText(Client, Hud, Msg);
	}
	CreateTimer(Time, Hud_Timer, Hud);
}
Action Hud_Timer(Handle Timer, any Hud)
{
	if (Hud == INVALID_HANDLE)
	{
		return Plugin_Handled;
	}
	CloseHandle(Hud);
	Hud = INVALID_HANDLE;
	return Plugin_Handled;
}
stock void DealDamage(int Client, int Damage, int Attack = 0, int Type = DMG_GENERIC, char Weapon[] = "")
{
	if (!CheckAlive(Client) || Damage <= 0)
	{
		return;
	}
	char DamageStr[16];
	IntToString(Damage, DamageStr, sizeof(DamageStr));
	char TypeStr[32];
	IntToString(Type, TypeStr, sizeof(TypeStr));
	int Entity = CreateEntityByName("point_hurt");
	if (Entity == -1)
	{
		return;
	}
	DispatchKeyValue(Client, "targetname", "war3_hurtme");
	DispatchKeyValue(Entity, "DamageTarget", "war3_hurtme");
	DispatchKeyValue(Entity, "Damage", DamageStr);
	DispatchKeyValue(Entity, "DamageType", TypeStr);
	if (!StrEqual(Weapon, "", false))
	{
		DispatchKeyValue(Entity, "classname", Weapon);
	}
	DispatchSpawn(Entity);
	AcceptEntityInput(Entity, "Hurt", (Attack > 0) ? Attack : -1);
	DispatchKeyValue(Entity, "classname", "point_hurt");
	DispatchKeyValue(Client, "targetname", "war3_donthurtme");
	AcceptEntityInput(Entity, "Kill");
}
stock void makeExplosion(int Attack = 0, int Inflictor = -1, const float Position[3], const char Weapon[] = "", int Magnitude = 100, int Radius = 0, float Damage = 0.0, int Flags = 0)
{
	int Entity = CreateEntityByName("env_explosion");
	if (Entity == -1)
	{
		return;
	}
	DispatchKeyValueVector(Entity, "Origin", Position);
	char Buffer[64];
	IntToString(Magnitude, Buffer, sizeof(Buffer));
	DispatchKeyValue(Entity, "iMagnitude", Buffer);
	if (Radius > 0)
	{
		IntToString(Radius, Buffer, sizeof(Buffer));
		DispatchKeyValue(Entity, "iRadiusOverride", Buffer);
	}
	if (Damage > 0.0)
	{
		DispatchKeyValueFloat(Entity, "Damage", Damage);
	}
	if (Flags != 0)
	{
		IntToString(Flags, Buffer, sizeof(Buffer));
		DispatchKeyValue(Entity, "spawnflags", Buffer);
	}
	if (!StrEqual(Weapon, "", false))
	{
		DispatchKeyValue(Entity, "classname", Weapon);
		if (Inflictor != -1)
		{
			DispatchKeyValue(Inflictor, "classname", Weapon);
		}
	}
	DispatchSpawn(Entity);
	if (IsClientInGame(Attack))
	{
		SetEntPropEnt(Entity, Prop_Send, "m_hOwnerEntity", Attack);
	}
	if (Inflictor != -1)
	{
		SetEntPropEnt(Entity, Prop_Data, "m_hInflictor", Inflictor);
	}
	AcceptEntityInput(Entity, "Explode");
	AcceptEntityInput(Entity, "Kill");
}
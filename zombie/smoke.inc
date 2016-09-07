void Smoke_Client(int Client)
{
	float Origin[3];
	GetClientEyePosition(Client, Origin);
	float Angles[3];
	GetClientEyeAngles(Client, Angles);
	CreateSmoke(Client, Origin, Angles);
	float Vector[3];
	GetAngleVectors(Angles, Vector, NULL_VECTOR, NULL_VECTOR);
	float Points[3];
	Points[0] = Origin[0] + Vector[0] * 100.0;
	Points[1] = Origin[1] + Vector[1] * 100.0;
	Points[2] = Origin[2] + Vector[2] * 100.0;
	Handle Trace = TR_TraceRayFilterEx(Origin, Points, MASK_PLAYERSOLID, RayType_EndPoint, Smoke_Filter, Client);
	CloseHandle(Trace);
}
bool Smoke_Filter(int Entity, int Mask, any Client)
{
	if (Entity == Client)
	{
		return false;
	}
	if (CheckHuman(Entity))
	{
		DealDamage(Entity, GetEntityFlags(Entity) & FL_DUCKING ? 180 : 90, Client, DMG_SLASH);
		return true;
	}
	return false;
}
void CreateSmoke(int Client, float Origin[3], float Angles[3])
{
	int Entity = CreateEntityByName("env_steam");
	char Name[32];
	Format(Name, sizeof(Name), "target%d", Client);
	DispatchKeyValue(Client, "targetname", Name);
	PrecacheSound("weapons/rpg/rocketfire1.wav", true);
	EmitSoundToAll("weapons/rpg/rocketfire1.wav", _, _, _, _, 1.0);
	Format(Name, sizeof(Name), "smoke%d", Client);
	DispatchKeyValue(Client, "targetname", Name);
	Format(Name, sizeof(Name), "target%d", Client);
	DispatchKeyValue(Entity, "parentname", Name);
	DispatchKeyValue(Entity, "SpawnFlags", "1");
	DispatchKeyValue(Entity, "Type", "0");
	DispatchKeyValue(Entity, "InitialState", "1");
	DispatchKeyValue(Entity, "Spreadspeed", "10");
	DispatchKeyValue(Entity, "Speed", "800");
	DispatchKeyValue(Entity, "Startsize", "15");
	DispatchKeyValue(Entity, "EndSize", "250");
	DispatchKeyValue(Entity, "Rate", "30");
	DispatchKeyValue(Entity, "JetLength", "100");
	DispatchKeyValue(Entity, "RenderColor", "20 225 8");
	DispatchKeyValue(Entity, "RenderAmt", "180");
	DispatchSpawn(Entity);
	TeleportEntity(Entity, Origin, Angles, NULL_VECTOR);
	SetVariantString(Name);
	AcceptEntityInput(Entity, "SetParent", Entity, Entity, 0);
	CreateTimer(0.5, Smoke_Timer, Entity);
}
Action Smoke_Timer(Handle Timer, any Entity)
{
	if (IsValidEntity(Entity))
	{
		AcceptEntityInput(Entity, "Kill");
	}
	return Plugin_Handled;
}
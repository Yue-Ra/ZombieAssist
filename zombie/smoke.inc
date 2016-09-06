void Smoke_Client(int Client)
{
	float Origin[3];
	GetClientEyePosition(Client, Origin);
	float Angles[3];
	GetClientEyeAngles(Client, Angles);
	CreateSmoke(Client, Origin, Angles);
}
void CreateSmoke(int Client, float Origin[3], float Angles[3])
{
	int Entity = CreateEntityByName("env_steam");
	char Name[32];
	Format(Name, sizeof(Name), "Smoke%d", Client);
	DispatchKeyValue(Entity, "TargetName", Name);
	Format(Name, sizeof(Name), "Target%d", Client);
	DispatchKeyValue(Entity, "ParentName", Name);
	DispatchKeyValue(Entity, "InitialState", "1");
	DispatchKeyValue(Entity, "ParticleType", "0");
	DispatchKeyValue(Entity, "SpreadSpeed", "10");
	DispatchKeyValue(Entity, "SpreadSpeed", "800");
	DispatchKeyValue(Entity, "StartSize", "10");
	DispatchKeyValue(Entity, "EndSize", "10");
	DispatchKeyValue(Entity, "Rate", "30");
	DispatchKeyValue(Entity, "JetLength", "100");
	DispatchKeyValue(Entity, "RenderColor", "0 225 0");
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
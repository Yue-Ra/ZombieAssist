bool Mode_True = false;
bool Mode_Start = false;

void Mode_OnConfigsExecuted()
{
	SetConVarIntValue("sm_parachute_enabled", 0, false);
	SetConVarIntValue("sm_parachute_roundmsg", 0, false);
	SetConVarIntValue("sm_parachute_welcome", 0, false);
	SetConVarIntValue("sm_parachute_msgtype", 0, false);
	SetConVarIntValue("sm_parachute_button", 2, false);
	SetConVarIntValue("sv_enablebunnyhopping", 0, true);
	SetConVarIntValue("sv_gravity", 800, true);
	if (!Mode_True)
	{
		return;
	}
	SetConVarIntValue("mp_timelimit", 15, true);
	SetConVarFloatValue("mp_roundtime", 3.5, true);
	SetConVarIntValue("sm_parachute_enabled", 1, false);
	SetConVarIntValue("sv_enablebunnyhopping", 1, true);
	SetConVarIntValue("zr_account_cashdmg", 1, false);
	SetConVarIntValue("zr_damage_suicide_human", 0, false);
	SetConVarIntValue("zr_infect_mzombie_countdown", 0, false);
	SetConVarIntValue("zr_infect_mzombie_respawn", 0, false);
	SetConVarIntValue("zr_ztele_human_after", 1, false);
	SetConVarIntValue("zr_ztele_max_human", 3, false);
	SetConVarIntValue("zr_ztele_max_zombie", 5, false);
	SetConVarFloatValue("zr_infect_spawntime_max", 15.0, false);
	SetConVarFloatValue("zr_infect_spawntime_min", 5.0, false);
	SetConVarFloatValue("zr_ztele_delay_human", 3.0, false);
}
void Mode_Check(int Time)
{
	if (Time == 180)
	{
		Mode_Start = true;
		CPrintToChatAll("{green}[ZA][感染] {blue}疫苗研究已完成");
		PrintCenterTextAll("[感染]疫苗研究已完成");
	}
}
void Mode_OnMapEnd()
{
	if (Mode_True)
	{
		Mode_True = false;
	}
}
void Mode_OnRoundEnd()
{
	if (Mode_Start)
	{
		Mode_Start = false;
	}
}
bool Mode_True = false;
bool Mode_Start = false;

void Mode_OnConfigsExecuted()
{
	SetConVarInt(FindConVar("sm_parachute_enabled"), 0, false, false);
	SetConVarInt(FindConVar("sm_parachute_roundmsg"), 0, false, false);
	SetConVarInt(FindConVar("sm_parachute_welcome"), 0, false, false);
	SetConVarInt(FindConVar("sm_parachute_msgtype"), 0, false, false);
	SetConVarInt(FindConVar("sm_parachute_button"), 2, false, false);
	SetConVarInt(FindConVar("sv_enablebunnyhopping"), 0, true, false);
	SetConVarInt(FindConVar("sv_gravity"), 800, true, false);
	SetConVarFloat(FindConVar("mp_roundtime"), 10.0, true, false);
	if (!Mode_True)
	{
		return;
	}
	SetConVarInt(FindConVar("mp_timelimit"), 30, true, false);
	SetConVarInt(FindConVar("sm_parachute_enabled"), 1, false, false);
	SetConVarInt(FindConVar("sv_enablebunnyhopping"), 1, true, false);
	SetConVarInt(FindConVar("zr_account_cashdmg"), 1, false, false);
	SetConVarInt(FindConVar("zr_damage_suicide_human"), 0, false, false);
	SetConVarInt(FindConVar("zr_infect_mzombie_countdown"), 0, false, false);
	SetConVarInt(FindConVar("zr_infect_mzombie_respawn"), 0, false, false);
	SetConVarInt(FindConVar("zr_ztele_human_after"), 1, false, false);
	SetConVarInt(FindConVar("zr_ztele_max_human"), 3, false, false);
	SetConVarInt(FindConVar("zr_ztele_max_zombie"), 5, false, false);
	SetConVarFloat(FindConVar("mp_roundtime"), 3.5, true, false);
	SetConVarFloat(FindConVar("zr_infect_spawntime_max"), 15.0, false, false);
	SetConVarFloat(FindConVar("zr_infect_spawntime_min"), 5.0, false, false);
	SetConVarFloat(FindConVar("zr_ztele_delay_human"), 3.0, false, false);
}
void Mode_Check(int Time)
{
	if (Time == 180)
	{
		Mode_Start = true;
		CPrintToChatAll("{green}[ZA][感染]{blue} 疫苗研究已完成");
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
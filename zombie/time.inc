int Time_Time = 0;
Handle Time_Handle = INVALID_HANDLE;

void Time_OnRoundFreezeEnd()
{
	Time_Time = 0;
	Time_Handle = CreateTimer(1.0, Time_Timer, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}
void Time_OnRoundStart()
{
	ClearTimer(Time_Handle);
}
Action Time_Timer(Handle Time_Timer)
{
	Time_Time++;
	return Plugin_Handled;
}
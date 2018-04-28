//#include "Features.h"
//#include "Settings.h"
//#include "Entity.h"
//#include "Interfaces.h"
//#include "Convar.h"
//#include "Resolver.h"
//
//ConVar *sv_unlag;
//ConVar *sv_maxunlag;
//ConVar *sv_lagflushbonecache;
//ConVar *sv_lagpushticks;
//ConVar *sv_client_min_interp_ratio;
//ConVar *sv_client_max_interp_ratio;
//ConVar *sv_maxupdaterate;
//ConVar *sv_minupdaterate;
//ConVar *cl_updaterate;
//ConVar *cl_interp_ratio = nullptr;
//
//void SaveNetvars(StoredNetvars *dest, CBaseEntity *Entity, CBaseEntity *pCPlayer)
//{
//	dest->aimpunch = Entity->GetPunch();
//	dest->angles = Entity->GetAngleRotation();
//	dest->mins = Entity->GetMins();
//	dest->maxs = Entity->GetMaxs();
//	dest->lowerbodyyaw = Entity->GetLowerBodyYaw();
//	dest->fallvelocity = Entity->GetFallVelocity();
//	dest->origin = Entity->GetNetworkOrigin(); //Entity->GetOrigin();
//	dest->networkorigin = dest->origin;
//	dest->viewoffset = Entity->GetViewOffset();
//	dest->velocity = Entity->GetVelocity();
//	dest->velocitymodifier = Entity->GetVelocityModifier();
//	dest->flags = Entity->GetFlags();
//	//dest->headorigin = Entity->GetBonePosition(HITBOX_HEAD);
//	dest->cycle = Entity->GetCycle();
//	dest->basevelocity = Entity->GetBaseVelocity();
//	dest->playbackrate = Entity->GetPlaybackRate();
//	dest->sequence = Entity->GetSequence();
//	dest->simulationtime = Entity->GetSimulationTime();
//	dest->groundentity = Entity->GetGroundEntity();
//	dest->eyeangles = pCPlayer->EyeAngles;
//	dest->eyeyaw = pCPlayer->EyeAngles.y;
//	dest->isducked = Entity->IsDucked();
//	dest->isducking = Entity->IsDucking();
//	dest->isinduckjump = Entity->IsInDuckJump();
//	dest->ducktimemsecs = Entity->GetDuckTimeMsecs();
//	dest->jumptimemsecs = Entity->GetJumpTimeMsecs();
//	dest->duckamount = Entity->GetDuckAmount();
//	dest->duckspeed = Entity->GetDuckSpeed();
//	dest->laggedmovement = Entity->GetLaggedMovement();
//	dest->laddernormal = Entity->GetVecLadderNormal();
//	dest->animtime = Entity->GetAnimTime();
//	float* flPose = (float*)((DWORD)Entity + m_flPoseParameter);
//	for (int i = 0; i < 23; i++)
//	{
//		dest->flPoseParameter[i] = ReadFloat((uintptr_t)&flPose[i]);
//	}
//}
//
//void RestoreNetvars(StoredNetvars *src, CBaseEntity *Entity)
//{
//	Entity->SetViewOffset(src->viewoffset);
//	Entity->SetVelocity(src->velocity);
//	Entity->SetVelocityModifier(src->velocitymodifier);
//	Entity->SetFallVelocity(src->fallvelocity);
//	Entity->SetFlags(src->flags);
//	//Entity->SetSimulationTime(src->simulationtime); //DO NOT DO THIS
//	Entity->SetBaseVelocity(src->basevelocity);
//	Entity->SetCycle(src->cycle);
//	Entity->SetMaxs(src->maxs);
//	Entity->SetMins(src->mins);
//	//Entity->SetOrigin(src->networkorigin);// src->origin);
//	//Entity->SetNetworkOrigin(src->networkorigin);
//	Entity->SetSequence(src->sequence);
//	Entity->SetPlaybackRate(src->playbackrate);
//	Entity->SetPunch(src->aimpunch);
//	Entity->SetAngleRotation(src->angles);
//	//Entity->SetLowerBodyYaw(src->lowerbodyyaw);
//	Entity->SetGroundEntity(src->groundentity); //could possibly crash if entity is gone
//	Entity->SetEyeAngles(src->eyeangles);
//	Entity->SetVecLadderNormal(src->laddernormal);
//	Entity->SetDucked(src->isducked);
//	Entity->SetDucking(src->isducking);
//	Entity->SetInDuckJump(src->isinduckjump);
//	Entity->SetDuckTimeMsecs(src->ducktimemsecs);
//	Entity->SetJumpTimeMsecs(src->jumptimemsecs);
//	Entity->SetDuckAmount(src->duckamount);
//	Entity->SetDuckSpeed(src->duckspeed);
//	Entity->SetLaggedMovement(src->laggedmovement);
//	Entity->SetAnimTime(src->animtime);
//	float* flPose = (float*)((DWORD)Entity + m_flPoseParameter);
//	for (int i = 0; i < 23; i++)
//	{
//		WriteFloat((uintptr_t)&flPose[i], src->flPoseParameter[i]);
//	}
//}
//
//int GetEstimatedServerTickCount(float latency)
//{
//	return (int)floorf((float)((float)(latency) / ReadFloat((uintptr_t)&Interfaces::Globals->interval_per_tick)) + 0.5) + 1 + ReadInt((uintptr_t)&Interfaces::Globals->tickcount);
//}
//
//float inline GetEstimatedServerTime()
//{
//	return LocalPlayer->GetTickBase() * ReadFloat((uintptr_t)&Interfaces::Globals->interval_per_tick);
//}
//
//float GetNetworkLatency()
//{
//	// Get true latency
//	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
//	if (nci)
//	{
//		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
//		//float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
//		return IncomingLatency;// +OutgoingLatency;
//	}
//	return 0.0f;
//}
//
////See if a tick is within limits to be accepted by the server for hit registration
//bool bTickIsValid(int tick)
//{
//#define ALIENSWARM_VERSION
//#ifdef ALIENSWARM_VERSION
//	float latency = GetNetworkLatency();
//	const float SV_MAXUNLAG = 1.0f;
//	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
//	if (RemoveInterpChk.Checked)
//		tick += TIME_TO_TICKS(m_flLerpTime);
//	float correct = clamp(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);
//	float flTargetTime = TICKS_TO_TIME(tick) - m_flLerpTime;
//	float deltaTime = correct - (GetEstimatedServerTime() - flTargetTime);
//	if (fabs(deltaTime) > 0.2f)
//	{
//		return false;
//	}
//	return true;
//#else
//	//SDK 2013 VERSION
//	float latency = GetNetworkLatency();
//	const float SV_MAXUNLAG = 1.0f;
//	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
//	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
//	if (RemoveInterpChk.Checked)
//		tick += lerpTicks;
//	float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
//	int targettick = tick - lerpTicks;
//	float deltaTime = correct - TICKS_TO_TIME(GetEstimatedServerTickCount(latency) - targettick);
//	if (fabs(deltaTime) > 0.2f)
//	{
//		return false;
//	}
//	return true;
//#endif
//}
//
//extern void GetIdealPitch(float& pitch, int correctedindex, CBaseEntity* pCPlayer, CBaseEntity* Entity, CBaseEntity* pLocalEntity);
//
////Update was received from the server, so update netvars
//void BeginLagCompensation(CBaseEntity *pCPlayer, CBaseEntity* Entity)
//{
//	if (!pCPlayer->Dormant && !pCPlayer->Health <= 0)
//	{
//		bool bLowerBodyUpdated = false;
//		bool bIsAimingAtUs = Entity->IsTargetingLocal();
//		float curtime = ReadFloat((uintptr_t)&Interfaces::Globals->curtime);
//		int tickcount = ReadInt((uintptr_t)&Interfaces::Globals->tickcount);
//		float flCurrentBodyYaw = Entity->GetLowerBodyYaw();
//		StoredNetvars *LastUpdatedNetVars = &pCPlayer->LastUpdatedNetVars;
//		StoredNetvars *CurrentNetVars = &pCPlayer->CurrentNetVars;
//
//		//Find out how many ticks player is choking
//		float flSimulationTime = Entity->GetSimulationTime();
//		float flSimDiff = curtime - flSimulationTime;
//		float latency = GetNetworkLatency();
//		pCPlayer->tickschoked = TIME_TO_TICKS(max(0.0f, flSimDiff - latency));
//
//		float PlayerIsMoving = abs(Entity->GetVelocity().Length());
//
//		//Save current netvars received from the server
//		//if (!pCPlayer->tickschoked)
//		SaveNetvars(CurrentNetVars, Entity, pCPlayer);
//
//		if (LastUpdatedNetVars->lowerbodyyaw != flCurrentBodyYaw)
//		{
//			bLowerBodyUpdated = true;
//			pCPlayer->TimeLowerBodyUpdated = curtime;
//		}
//
//		float bodyeyedelta = pCPlayer->EyeAngles.y - flCurrentBodyYaw;
//		NormalizeAngle(bodyeyedelta);
//		bodyeyedelta = fabsf(bodyeyedelta);
//
//		if (GetAsyncKeyState(VK_CAPITAL))
//		{
//			//Force eye angles to lower body yaw..
//			Entity->SetEyeAngles(QAngle(pCPlayer->EyeAngles.x, flCurrentBodyYaw, 0.0f));
//		}
//
//		if (PlayerIsMoving || bLowerBodyUpdated || bIsAimingAtUs)// || LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
//		{
//			pCPlayer->LastUpdatedNetVars = pCPlayer->CurrentNetVars;;
//
//			//If player is aiming at us, don't use lower body yaw for yaw
//			if (bLowerBodyUpdated || (PlayerIsMoving && bodyeyedelta >= 35.0f) && !bIsAimingAtUs)
//			{
//				LastUpdatedNetVars->eyeangles.y = flCurrentBodyYaw;
//			}
//
//			pCPlayer->IsUsingFakeAngles = false;
//			LastUpdatedNetVars->tickcount = pCPlayer->tickschoked ? TIME_TO_TICKS(flSimulationTime) : tickcount;
//		}
//		else
//		{
//			//See if player is faking their angles
//			if (bodyeyedelta > 35.0f && curtime - pCPlayer->TimeLowerBodyUpdated > 2.5f)
//			{
//				//Force an eye angle update
//				LastUpdatedNetVars->eyeangles.y = LastUpdatedNetVars->lowerbodyyaw;
//				//LastUpdatedNetVars->tickcount = pCPlayer->tickschoked ? TIME_TO_TICKS(flSimulationTime) : tickcount;
//				pCPlayer->TimeLowerBodyUpdated = curtime;
//				pCPlayer->IsUsingFakeAngles = true;
//			}
//			else
//			{
//				pCPlayer->IsUsingFakeAngles = false;
//			}
//		}
//	}
//}
//
////Restores player back to most recent server update if possible
//bool BacktrackPlayer(CBaseEntity *pCPlayer, CBaseEntity* pLocalEntity, CBaseEntity* Entity)
//{
//	if (!pCPlayer->Dormant && !pCPlayer->Health <= 0)
//	{
//		StoredNetvars *LastUpdatedNetVars = &pCPlayer->LastUpdatedNetVars;
//		StoredNetvars *CurrentNetVars = &pCPlayer->CurrentNetVars;
//		int tickcount = ReadInt((uintptr_t)&Interfaces::Globals->tickcount);
//
//		bool IsUsingFakeAngles = pCPlayer->IsUsingFakeAngles;
//		bool TickIsValid = bTickIsValid(LastUpdatedNetVars->tickcount);
//
//		//If we have new enough data to backtrack to, then force the player back in time as long as we can.
//
//		if (IsUsingFakeAngles || TickIsValid)
//		{
//			//Restore player's netvars to the last values received from the server
//			RestoreNetvars(&pCPlayer->LastUpdatedNetVars, Entity);
//			QAngle neweyeangles = LastUpdatedNetVars->eyeangles;
//			if (IsUsingFakeAngles)
//			{
//				neweyeangles.y = LastUpdatedNetVars->lowerbodyyaw;
//			}
//			Personalize *pPersonalize = &pCPlayer->Personalize;
//			//Allow player to correct their pitch, or let automatic pitch do its thing
//			int correctedindex = pPersonalize->isCorrected ? pPersonalize->correctedindex : pPersonalize->miss;
//			GetIdealPitch(neweyeangles.x, correctedindex, pCPlayer, Entity, pLocalEntity);
//			ClampX(neweyeangles.x);
//			Entity->SetEyeAngles(neweyeangles);
//			//Let other functions know that the player is backtracked
//			if (TickIsValid || !IsUsingFakeAngles)
//			{
//				pCPlayer->Backtracked = true;
//			}
//			return true;
//		}
//		//Check to see if we are backtracked, if we are then restore back to current netvars
//		if (pCPlayer->Backtracked)
//		{
//			//if (!IsUsingFakeAngles)
//			RestoreNetvars(&pCPlayer->CurrentNetVars, Entity);
//			pCPlayer->Backtracked = false;
//		}
//	}
//	return false;
//}
//
////Called on frame end to restore players back to current netvars
//void FinishLagCompensation(CBaseEntity *pLocalEntity)
//{
//	//Do we need this anymore now that we restore on new frame already?
//	return;
//
//	for (int i = 0; i < MAX_PLAYERS; i++)
//	{
//		CBaseEntity* Entity = Interfaces::ClientEntList->GetClientEntity(i);
//		if (Entity && Entity != pLocalEntity && Entity->IsPlayer())
//		{
//			CBaseEntity* pCPlayer = &AllPlayers[ReadInt((uintptr_t)&Entity->index)];
//			if (pCPlayer->Backtracked)
//			{
//				//FIXME: THIS BREAKS DOWN FAKE STATIC SOMETIMES FROM EVOLVE
//				RestoreNetvars(&pCPlayer->CurrentNetVars, Entity);
//			}
//		}
//	}
//}
//
////Called on new frame to backtrack player if possible
//void BacktrackPlayersOnNewFrame(CBaseEntity *pLocalEntity)
//{
//	for (int i = 0; i < MAX_PLAYERS; i++)
//	{
//		CBaseEntity* Entity = Interfaces::ClientEntList->GetClientEntity(i);
//		if (Entity && Entity != pLocalEntity && Entity->IsPlayer())
//		{
//			CBaseEntity* pCPlayer = &AllPlayers[ReadInt((uintptr_t)&Entity->index)];
//			if (!pCPlayer->Dormant && pCPlayer->Health > 0 && pCPlayer->Personalize.ResolvePitch || pCPlayer->Personalize.ResolveYaw)
//			{
//				if (!BacktrackPlayer(pCPlayer, pLocalEntity, Entity))
//				{
//					return;
//					StoredNetvars *LastUpdatedNetVars = &pCPlayer->LastUpdatedNetVars;
//					StoredNetvars *CurrentNetVars = &pCPlayer->CurrentNetVars;
//					//If data updated since last lower body yaw, check to see if it's been long enough to force an update
//					//if (LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
//					{
//						int tickcount = ReadInt((uintptr_t)&Interfaces::Globals->tickcount);
//						if (/*LastUpdatedNetVars->eyeangles.x == CurrentNetVars->eyeangles.x && LastUpdatedNetVars->eyeyaw == CurrentNetVars->eyeangles.y || */tickcount - LastUpdatedNetVars->tickcount > TIME_TO_TICKS(5.0f))
//						{
//							//Too long since lower body yaw update, force an update
//							pCPlayer->LastUpdatedNetVars = pCPlayer->CurrentNetVars;
//							LastUpdatedNetVars->tickcount = tickcount;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void PostCreateMoveLagCompensation(bool bLocalPlayerIsAlive, int buttons, CUserCmd *cmd, CBaseCombatWeapon *weapon)
//{
//	if (LagCompensationConvarsReceived && bLocalPlayerIsAlive && ((buttons & IN_ATTACK) || (buttons & IN_ATTACK2 && weapon && weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)))
//	{
//		int lerpTicks = RemoveInterpChk.Checked ? TIME_TO_TICKS(cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat()) : 0;
//
//		if (LastTargetIndex != INVALID_PLAYER)
//		{
//			CBaseEntity *pCPlayer = &AllPlayers[LastTargetIndex];
//			int tickcount = ReadInt((uintptr_t)&Interfaces::Globals->tickcount);
//			if (ResolverChk.Checked && pCPlayer->Backtracked /*&& (tickcount - pCPlayer->LastUpdatedNetVars.tickcount) < (tickcount - GetOldestPossibleBacktrackTick())*/)
//			{
//#if 0
//				//If target is fakelagging, only shoot them if they are the peak of their jump
//				if (pCPlayer->tickschoked > 0 && !(pCPlayer->CurrentNetVars.flags & FL_ONGROUND) && (fabsf(pCPlayer->CurrentNetVars.velocity.z) > 5.5f))
//				{
//					cmd->buttons &= ~IN_ATTACK;
//				}
//				else
//#endif
//				{
//					cmd->tick_count = pCPlayer->LastUpdatedNetVars.tickcount + lerpTicks;
//				}
//			}
//			else
//			{
//				int targettick = TIME_TO_TICKS(pCPlayer->BaseEntity->GetSimulationTime());
//				if (!bTickIsValid(targettick))
//					targettick = cmd->tick_count;
//
//				//If player is manually firing, then shoot immediately
//				if (GetAsyncKeyState(VK_LBUTTON))
//				{
//					cmd->tick_count = targettick + lerpTicks;
//				}
//				else
//				{
//					//Wait for resolver to allow firing
//					if (pCPlayer->Personalize.ResolvePitch || pCPlayer->Personalize.ResolveYaw)
//						cmd->buttons &= ~IN_ATTACK;
//					else
//						cmd->tick_count = targettick + lerpTicks;
//				}
//			}
//		}
//		else
//		{
//			//No target, just adjust based on interp
//			cmd->tick_count += lerpTicks;
//		}
//	}
//}

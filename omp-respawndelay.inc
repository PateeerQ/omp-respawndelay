/*
*
*    Vehicle Respawn Delay (openmp)
*    [+] made since open.mp has own config to adjust vehicle respawn delay (game.vehicle_respawn_time)
*
*/

#if defined Iterator@Player
    #define VRD_PlayerForeach(%0) foreach(new %0 : Player)
#else
    #define VRD_PlayerForeach(%0) for(new %0 = 0; %0 < MAX_PLAYERS; %0++) if(IsPlayerConnected(%0))
#endif

enum VRD_PLAYER
{
    vrd_LastVehicle
};

new
    playerRespawnDelay[MAX_PLAYERS][VRD_PLAYER]
;

enum VRD_VEHICLE
{
    vrd_respawnDelay,
    vrd_respawnTimer
};

new
    vehicleRespawnDelay[MAX_VEHICLES][VRD_VEHICLE]
;

stock VRD_CreateVehicle(modelid,Float:x,Float:y,Float:z,Float:angle,color1,color2,respawn_delay,addsiren = 0){
    new
        vehicleid = CreateVehicle(modelid,x,y,z,angle,color1,color2,respawn_delay,addsiren)
    ;

    vehicleRespawnDelay[vehicleid][vrd_respawnDelay] = respawn_delay;

    if(IsValidVehicle(vehicleid) && respawn_delay != -1)
    {
        KillTimer(vehicleRespawnDelay[vehicleid][vrd_respawnTimer]);
        vehicleRespawnDelay[vehicleid][vrd_respawnTimer] = -1;
    }
    return vehicleid;
}

stock VRD_DestroyVehicle(vehicleid)
{
    if(vehicleid > -1 && vehicleid < MAX_VEHICLES)
    {
        KillTimer(vehicleRespawnDelay[vehicleid][vrd_respawnTimer]);
        vehicleRespawnDelay[vehicleid][vrd_respawnTimer] = -1;

        vehicleRespawnDelay[vehicleid][vrd_respawnDelay] = -1;
    }
    return DestroyVehicle(vehicleid);
}

public OnPlayerStateChange(playerid,newstate,oldstate) {
    if(newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
    {
        new
            vehicleid = GetPlayerVehicleID(playerid)
        ;

        if(IsValidVehicle(vehicleid) && vehicleRespawnDelay[vehicleid][vrd_respawnDelay] != -1)
        {
            KillTimer(vehicleRespawnDelay[vehicleid][vrd_respawnTimer]);
            vehicleRespawnDelay[vehicleid][vrd_respawnTimer] = -1;

            playerRespawnDelay[playerid][vrd_LastVehicle] = vehicleid;
        }
    }
    if(oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
    {
        new
            vehicleid = playerRespawnDelay[playerid][vrd_LastVehicle]
        ;

        if(IsValidVehicle(vehicleid) && vehicleRespawnDelay[vehicleid][vrd_respawnDelay] != -1)
        {
            if(!VRD_IsVehicleOccupied(vehicleid))
            {
                KillTimer(vehicleRespawnDelay[vehicleid][vrd_respawnTimer]);
                vehicleRespawnDelay[vehicleid][vrd_respawnTimer] = -1;

                vehicleRespawnDelay[vehicleid][vrd_respawnTimer] = SetTimerEx("OnVehicleDespawn", vehicleRespawnDelay[vehicleid][vrd_respawnDelay] * 1000, false, "i", vehicleid);
            }
        }
    }
    #if defined VRD_OnPlayerStateChange
		VRD_OnPlayerStateChange(playerid,newstate,oldstate);
	#endif
    return 1;
}

forward OnVehicleDespawn(vehicleid);

// Foreach Function
stock VRD_IsVehicleOccupied(vehicleid){
	VRD_PlayerForeach(i) {
		if(IsPlayerInVehicle(i, vehicleid)) {
			return true;
		}
	}
	return false;
}

// ALS Hooks
#if defined VRD_OnPlayerStateChange
	forward VRD_OnPlayerStateChange(playerid,newstate,oldstate);
#endif
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange VRD_OnPlayerStateChange

#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif
#define DestroyVehicle VRD_DestroyVehicle

#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif
#define CreateVehicle VRD_CreateVehicle
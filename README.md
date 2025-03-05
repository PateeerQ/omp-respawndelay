# open.mp - respawndelay
Additional callbacks for respawn_delay parameter on CreateVehicle since open.mp has custom game.vehicle_respawn_time

Callback:
```h
public OnVehicleDespawn(vehicleid) { // This callback called after vehicle despawned/passing respawn_delay param
    // Your function here!
    return 1;
}
```

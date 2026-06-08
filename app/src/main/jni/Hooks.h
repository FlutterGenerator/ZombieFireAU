lib l2cpp;
Address transformOff,
        getpos, 
        wtsp, 
        getCam, 
        isActiveOff, 
        TakeDamage,
        UpDate,
        OnDesTroy,
        DeadOff, 
        SetGun,
        PlayerUpdate, 
        NoAttack,
        AmmoOffset,
        MoneyOffset,
        damageBullet,
        MaxHp,
        isMachine,
        DiamondOffset,
        getAimKill,
        noBulMinus,
        WPupdate,
        RpidFire,
        CamUpdate,
        DoRecoilOff,
        user_coin,
        user_gold, 
        Zombie_Scripts,
        user_level,
        Zombie_ALL_Damage,
        Get_Device_Unique_ID,
        Make_Zombie,
        Make_Boss,
        
        UnGloveUpdate;
Uclass GAMEPLAY_Script;
Uclass ZOMBIE_Script;
Uclass WEAPON_Script;
Uclass GAMECAMERA_Script;
void getAddress() {
    l2cpp.name = "libil2cpp.so";
    l2cpp.open();
    
    Uclass Component = l2cpp.getClass("UnityEngine", "Component");
    transformOff = Component.dump("get_transform");
    
    Uclass Tranf = l2cpp.getClass("UnityEngine", "Transform");
    getpos = Tranf.dump("get_position_Injected", 1);
    
    Uclass Cam = l2cpp.getClass("UnityEngine", "Camera");
    wtsp = Cam.dump("WorldToScreenPoint_Injected", 3);
    getCam = Cam.dump("get_main");
    
    ZOMBIE_Script = l2cpp.getClass("ZOMBIE_Script");
    isActiveOff = ZOMBIE_Script.dump("Zombie_NOW_HP", 0, true);
    MaxHp = ZOMBIE_Script.dump("Zombie_MAX_HP", 0, true);
    getAimKill = ZOMBIE_Script.dump("Helmet_Hit_Operation", 2);
     UpDate = ZOMBIE_Script.dump("Update");
     
    GAMEPLAY_Script = l2cpp.getClass("GAMEPLAY_Script");
    Make_Zombie = GAMEPLAY_Script.dump("Make_Zombie");
    Make_Boss = GAMEPLAY_Script.dump("Make_Boss");
    Zombie_ALL_Damage = GAMEPLAY_Script.dump("GameCamera_Rotation_Operation");
    Zombie_Scripts = GAMEPLAY_Script.dump("");
    
    WEAPON_Script = l2cpp.getClass("WEAPON_Script");
    noBulMinus = WEAPON_Script.dump("Bullet_Minus");
    RpidFire = WEAPON_Script.dump("Ani_Speed_Fire", 0, true);
    WPupdate = WEAPON_Script.dump("Update");
    
    Uclass genericShooter = l2cpp.getClass("genericShooter");
    DoRecoilOff = genericShooter.dump("fire");
    CamUpdate = genericShooter.dump("Update");
    
    GAMECAMERA_Script = l2cpp.getClass("GAMECAMERA_Script");
    }

void *getTransform(void *player) {
    if (!player) return NULL;
    static const auto get_transform_injected = reinterpret_cast<uint64_t(__fastcall *)(void *)>(transformOff.getAddr());
    return (void *) get_transform_injected(player);
}
Vector3 get_position(void *transform) {
    if (!transform)return Vector3();
    Vector3 position;
    static const auto get_position_injected = reinterpret_cast<uint64_t(__fastcall *)(void *,Vector3 &)>(getpos.getAddr());
    get_position_injected(transform, position);
    return position;
}
Vector3 WorldToScreenPoint(void *transform, Vector3 test) {
    if (!transform)return Vector3();
    Vector3 position;
    static const auto WorldToScreenPoint_Injected = reinterpret_cast<uint64_t(__fastcall *)(void *,Vector3, int, Vector3 &)>(wtsp.getAddr());
      WorldToScreenPoint_Injected(transform, test, 4, position);
      return position;
}
void *get_camera() {
    static const auto get_camera_injected = reinterpret_cast<uint64_t(__fastcall *)()>(getCam.getAddr());
    return (void *) get_camera_injected();
}
Vector3 GetPlayerLocation(void *player) {
    return get_position(getTransform(player));
}
float getHp(void *player) {
    return *(float *)((uintptr_t) player + isActiveOff.getAddr());
}
float getMax(void *player) {
    return *(float *)((uintptr_t) player + MaxHp.getAddr());
}
bool PlayerAlive(void *player) {
    return player != NULL && getHp(player) > 0;
}

bool IsPlayerDead(void *player) {
    return player == NULL && getHp(player) < 1;
}
int string_to_int(string str) 
{ 
    int value;
    value = std::stoi(str);
    return value;
}
string float_to_string (int value) 
{
    string str; 
    str = std::to_string(value);
    str += "M";
    return str;
}

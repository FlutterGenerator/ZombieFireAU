#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include "Unity/Vector2.h"
#include "Unity/Vector3.h"
#include "Unity/Rect.h"
#include "Unity/Color.h"
#include "Unity/unity.h"
#include "Unity/Quaternion.h"
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "Includes/Chams.h"
#include "Includes/MonoString.h"
#include "Includes/Strings.h"
#include "And64InlineHook/And64InlineHook.hpp"
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"
#include "NepDraw.h"
#include "NepAU/AutoUpdate.cpp"
using namespace NepSymbol;
#include "Hooks.h"
#include "ESPManager.h"

ESPManager *espManager;
NepEsp es;

//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

#include "Includes/Macros.h"

// fancy struct for patches for kittyMemory
struct My_Patches {
    // let's assume we have patches for these functions for whatever game
    // like show in miniMap boolean function
    MemoryPatch GodMode, GodMode2, SliderExample;
    // etc...
} hexPatches;

int gunType = 0;
bool ESP, ESPLine, ESPBox, ESPBox3d, ESPHealth, ESPDistance, coins, HDMG, hdml, noaten;
void *playerlate = NULL;
void *get_Player(void *player) {
    playerlate = player;
    return playerlate;
}
bool autofire, autonade, hsCombo, unlimitedgrenade, srank, noRecoil;
            
bool SZombie, SBoss, nohit, godmode, stopz, stopb, shakeCamera;
bool NReload, NRcl, ZcantMove, BCantMove;
Color Ka = Color(0,0,0,255);
bool R;
bool G;
bool onehitkill;
bool B;
Color getColor() {
    
    if(Ka.r == 0 && Ka.g == 0 && Ka.b == 0) {
        Ka.r = 255;
        G = true;
        Ka.b = 0;
    }
    if(Ka.r == 0 && Ka.g == 255 && Ka.b == 0) {
        B = true;
        R = false;
        G = false;
    }
    if(Ka.r == 0 && Ka.g == 0 && Ka.b == 255) {
        R = true;
        G = false;
        B = false;
    }
    if(Ka.r == 255 && Ka.g == 0 && Ka.b == 0) {
        G = true;
        R = false;
        B = false;
    }
    if(R) {
        Ka.r += 1;
        Ka.g = 0;
        Ka.b -= 1;
    }
    if(G) {
        Ka.r -= 1;
        Ka.g += 1;
        Ka.b = 0;
    }
    if(B) {
        Ka.r = 0;
        Ka.g -= 1;
        Ka.b += 1;
    }
    
    return Ka;
    
}
void DrawESP(NepEsp esp, int screenWidth, int screenHeight) {
    Color textC = getColor();
    int max = 100;
    textC.r += rand()%max;
    textC.g += rand()%max;
    textC.b += rand()%max;
    esp.DrawText(textC, "Platinmods.com", Vector2(screenWidth / 2, screenHeight/1.2), 27);

void *player = playerlate;

if(ESP && player != nullptr) {
//Code Goes Inside it
    if (espManager->enemies->empty()) {
        return;
    }
    for (int i = 0; i < espManager->enemies->size(); i++) {
        void *Cam = get_camera();
        if(Cam != nullptr) {
        
        //Code Goes Here
        void *Player = (*espManager->enemies)[i]->object;
        
        if (Player != nullptr) {
            if(PlayerAlive(Player)) {
            //Code Goes Here
            Color clr;
                clr.a = 255;
                Rect rect;
                Vector3 PlayerPos = GetPlayerLocation(Player);
                
                Vector3 PosNew = {0.f, 0.f, 0.f};
                PosNew = WorldToScreenPoint(Cam, PlayerPos);
                if (PosNew.z < 1.f) continue;
                

                Vector3 Origin;
                Origin = PlayerPos;
                Origin.y += 0.7f;
                float posnum = 10 * 0.1; /******* Hight slider * 0.1 ********/
                float posnum2 = 10 * 0.1;/******* Size slider * 0.1 ********/
                Origin.y += posnum;
                Vector3 BoxPosNew = {0.f, 0.f, 0.f};
                BoxPosNew = WorldToScreenPoint(Cam, Origin);
                float Hight =
                abs(BoxPosNew.y - PosNew.y) * (posnum2 / posnum), Width = Hight * 0.6f;

                rect = Rect(BoxPosNew.x - Width / 2.f,
                            screenHeight - BoxPosNew.y,
                            Width, Hight
                );
                Vector2 DrawFrom = Vector2(screenWidth /2 , screenHeight);
                Vector2 DrawTo = Vector2((screenWidth- (screenWidth - PosNew.x)) + 5, (screenHeight - PosNew.y));
            
             if (ESPLine) {
                esp.DrawLine(getColor(), 2, DrawFrom, DrawTo);
            }
            if(ESPBox) {
                esp.DrawBox(getColor(), 2, rect);
            }
            if(ESPHealth) {
                esp.DrawHorizontalHealthBar(Vector2((screenWidth- (screenWidth - BoxPosNew.x)) + 5, (screenHeight - BoxPosNew.y)), 50, getMax(Player), getHp(Player));
            }
            if(ESPDistance) {
                int distance = (int) Vector2::Distance(Vector2(screenWidth , screenHeight), Vector2(BoxPosNew.x, BoxPosNew.y));
                std::string distances;
               
                
                   distances = float_to_string(distance / 100);
             
               
               
                esp.DrawText(getColor(),  distances.c_str(), Vector2((screenWidth - (screenWidth - BoxPosNew.x)), (screenHeight - BoxPosNew.y - 10.0f)), 17);
                
            }
        } else {
            espManager->removeEnemyGivenObject(Player);
        }
    }
    }

}
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_DrawOn(JNIEnv *env, jclass type, jobject espView, jobject canvas) {
                es = NepEsp(env, espView, canvas);
    if (es.isValid()){
        DrawESP(es, es.getWidth(), es.getHeight());
    }
    
}

void *enemyPlayer = NULL;
void (*updateP)(void *player);
void _update(void *player) {
    if (player != NULL) {
        enemyPlayer = player;
      
    }
    if (ESP) {
        if (enemyPlayer) {
            espManager->tryAddEnemy(player);
            
        }
    }
    updateP(player);
    get_Player(player);
}

void (*_NoReload)(void *player);
void NoReload(void *player) {
    if(player != NULL) {
        if(NReload) {
            return;
        }
    }
    _NoReload(player);
}

void (*_UpdateWeapon)(void *player);
void UpdateWeapon(void *player) {
    if(player != NULL) {
        if(noaten) {
            *(float *)((uintptr_t) player + RpidFire.getAddr()) = 999;
        }
        if(onehitkill) {
            *(float *)((uintptr_t) player + WEAPON_Script.dump("ATK", 0, true).getAddr()) = 9999;
        }
        if(noRecoil) {
                *(float *)((uintptr_t) player + WEAPON_Script.dump("ZoomIn_Speed", 0, true).getAddr()) = 0;
        *(float *)((uintptr_t) player + WEAPON_Script.dump("Aim_Move_Distance_Minus", 0, true).getAddr()) = 9999;
         *(float *)((uintptr_t) player + WEAPON_Script.dump("Aim_Move_Distance_Plus", 0, true).getAddr()) = 0;
      *(float *)((uintptr_t) player + WEAPON_Script.dump("Aim_Move_Distance_Start", 0, true).getAddr()) = 0;
     
        }
    
        
    }
    _UpdateWeapon(player);
}

void(*DoRecoil)(void *player);
void (*_ControllRecoil)(void *player);
void ControllRecoil(void *player) {
    if(player != NULL) {
        if(NRcl) {
            DoRecoil(player);
        }
    }
    _ControllRecoil(player);
}

void (*SpawnZombie)(void *game);
void (*SpawnBoss)(void *game);
void (*AutoFire)(void *player, bool down_check);
void (*AutoNade)(void *player);
void(*_ZUpdate)(void *player);
void ZUpdate(void *player) {
    if(player != NULL) {
        
        if(SZombie) {
            SpawnZombie(player);
        }
        if(SBoss) {
            SpawnBoss(player);
        }
         *(float *)((uintptr_t) player + GAMEPLAY_Script.dump("Player_HP_MAX", 0, true).getAddr()) = 100;
         *(float *)((uintptr_t) player + GAMEPLAY_Script.dump("Player_HP", 0, true).getAddr()) = 100;
       
         if(hsCombo) {
             *(int *)((uintptr_t) player + GAMEPLAY_Script.dump("Head_Shot_Combo_Count", 0, true).getAddr()) = 9999;
         }
           *(int *)((uintptr_t) player + GAMEPLAY_Script.dump("Bonus_Coin", 0, true).getAddr()) = 99999999;
        if(unlimitedgrenade) {
            *(int *)((uintptr_t) player + GAMEPLAY_Script.dump("Item_Grenade_Count", 0, true).getAddr()) = 99999999;
             *(int *)((uintptr_t) player + GAMEPLAY_Script.dump("Item_Shield_Count", 0, true).getAddr()) = 99999999;
        }
        if(srank) {
            *(int *)((uintptr_t) player + GAMEPLAY_Script.dump("Game_Rank_Point", 0, true).getAddr()) = 1;
            *(int *)((uintptr_t) player + GAMEPLAY_Script.dump("Game_Rank_Section", 0, true).getAddr()) = 1;
        
        }

        if(autofire) {
            AutoFire(player, 1);
        }
        if(autonade) {
            AutoNade(player);
        }
       // bool autofire, autonade, hsCombo, unlimitedgrenade, srank;
      
    }
    _ZUpdate(player);
    
}

int fov = 0;

void (*_NoHit)(void *player);
void NoHit(void *player) {
    if(player != NULL) {
        if(nohit) {
            return;
        }
    }
    _NoHit(player);
}
void (*_GodMode)(void *player);
void GodMode(void *player) {
    if(player != NULL) {
        if(godmode) {
            return;
        }
    }
    _GodMode(player);
}
void (*_StopZombie)(void *player);
void StopZombie(void *player) {
    if(player != NULL) {
        if(stopz) {
            return;
        }
    }
    _StopZombie(player);
}
void (*_StopBoss)(void *player);
void StopBoss(void *player) {
    if(player != NULL) {
        if(stopb) {
            return;
        }
    }
    _StopBoss(player);
}
void (*SetFov)(void *player, float fov);
void (*ShakeCam)(void *player);
void (*_setFov)(void *player);
void setFov(void *player) {
    if(player != NULL) {
        if(fov > 0) {
            SetFov(player, fov);
        }
        if(shakeCamera) {
            ShakeCam(player);
        }
    }
    _setFov(player);
}
void *hack_thread(void *) {
    ProcMap il2cppMap;
    do {
        il2cppMap = KittyMemory::getLibraryMap(targetLibName);
        sleep(1);
    } while (!isLibraryLoaded(targetLibName) && mlovinit());
    espManager = new ESPManager();
    setShader("_BumpMap");
    LogShaders();
    Wallhack();
    
    do {
        sleep(1);
    } while (!isLibraryLoaded (OBFUSCATE("libMyLibName.so")));
    getAddress();
    SpawnZombie = (void (*) (void *)) Make_Zombie.getAddr();
    SpawnBoss =  (void (*) (void *)) Make_Boss.getAddr();
    AutoFire = (void (*) (void *, bool)) GAMEPLAY_Script.dump("Shot_Button_Event", 1).getAddr();
    AutoNade = (void (*) (void *)) GAMEPLAY_Script.dump("Grenade_Button_Event").getAddr();
    SetFov = (void (*) (void *, float)) GAMECAMERA_Script.dump("set_Play_Camera_FOV", 1).getAddr();
    ShakeCam = (void (*) (void *)) GAMECAMERA_Script.dump("Camera_Shake").getAddr();
    HOOKAU(UpDate, _update, updateP);
    HOOKAU(noBulMinus, NoReload, _NoReload);
    HOOKAU(WPupdate, UpdateWeapon, _UpdateWeapon);
    HOOKAU(DoRecoilOff, ControllRecoil, _ControllRecoil);
    HOOKAU(GAMEPLAY_Script.dump("Update"), ZUpdate, _ZUpdate);
    HOOKAU(GAMEPLAY_Script.dump("Player_HP_Minus", 2), GodMode, _GodMode);
    HOOKAU(GAMECAMERA_Script.dump("LateUpdate"), setFov, _setFov);
    HOOKAU(ZOMBIE_Script.dump("Zombie_Move_Operation"), StopZombie, _StopZombie);
    HOOKAU(ZOMBIE_Script.dump("Boss_Move_Operation"), StopBoss, _StopBoss);
    
    return NULL;
}

extern "C" {
    JNIEXPORT jobjectArray
    JNICALL
    Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject context) {
        jobjectArray ret;
        const char *features[] = {
            OBFUSCATE("Collapse_ESP AREA"),//0
            OBFUSCATE("0_CollapseAdd_Toggle_ENABLE ESP"),//0
            OBFUSCATE("1_CollapseAdd_Toggle_ESP LINE"),//1
            OBFUSCATE("2_CollapseAdd_Toggle_ESP BOX"),//1      
            OBFUSCATE("3_CollapseAdd_Toggle_ESP Distance"),//1
            OBFUSCATE("4_CollapseAdd_Toggle_ESP Health"),
            
            OBFUSCATE("Collapse_PLAYER AREA"),//0
            OBFUSCATE("10000_CollapseAdd_Category_Unlimited Hp Enabled"),//0
            OBFUSCATE("10000_CollapseAdd_Category_For Unlimited Coin, Play Bonus Mode"),//0
            OBFUSCATE("19_CollapseAdd_SeekBar_View Fov_0_360"),//0
            OBFUSCATE("7_CollapseAdd_Toggle_One Hit"),//0
            OBFUSCATE("5_CollapseAdd_Toggle_Unlimited Ammo"),//0
            OBFUSCATE("6_CollapseAdd_Toggle_Rapid Fire"),//0
            OBFUSCATE("18_CollapseAdd_Toggle_No Recoil"),//0 
            OBFUSCATE("16_CollapseAdd_Toggle_AutoFire"),//0 
            OBFUSCATE("17_CollapseAdd_Toggle_Auto Grenade"),//0 
            OBFUSCATE("13_CollapseAdd_Toggle_HeadShot Combo Max"),//0
            OBFUSCATE("14_CollapseAdd_Toggle_Unlimted Grenade / Shield"),//0
            OBFUSCATE("15_CollapseAdd_Toggle_*S* Rank Always"),//0
            // OBFUSCATE("20_CollapseAdd_Toggle_Shake Camera"),//0
          
            OBFUSCATE("Collapse_ZOMBIE AREA"),//0
            OBFUSCATE("11_CollapseAdd_Toggle_Stop Zombies"),//0
            OBFUSCATE("12_CollapseAdd_Toggle_Stop Boss"),//0
            OBFUSCATE("10_CollapseAdd_Toggle_Zombie Cant Hit"),//0
            
            OBFUSCATE("Collapse_SPAWN ARENA"),//0
            OBFUSCATE("8_CollapseAdd_Toggle_Spawn Zombies"),//0
            OBFUSCATE("9_CollapseAdd_Toggle_Spawn Boss"),//0
            
};
        int Total_Feature = (sizeof features / sizeof features[0]);
        ret = (jobjectArray)
              env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                  env->NewStringUTF(""));

        for (int i = 0; i < Total_Feature; i++)
            env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

        return (ret);
    }


    JNIEXPORT void JNICALL
    Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                            jint featNum, jstring featName, jint value,
                                            jboolean boolean, jstring str) {

        switch (featNum) {
        case 0:
            ESP = boolean;
            break;
        case 1:
            ESPLine = boolean;
            
            break;
         case 2:
            ESPBox = boolean;
            
            break;               
        case 3:
            ESPDistance = boolean;
            break;
        case 4:
           ESPHealth  = boolean;
            break;
        case 5:
           NReload  = boolean;
            break;
        case 6:
            noaten = boolean;
            break;
        case 7:
            onehitkill = boolean;
            break;
        case 8:
            SZombie = boolean;
            break;
        case 9:
            SBoss = boolean;
            break;
        case 10:
            godmode = boolean;
            break;
        case 11:
            stopz = boolean;
            break;
        case 12:
            stopb = boolean;
            break;
        case 13:
            hsCombo = boolean;
            break;
        case 14:
            unlimitedgrenade = boolean;
            break;
        case 15:
            srank = boolean;
            break;
        case 16:
            autofire = boolean;
            break;
        case 17:
            autonade = boolean;
            break;
        case 18:
            noRecoil = boolean;
            break;
        case 19:
            fov = value;
            break;
        case 20:
            shakeCamera = boolean;
            break;
         case 25:
            SetWallhack(boolean); 
            break;    
        case 26:           
            SetWallhackW(boolean);
            break;    
        case 27:          
            SetWallhackG(boolean);
            break;     
        case 28:          
            SetWallhackO(boolean);
            break;    
        case 29:     
            SetRainbow(boolean);
            break;
        case 30:
            SetW(value);
            break;         
        case 31:
            SetR(value);
            break;
        case 32:
            SetG(value);
            break;
        case 33:
            SetB(value);
            break; 	
        }
        
    }
}

__attribute__((constructor))
void lib_main() {

    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

#pragma once
#define HAS_EXCEPTIONS 0
#include <Windows.h>

#include "xor\xor.h"
#include "xor\xor_data.h"
#include "xor\xor_value.h"

#include "minicrt\mincrt.h"
#include "importer\lazy_importer.h"
#include "memory/scanner/scanner.h"

inline bool ShowSexyMenu = true;

// AIMBOT TAB
inline bool aimbot = false;
inline bool aimVisCheck = true;
inline bool AimLine = false;
inline float FOVRange = 250;
inline float smooth = 2.0f;
inline float AimSpeed = 4.f;
inline int SmoothShow = 5;
inline int BoneTarget = 0;
inline bool closeaim = false;
inline float closeaim_dist = 10;

inline bool hasShotgun = false;
inline float ShotgunFov = false;
inline float shotgun_smooth = 8.f;

inline bool hasSMG = false;
inline float SMGFOV = false;
inline float SMGSmooth = 8.f;

// VISUALS TAB
inline bool playerBoxREAL = false;
inline bool visuals = true;
inline bool DrawRadarESP = false;
inline bool playerDistance = true;
inline bool playerName = true;
inline bool playerBox = false;
inline bool playerCornerBox = true;
inline bool Skeletons = true;
inline bool Weapons = true;
inline bool playerBox2 = false;
inline bool drawFOV2 = false;
inline bool Ammo = true;
inline bool HeadCircle = true;

inline bool visCheck = true;
inline bool drawSelf = false;
inline bool Chams = false;
inline bool IgnoreKnocked = true;

inline // VISUALS TAB
inline float overallDistance = 125;
inline float overallPortalDistance = 125;
inline int RaderX = 1380.f;
inline int RaderY = 30.f;
inline float RadarSize = 210.f;

// World VISUALS
inline bool drawInteractable = true;
inline bool drawNormalChest = true;
inline bool drawTrap = true;
inline bool drawPortalz = true;
inline bool drawSpecialChest = true;
inline bool drawHealPot = true;
inline bool allLoot = false;
inline float overallChestDistance = 25;
inline float trapDistance = 25;
inline float LuckPotDistance = 25;
inline float HealPotDistance = 25;
inline float SpecialChestDistance = 80;
inline float InteractableDistance = 40;
inline bool Chests = false;
inline bool LootItems = false;
inline bool Vehicles = false;
inline bool AmmoBox = false;
inline bool WorldAmmo = false;

inline float lootMinimalTier = 2;

// MISC TAB
inline bool drawFOV = true;

inline int Aimbotkey = 1;
inline bool blur = false;
inline bool infFuel = false; // Vehicle
inline bool bInstantOpenChest = false;
inline bool RapidFire = false;
inline bool InstaReboot = false;
inline bool InstaReload = false;
inline bool FOVChanger = true;
inline float FOVSize = 120;

#include "SDK/SDK.h"
#include "SDK/Structs/Structs.h"
#include "SDK/Functions/Functions.h"
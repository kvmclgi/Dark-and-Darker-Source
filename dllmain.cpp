#include "stdafx.h"
#include "gui/GUi.h"

bool init_objects = false;

static void initObjects() {
	defines::KismetSystemLibrary = SDK::Functions::pLoadObject(xor_w(L"Engine.Default__KismetSystemLibrary"));
	defines::KismetStringLibrary = SDK::Functions::pLoadObject(xor_w(L"Engine.Default__KismetStringLibrary"));
	defines::KismetMathLibrary = SDK::Functions::pLoadObject(xor_w(L"Engine.Default__KismetMathLibrary"));
	defines::GameplayStatics = SDK::Functions::pLoadObject(xor_w(L"Engine.Default__GameplayStatics"));
	defines::KismetMaterialLibrary = SDK::Functions::pLoadObject(xor_w(L"Engine.Default__KismetMaterialLibrary")); //Engine.
	defines::KismettextLibrary = SDK::Functions::pLoadObject(xor_w(L"Engine.Default__KismetTextLibrary"));

	init_objects = true;
}

//void SetChams(uintptr_t GWorld, uintptr_t mesh, SDK::FLinearColor color, bool wireframe = false)
//{
//	if (!mesh) return;
//
//	static PVOID chams = 0;
//	if (!chams) chams = UObject::FindObject<PVOID>(xorstr("MaterialInstanceConstant SprayGun_Liquid_MIC.SprayGun_Liquid_MIC"));
//
//	int NumMaterials = GetNumMaterials(mesh);
//	if (NumMaterials > 0)
//	{
//		uintptr_t Material = Engine::GetMaterial(mesh, 0);
//		if (Engine::GetBaseMaterial(Material) == Engine::GetBaseMaterial((uintptr_t)chams))
//			return;
//
//		static PVOID KismetLibrary = 0;
//		if (!KismetLibrary) KismetLibrary = UObject::FindObject<PVOID>(xorstr("Class Engine.KismetMaterialLibrary"));
//
//		uintptr_t DynamicMaterial = CreateDynamicMaterialInstance((uintptr_t)KismetLibrary, GWorld, (uintptr_t)chams);
//		if (!DynamicMaterial) return;
//
//		CopyMaterialInstanceParameters(DynamicMaterial, (uintptr_t)chams);
//		*(PVOID*)(DynamicMaterial + 0x68) = chams;
//
//		uintptr_t BaseDynamicMaterial = GetBaseMaterial(DynamicMaterial);
//		if (!BaseDynamicMaterial) return;
//		((UMaterialWireFrame*)BaseDynamicMaterial)->Wireframe = Settings::Setings.Visuals.force_wireframe;
//		((UMaterialDepthTest*)BaseDynamicMaterial)->bDisableDepthTest = TRUE;
//		*(uint8_t*)(BaseDynamicMaterial + 0x349) = 2;
//
//		TArray<FVectorParameterValue> VectorParameterValues = *(TArray<FVectorParameterValue>*)(DynamicMaterial + 0xB8);
//		for (int i = 0; i < VectorParameterValues.Num(); ++i)
//		{
//			FVectorParameterValue Value = VectorParameterValues[i];
//			SetVectorParameterValue(DynamicMaterial, Value.ParameterName, color);
//		}
//
//		for (int i = 0; i < NumMaterials; ++i)
//		{
//			if (GetMaterial(mesh, i) == 0)
//				continue;
//
//			if (GetMaterial(mesh, i) == DynamicMaterial)
//				continue;
//
//			SetMaterial(mesh, DynamicMaterial, i);
//		}
//	}
//}

SDK::FString ExtractShortName(const SDK::FString& fullName)
{
	const wchar_t* str = fullName.c_str();
	int start = -1;
	int end = -1;

	// Safety: Check if the string is valid
	if (!str)
	{
		return SDK::FString();
	}

	// Find first underscore (should be right after "BP")
	for (int i = 0; str[i] != L'\0'; ++i)
	{
		if (str[i] == L'_')
		{
			start = i + 1;
			break;
		}
	}

	// Find the next underscore after the start
	for (int i = start; str[i] != L'\0' && str[i] != L'_'; ++i)
	{
		end = i;
	}

	if (start != -1 && end != -1)
	{
		wchar_t result[256] = {};  // Fixed-size buffer
		int len = end - start + 1;

		if (len > 255)  // Ensure we don't exceed buffer size
		{
			len = 255;
		}

		for (int i = 0; i < len; ++i)
		{
			result[i] = str[start + i];
		}
		result[len] = L'\0';  // Null-terminate the string

		return SDK::FString(result);
	}

	return SDK::FString();
}


uintptr_t projectiles = 0;

void MyPostrender(void* UObject, void* pCanvas) {

	if (!UObject) return;
	if (!pCanvas) return;

	// Assign the UObject and UCanvas object
	defines::UObjects = (uint64_t)UObject;
	defines::UCanvas = (uint64_t)pCanvas;

	if (pCanvas)
	{
		defines::MyPlayer = NULL;
		defines::halt = NULL;

		defines::GWorld = SDK::Functions::read<uint64_t>(defines::BaseAddr + 0x7B767D0);

		// Gets the local playercontroller
		defines::PlayerController = SDK::Functions::STATIC_GetPlayerController();

		// Gets the local playercameramanager
		defines::PlayerCameraManager = SDK::Functions::STATIC_GetPlayerCameraManager();
		if (!defines::PlayerCameraManager) return;

		// Gets the local pawn
		defines::MyPlayer = SDK::Functions::K2_GetPawn();

		SDK::Functions::GetViewportSize(defines::PlayerController, &defines::Width, &defines::Height);

		SDK::Functions::fInitializeOnce();

		static uintptr_t Entities_class = 0;
		if (!Entities_class) Entities_class = SDK::Functions::pLoadObject(xor_w(L"DungeonCrawler:DCCharacterBase"));

		static uintptr_t Player_class = 0;
		if (!Player_class) Player_class = SDK::Functions::pLoadObject(xor_w(L"BP_PlayerCharacterDungeon_C.BP_PlayerCharacterDungeon_C"));

		static uintptr_t Projectile = 0;
		if (!Projectile) Projectile = SDK::Functions::pLoadObject(xor_w(L"DungeonCrawler.ProjectileActor"));

		static uintptr_t FloorPortal = 0;
		if (!FloorPortal) FloorPortal = SDK::Functions::pLoadObject(xor_w(L"DungeonCrawler.FloorPortalScrollBase"));

		static uintptr_t Trap = 0;
		if (!Trap) Trap = SDK::Functions::pLoadObject(xor_w(L"BP_TrapBase.BP_TrapBase_C"));

		static uintptr_t chest = 0;
		if (!chest) chest = SDK::Functions::pLoadObject(xor_w(L"BP_Chest_Base.BP_Chest_Base_C"));

		static uintptr_t LuckPot = 0;
		if (!LuckPot) LuckPot = SDK::Functions::pLoadObject(xor_w(L"BP_LuckPotion.BP_LuckPotion_C"));

		static uintptr_t HealPot = 0;
		if (!HealPot) HealPot = SDK::Functions::pLoadObject(xor_w(L"BP_HealingPotion.BP_HealingPotion_C"));

		static uintptr_t SpecialChest = 0;
		if (!SpecialChest) SpecialChest = SDK::Functions::pLoadObject(xor_w(L"BP_ChestSpecial_Base.BP_ChestSpecial_Base_C"));

		static uintptr_t HolderBase = 0;
		if (!HolderBase) HolderBase = SDK::Functions::pLoadObject(xor_w(L"DungeonCrawler.ItemHolderActorBase"));

		auto VisibleColor = SDK::FLinearColor(0.f, 255.f, 0.f, 255.f);
		auto NoVisibleColor = SDK::FLinearColor(255, 0.f, 0.f, 255.f);

		float closestDistance = FLT_MAX;

		int yoffset = 5;

		auto Levels = *(SDK::TArray<uintptr_t>*)(defines::GWorld + 0x170);
		for (int level = 0; level < Levels.Num(); ++level) {
			if (!Levels.IsValidIndex(level)) continue;

			auto current_level = Levels[level];
			if (!current_level) continue;

			auto Actors = *(SDK::TArray<uintptr_t>*)(current_level + 0x98);
			if (!Actors.Num()) break;

			for (int index = 0; index < Actors.Num(); ++index) {
				if (!Actors.IsValidIndex(index)) continue;

				uintptr_t current_actor = Actors[index];
				if (!current_actor) continue;

				if (SDK::Functions::IsA(current_actor, Entities_class))
				{
					uintptr_t mesh = SDK::Functions::read<uintptr_t>(current_actor + 0x318);
					if (!mesh) continue;

					defines::PlayerMesh = mesh;

					uintptr_t PlayerState = SDK::Functions::read<uintptr_t>(defines::PlayerController + 0x298);
					if (!PlayerState) return;

					/*auto PlayerInfo = SDK::Functions::read<uint64_t>(PlayerState + 0x0298);
					if (!PlayerState) return;

					SDK::FString PlayerName = SDK::Functions::read<SDK::FString>(PlayerInfo + 0x0018);*/
					 

					uintptr_t AckPawn = SDK::Functions::read<uintptr_t>(defines::PlayerController + 0x338);

					auto AbilitySystemComponent = SDK::Functions::read<uint64_t>(current_actor + 0x6B0);
					auto SpawnedAttributes = SDK::Functions::read<uint64_t>(AbilitySystemComponent + 0x11F0);
					auto SpawnedAttribute = SDK::Functions::read<uint64_t>(SpawnedAttributes);
					auto Health = SDK::Functions::read<float>(SpawnedAttribute + 0x654);
					float MaxHealth = SDK::Functions::read<float>(SpawnedAttribute + 0x0658);
					
					if (Health <= 0) continue;

					switch (Aimbotkey) {
					case 0:
						defines::KeyBind = defines::LeftClick;
						break;
					case 1:
						defines::KeyBind = defines::RightClick;
						break;
					case 2:
						defines::KeyBind = defines::MiddleMouseButton;
						break;
					case 3:
						defines::KeyBind = defines::ThumbMouseButton2;
						break;
					case 4:
						defines::KeyBind = defines::ThumbMouseButton;
						break;
					case 5:
						defines::KeyBind = defines::LeftShift;
						break;
					case 6:
						defines::KeyBind = defines::LeftControl;
						break;
					case 7:
						defines::KeyBind = defines::LeftAlt;
						break;
					case 8:
						defines::KeyBind = defines::Enter;
						break;
					case 9:
						defines::KeyBind = defines::Gamepad_LeftTrigger;
						break;
					case 10:
						defines::KeyBind = defines::Gamepad_RightTrigger;
						break;
					}

					auto RootBone = SDK::Functions::GetSocketLocation(mesh, defines::RootName);
					auto HeadBone = SDK::Functions::GetSocketLocation(mesh, defines::HeadName);
					HeadBone.Z += 15;

					auto IsVisible = SDK::Functions::LineOfSightTo(defines::PlayerController, current_actor, SDK::Functions::GetCameraLocation(),
						false);

					bool aimVisible = IsVisible;

					if (!visCheck) aimVisible = true;
					if (!visCheck) IsVisible = true;

					double distance = SDK::Functions::STATIC_Vector_Distance(RootBone, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > overallDistance) continue;

					auto BoxLineColor = IsVisible ? VisibleColor : NoVisibleColor;

					SDK::FString PlayerName = SDK::Functions::read<SDK::FString>(current_actor + 0x7C0);
					if (PlayerName.IsEmpty()) {
						BoxLineColor = SDK::FLinearColor(255.f, 255.f, 255.f, 255.f);
					}

					if (FOVChanger) SDK::Functions::FOV(FOVSize);

					SDK::Functions::GetAllBones(mesh);
					SDK::FVector head = SDK::Functions::K2_Project(HeadBone);
					bool foxhunter =
						SDK::Functions::IsValidScreenLoc(head) &&
						SDK::Functions::IsValidScreenLoc(SDK::Functions::vHeadBoneOut) &&
						SDK::Functions::IsValidScreenLoc(SDK::Functions::vRootOut) &&
						SDK::Functions::IsValidScreenLoc(SDK::Functions::vChestOut) &&
						SDK::Functions::IsValidScreenLoc(SDK::Functions::vNeckOut);
					if (foxhunter) {
						auto sha = SDK::FLinearColor(0.f, 0.f, 0.f, 255.f);
						float heightA = SDK::Functions::UKismetMathLibrarySTATIC_Abs(head.Y - SDK::Functions::vRootOut.Y);
						float widthA = heightA * 0.35f;//0.65f;
						auto Size = SDK::FVector2D(widthA, heightA);
						auto Pos = SDK::FVector2D(head.X - widthA / 2, head.Y);

						auto topLeft = Pos;
						auto bottomRight = Pos + Size;
						auto theAdditionY = (bottomRight.Y - topLeft.Y) / (5.5f);
						auto theAdditionX = (bottomRight.X - topLeft.X) / 4.f;
						auto topRight = SDK::FVector2D(bottomRight.X, topLeft.Y);
						auto bottomLeft = SDK::FVector2D(topLeft.X, bottomRight.Y);
						auto ctup = SDK::FVector2D(Pos.X + (Size.X / 2), Pos.Y);

						if (playerCornerBox) {
							//Color
							SDK::Functions::K2_DrawLine(topLeft, SDK::FVector2D(topLeft.X, topLeft.Y + theAdditionY), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(topLeft, SDK::FVector2D(topLeft.X + theAdditionX, topLeft.Y), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(topRight, SDK::FVector2D(topRight.X, topRight.Y + theAdditionY), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(topRight, SDK::FVector2D(topRight.X - theAdditionX, topRight.Y), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(bottomLeft, SDK::FVector2D(bottomLeft.X, bottomLeft.Y - theAdditionY), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(bottomLeft, SDK::FVector2D(bottomLeft.X + theAdditionX, bottomLeft.Y), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(bottomRight, SDK::FVector2D(bottomRight.X, bottomRight.Y - theAdditionY), 1.5f, BoxLineColor);
							SDK::Functions::K2_DrawLine(bottomRight, SDK::FVector2D(bottomRight.X - theAdditionX, bottomRight.Y), 1.5f, BoxLineColor);
						}

						if (Skeletons) {
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vHeadBoneOut.X, SDK::Functions::vHeadBoneOut.Y }), SDK::FVector2D({ SDK::Functions::vNeckOut.X, SDK::Functions::vNeckOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::pelvisOut.X, SDK::Functions::pelvisOut.Y }), SDK::FVector2D({ SDK::Functions::vNeckOut.X, SDK::Functions::vNeckOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vUpperArmLeftOut.X, SDK::Functions::vUpperArmLeftOut.Y }), SDK::FVector2D({ SDK::Functions::vNeckOut.X, SDK::Functions::vNeckOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vUpperArmRightOut.X, SDK::Functions::vUpperArmRightOut.Y }), SDK::FVector2D({ SDK::Functions::vNeckOut.X, SDK::Functions::vNeckOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vLeftHandOut.X, SDK::Functions::vLeftHandOut.Y }), SDK::FVector2D({ SDK::Functions::vUpperArmLeftOut.X, SDK::Functions::vUpperArmLeftOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vRightHandOut.X, SDK::Functions::vRightHandOut.Y }), SDK::FVector2D({ SDK::Functions::vUpperArmRightOut.X, SDK::Functions::vUpperArmRightOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vLeftHandOut.X, SDK::Functions::vLeftHandOut.Y }), SDK::FVector2D({ SDK::Functions::vLeftHandOut1.X, SDK::Functions::vLeftHandOut1.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vRightHandOut.X, SDK::Functions::vRightHandOut.Y }), SDK::FVector2D({ SDK::Functions::vRightHandOut1.X, SDK::Functions::vRightHandOut1.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vLeftThighOut.X, SDK::Functions::vLeftThighOut.Y }), SDK::FVector2D({ SDK::Functions::pelvisOut.X, SDK::Functions::pelvisOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vRightThighOut.X, SDK::Functions::vRightThighOut.Y }), SDK::FVector2D({ SDK::Functions::pelvisOut.X, SDK::Functions::pelvisOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vLeftKneeOut.X, SDK::Functions::vLeftKneeOut.Y }), SDK::FVector2D({ SDK::Functions::vLeftThighOut.X, SDK::Functions::vLeftThighOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::vRightKneeOut.X, SDK::Functions::vRightKneeOut.Y }), SDK::FVector2D({ SDK::Functions::vRightThighOut.X, SDK::Functions::vRightThighOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::leftFeet2d.X, SDK::Functions::leftFeet2d.Y }), SDK::FVector2D({ SDK::Functions::vLeftKneeOut.X, SDK::Functions::vLeftKneeOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::rightFeet2d.X, SDK::Functions::rightFeet2d.Y }), SDK::FVector2D({ SDK::Functions::vRightKneeOut.X, SDK::Functions::vRightKneeOut.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::leftFeet2d.X, SDK::Functions::leftFeet2d.Y }), SDK::FVector2D({ SDK::Functions::leftFeetFinger2d.X, SDK::Functions::leftFeetFinger2d.Y }), 1.3f, BoxLineColor);
							SDK::Functions::K2_DrawLine(SDK::FVector2D({ SDK::Functions::rightFeet2d.X, SDK::Functions::rightFeet2d.Y }), SDK::FVector2D({ SDK::Functions::rightFeetFinger2d.X, SDK::Functions::rightFeetFinger2d.Y }), 1.3f, BoxLineColor);
						}

						
						SDK::FString StringDistance = SDK::Functions::STATIC_BuildString_Int(xor_w(L""), xor_w(L" "), distance, xor_w(L"m"));
						SDK::FString HealthString = SDK::Functions::STATIC_BuildString_Int(xor_w(L""), xor_w(L"Health "), Health, xor_w(L""));
						SDK::Functions::K2_DrawLine(SDK::FVector2D(defines::Width / 2 - 5, defines::Height / 2), SDK::FVector2D(defines::Width / 2 + 5, defines::Height / 2),
							1.f, SDK::FLinearColor(255, 255, 255, 255));
						SDK::Functions::K2_DrawLine(SDK::FVector2D(defines::Width / 2, defines::Height / 2 - 5), SDK::FVector2D(defines::Width / 2, defines::Height / 2 + 5),
							1.f, SDK::FLinearColor(255, 255, 255, 255));
						SDK::FString NameString;
						if (playerName) {
							if (current_actor) {
							
								if (!PlayerName.IsEmpty()) {
									NameString = PlayerName;
								}
								else {
									NameString = ExtractShortName(SDK::Functions::STATIC_GetDisplayName(current_actor));							
								}
							}
						}

						if (!NameString.IsEmpty()) {
							auto size = SDK::Functions::K2_TextSize(defines::fnFont2, NameString, SDK::FVector2D(0.3f, 0.3f));
							auto textLocation = SDK::FVector2D(ctup.X - (size.X / 2), ctup.Y - 18);
							SDK::Functions::K2_DrawText(defines::fnFont2, NameString, SDK::FVector2D(textLocation.X, textLocation.Y), SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(255, 255, 255, 1), 1.0f, SDK::FLinearColor(0, 0, 0, 1), SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
						}

						if (playerDistance) {
							auto nameSize = SDK::Functions::K2_TextSize(defines::fnFont2, StringDistance, SDK::FVector2D(0.3f, 0.3f));
							auto textLocation = SDK::FVector2D(((topLeft.X + bottomRight.X) / 2.0f) - (nameSize.X / 2), bottomRight.Y);
							SDK::Functions::K2_DrawText(defines::fnFont2, StringDistance, SDK::FVector2D(textLocation.X, textLocation.Y), SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(255, 255, 255, 1), 1.0f, SDK::FLinearColor(0, 0, 0, 1), SDK::FVector2D(), true, false, true, SDK::FLinearColor(0, 0, 0, 1));
						}

						auto nameSize = SDK::Functions::K2_TextSize(defines::fnFont2, HealthString, SDK::FVector2D(0.3f, 0.3f));
						auto textLocation = SDK::FVector2D(((topLeft.X + bottomRight.X) / 2.0f) - (nameSize.X / 2), bottomRight.Y + 12);
						SDK::Functions::K2_DrawText(defines::fnFont2, HealthString, SDK::FVector2D(textLocation.X, textLocation.Y),
							SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(255, 255, 255, 1), 1.0f, 
							SDK::FLinearColor(0, 0, 0, 1), SDK::FVector2D(), true, false, true, SDK::FLinearColor(0, 0, 0, 1));

					}

					auto NeckBone = SDK::Functions::GetSocketLocation(mesh, defines::NeckName);
					auto HeadBones = SDK::Functions::GetSocketLocation(mesh, defines::HeadName);
					auto LowerNeckBone = SDK::Functions::GetSocketLocation(mesh, defines::LowerNeckName);
					auto ChestBone = SDK::Functions::GetSocketLocation(mesh, defines::ChestName);

					if (defines::MyPlayer && defines::MyPlayer != current_actor) {
						auto dist = SDK::Functions::UKismetMathLibrarySTATIC_Sqrt((SDK::Functions::vHeadBoneOut.X - (defines::Width / 2)) * (SDK::Functions::vHeadBoneOut.X - (defines::Width / 2)) + (SDK::Functions::vHeadBoneOut.Y - (defines::Height / 2)) * (SDK::Functions::vHeadBoneOut.Y - (defines::Height / 2)));
						if (aimVisible && dist < FOVRange && dist < closestDistance) {
							closestDistance = dist;

							defines::halt = current_actor;
							switch (BoneTarget) {
							case 0:
								defines::AimLocation = HeadBones;
								break;
							case 1:
								defines::AimLocation = LowerNeckBone;
								break;
							case 2:
								defines::AimLocation = ChestBone;
								break;
							}

						}
					}
				}

				if (SDK::Functions::IsA(current_actor, FloorPortal)) {

					auto PortalScrollType = SDK::Functions::read<uint8_t>(current_actor + 0x3E0);

					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector ScreenLocation = SDK::Functions::K2_Project(location);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > overallPortalDistance) continue;

					auto valid_info = SDK::Functions::IsValidScreenLoc(ScreenLocation);

					if (valid_info && drawPortalz) {
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, L"[Portal]", SDK::FVector2D(0.6f, 0.6f));
						auto textLocation = SDK::FVector2D(ScreenLocation.X - (size.X / 2), ScreenLocation.Y);
						SDK::Functions::K2_DrawText(defines::fnFont2, L"[Portal]", SDK::FVector2D(textLocation.X, textLocation.Y),
							SDK::FVector2D(0.6f, 0.6f), SDK::FLinearColor(245.0f, 40.0f, 145.0f, 0.8f), 1.0f,
							SDK::FLinearColor(0, 0, 0, 1), SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}

				}

				if (SDK::Functions::IsA(current_actor, chest )) {
					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector Screen = SDK::Functions::K2_Project(location);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > overallChestDistance) continue;

					auto valid_info = SDK::Functions::IsValidScreenLoc(Screen);

					if (valid_info && drawNormalChest ) {
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, L"[Chest]", SDK::FVector2D(0.3f, 0.3f));
						auto textLocation = SDK::FVector2D(Screen.X - (size.X / 2), Screen.Y);
						SDK::Functions::K2_DrawText(defines::fnFont2, L"[Chest]", SDK::FVector2D(textLocation.X, textLocation.Y), 
							SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(236.0f, 252.0f, 3.0f, 0.95f), 1.0f, SDK::FLinearColor(0, 0, 0, 1),
							SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}
				}

				if (SDK::Functions::IsA(current_actor, Trap )) {
					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector Screen = SDK::Functions::K2_Project(location);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > trapDistance) continue;

					auto valid_info = SDK::Functions::IsValidScreenLoc(Screen);

					if (valid_info && drawTrap ) {
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, L"[Trap]", SDK::FVector2D(0.4f, 0.3f));
						auto textLocation = SDK::FVector2D(Screen.X - (size.X / 2), Screen.Y);
							SDK::Functions::K2_DrawText(defines::fnFont2, L"[Trap]", SDK::FVector2D(textLocation.X, textLocation.Y), 
							SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(255.0f, 169.0f, 0.0f, 1.0f), 1.0f,
							SDK::FLinearColor(0, 0, 0, 1), SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}

				}

				if (SDK::Functions::IsA(current_actor, LuckPot)) {
					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector Screen = SDK::Functions::K2_Project(location);

					auto valid_info = SDK::Functions::IsValidScreenLoc(Screen);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > LuckPotDistance) continue;

					if (valid_info) {
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, L"[Luck Potion]", SDK::FVector2D(0.3f, 0.3f));
						auto textLocation = SDK::FVector2D(Screen.X - (size.X / 2), Screen.Y);
						SDK::Functions::K2_DrawText(defines::fnFont2, L"[Luck Potion]", SDK::FVector2D(textLocation.X, textLocation.Y),
							SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(63.0f, 245.0f, 39.0f, 0.8f), 1.0f, SDK::FLinearColor(0, 0, 0, 1),
							SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}

				}

				if (SDK::Functions::IsA(current_actor, HealPot)) {
					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector Screen = SDK::Functions::K2_Project(location);

					auto valid_info = SDK::Functions::IsValidScreenLoc(Screen);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > HealPotDistance) continue;

					if (valid_info && drawHealPot) {
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, L"[Healing Potion]", SDK::FVector2D(0.3f, 0.3f));
						auto textLocation = SDK::FVector2D(Screen.X - (size.X / 2), Screen.Y);
						SDK::Functions::K2_DrawText(defines::fnFont2, L"[Healing Potion]", SDK::FVector2D(textLocation.X, textLocation.Y), 
							SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(234.0f, 39.0f, 245.0f, 0.8f), 1.0f, SDK::FLinearColor(0, 0, 0, 1),
							SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}

				}

				if (SDK::Functions::IsA(current_actor, SpecialChest )) 
				{
					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector Screen = SDK::Functions::K2_Project(location);

					auto valid_info = SDK::Functions::IsValidScreenLoc(Screen);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > SpecialChestDistance) continue;

					if (valid_info && drawSpecialChest ) {
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, L"[Special Chest]", SDK::FVector2D(0.3f, 0.3f));
						auto textLocation = SDK::FVector2D(Screen.X - (size.X / 2), Screen.Y);
						SDK::Functions::K2_DrawText(defines::fnFont2, L"[Special Chest]", SDK::FVector2D(textLocation.X, textLocation.Y),
							SDK::FVector2D(0.3f, 0.3f), SDK::FLinearColor(255.0f, 0.0f, 255.0f, 1.0f), 1.0f, SDK::FLinearColor(0, 0, 0, 1),
							SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}
				}

				if (SDK::Functions::IsA(current_actor, HolderBase))
				{
					auto location = SDK::Functions::K2_GetActorLocation(current_actor);

					SDK::FVector Screen = SDK::Functions::K2_Project(location);

					auto valid_info = SDK::Functions::IsValidScreenLoc(Screen);

					double distance = SDK::Functions::STATIC_Vector_Distance(location, SDK::Functions::GetCameraLocation()) * 0.01;
					if (distance > HealPotDistance) continue;

					auto Name = SDK::Functions::read<SDK::FName>(current_actor + 0x348);

					auto NameString = SDK::Functions::STATIC_Conv_TextToString(SDK::Functions::STATIC_Conv_NameToText(Name));

					if (SDK::Functions::STATIC_EndsWith(NameString, L"_2001", SDK::ESearchCase::CaseSensitive)) 
						defines::LootColour = SDK::FLinearColor(255, 255, 255, 255);

					else if (SDK::Functions::STATIC_EndsWith(NameString, L"_3001", SDK::ESearchCase::CaseSensitive))
						defines::LootColour = SDK::FLinearColor(0, 255, 0, 255);

					else if (SDK::Functions::STATIC_EndsWith(NameString, L"_4001", SDK::ESearchCase::CaseSensitive))
						defines::LootColour = SDK::FLinearColor(0, 255, 255, 255);

					else
						defines::LootColour = SDK::FLinearColor(192, 192, 192, 192);


					SDK::FString ItemName = SDK::Functions::STATIC_Concat_StrStr(xor_w(L""), NameString);

					if (valid_info && drawInteractable && SDK::Functions::IsInputKeyDown(defines::LeftAlt))
					{
						auto size = SDK::Functions::K2_TextSize(defines::fnFont2, ItemName, SDK::FVector2D(0.3f, 0.3f));
						auto textLocation = SDK::FVector2D(Screen.X - (size.X / 2), Screen.Y);
						
						SDK::Functions::K2_DrawText(defines::fnFont2, ItemName, SDK::FVector2D(textLocation.X, textLocation.Y),
							SDK::FVector2D(0.3f, 0.3f), defines::LootColour, 1.0f, SDK::FLinearColor(0, 0, 0, 1), 
							SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
					}
				}
			}
		}

		if (SDK::Functions::WasInputKeyJustPressed(defines::PlayerController, defines::Insert)) {
			ShowSexyMenu = !ShowSexyMenu;
		}

		if (ShowSexyMenu) {
			DrawMenu(defines::UCanvas);
		}

		if (defines::halt) {
			if (aimbot && SDK::Functions::IsInputKeyDown(defines::ThumbMouseButton2))
			{
				SDK::Functions::InitializeAimbot(defines::halt, defines::AimLocation);
			}
		}
	}
	return origin_renderer(UObject, pCanvas);
}

bool Main() {

	defines::BaseAddr = (uintptr_t)GetModuleBase_Wrapper(nullptr);

	if (defines::BaseAddr) {

		uintptr_t gadget = (uintptr_t)United((HMODULE)defines::BaseAddr,
			xor_a("\xFF\xE3"),
			xor_a("xx"), 0);

		uintptr_t tramp1 = (uintptr_t)United((HMODULE)defines::BaseAddr,
			xor_a("\x48\x83\xC4\x68\xC3"),
			xor_a("xxxxx"), 0);

		legitTrampoline = (PVOID)tramp1;
		legitTrampoline2 = (PVOID)gadget;

		auto StaticFindObjectPtr = (uintptr_t)United((HMODULE)defines::BaseAddr,
			xor_a("\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x68\xFE\xFF\xFF\x48\x81\xEC\x98\x02\x00\x00\x48\x8B\x05\xCC\xCC\xCC\xCC\x48\x33\xC4\x48\x89\x85\x80\x01\x00\x00\x4C\x8B"),
			xor_a("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx"), 0);
		defines::StaticFindFirstObject_t = reinterpret_cast<decltype(defines::StaticFindFirstObject_t)>(StaticFindObjectPtr);

		auto StaticLoadObjectPtr = (uintptr_t)United((HMODULE)defines::BaseAddr,
			xor_a("\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\xA8\xFE\xFF\xFF\x48\x81\xEC\x58\x02\x00\x00\x48\x8B\x05\xCC\xCC\xCC\xCC\x48\x33\xC4\x48\x89\x85\x48"),
			xor_a("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxx"), 0);
		defines::StaticLoadObject_t = reinterpret_cast<decltype(defines::StaticLoadObject_t)>(StaticLoadObjectPtr);

		initObjects();
		
		if (init_objects) {

			auto world = SDK::Functions::read<uint64_t>(defines::BaseAddr + 0x7B767D0);

			uint64_t OwningGameInstance = SDK::Functions::read<uint64_t>(world + 0x1B8);
			
			uint64_t LocalPlayers = SDK::Functions::read<uint64_t>(OwningGameInstance + 0x38);
			uint64_t LocalPlayer = SDK::Functions::read<uint64_t>(LocalPlayers);
			uint64_t PlayerController = SDK::Functions::read<uint64_t>(LocalPlayer + 0x30);
			uint64_t ViewportClient = SDK::Functions::read<uint64_t>(LocalPlayer + 0x78);

			SDK::Functions::ShadowHookObject((uint64_t)ViewportClient,
				MyPostrender, (void**)&origin_renderer, 110);
		
			/*SDK::Functions::ShadowHookObject((uint64_t)PlayerController,
				SDK::GetPlayerViewPoint_hook, (void**)&defines::GetPlayerViewPoint_original, 236);*/
		}
	}

	return false;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == 1) {
		Main();
	}
	return true;
}

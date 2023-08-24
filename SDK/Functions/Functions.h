#pragma once
#include <string>

namespace SDK {

	namespace Functions {

		static void SecureZeroMemoryCustom(void* pDest, size_t nSize)
		{
			unsigned char* p = (unsigned char*)pDest;
			for (size_t i = 0; i < nSize; i++)
			{
				p[i] = 0;
			}
		}

		static void dbgprint(const char* string, ...)
		{
			char buf[512];
			va_list arg_list;

			SecureZeroMemoryCustom(buf, sizeof(buf));

			va_start(arg_list, string);
			LI_FN(vsnprintf).get()(buf, sizeof(buf), string, arg_list);
			va_end(arg_list);

			LI_FN(OutputDebugStringA).get()(buf);

			return;
		}

		template <class T>
		inline bool write(uint64_t address, T value)
		{
			*(T*)(address) = value;
			return true;
		}

		template <class t>
		t read(uint64_t address)
		{
			return *(t*)(address);
		}

#define vpnter(ptr) (((unsigned long long)ptr < 0x7FFFFFFEFFFF && (unsigned long long)ptr > 0x00010000))
		static SIZE_T GetVtableSize(uint64_t class_member)
		{
			auto addr = read<uintptr_t>(class_member);
			if (!addr) return 0;

			int total_size = 0, current_table = 0;

			while (vpnter(read<uint64_t>(addr + (current_table * sizeof(uint64_t)))))
			{
				total_size += 1;
				current_table++;
			}

			return total_size;
		}

		static bool ShadowHookObject(uint64_t Object, void* HookFunction, void** HookOrig, DWORD Index)
		{
			SIZE_T VtableSize = GetVtableSize(Object);

			uint64_t Page = 0; PVOID Page2 = 0;
			if (!Page)
				Page = (uint64_t)LI_FN(VirtualAlloc).safe_cached()(0, VtableSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			uint64_t CustomVT = (uint64_t)LI_FN(calloc).safe_cached()(VtableSize, sizeof(uint64_t));

			void* ObjectVTable = read<void*>(Object);

			custom_MemCpy((void*)CustomVT, ObjectVTable, VtableSize * sizeof(uint64_t));

			uint8_t HookTemplate[14] = { 0xFF, 0x25, 0, 0, 0, 0 };

			*(void**)&HookTemplate[6] = HookFunction;
			custom_MemCpy((void*)Page, HookTemplate, sizeof(HookTemplate));

			*(uint64_t*)&HookTemplate[6] = *(uint64_t*)((unsigned char*)CustomVT + (Index * sizeof(PVOID)));

			custom_MemCpy((void*)(Page + sizeof(HookTemplate)), HookTemplate, sizeof(HookTemplate));

			*HookOrig = (void*)(Page + sizeof(HookTemplate));
			*(uint64_t*)((unsigned char*)CustomVT + (Index * sizeof(PVOID))) = Page;

			uint64_t VT = (uint64_t)CustomVT;
			write<uint64_t>(Object, VT);

			return true;
		}

		static void ProcessEvent(uint64_t Object, uint64_t Function, void* Args)
		{
			auto vtable = *reinterpret_cast<void***>(Object);
			auto fn = reinterpret_cast<void(*)(uint64_t, uint64_t, void*)>(vtable[0x4C]);
			fn(Object, Function, Args);
		}

		static uint64_t pLoadObject(const wchar_t* ObjectName)
		{
			return (uint64_t)defines::StaticFindFirstObject_t(0, (WCHAR*)ObjectName, 0, 0, 0);
		}

		static uint64_t StaticLoadObject(const wchar_t* InName)
		{
			return (uint64_t)SpoofCaller(defines::StaticLoadObject_t, nullptr, nullptr, (TCHAR*)InName, nullptr, 0, nullptr, false, nullptr);
		}

		static uint64_t STATIC_GetPlayerController()
		{
			if (!defines::fnGetPlayerController) {
				defines::fnGetPlayerController = pLoadObject(xor_w(L"GameplayStatics:GetPlayerController"));
			}

			UGameplayStatics_GetPlayerController_Params params;
			params.WorldContextObject = defines::GWorld;
			params.PlayerIndex = 0;

			ProcessEvent(defines::GameplayStatics, defines::fnGetPlayerController, &params);

			return params.ReturnValue;
		}

		static uint64_t STATIC_GetPlayerCameraManager()
		{
			if (!defines::fnGetPlayerCameraManager) {
				defines::fnGetPlayerCameraManager = pLoadObject(xor_w(L"GameplayStatics:GetPlayerCameraManager"));
			}

			UGameplayStatics_GetPlayerCameraManager_Params params;
			params.WorldContextObject = defines::GWorld;
			params.PlayerIndex = 0;

			ProcessEvent(defines::GameplayStatics, defines::fnGetPlayerCameraManager, &params);

			return params.ReturnValue;
		}

		static uint64_t K2_GetPawn()
		{
			if (!defines::fnGetPawn) {
				defines::fnGetPawn = pLoadObject(xor_w(L"Controller:K2_GetPawn"));
			}

			AController_K2_GetPawn_Params params;

			ProcessEvent(defines::PlayerController, defines::fnGetPawn, &params);

			return params.ReturnValue;
		}

		static void GetViewportSize(uintptr_t PlayerController, int* SizeX, int* SizeY)
		{

			if (!defines::fnGetViewportSize) {
				defines::fnGetViewportSize = pLoadObject(xor_w(L"PlayerController:GetViewportSize"));
			}

			APlayerController_GetViewportSize_Params params;

			ProcessEvent(PlayerController, defines::fnGetViewportSize, &params);

			if (SizeX != nullptr)
				*SizeX = params.SizeX;
			if (SizeY != nullptr)
				*SizeY = params.SizeY;
		}

		static FName STATIC_Conv_StringToName(const struct FString& InString)
		{
			if (!defines::Conv_StringToName) {
				defines::Conv_StringToName = pLoadObject(xor_w(L"KismetStringLibrary:Conv_StringToName"));
			}

			UKismetStringLibrary_Conv_StringToName_Params params;
			params.InString = InString;

			ProcessEvent(defines::KismetStringLibrary, defines::Conv_StringToName, &params);

			return params.ReturnValue;
		}

		static void STATIC_GetAllActorsOfClass(uint64_t ActorClass, TArray<uint64_t*>* OutActors)
		{
			if (!defines::fnGetAllActorsOfClass) {
				defines::fnGetAllActorsOfClass = pLoadObject(xor_w(L"GameplayStatics:GetAllActorsOfClass"));
			}

			UGameplayStatics_GetAllActorsOfClass_Params params;
			params.WorldContextObject = defines::GWorld;
			params.ActorClass = ActorClass;

			ProcessEvent(defines::GameplayStatics, defines::fnGetAllActorsOfClass, &params);

			if (OutActors != nullptr)
				*OutActors = params.OutActors;
		}

		static void K2_DrawBox(const struct FVector2D& ScreenPosition, const struct FVector2D& ScreenSize, float Thickness, const struct FLinearColor& RenderColor)
		{
			if (!defines::fnK2_DrawBox) {
				defines::fnK2_DrawBox = pLoadObject(xor_w(L"Canvas:K2_DrawBox"));
			}

			UCanvas_K2_DrawBox_Params params;
			params.ScreenPosition = ScreenPosition;
			params.ScreenSize = ScreenSize;
			params.Thickness = Thickness;
			params.RenderColor = RenderColor;

			ProcessEvent(defines::UCanvas, defines::fnK2_DrawBox, &params);
		}

		static double UKismetMathLibrarySTATIC_Cos(double A)
		{
			if (!defines::KIMATH_Cos) {
				defines::KIMATH_Cos = pLoadObject(xor_w(L"KismetMathLibrary:Cos"));
			}

			UKismetMathLibrary_Cos_Params params;
			params.A = A;

			ProcessEvent(defines::KismetMathLibrary, defines::KIMATH_Cos, &params);

			return params.ReturnValue;
		}

		static double UKismetMathLibrarySTATIC_Abs(double A)
		{

			if (!defines::KIMATH_ABS) {
				defines::KIMATH_ABS = pLoadObject(xor_w(L"KismetMathLibrary:Abs"));
			}

			UKismetMathLibrary_Abs_Params params;
			params.A = A;

			ProcessEvent(defines::KismetMathLibrary, defines::KIMATH_ABS, &params);

			return params.ReturnValue;
		}

		static void K2_DrawLine(FVector2D A, FVector2D B, float Thickness, FLinearColor Color)
		{
			if (!defines::fnK2_DrawLine) {
				defines::fnK2_DrawLine = pLoadObject(xor_w(L"Canvas:K2_DrawLine"));
			}

			UCanvas_K2_DrawLine_Params params;
			params.ScreenPositionA = A;
			params.ScreenPositionB = B;
			params.Thickness = Thickness;
			params.RenderColor = Color;

			ProcessEvent(defines::UCanvas, defines::fnK2_DrawLine, &params);
		}

		static void K2_DrawText(uintptr_t font, const struct FString& RenderText, const struct FVector2D& ScreenPosition, const struct FVector2D& Scale, const struct FLinearColor& RenderColor, double Kerning, const struct FLinearColor& ShadowColor, const struct FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, const struct FLinearColor& OutlineColor)
		{
			if (!defines::fnK2_DrawText) {
				defines::fnK2_DrawText = pLoadObject(xor_w(L"Canvas:K2_DrawText"));
			}

			UCanvas_K2_DrawText_Params params;
			params.RenderFont = font;
			params.RenderText = RenderText;
			params.ScreenPosition = ScreenPosition;
			params.Scale = Scale;
			params.RenderColor = RenderColor;
			params.Kerning = Kerning;
			params.ShadowColor = ShadowColor;
			params.ShadowOffset = ShadowOffset;
			params.bCentreX = bCentreX;
			params.bCentreY = bCentreY;
			params.bOutlined = bOutlined;
			params.OutlineColor = OutlineColor;

			ProcessEvent(defines::UCanvas, defines::fnK2_DrawText, &params);
		}

		static bool GetMousePosition(float* LocationX, float* LocationY)
		{
			if (!defines::fnGetMousePosition) {
				defines::fnGetMousePosition = pLoadObject(xor_w(L"Engine.PlayerController.GetMousePosition"));
			}

			APlayerController_GetMousePosition_Params params;

			ProcessEvent(defines::PlayerController, defines::fnGetMousePosition, &params);

			if (LocationX != nullptr)
				*LocationX = params.LocationX;
			if (LocationY != nullptr)
				*LocationY = params.LocationY;

			return params.ReturnValue;
		}

		static void FOV(float NewFOV)
		{
			if (!defines::fnFOV) {
				defines::fnFOV = pLoadObject(xor_w(L"PlayerController:FOV"));
			}

			APlayerController_FOV_Params params;
			params.NewFOV = NewFOV;

			ProcessEvent(defines::PlayerController, defines::fnFOV, &params);
		}

		static bool WasInputKeyJustPressed(uint64_t object, const struct FKey& Key)
		{
			if (!defines::fnWasInputKeyJustPressed) {
				defines::fnWasInputKeyJustPressed = pLoadObject(xor_w(L"PlayerController:WasInputKeyJustPressed"));
			}

			APlayerController_WasInputKeyJustPressed_Params params;
			params.Key = Key;

			ProcessEvent(object, defines::fnWasInputKeyJustPressed, &params);

			return params.ReturnValue;
		}

		static bool WasInputKeyJustReleased(const struct FKey& Key)
		{
			if (!defines::fnWasInputKeyJustReleased) {
				defines::fnWasInputKeyJustReleased = pLoadObject(xor_w(L"PlayerController:WasInputKeyJustReleased"));
			}

			APlayerController_WasInputKeyJustReleased_Params params;
			params.Key = Key;

			ProcessEvent(defines::PlayerController, defines::fnWasInputKeyJustReleased, &params);

			return params.ReturnValue;
		}

		static bool IsInputKeyDown(const struct FKey& Key)
		{

			if (!defines::fnIsInputKeyDown) {
				defines::fnIsInputKeyDown = pLoadObject(xor_w(L"PlayerController:IsInputKeyDown"));
			}

			APlayerController_IsInputKeyDown_Params params;
			params.Key = Key;

			ProcessEvent(defines::PlayerController, defines::fnIsInputKeyDown, &params);

			return params.ReturnValue;
		}

		static FVector GetSocketLocation(uint64_t object, const struct FName& InSocketName)
		{
			if (!defines::fnGetSocketLocation) {
				defines::fnGetSocketLocation = pLoadObject(xor_w(L"SceneComponent:GetSocketLocation"));
			}

			USceneComponent_GetSocketLocation_Params params;
			params.InSocketName = InSocketName;

			ProcessEvent(object, defines::fnGetSocketLocation, &params);

			return params.ReturnValue;
		}

		static FVector K2_GetActorLocation(uint64_t actor)
		{
			if (!defines::fnGetActorLocation) {
				defines::fnGetActorLocation = pLoadObject(xor_w(L"Actor:K2_GetActorLocation"));
			}

			AActor_K2_GetActorLocation_Params params;

			ProcessEvent(actor, defines::fnGetActorLocation, &params);

			return params.ReturnValue;
		}

		static int32_t AddMeshHalfTranslucent(uint64_t actor, uint64_t InMesh, uint64_t InMaterial, bool IsRenderingPriority)
		{
			if (!defines::fnAddMeshHalfTranslucent) {
				defines::fnAddMeshHalfTranslucent = pLoadObject(xor_w(L"DCPostProcessingComponent:AddMeshHalfTranslucent"));
			}

			UDCPostProcessingComponent_AddMeshHalfTranslucent_Params params{};
			params.InMesh = InMesh;
			params.InMaterial = InMaterial;
			params.IsRenderingPriority = IsRenderingPriority;

			ProcessEvent(actor, defines::fnAddMeshHalfTranslucent, &params);

			return params.ReturnValue;
		}

		static FVector K2_Project(const struct FVector& WorldLocation)
		{
			if (!defines::fnK2_Project) {
				defines::fnK2_Project = pLoadObject(xor_w(L"Canvas:K2_Project"));
			}

			UCanvas_K2_Project_Params params;
			params.WorldLocation = WorldLocation;

			ProcessEvent(defines::UCanvas, defines::fnK2_Project, &params);

			return params.ReturnValue;
		}

		static bool IsValidScreenLoc(const FVector& Loc)
		{
			return Loc.X >= 0 && Loc.Y >= 0 && Loc.Z > 0;
		}

		static FVector vRootOut, vHeadBoneOut, vNeckOut, vLowerNeckOut, vChestOut, pelvisOut, vUpperArmLeftOut, vUpperArmRightOut, vLeftHandOut, vRightHandOut, vLeftHandOut1, vRightHandOut1, vRightThighOut, vLeftThighOut, vRightKneeOut, vLeftKneeOut, vLeftLowCalfOut, vRightLowCalfOut, leftFeet2d, rightFeet2d, leftFeetFinger2d, rightFeetFinger2d;
		struct Bone {
			FName name;
			FVector location;
		};
		static Bone bones[22];

		static void GetAllBones(uint64_t PlayerMesh) {
			// Known index of the head bone
		//	const int headBoneIndex = 1;

			static const FName boneNames[] = {
				defines::RootName, defines::HeadName, defines::NeckName, defines::LowerNeckName, defines::ChestName, defines::PelvisName, defines::Upperarm_lName, defines::Upperarm_rName, defines::Lowerarm_lName,
				defines::Lowerarm_rName,defines::Hand_lName, defines::Hand_rName, defines::Thigh_rName, defines::Thigh_lName, defines::Calf_rName, defines::Calf_lName,
				defines::LowerCalf_lName, defines::LowerCalf_rName, defines::Foot_lName, defines::Foot_rName, defines::Toe_lName, defines::Toe_rName
			};

			for (int i = 0; i < 22; i++) {
				bones[i].name = boneNames[i];
				bones[i].location = GetSocketLocation(PlayerMesh, boneNames[i]);

				// If this is the head bone, add 15 to Z axis
				/*if (i == headBoneIndex) {
					bones[i].location.Z += 15;
				}*/

				// Project the location of each bone onto the screen
				bones[i].location = K2_Project(bones[i].location);
			}

			// Assign the correct values to the variables
			vRootOut = bones[0].location;
			vHeadBoneOut = bones[1].location;
			vNeckOut = bones[2].location;
			vLowerNeckOut = bones[3].location;
			vChestOut = bones[4].location;
			pelvisOut = bones[5].location;
			vUpperArmLeftOut = bones[6].location;
			vUpperArmRightOut = bones[7].location;
			vLeftHandOut = bones[8].location;
			vRightHandOut = bones[9].location;
			vLeftHandOut1 = bones[10].location;
			vRightHandOut1 = bones[11].location;
			vRightThighOut = bones[12].location;
			vLeftThighOut = bones[13].location;
			vRightKneeOut = bones[14].location;
			vLeftKneeOut = bones[15].location;
			vLeftLowCalfOut = bones[16].location;
			vRightLowCalfOut = bones[17].location;
			leftFeet2d = bones[18].location;
			rightFeet2d = bones[19].location;
			leftFeetFinger2d = bones[20].location;
			rightFeetFinger2d = bones[21].location;
		}

		static bool IsA(uintptr_t actor, uintptr_t other)
		{
			for (uintptr_t super = read<uintptr_t>(actor + 0x10); super; super = read<uintptr_t>(super + 0x40))
			{
				if (super == other)
				{
					return true;
				}
			}

			return false;
		}

		static bool InitializeOnce = true;
		static void fInitializeOnce() {
			if (InitializeOnce == true) {
				//// Assign FKeys ////
				defines::Insert = FKey(STATIC_Conv_StringToName(xor_w(L"Insert")));/*FKey(FName(*xor_w(L"Insert")));*/
				defines::PageUp = FKey(STATIC_Conv_StringToName(xor_w(L"PageUp")));

				// Controller Stuff
				defines::Gamepad_DPad_Up = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_DPad_Up")));
				defines::Gamepad_DPad_Down = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_DPad_Down")));
				defines::Gamepad_DPad_Left = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_DPad_Left")));
				defines::Gamepad_DPad_Right = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_DPad_Right")));
				defines::Gamepad_LeftShoulder = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_LeftShoulder")));
				defines::Gamepad_RightShoulder = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_RightShoulder")));
				defines::Gamepad_LeftTrigger = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_LeftTrigger")));
				defines::Gamepad_RightTrigger = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_RightTrigger")));
				defines::Gamepad_FaceButton_Bottom = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_FaceButton_Bottom")));
				defines::Gamepad_Special_Right = FKey(STATIC_Conv_StringToName(xor_w(L"Gamepad_Special_Right")));

				// Mouse buttons
				defines::LeftClick = FKey(STATIC_Conv_StringToName(xor_w(L"LeftMouseButton")));
				defines::RightClick = FKey(STATIC_Conv_StringToName(xor_w(L"RightMouseButton")));
				defines::MiddleMouseButton = FKey(STATIC_Conv_StringToName(xor_w(L"MiddleMouseButton")));
				defines::ThumbMouseButton = FKey(STATIC_Conv_StringToName(xor_w(L"ThumbMouseButton")));
				defines::ThumbMouseButton2 = FKey(STATIC_Conv_StringToName(xor_w(L"ThumbMouseButton2")));

				defines::LeftShift = FKey(STATIC_Conv_StringToName(xor_w(L"LeftShift")));
				defines::LeftControl = FKey(STATIC_Conv_StringToName(xor_w(L"LeftControl")));
				defines::LeftAlt = FKey(STATIC_Conv_StringToName(xor_w(L"LeftAlt")));
				defines::Enter = FKey(STATIC_Conv_StringToName(xor_w(L"Enter")));

				defines::Up = FKey(STATIC_Conv_StringToName(xor_w(L"Up")));
				defines::Down = FKey(STATIC_Conv_StringToName(xor_w(L"Down")));
				defines::Left = FKey(STATIC_Conv_StringToName(xor_w(L"Left")));
				defines::Right = FKey(STATIC_Conv_StringToName(xor_w(L"Right")));

				defines::Zero = FKey(STATIC_Conv_StringToName(xor_w(L"Zero")));
				defines::One = FKey(STATIC_Conv_StringToName(xor_w(L"One")));
				defines::Two = FKey(STATIC_Conv_StringToName(xor_w(L"Two")));
				defines::Three = FKey(STATIC_Conv_StringToName(xor_w(L"Three")));

				defines::F1 = FKey(STATIC_Conv_StringToName(xor_w(L"F1")));
				defines::F2 = FKey(STATIC_Conv_StringToName(xor_w(L"F2")));
				defines::F3 = FKey(STATIC_Conv_StringToName(xor_w(L"F3")));
				defines::F4 = FKey(STATIC_Conv_StringToName(xor_w(L"F4")));
				defines::F6 = FKey(STATIC_Conv_StringToName(xor_w(L"F6")));

				// Assign Fonts
				if (!defines::fnFont) {
					defines::fnFont = pLoadObject(xor_w(L"NotoSans.NotoSans"));
				}
				if (!defines::fnFont2) {
					defines::fnFont2 = pLoadObject(xor_w(L"SaintKDG_Font.SaintKDG_Font"));
				}
				if (!defines::RobotoFont) {
					defines::RobotoFont = pLoadObject(xor_w(L"Roboto.Roboto"));
				}

				defines::HeadName = STATIC_Conv_StringToName(xor_w(L"head"));
				defines::RootName = STATIC_Conv_StringToName(xor_w(L"Root"));
				defines::NeckName = STATIC_Conv_StringToName(xor_w(L"neck_01"));
				defines::LowerNeckName = STATIC_Conv_StringToName(xor_w(L"clavicle_l"));
				defines::ChestName = STATIC_Conv_StringToName(xor_w(L"spine_03"));
				defines::PelvisName = STATIC_Conv_StringToName(xor_w(L"pelvis"));
				defines::ChestName = STATIC_Conv_StringToName(xor_w(L"spine_03"));
				defines::Upperarm_lName = STATIC_Conv_StringToName(xor_w(L"upperarm_l"));
				defines::Upperarm_rName = STATIC_Conv_StringToName(xor_w(L"upperarm_r"));
				defines::Lowerarm_lName = STATIC_Conv_StringToName(xor_w(L"lowerarm_l"));
				defines::Lowerarm_rName = STATIC_Conv_StringToName(xor_w(L"lowerarm_r"));
				defines::Hand_lName = STATIC_Conv_StringToName(xor_w(L"hand_l"));
				defines::Hand_rName = STATIC_Conv_StringToName(xor_w(L"hand_r"));
				defines::Thigh_lName = STATIC_Conv_StringToName(xor_w(L"thigh_l"));
				defines::Thigh_rName = STATIC_Conv_StringToName(xor_w(L"thigh_r"));
				defines::Calf_lName = STATIC_Conv_StringToName(xor_w(L"calf_l"));
				defines::Calf_rName = STATIC_Conv_StringToName(xor_w(L"calf_r"));
				defines::LowerCalf_lName = STATIC_Conv_StringToName(xor_w(L"calf_twist_02_l"));
				defines::LowerCalf_rName = STATIC_Conv_StringToName(xor_w(L"calf_twist_02_r"));
				defines::Foot_lName = STATIC_Conv_StringToName(xor_w(L"ik_foot_l"));
				defines::Foot_rName = STATIC_Conv_StringToName(xor_w(L"ik_foot_r"));
				defines::Toe_lName = STATIC_Conv_StringToName(xor_w(L"ball_l"));
				defines::Toe_rName = STATIC_Conv_StringToName(xor_w(L"ball_r"));

				InitializeOnce = false;
			}
		}

		static FRotator SmoothMe(FRotator startRotation, FRotator endRotation, float SmoothScale)
		{
			FRotator ret = endRotation;

			ret.Pitch -= startRotation.Pitch;
			ret.Yaw -= startRotation.Yaw;

			//clamp
			if (ret.Yaw < -180.0f)
				ret.Yaw += 360.0f;

			if (ret.Yaw > 180.0f)
				ret.Yaw -= 360.0f;

			if (ret.Pitch < -74.0f)
				ret.Pitch = -74.0f;

			if (ret.Pitch > 74.0f)
				ret.Pitch = 74.0f;

			ret.Pitch = (ret.Pitch / SmoothScale) + startRotation.Pitch;
			ret.Yaw = (ret.Yaw / SmoothScale) + startRotation.Yaw;

			//clamp
			if (ret.Yaw < -180.0f)
				ret.Yaw += 360.0f;

			if (ret.Yaw > 180.0f)
				ret.Yaw -= 360.0f;

			if (ret.Pitch < -74.0f)
				ret.Pitch = -74.0f;

			if (ret.Pitch > 74.0f)
				ret.Pitch = 74.0f;

			ret.Roll = 0;

			return ret;
		}

		static struct FRotator STATIC_FindLookAtRotation(const struct FVector& Start, const struct FVector& Target)
		{
			if (!defines::fnFindLookAtRotation) {
				defines::fnFindLookAtRotation = pLoadObject(xor_w(L"KismetMathLibrary:FindLookAtRotation"));
			}

			UKismetMathLibrary_FindLookAtRotation_Params params;
			params.Start = Start;
			params.Target = Target;

			ProcessEvent(defines::KismetMathLibrary, defines::fnFindLookAtRotation, &params);

			return params.ReturnValue;
		}

		static struct FRotator GetCameraRotation()
		{
			if (!defines::fnGetCameraRotation) {
				defines::fnGetCameraRotation = pLoadObject(xor_w(L"PlayerCameraManager:GetCameraRotation"));
			}

			APlayerCameraManager_GetCameraRotation_Params params;

			ProcessEvent(defines::PlayerCameraManager, defines::fnGetCameraRotation, &params);

			return params.ReturnValue;
		}

		static struct FVector GetCameraLocation()
		{
			if (!defines::fnGetCameraLocation) {
				defines::fnGetCameraLocation = pLoadObject(xor_w(L"PlayerCameraManager:GetCameraLocation"));
			}

			APlayerCameraManager_GetCameraLocation_Params params;

			ProcessEvent(defines::PlayerCameraManager, defines::fnGetCameraLocation, &params);

			return params.ReturnValue;
		}

		static void SetControlRotation(const struct FRotator& NewRotation)
		{
			if (!defines::fnSetControlRotation) {
				defines::fnSetControlRotation = pLoadObject(xor_w(L"Controller:SetControlRotation"));
			}

			AController_SetControlRotation_Params params;
			params.NewRotation = NewRotation;

			ProcessEvent(defines::PlayerController, defines::fnSetControlRotation, &params);
		}

		static double UKismetMathLibrarySTATIC_Sqrt(double A)
		{
			if (!defines::KIMATH_Sqrt) {
				defines::KIMATH_Sqrt = pLoadObject(xor_w(L"KismetMathLibrary:Sqrt"));
			}

			UKismetMathLibrary_Sqrt_Params params;
			params.A = A;

			ProcessEvent(defines::KismetMathLibrary, defines::KIMATH_Sqrt, &params);

			return params.ReturnValue;
		}

		static uintptr_t GetMaterial(uint64_t mesh, int32_t MaterialIndex)
		{
			if (!defines::fnGetMaterial) {
				defines::fnGetMaterial = pLoadObject(xor_w(L"StaticMesh:GetMaterial"));
			}

			UStaticMesh_GetMaterial_Params params{};
			params.MaterialIndex = MaterialIndex;

			ProcessEvent(mesh, defines::fnGetMaterial, &params);

			return params.ReturnValue;
		}

		static void SetMaterial(uint64_t Primcomponent, int32_t ElementIndex, uintptr_t Material)
		{
			if (!defines::fnSetMaterial) {
				defines::fnSetMaterial = pLoadObject(xor_w(L"PrimitiveComponent:SetMaterial"));
			}

			UPrimitiveComponent_SetMaterial_Params params{};
			params.ElementIndex = ElementIndex;
			params.Material = Material;

			ProcessEvent(Primcomponent, defines::fnSetMaterial, &params);
		}

		static int32_t GetNumMaterials(uint64_t mesh)
		{
			if (!defines::fnGetNumMaterial) {
				defines::fnGetNumMaterial = pLoadObject(xor_w(L"PrimitiveComponent:GetNumMaterials"));
			}

			UPrimitiveComponent_GetNumMaterials_Params params{};

			ProcessEvent(mesh, defines::fnGetNumMaterial, &params);

			return params.ReturnValue;
		}

		static void K2_CopyMaterialInstanceParameters(uintptr_t Material, uintptr_t SourceMaterial)
		{
			if (!defines::fnK2_CopyMaterialInstanceParameters) {
				defines::fnK2_CopyMaterialInstanceParameters = pLoadObject(xor_w(L"MaterialInstanceDynamic:K2_CopyMaterialInstanceParameters"));
			}

			struct {
				uintptr_t SourceMaterial;
			}params{};
			params.SourceMaterial = SourceMaterial;

			ProcessEvent(Material, defines::fnK2_CopyMaterialInstanceParameters, &params);
		}

		static bool LineOfSightTo(uint64_t Playercontroller, uint64_t Other, FVector ViewPoint, bool bAlternateChecks)
		{
			if (!defines::fnLineOfSightTo) {
				defines::fnLineOfSightTo = pLoadObject(xor_w(L"Controller:LineOfSightTo"));
			}

			AController_LineOfSightTo_Params params{};
			params.Other = Other;
			params.ViewPoint = ViewPoint;
			params.bAlternateChecks = bAlternateChecks;

			ProcessEvent(Playercontroller, defines::fnLineOfSightTo, &params);

			return params.ReturnValue;
		}

		static void MoveInterpolationTarget(uintptr_t Movement, FVector Loc, FRotator Rot)
		{
			if (!Movement) return;

			static uint64_t addr = 0;
			if (!addr) addr = pLoadObject(xor_w(L"ProjectileMovementComponent:MoveInterpolationTarget"));

			struct
			{
				FVector Loc;
				FRotator Rot;
			} params;

			params.Loc = Loc;
			params.Rot = Rot;

			ProcessEvent(Movement, addr, &params);
		}

		static void StopMovementImmediately(uintptr_t Movement)
		{
			if (!Movement) return;

			static uint64_t addr = 0;
			if (!addr) addr = pLoadObject(xor_w(L"MovementComponent:StopMovementImmediately"));

			struct
			{
			} params;

			ProcessEvent(Movement, addr, &params);
		}

		static class FText STATIC_Conv_NameToText(FName& InName)
		{
			static uint64_t addr = 0;
			if (!addr) addr = pLoadObject(xor_w(L"KismetTextLibrary:Conv_NameToText"));

			UKismetTextLibrary_Conv_NameToText_Params params{};
			params.InName = InName;

			ProcessEvent(defines::KismettextLibrary, addr, &params);

			return params.ReturnValue;
		}

		static bool K2_SetActorLocation(uint64_t actor, FVector& NewLocation, bool bSweep, bool bTeleport, uint8_t* SweepHitResult = 0)
		{
			static uint64_t addr = 0;
			if (!addr) addr = pLoadObject(xor_w(L"Actor:K2_SetActorLocation"));

			AActor_K2_SetActorLocation_Params params;
			params.NewLocation = NewLocation;
			params.bSweep = bSweep;
			params.bTeleport = bTeleport;

			ProcessEvent(actor, addr, &params);

			if (SweepHitResult != NULL)
				*SweepHitResult = params.SweepHitResult;

			return params.ReturnValue;
		}

		static uintptr_t CreateDynamicMaterialInstance(uintptr_t kismet, uintptr_t World, uintptr_t Parent)
		{
			if (!defines::fnCreateDynamicMaterialInstance) {
				defines::fnCreateDynamicMaterialInstance = pLoadObject(xor_w(L"KismetMaterialLibrary:CreateDynamicMaterialInstance"));
			}

			struct {
				uintptr_t World;
				uintptr_t Parent;
				uintptr_t Return;
			}params{};
			params.World = World;
			params.Parent = Parent;

			ProcessEvent(kismet, defines::fnCreateDynamicMaterialInstance, &params);

			return params.Return;
		}

		static class FString STATIC_Conv_TextToString(FText InText)
		{
			static uint64_t addr = 0;
			if (!addr) addr = pLoadObject(xor_w(L"KismetTextLibrary:Conv_TextToString"));

			UKismetTextLibrary_Conv_TextToString_Params params{};
			params.InText = InText;

			ProcessEvent(defines::KismettextLibrary, addr, &params);

			return params.ReturnValue;
		}

		static struct FString STATIC_BuildString_Int(const struct FString& AppendTo, const struct FString& Prefix, int32_t inInt, const struct FString& Suffix)
		{
			if (!defines::fnBuildString_Int) {
				defines::fnBuildString_Int = pLoadObject(xor_w(L"KismetStringLibrary:BuildString_Int"));
			}

			UKismetStringLibrary_BuildString_Int_Params params;
			params.AppendTo = AppendTo;
			params.Prefix = Prefix;
			params.inInt = inInt;
			params.Suffix = Suffix;

			ProcessEvent(defines::KismetStringLibrary, defines::fnBuildString_Int, &params);

			return params.ReturnValue;
		}

		static struct FString STATIC_Concat_StrStr(const struct FString& A, const struct FString& B)
		{
			if (!defines::fnConcat_StrStr) {
				defines::fnConcat_StrStr = pLoadObject(xor_w(L"KismetStringLibrary:Concat_StrStr"));
			}

			UKismetStringLibrary_Concat_StrStr_Params params;
			params.A = A;
			params.B = B;

			ProcessEvent(defines::KismetStringLibrary, defines::fnConcat_StrStr, &params);

			return params.ReturnValue;
		}

		static struct FString STATIC_GetDisplayName(uintptr_t Object)
		{
			if (!defines::fnGetDisplayName) {
				defines::fnGetDisplayName = pLoadObject(xor_w(L"KismetSystemLibrary:GetDisplayName"));
			}
			
			UKismetSystemLibrary_GetDisplayName_Params params;
			params.Object = Object;

			ProcessEvent(defines::KismetSystemLibrary, defines::fnGetDisplayName, &params);

			return params.ReturnValue;
		}

		static uintptr_t GetBaseMaterial(uintptr_t mesh)
		{
			static uintptr_t object = 0;
			if (!object) object = pLoadObject(xor_w(L"MaterialInterface:GetBaseMaterial"));

			struct {
				uintptr_t return_value;
			} params;

			ProcessEvent(mesh, object, &params);

			return params.return_value;
		}

		static double STATIC_Vector_Distance(const struct FVector& v1, const struct FVector& v2)
		{
			if (!defines::fnVectorDistance) {
				defines::fnVectorDistance = pLoadObject(xor_w(L"KismetMathLibrary:Vector_Distance"));
			}

			UKismetMathLibrary_Vector_Distance_Params params;
			params.v1 = v1;
			params.v2 = v2;

			ProcessEvent(defines::KismetMathLibrary, defines::fnVectorDistance, &params);

			return params.ReturnValue;
		}

		static FVector2D K2_TextSize(uintptr_t font, const struct FString& RenderText, const struct FVector2D& Scale)
		{
			if (!defines::fnK2_TextSize) {
				defines::fnK2_TextSize = pLoadObject(xor_w(L"Canvas:K2_TextSize"));
			}

			UCanvas_K2_TextSize_Params params;
			params.RenderFont = font;
			params.RenderText = RenderText;
			params.Scale = Scale;

			ProcessEvent(defines::UCanvas, defines::fnK2_TextSize, &params);

			return params.ReturnValue;
		}

		static void LookAt(uint64_t PlayerController, FRotator angles)
		{
			uintptr_t vTable = *(uintptr_t*)PlayerController;

			if (!vTable) return;

			uintptr_t func = *(uintptr_t*)(vTable + 0x720);

			if (!func) return;

			auto LookAtRot = reinterpret_cast<void(__fastcall*)(uintptr_t PlayerController, FRotator NewAngle)>(func);

			SpoofCall(LookAtRot, (uintptr_t)PlayerController, angles);
		}

		static void SetVectorParameterValue(uint64_t MaterialInstanceDynamic, const struct FName& ParameterName, const struct FLinearColor& Value)
		{
			if (!defines::fnSetVectorParameterValue) {
				defines::fnSetVectorParameterValue = pLoadObject(xor_w(L"MaterialInstanceDynamic:SetVectorParameterValue"));
			}

			UMaterialInstanceDynamic_SetVectorParameterValue_Params params;
			params.ParameterName = ParameterName;
			params.Value = Value;

			ProcessEvent(MaterialInstanceDynamic, defines::fnSetVectorParameterValue, &params);
		}

		static bool IsDead(uint64_t actor)
		{
			static uint64_t fn = 0;
			if (!fn)
				fn = pLoadObject(xor_w(L"DCCharacterBase:IsDead"));

			ADCCharacterBase_IsDead_Params params{};

			ProcessEvent(actor, fn, &params);

			return params.ReturnValue;
		}

		static void DrawFilledRect(FVector2D initial_pos, float width, float height, FLinearColor color)
		{
			for (float i = 0.0f; i < height; i += 1.0f)
				K2_DrawLine(FVector2D(initial_pos.X, initial_pos.Y + i), FVector2D(initial_pos.X + width, initial_pos.Y + i), 1.0f, color);
		}

		static __int32 UKismetMathLibrarySTATIC_Round(double A)
		{

			if (!defines::KIMATH_ROUND) {
				defines::KIMATH_ROUND = pLoadObject(xor_w(L"KismetMathLibrary:Round"));
			}

			UKismetMathLibrary_Round_Params params;
			params.A = A;

			ProcessEvent(defines::KismetMathLibrary, defines::KIMATH_ROUND, &params);

			return params.ReturnValue;
		}

		static double UKismetMathLibrarySTATIC_Sin(double A)
		{
			if (!defines::KIMATH_Sin) {
				defines::KIMATH_Sin = pLoadObject(xor_w(L"KismetMathLibrary:Sin"));
			}

			UKismetMathLibrary_Sin_Params params;
			params.A = A;

			ProcessEvent(defines::KismetMathLibrary, defines::KIMATH_Sin, &params);

			return params.ReturnValue;
		}

		static FVector GetVelocity(uintptr_t actor)
		{
			if (!defines::fnGetVelocity) {
				defines::fnGetVelocity = pLoadObject(xor_w(L"Actor:GetVelocity"));
			}

			AActor_GetVelocity_Params params{};

			ProcessEvent(actor, defines::fnGetVelocity, &params);

			return params.ReturnValue;
		}
		
		static void StartFire(unsigned char FireModeNum)
		{
			static uint64_t fn = 0;
			if (!fn)
				fn = pLoadObject(xor_w(L"PlayerController:StartFire"));

			APlayerController_StartFire_Params params{};
			params.FireModeNum = FireModeNum;

			ProcessEvent(defines::PlayerController, fn, &params);
		}

		static class FString STATIC_Replace(const class FString& SourceString, const class FString& From, const class FString& To, ESearchCase SearchCase)
		{
			static uint64_t fn = 0;
			if (!fn)
				fn = pLoadObject(xor_w(L"KismetStringLibrary:Replace"));

			UKismetStringLibrary_Replace_Params params{};
			params.SourceString = SourceString;
			params.From = From;
			params.To = To;
			params.SearchCase = SearchCase;

			ProcessEvent(defines::KismetStringLibrary, fn, &params);

			return params.ReturnValue;
		}

		static int32_t STATIC_Len(const class FString& S)
		{
			static uint64_t fn = 0;
			if (!fn)
				fn = pLoadObject(xor_w(L"KismetStringLibrary.Len"));

			UKismetStringLibrary_Len_Params params{};
			params.S = S;

			ProcessEvent(defines::KismetStringLibrary, fn, &params);

			return params.ReturnValue;
		}

		static bool STATIC_EndsWith(const class FString& SourceString, const class FString& InSuffix, ESearchCase SearchCase)
		{
			static uint64_t fn = 0;
			if (!fn)
				fn = pLoadObject(xor_w(L"KismetStringLibrary.EndsWith"));

			UKismetStringLibrary_EndsWith_Params params{};
			params.SourceString = SourceString;
			params.InSuffix = InSuffix;
			params.SearchCase = SearchCase;

			ProcessEvent(defines::KismetStringLibrary, fn, &params);

			return params.ReturnValue;
		}

		static void InitializeAimbot(uintptr_t actor, FVector AimLocation) {
			SDK::FRotator AimRotation, CurrentRotation;

			//Original Rotation
			static SDK::FRotator OrigRotation = GetCameraRotation();
			static SDK::FVector OrigLocation = GetCameraLocation();

			// Get aim rotation
			AimRotation = STATIC_FindLookAtRotation(GetCameraLocation(), AimLocation);

			OrigRotation = STATIC_FindLookAtRotation(AimLocation, OrigLocation);

			// Set aim
			LookAt(defines::PlayerController, AimRotation);
		}

	}

	static void DrawCircle(int x, int y, int radius, int sides, FLinearColor color)
	{
		float Step = mpe * 2.0 / sides;
		for (float a = 0; a < mpe * 2.0; a += Step)
		{
			float X1 = radius * SDK::Functions::UKismetMathLibrarySTATIC_Cos(a) + x;
			float Y1 = radius * SDK::Functions::UKismetMathLibrarySTATIC_Sin(a) + y;
			float X2 = radius * SDK::Functions::UKismetMathLibrarySTATIC_Cos(a + Step) + x;
			float Y2 = radius * SDK::Functions::UKismetMathLibrarySTATIC_Sin(a + Step) + y;
			SDK::Functions::K2_DrawLine({ X1, Y1 }, { X2, Y2 }, 1.5f, color);
		}
	}

	static int retoffsets[100] = { 0 };
	static int loaded_offsets = 0;
	static int current_ret_index = 0;

	static void* __fastcall GetPlayerViewPoint_hook(void* a1, FVector* a2, FRotator* a3)
	{
		void* ret = NULL;
		uint64_t return_address = (uint64_t)_ReturnAddress();

		void* (__fastcall * fn)(void*, FVector*, FRotator*) = decltype(fn)(defines::GetPlayerViewPoint_original);

		ret = fn(a1, a2, a3);

		bool already_loaded = false;
		int i = 0;

		int retoffset = return_address - (uint64_t)defines::BaseAddr;

		for (i = 0; i < 100; i++)
		{
			if (retoffsets[i] == retoffset)
			{
				already_loaded = true;
				break;
			}
		}

		if (!already_loaded)
		{
			retoffsets[loaded_offsets++] = retoffset;
		}

		static int current_ret = 0;

		if (SDK::Functions::WasInputKeyJustPressed(defines::PlayerController, defines::F1) && loaded_offsets > 0)
		{
			current_ret = retoffsets[current_ret_index];

			SDK::Functions::dbgprint("[DEBUG] current ret: %X", current_ret);

			if (current_ret_index == loaded_offsets)
			{
				current_ret_index = 0;
			}
			else
				current_ret_index++;

		}

		SDK::FRotator* pitch;

		if (current_ret != 0 && return_address == ((uint64_t)defines::BaseAddr + current_ret))
		{
			a3 = pitch;
		}

		return ret;
	}


	//static uint64_t __fastcall GetPlayerViewPoint_hook(uint64_t a1, FVector* a2, FRotator* a3)
	//{
	//	uint64_t return_address = (uint64_t)_ReturnAddress();

	//	uint64_t(__fastcall * GetPlayerViewPoint_func)(uint64_t, FVector*, FRotator*) = decltype(GetPlayerViewPoint_func)(GetPlayerViewPoint_original);

	//	auto ret = GetPlayerViewPoint_func(a1, a2, a3);
	//	/*
	//	if (return_address == ((uint64_t)imageBase() + 0x6C3A05B)) //last one
	//	{
	//		dbgprint("[DEBUG] rotation: %f %f %f", a3->Pitch, a3->Yaw, a3->Roll);
	//		a3->Pitch = 89.0f;
	//	}*/

	//	if (halt && return_address == ((uint64_t)imageBase() + 0x6C3A05B)) {
	//		auto mepr = halt->Mesh;
	//		auto sousouman = conservative(mepr, 68);
	//		FVector delta = *a2 - sousouman;
	//		FRotator angles;
	//		float Speed = jockey->STATIC_Sqrt(delta.X * delta.X + delta.Y * delta.Y);
	//		angles.Pitch = (jockey->STATIC_Atan(delta.Z / Speed) * 57.295779513082f) * -1.f;
	//		angles.Yaw = jockey->STATIC_Atan(delta.Y / delta.X) * 57.295779513082f;
	//		angles.Roll = 0.0f;
	//		if (delta.X >= 0.f) angles.Yaw += 180.f;
	//		if (angles.Pitch != 0.0f && angles.Yaw != 0.0f) {
	//			a3->Pitch = angles.Pitch;
	//			a3->Yaw = angles.Yaw;
	//			a3->Roll = angles.Roll;
	//		}
	//	}

	//	return ret;
	//}
}
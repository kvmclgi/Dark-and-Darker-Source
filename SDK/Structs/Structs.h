#pragma once


namespace SDK {

	struct UGameplayStatics_GetPlayerController_Params
	{
		uintptr_t WorldContextObject; 
		int PlayerIndex;
		uint64_t ReturnValue;
	};

	struct UGameplayStatics_GetPlayerCameraManager_Params
	{
		uintptr_t WorldContextObject;
		int PlayerIndex;
		uint64_t ReturnValue;
	};

	struct AController_K2_GetPawn_Params
	{
		uint64_t ReturnValue;
	};

	struct APlayerController_GetViewportSize_Params
	{
		int SizeX;
		int SizeY;
	};

	struct UKismetStringLibrary_Conv_StringToName_Params
	{
		struct FString InString;
		struct FName ReturnValue;
	};

	struct UGameplayStatics_GetAllActorsOfClass_Params
	{
		uint64_t WorldContextObject;
		uint64_t ActorClass;
		TArray<uint64_t*> OutActors;
	};

	struct USceneComponent_GetSocketLocation_Params
	{
		struct FName InSocketName;
		struct FVector ReturnValue;
	};

	struct UCanvas_K2_DrawBox_Params
	{
		struct FVector2D ScreenPosition;
		struct FVector2D ScreenSize; 
		float Thickness;   
		struct FLinearColor RenderColor;
	};

	struct UCanvas_K2_Project_Params
	{
		struct FVector WorldLocation; 
		struct FVector ReturnValue;   
	};

	struct UCanvas_K2_DrawLine_Params
	{
		struct FVector2D ScreenPositionA;  
		struct FVector2D ScreenPositionB;  
		float Thickness;                   
		struct FLinearColor RenderColor;   
	};

	struct UKismetMathLibrary_Abs_Params
	{
		double A;          
		double ReturnValue;
	};

	struct APlayerController_GetMousePosition_Params
	{
		float LocationX; 
		float LocationY; 
		bool ReturnValue;
	};

	struct UCanvas_K2_DrawText_Params
	{
		uint64_t RenderFont;                                             
		struct FString RenderText;   
		struct FVector2D ScreenPosition;
		struct FVector2D Scale;        
		struct FLinearColor RenderColor;  
		double Kerning;      
		struct FLinearColor ShadowColor;  
		struct FVector2D ShadowOffset; 
		bool bCentreX;     
		bool bCentreY;     
		bool bOutlined;    
		struct FLinearColor OutlineColor; 
	};

	struct APlayerController_WasInputKeyJustPressed_Params
	{
		struct FKey Key;  
		bool ReturnValue;
	};

	struct APlayerController_WasInputKeyJustReleased_Params
	{
		struct FKey Key; 
		bool ReturnValue;
	};

	struct APlayerController_IsInputKeyDown_Params
	{
		struct FKey Key;   
		bool ReturnValue;  
	};

	struct AController_SetControlRotation_Params
	{
		struct FRotator NewRotation;
	};

	struct UKismetMathLibrary_FindLookAtRotation_Params
	{
		struct FVector Start;      
		struct FVector Target;     
		struct FRotator ReturnValue;
	};

	struct APlayerCameraManager_GetCameraLocation_Params
	{
		struct FVector ReturnValue;
	};

	struct APlayerCameraManager_GetCameraRotation_Params
	{
		struct FRotator ReturnValue;
	};

	struct UKismetMathLibrary_Sqrt_Params
	{
		double A;           
		double ReturnValue; 
	};

	struct UStaticMesh_GetMaterial_Params
	{
	public:
		int32_t MaterialIndex;        
		unsigned char UnknownData_DF54[0x4];
		uintptr_t ReturnValue;
	};

	struct UKismetMathLibrary_Vector_Distance_Params
	{
		struct FVector v1; 
		struct FVector v2; 
		double ReturnValue;
	};

	struct UPrimitiveComponent_SetMaterial_Params
	{
	public:
		int32_t ElementIndex;         
		unsigned char UnknownData_242Q[0x4];
		uintptr_t Material;
	};

	struct UPrimitiveComponent_GetNumMaterials_Params
	{
	public:
		int32_t ReturnValue;
	};

	struct UKismetStringLibrary_Concat_StrStr_Params
	{
		struct FString A;            
		struct FString B;            
		struct FString ReturnValue;  
	};

	struct UKismetSystemLibrary_GetDisplayName_Params
	{
		uintptr_t Object;                                                   // (ConstParm, Parm, ZeroConstructor)
		struct FString                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
	};

	struct UKismetStringLibrary_BuildString_Int_Params
	{
		struct FString                                     AppendTo;                                                 // (Parm, ZeroConstructor)
		struct FString                                     Prefix;                                                   // (Parm, ZeroConstructor)
		int32_t                                                inInt;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
		struct FString                                     Suffix;                                                   // (Parm, ZeroConstructor)
		struct FString                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
	};

	struct APlayerState_GetPlayerName_Params
	{
	public:
		class FString ReturnValue;
	};

	struct UCanvas_K2_TextSize_Params
	{
		uint64_t RenderFont;                                               // (Parm, ZeroConstructor, IsPlainOldData)
		struct FString                                     RenderText;                                               // (Parm, ZeroConstructor)
		struct FVector2D                                   Scale;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
		struct FVector2D                                   ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct UFortKismetLibrary_GetHumanReadableName_Params
	{
		uint64_t Actor;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
		struct FString                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
	};

	

	struct AActor_GetVelocity_Params
	{
	public:
		struct FVector                                             ReturnValue;                                             // 0x0000(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	enum class EMaterialParameterAssociation : uint8_t
	{
		LayerParameter = 0,
		BlendParameter = 1,
		GlobalParameter = 2,
		MAX = 3
	};

	struct AController_LineOfSightTo_Params
	{
	public:
		uint64_t Other;                                                   // 0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                             ViewPoint;                                               // 0x0008(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                       bAlternateChecks;                                        // 0x0020(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                       ReturnValue;                                             // 0x0021(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct AActor_K2_SetActorLocation_Params
	{
		struct FVector                                     NewLocation;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		bool                                               bSweep;                                                   // (Parm, ZeroConstructor, IsPlainOldData)
		uint8_t                                  SweepHitResult;                                           // (Parm, OutParm, IsPlainOldData)
		bool                                               bTeleport;                                                // (Parm, ZeroConstructor, IsPlainOldData)
		bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	enum class EDCPortalScrollType : uint8_t
	{
		None = 0,
		Escape = 1,
		Down = 2,
		MAX = 3
	};

	struct AActor_K2_GetActorLocation_Params
	{
		struct FVector                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct FMaterialParameterInfo
	{
	public:
		class FName                                                Name;                                                    // 0x0000(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		EMaterialParameterAssociation                              Association;                                             // 0x0008(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              UnknownData_Y46H[0x3];                                   // 0x0009(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		int32_t                                                    Index;                                                   // 0x000C(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	};

	struct FVectorParameterValue
	{
	public:
		struct FMaterialParameterInfo                              ParameterInfo;                                           // 0x0000(0x0010) Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		struct FLinearColor                                        ParameterValue;                                          // 0x0010(0x0010) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		//struct FGuid                                               ExpressionGUID;                                          // 0x0020(0x0010) ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	};

	struct UMaterialInstanceDynamic_SetVectorParameterValue_Params
	{
		struct FName                                       ParameterName;                                            // (Parm, ZeroConstructor, IsPlainOldData)
		struct FLinearColor                                Value;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
	};

	struct UDCPostProcessingComponent_AddMeshHalfTranslucent_Params
	{
	public:
		uint64_t InMesh;                                                  // 0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint64_t InMaterial;                                              // 0x0008(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                       IsRenderingPriority;                                     // 0x0010(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		unsigned char                                              UnknownData_P4C3[0x3];                                   // 0x0011(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		int32_t                                                    ReturnValue;                                             // 0x0014(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct APlayerController_ProjectWorldLocationToScreen_Params
	{
		struct FVector WorldLocation;
		struct FVector2D ScreenLocation;
		bool bPlayerViewportRelative;
		bool ReturnValue;
	};

	struct ADCCharacterBase_IsDead_Params
	{
	public:
		bool                                                       ReturnValue;                                             // 0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct FGenericTeamId
	{
	public:
		unsigned char                                              TeamID;                                                  // 0x0000(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
	};

	struct UKismetMathLibrary_Sin_Params
	{
		double                                              A;                                                        // (Parm, ZeroConstructor, IsPlainOldData)
		double                                              ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct UKismetMathLibrary_Round_Params
	{
		double                                       A;                                                 // 0x0(0x8)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		__int32                                        ReturnValue;                                       // 0x8(0x4)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		__int8                                        Pad_999[0x4];                                      // Fixing Size Of Struct..
	};

	struct UKismetMathLibrary_Cos_Params
	{
		double                                              A;                                                        // (Parm, ZeroConstructor, IsPlainOldData)
		double                                              ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
	};

	struct APlayerController_FOV_Params
	{
		float                                              NewFOV;                                                   // (Parm, ZeroConstructor, IsPlainOldData)
	};

	struct UKismetTextLibrary_Conv_NameToText_Params
	{
	public:
		class FName                                                InName;                                                  // 0x0000(0x0008)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FText                                                ReturnValue;                                             // 0x0008(0x0018)  (Parm, OutParm, ReturnParm, NativeAccessSpecifierPublic)
	};

	struct APlayerController_StartFire_Params
	{
	public:
		unsigned char                                              FireModeNum;                                             // 0x0000(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	enum class ESearchCase : uint8_t
	{
		CaseSensitive = 0,
		IgnoreCase = 1,
		MAX = 2
	};

	struct UKismetTextLibrary_Conv_TextToString_Params
	{
	public:
		class FText                                                InText;                                                  // 0x0000(0x0018)  (ConstParm, Parm, OutParm, ReferenceParm, NativeAccessSpecifierPublic)
		class FString                                              ReturnValue;                                             // 0x0018(0x0010)  (Parm, OutParm, ZeroConstructor, ReturnParm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct UKismetStringLibrary_Replace_Params
	{
	public:
		class FString                                              SourceString;                                            // 0x0000(0x0010)  (Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FString                                              From;                                                    // 0x0010(0x0010)  (Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FString                                              To;                                                      // 0x0020(0x0010)  (Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		ESearchCase                                                SearchCase;                                              // 0x0030(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		unsigned char                                              UnknownData_3A9N[0x7];                                   // 0x0031(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		class FString                                              ReturnValue;                                             // 0x0038(0x0010)  (Parm, OutParm, ZeroConstructor, ReturnParm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct UKismetStringLibrary_EndsWith_Params
	{
	public:
		class FString                                              SourceString;                                            // 0x0000(0x0010)  (Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FString                                              InSuffix;                                                // 0x0010(0x0010)  (Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		ESearchCase                                                SearchCase;                                              // 0x0020(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                       ReturnValue;                                             // 0x0021(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct UKismetStringLibrary_Len_Params
	{
	public:
		class FString                                              S;                                                       // 0x0000(0x0010)  (Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32_t                                                    ReturnValue;                                             // 0x0010(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	enum class EDCCharacterClass : uint8_t
	{
		None = 0,
		Wizard = 1,
		Rogue = 2,
		Ranger = 3,
		Fighter = 4,
		Cleric = 5,
		Barbarian = 6,
		Bard = 7,
		Warlock = 8,
		Druid = 9,
		MAX = 10
	};

}
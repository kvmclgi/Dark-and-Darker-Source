#pragma once

#define rva(addr, size) (reinterpret_cast<unsigned char*>(addr + *reinterpret_cast<int*>(addr + (size - 4)) + size))
#define mpe 3.14159265358979323846264338327950288419716939937510582f

namespace SDK {

	template<class T>
	struct TArray {
		friend struct FString;

	public:
		inline TArray() {
			Data = nullptr;
			Count = Max = 0;
		};

		inline int Num() const {
			return Count;
		};

		inline T& operator[](int i) {
			return Data[i];
		};

		inline const T& operator[](int i) const {
			return Data[i];
		};

		inline bool IsValidIndex(int i) const {
			return i < Num();
		}

	private:
		T* Data;
		int32_t Count;
		int32_t Max;
	};

	struct FString : private TArray<wchar_t>
	{
		inline FString()
		{
		};

		FString(const wchar_t* other)
		{
			Max = Count = *other ? My_wcslen(other) + 1 : 0;

			if (Count)
			{
				Data = const_cast<wchar_t*>(other);
			}
		};

		inline FString operator=(const wchar_t*&& Other)
		{
			return FString(Other);
		}

		inline bool IsValid() const
		{
			return Data != nullptr;
		}

		inline const wchar_t* c_str() const
		{
			return Data;
		}

		bool IsEmpty() const
		{
			// Here we check if the string only contains the null terminator
			return Count <= 1;
		}
	};

	static FString GlobalString;

	template<class TEnum>
	class TEnumAsByte
	{
	public:
		inline TEnumAsByte()
		{
		}

		inline TEnumAsByte(TEnum _value)
			: value(static_cast<uint8_t>(_value))
		{
		}

		explicit inline TEnumAsByte(int32_t _value)
			: value(static_cast<uint8_t>(_value))
		{
		}

		explicit inline TEnumAsByte(uint8_t _value)
			: value(_value)
		{
		}

		inline operator TEnum() const
		{
			return (TEnum)value;
		}

		inline TEnum GetValue() const
		{
			return (TEnum)value;
		}

	private:
		uint8_t value;
	};

	class FName {
	public:
		FName() : comparison_index(std::uint32_t()), number(std::int32_t()) {};
		FName(std::uint32_t index) : comparison_index(index), number(std::int32_t()) {};

	public:
		std::uint32_t comparison_index;
		std::int32_t number;
	};

	class FTextData {
	public:
		char pad_0x0000[0x38];  //0x0000
		wchar_t* Name;          //0x0038 
		__int32 Length;         //0x0040 
	};

	struct FText {
		FTextData* Data;
		char UnknownData[0x10];

		wchar_t* GetText() const {
			if (Data)
				return Data->Name;

			return nullptr;
		}
	};

	struct FVector
	{
		double                                             X;                                                        // 0x0000(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		double                                             Y;                                                        // 0x0008(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		double                                             Z;                                                        // 0x0010(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

		inline FVector()
			: X(0), Y(0), Z(0)
		{ }

		inline FVector(double x, double y, double z)
			: X(x),
			Y(y),
			Z(z)
		{ }

		__forceinline FVector operator-(const FVector& V) {
			return FVector(X - V.X, Y - V.Y, Z - V.Z);
		}

		__forceinline FVector operator+(const FVector& V) {
			return FVector(X + V.X, Y + V.Y, Z + V.Z);
		}

		__forceinline FVector operator*(float Scale) const {
			return FVector(X * Scale, Y * Scale, Z * Scale);
		}

		__forceinline FVector operator/(float Scale) const {
			const float RScale = 1.f / Scale;
			return FVector(X * RScale, Y * RScale, Z * RScale);
		}

		__forceinline FVector operator+(float A) const {
			return FVector(X + A, Y + A, Z + A);
		}

		__forceinline FVector operator-(float A) const {
			return FVector(X - A, Y - A, Z - A);
		}

		__forceinline FVector operator*(const FVector& V) const {
			return FVector(X * V.X, Y * V.Y, Z * V.Z);
		}

		__forceinline FVector operator/(const FVector& V) const {
			return FVector(X / V.X, Y / V.Y, Z / V.Z);
		}

		__forceinline float operator|(const FVector& V) const {
			return X * V.X + Y * V.Y + Z * V.Z;
		}

		__forceinline float operator^(const FVector& V) const {
			return X * V.Y - Y * V.X - Z * V.Z;
		}

		__forceinline FVector& operator+=(const FVector& v) {
			X += v.X;
			Y += v.Y;
			Z += v.Z;
			return *this;
		}

		__forceinline FVector& operator-=(const FVector& v) {
			X -= v.X;
			Y -= v.Y;
			Z -= v.Z;
			return *this;
		}

		__forceinline FVector& operator*=(const FVector& v) {
			X *= v.X;
			Y *= v.Y;
			Z *= v.Z;
			return *this;
		}

		__forceinline FVector& operator/=(const FVector& v) {
			X /= v.X;
			Y /= v.Y;
			Z /= v.Z;
			return *this;
		}

		__forceinline bool operator==(const FVector& src) const {
			return (src.X == X) && (src.Y == Y) && (src.Z == Z);
		}

		__forceinline bool operator!=(const FVector& src) const {
			return (src.X != X) || (src.Y != Y) || (src.Z != Z);
		}

		__forceinline float SizeSquared() const {
			return X * X + Y * Y + Z * Z;
		}

		float Size() const {
			return LI_FN(sqrt).safe_cached()(X * X + Y * Y + Z * Z);
		}

		__forceinline float SizeSquared2D() const {
			return X * X + Y * Y;
		}

		__forceinline float Dot(const FVector& vOther) const {
			const FVector& a = *this;

			return (a.X * vOther.X + a.Y * vOther.Y + a.Z * vOther.Z);
		}

		float Distance(FVector v)
		{
			return ((X - v.X) * (X - v.X) +
				(Y - v.Y) * (Y - v.Y) +
				(Z - v.Z) * (Z - v.Z));
		}

	};

	struct FVector2D
	{
		double                                             X;                                                        // 0x0000(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		double                                             Y;                                                        // 0x0008(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

		inline FVector2D()
			: X(0), Y(0)
		{ }

		inline FVector2D(double x, double y)
			: X(x),
			Y(y)
		{ }

		__forceinline FVector2D operator-(const FVector2D& V) {
			return FVector2D(X - V.X, Y - V.Y);
		}

		__forceinline FVector2D operator+(const FVector2D& V) {
			return FVector2D(X + V.X, Y + V.Y);
		}

		__forceinline FVector2D operator*(double Scale) const {
			return FVector2D(X * Scale, Y * Scale);
		}

		__forceinline FVector2D operator/(double Scale) const {
			const float RScale = 1.f / Scale;
			return FVector2D(X * RScale, Y * RScale);
		}

		__forceinline FVector2D operator+(double A) const {
			return FVector2D(X + A, Y + A);
		}

		__forceinline FVector2D operator-(double A) const {
			return FVector2D(X - A, Y - A);
		}

		__forceinline FVector2D operator*(const FVector2D& V) const {
			return FVector2D(X * V.X, Y * V.Y);
		}

		__forceinline FVector2D operator/(const FVector2D& V) const {
			return FVector2D(X / V.X, Y / V.Y);
		}

		__forceinline float operator|(const FVector2D& V) const {
			return X * V.X + Y * V.Y;
		}

		__forceinline float operator^(const FVector2D& V) const {
			return X * V.Y - Y * V.X;
		}

		__forceinline FVector2D& operator+=(const FVector2D& v) {
			X += v.X;
			Y += v.Y;
			return *this;
		}

		__forceinline FVector2D& operator-=(const FVector2D& v) {
			X -= v.X;
			Y -= v.Y;
			return *this;
		}

		__forceinline FVector2D& operator*=(const FVector2D& v) {
			X *= v.X;
			Y *= v.Y;
			return *this;
		}

		__forceinline FVector2D& operator/=(const FVector2D& v) {
			X /= v.X;
			Y /= v.Y;
			return *this;
		}

		friend FVector2D operator*(float scalar, const FVector2D& V) {
			return FVector2D(scalar * V.X, scalar * V.Y);
		}

		__forceinline bool operator==(const FVector2D& src) const {
			return (src.X == X) && (src.Y == Y);
		}

		__forceinline bool operator!=(const FVector2D& src) const {
			return (src.X != X) || (src.Y != Y);
		}

		__forceinline float SizeSquared() const {
			return X * X + Y * Y;
		}

		__forceinline float Dot(const FVector2D& vOther) const {
			const FVector2D& a = *this;

			return (a.X * vOther.X + a.Y * vOther.Y);
		}

		inline FVector2D GetSafeNormal(double Tolerance = 1.e-8) const
		{
			const double SquareSum = X * X + Y * Y;
			if (SquareSum > Tolerance)
			{
				const double Scale = 1.0 / sqrt(SquareSum);
				return FVector2D(X * Scale, Y * Scale);
			}
			return FVector2D(0, 0);
		}

	};

	static FVector2D Size;

	struct FRotator
	{
		double                                             Pitch;                                                    // 0x0000(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		double                                             Yaw;                                                      // 0x0008(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		double                                             Roll;                                                     // 0x0010(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	};

	struct FKey {
		FName KeyName;
		unsigned char UnknownData00[0x10];

		// Default constructor
		FKey() {
			// Initialize KeyName and UnknownData00 as needed
			for (int i = 0; i < 0x10; ++i) {
				UnknownData00[i] = 0;
			}
		}

		// Constructor that takes an FName object
		FKey(const FName& name) : KeyName(name) {
			// Initialize UnknownData00 with zeros
			for (int i = 0; i < 0x10; ++i) {
				UnknownData00[i] = 0;
			}
		}
	};

	struct FLinearColor
	{
		float                                              R;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              G;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              B;                                                        // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              A;                                                        // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

		inline FLinearColor()
			: R(0), G(0), B(0), A(0)
		{ }

		inline FLinearColor(float r, float g, float b, float a)
			: R(r / 255.0f),
			G(g / 255.0f),
			B(b / 255.0f),
			A(a)
		{ }

	};
}

namespace defines {
	static size_t textSize;
	static uintptr_t textBase;
	static uintptr_t BaseAddr;

	static SDK::FVector AimLocation;
	static SDK::FVector PotAimLocation;
	static uintptr_t ViewPointPointer;
	static void* (*StaticFindFirstObject_t)(void* Class, WCHAR* Name, UINT64 Options, UINT64 AmbiguousMessageVerbosity, const TCHAR* InCurrentOperation);
	static void* (*StaticLoadObject_t)(void* ObjectClass, void* InOuter, const TCHAR* InName, const TCHAR* Filename, UINT64 LoadFlags, void* Sandbox, bool bAllowObjectReconciliation, void* InstancingContext);
	static uint64_t(*GetPlayerViewPoint_original)(uint64_t, SDK::FVector*, SDK::FRotator*);

	static uint64_t KismetSystemLibrary = NULL;
	static uint64_t KismetStringLibrary = NULL;
	static uint64_t KismetMathLibrary = NULL;
	static uint64_t KismettextLibrary = NULL;
	static uint64_t GameplayStatics = NULL;
	static uint64_t KismetMaterialLibrary = NULL;
	static uint64_t PrimitiveComponent = NULL;

	static uint64_t chams = NULL;

	static uint64_t UObjects;
	static uint64_t UCanvas;
	static uintptr_t GWorld;
	static int Width = 0, Height = 0;
	static uint64_t MyPlayer = NULL;
	static uint64_t Pothalt = NULL;
	static uint64_t halt = NULL;
	static uint64_t PlayerMesh = NULL;

	static uintptr_t PlayerController;
	static uint64_t PlayerCameraManager;
	static uint64_t SceneComponent;
	static uint64_t LocalPlayer;

	static uintptr_t PlayerControllerSaved;

	static SDK::FLinearColor LootColour = SDK::FLinearColor(0, 0, 0, 0);

	// Functions
	uintptr_t static fnGetGameInstance = NULL;
	uintptr_t static Conv_StringToName = NULL;
	uintptr_t static fnGetViewportSize = NULL;
	uintptr_t static fnGetFortPlayerPawns = NULL;
	uintptr_t static fnGetCameraLocation = NULL;
	uintptr_t static fnGetCameraRotation = NULL;
	uintptr_t static fnSetVectorParameterValue = NULL;
	uintptr_t static fnWasInputKeyJustPressed = NULL;
	uintptr_t static fnWasInputKeyJustReleased = NULL;
	uintptr_t static fnIsInputKeyDown = NULL;
	uintptr_t static fnGetSocketLocation = NULL;
	uintptr_t static fnK2_Project = NULL;
	uintptr_t static fnK2_DrawLine = NULL;
	uintptr_t static fnK2_DrawBox = NULL;
	uintptr_t static fnK2_TextSize = NULL;
	uintptr_t static fnK2_DrawText = NULL;
	uintptr_t static fnGetVelocity = NULL;
	uintptr_t static KIMATH_ABS = NULL;
	uintptr_t static KIMATH_Sqrt = NULL; 
	uintptr_t static fnCreateDynamicMaterialInstance = NULL; 
	uintptr_t static fnK2_CopyMaterialInstanceParameters = NULL;
	uintptr_t static fnLineOfSightTo = NULL;
	uintptr_t static fnGetMaterial = NULL; 
	uintptr_t static fnSetMaterial = NULL;
	uintptr_t static fnGetNumMaterial = NULL;
	uintptr_t static KIMATH_Cos = NULL;
	uintptr_t static KIMATH_Sin = NULL;
	uintptr_t static KIMATH_Tan = NULL;
	uintptr_t static KIMATH_ROUND = NULL;
	uintptr_t static fnContains = NULL;
	uintptr_t static fnGetObjectName = NULL;
	uintptr_t static fnIsDead = NULL;
	uintptr_t static fnIsDBNO = NULL;
	uintptr_t static fnOnSameTeam = NULL;
	uintptr_t static fnGetPawn = NULL;
	uintptr_t static fnGetPlayerCameraManager = NULL;
	uintptr_t static fnGetPlayerController = NULL;
	uintptr_t static fnGetHumanReadableName = NULL;
	uintptr_t static fnSetActorRotation = NULL;
	uintptr_t static fnCheckLineOfSightToActorWithChannel = NULL;
	uintptr_t static fnVectorDistance = NULL;
	uintptr_t static fnToggleDebugCamera = NULL;
	uintptr_t static fnGhost = NULL;
	uintptr_t static fnBuildString_Double = NULL;
	uintptr_t static fnBuildString_Int = NULL;
	uintptr_t static fnConcat_StrStr = NULL;
	uintptr_t static fnGetDisplayName = NULL;
	uintptr_t static fnFindLookAtRotation = NULL;
	uintptr_t static fnAddPitchInput = NULL;
	uintptr_t static fnAddYawInput = NULL;
	uintptr_t static fnGetMousePosition = NULL;
	uintptr_t static fnGetAllActorsOfClass = NULL;
	uintptr_t static fnGetActorLocation = NULL;
	uintptr_t static fnAddMeshHalfTranslucent = NULL;
	uintptr_t static fnGetCurrentWeapon = NULL;
	uintptr_t static fnGetMuzzleLocation = NULL;
	uintptr_t static fnGetWeaponData = NULL;
	uintptr_t static fnGetPrimaryItemDefinition = NULL;
	uintptr_t static fnGetTier = NULL;
	uintptr_t static fnGetItemType = NULL;
	uintptr_t static fnGetWeaponDataItemType = NULL;
	uintptr_t static fnGetRemainingAmmo = NULL;
	uintptr_t static fnGetBulletsPerClip = NULL;
	uintptr_t static fnGetActorBounds = NULL;
	uintptr_t static fnVehicleGetDisplayName = NULL;
	uintptr_t static fnCanContainPlayers = NULL;
	uintptr_t static fnIsDrivingVehicle = NULL;
	uintptr_t static fnBP_GetVehicle = NULL;
	uintptr_t static fnOnGainedFuel = NULL;
	uintptr_t static fnSetForceInfiniteFuel = NULL;
	uintptr_t static fnIsAlreadySearched = NULL;
	uintptr_t static fnFOV = NULL;
	uintptr_t static fnSetControlRotation = NULL;
	uintptr_t static fnClientSetRotation = NULL;
	uintptr_t static fnSetGlobalTimeDilation = NULL;
	uintptr_t static fnProjectWorldToScreen = NULL;
	//

	// Fonts
	static uintptr_t RobotoFont = NULL;
	static uintptr_t fnFont = NULL;
	static uintptr_t fnFont2 = NULL;
	//
	//

	// FKeys
	inline SDK::FKey KeyBind;
	inline SDK::FKey PageUp;
	inline SDK::FKey Insert;
	inline SDK::FKey RightClick;
	inline SDK::FKey LeftClick;
	inline SDK::FKey MiddleMouseButton;
	inline SDK::FKey ThumbMouseButton;
	inline SDK::FKey ThumbMouseButton2;
	inline SDK::FKey LeftShift;
	inline SDK::FKey LeftControl;
	inline SDK::FKey LeftAlt;
	inline SDK::FKey Enter;
	inline SDK::FKey Up;
	inline SDK::FKey Down;
	inline SDK::FKey Left;
	inline SDK::FKey Right;
	inline SDK::FKey Zero;
	inline SDK::FKey One;
	inline SDK::FKey Two;
	inline SDK::FKey Three;
	inline SDK::FKey F1;
	inline SDK::FKey F2;
	inline SDK::FKey F3;
	inline SDK::FKey F4;
	inline SDK::FKey F5;
	inline SDK::FKey F6;
	inline SDK::FKey F7;
	inline SDK::FKey F8;
	inline SDK::FKey F9;
	inline SDK::FKey F10;
	inline SDK::FKey Gamepad_DPad_Up;
	inline SDK::FKey Gamepad_DPad_Down;
	inline SDK::FKey Gamepad_DPad_Left;
	inline SDK::FKey Gamepad_DPad_Right;
	inline SDK::FKey Gamepad_LeftShoulder;
	inline SDK::FKey Gamepad_RightShoulder;
	inline SDK::FKey Gamepad_LeftTrigger;
	inline SDK::FKey Gamepad_RightTrigger;
	inline SDK::FKey Gamepad_FaceButton_Bottom;
	inline SDK::FKey Gamepad_Special_Right;

	// Fnames
	inline SDK::FName HeadName;
	inline SDK::FName RootName;
	inline SDK::FName NeckName;
	inline SDK::FName LowerNeckName;
	inline SDK::FName ChestName;
	inline SDK::FName PelvisName;
	inline SDK::FName Upperarm_lName;
	inline SDK::FName Upperarm_rName;
	inline SDK::FName Lowerarm_lName;
	inline SDK::FName Lowerarm_rName;
	inline SDK::FName Hand_lName;
	inline SDK::FName Hand_rName;
	inline SDK::FName Thigh_lName;
	inline SDK::FName Thigh_rName;
	inline SDK::FName Calf_lName;
	inline SDK::FName Calf_rName;
	inline SDK::FName LowerCalf_lName;
	inline SDK::FName LowerCalf_rName;
	inline SDK::FName Foot_lName;
	inline SDK::FName Foot_rName;
	inline SDK::FName Toe_lName;
	inline SDK::FName Toe_rName;
}

typedef void (*PostRender_t)(void* pObject, void* pCanvas);
static inline PostRender_t origin_renderer;
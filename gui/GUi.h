#pragma once
// Defines
#define mpe 3.14159265358979323846264338327950288419716939937510582f
#define D2R(d) (d / 180.f) * mpe

// Variables
SDK::FVector2D Target_MenuInitialPos = { 400, 500 };
SDK::FVector2D borderStartPos;
SDK::FVector2D borderSize;
SDK::FLinearColor Target_ThemeColor = { 0, 89, 158, 1.f };
bool Target_bIsDragging = false;
float Target_MenuDragStartX = 0;
float Target_MenuDragStartY = 0;
float Target_MenuW = 539;
float Target_MenuH = 618;
int Target_TabIndex = 1;
SDK::FVector2D Target_ItemsCalculation;

//SDK::FString Target_MenuTitle = xor_w(L"Fortnite");

// Function headers
void MenuDrawTabs(uint64_t pCanvas);
void MenuDrawItemsFor(uint64_t pCanvas, int index);
bool IsInMenu(int x, int y);
void MenuHandleMouseClicks();

void DrawMenu(uint64_t pCanvas) {
	// Menu background
	SDK::Functions::DrawFilledRect(Target_MenuInitialPos, Target_MenuW - 2, Target_MenuH - 2, SDK::FLinearColor(0, 0, 0, 255.0f));
	// Menu tabs
	MenuDrawTabs(pCanvas);
	// Items in the menu
	MenuDrawItemsFor(pCanvas, Target_TabIndex);
	// Handle mouse clicks
	MenuHandleMouseClicks();

	float x, y;
	SDK::Functions::GetMousePosition(&x, &y);
	if (x && y) SDK::Functions::DrawFilledRect(SDK::FVector2D{ x - 3, y - 2 }, 5, 5, SDK::FLinearColor(0, 0, 0, 1.f));

	// Dev related
	/*if (Settings::devOutput) {
		if (GetAsyncKeyState(VK_F1)) {
			Settings::devFloat -= 1.f;
		}
		if (GetAsyncKeyState(VK_F2)) {
			Settings::devFloat += 1.f;
		}

		char tier_data[256];
		wchar_t tier_data_wide[256];
		sprintf(tier_data, xor_a("TARGET DEV OUTPUT:\nFloat (F1/F2): %d"), Settings::devFloat);
		AnsiToWide(tier_data, tier_data_wide);
		SDK::Functions::K2_DrawText(Globals::Font, tier_data_wide, { 30, 280 }, SDK::FVector2D(1.0f, 1.0f), SDK::FLinearColor(0.f, 255.f, 0.f, 1.f), 1.0f, SDK::FLinearColor(0, 0, 0, 0), SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 1.0f));
	}*/
}

bool IsInMenu(float x, float y)
{
	return (x >= Target_MenuInitialPos.X) && (x <= Target_MenuInitialPos.X + Target_MenuW) && (y >= Target_MenuInitialPos.Y) && (y <= Target_MenuInitialPos.Y + Target_MenuH);
}

bool IsInRectangle(float x1, float y1, float x2, float y2, float x, float y) {
	if (x > x1 && x < x2 && y > y1 && y < y2)
		return true;
	return false;
}

void MenuDrawTabs(uint64_t pCanvas)
{
	const wchar_t* pTabs[3];

	const wchar_t strAimbot[] = L" AIMBOT";
	const wchar_t strVisuals[] = L" VISUALS";
	const wchar_t strMisc[] = L"MISC";

	pTabs[0] = strAimbot;
	pTabs[1] = strVisuals;
	pTabs[2] = strMisc;

	auto inp = Target_MenuInitialPos + SDK::FVector2D(20, 35);

	//SDK::FString StringDistance = SDK::Functions::STATIC_BuildString_Int(xor_w(L""), xor_w(L" ["), distance, xor_w(L"m]"));
	SDK::FString Times = SDK::Functions::STATIC_Concat_StrStr(L" - ", TEXT(__DATE__));

	SDK::FString logoBuff = SDK::Functions::STATIC_Concat_StrStr(xor_w(L"Dark and Darker"), xor_w(L" "));

	//	SDK::FString logoBuff = SDK::FString::Printf(TEXT("%s (Build date: %s %s)"), Target_MenuTitle, TEXT(__DATE__), TEXT(__TIME__));
	SDK::Functions::K2_DrawText(defines::fnFont2, logoBuff, Target_MenuInitialPos + SDK::FVector2D(25, 11), SDK::FVector2D(.4f, .4f), SDK::FLinearColor(255.f, 255.f, 255.f, 1.0f), 1.0f, SDK::FLinearColor(0, 0, 0, 1.0f), SDK::FVector2D(), false, false, false, SDK::FLinearColor(0, 0, 0, 1.0f));

	auto tabsz = (Target_MenuW - 40) / 3;
	tabsz -= 2;
	SDK::FVector2D ip = inp + SDK::FVector2D(2, 2);

	// for border around tabs
	SDK::FVector2D startPos;
	SDK::FVector2D widthHeight;
	widthHeight.Y = 22;

	auto i = 0;
	for (int p = 0; p < 3; p++)
	{
		auto tab = pTabs[p];

		auto clr2 = SDK::FLinearColor(0.f, 0.f, 0.f, 1.f);
		auto clr = SDK::FLinearColor(255.f, 255.f, 255.f, 1.f);
		if (Target_TabIndex == i)
		{
			clr = SDK::FLinearColor(0.f, 0.f, 0.f, 1.f);
			clr2 = Target_ThemeColor;
		}

		SDK::Functions::DrawFilledRect(ip, tabsz, 22, clr2);

		float x1 = ip.X, y1 = ip.Y, x2 = ip.X + tabsz, y2 = ip.Y + 22;

		// given point
		float x, y;
		SDK::Functions::GetMousePosition(&x, &y);
		if (IsInRectangle(x1, y1, x2, y2, x, y))
		{
			Target_bIsDragging = false;
			if (SDK::Functions::WasInputKeyJustPressed(defines::PlayerController, defines::LeftClick))
				Target_TabIndex = i;
		}

		if (!startPos.X || startPos.X == 0)
			startPos = ip;

		widthHeight.X += tabsz + 2;

		SDK::FVector2D TextSize0 = SDK::Functions::K2_TextSize(defines::fnFont2, tab, SDK::FVector2D(1.0f, 1.0f));
		float bestFitByDanielle = (ip.X + (tabsz / 2)) - (TextSize0.X / 2);
		SDK::Functions::K2_DrawText(defines::fnFont2, tab, SDK::FVector2D(bestFitByDanielle, ip.Y + 3), SDK::FVector2D(.4f, .4f), clr, 1.0f, SDK::FLinearColor(255.f, 255.f, 255.f, 1.0f), SDK::FVector2D(), false, false, false, SDK::FLinearColor(0, 0, 0, 1.0f));

		ip.X += tabsz + 2;
		i++;
	}
	widthHeight.X -= 2;

	// draw border
	SDK::Functions::K2_DrawBox(startPos, widthHeight, 1.f, SDK::FLinearColor(255.f, 255.f, 255.f, 1.f));
	borderStartPos = startPos;
	borderSize = widthHeight;

	Target_ItemsCalculation = inp + SDK::FVector2D(0, 35);
}

void Render_Circle(uint64_t pCanvas, SDK::FVector2D pos, float r, SDK::FLinearColor color)
{
	float circum = mpe * 2.f * r;

	int seg;
	int inum = (int)circum;
	if (circum == (float)inum) {
		seg = inum;
	}
	seg = inum + 1;

	if (seg > 180) seg = 180;

	float theta = 0.f;
	float step = 180.f / seg;

	for (size_t i = 0; i < seg; ++i)
	{
		theta = i * step;
		auto delta = SDK::FVector2D((float)SDK::Functions::UKismetMathLibrarySTATIC_Round(r * SDK::Functions::UKismetMathLibrarySTATIC_Sin(D2R(theta))), (float)SDK::Functions::UKismetMathLibrarySTATIC_Round(r * SDK::Functions::UKismetMathLibrarySTATIC_Cos(D2R(theta))));
		SDK::Functions::K2_DrawLine(pos + delta, pos - delta, 1.f, color);
	}
}

void Render_Clear(uint64_t pCanvas, SDK::FVector2D one, SDK::FVector2D two, SDK::FLinearColor color) {
	for (auto x = one.X; x < two.X; x += 1.f)
	{
		SDK::Functions::K2_DrawLine(SDK::FVector2D(x, one.Y), SDK::FVector2D(x, two.Y), 1.f, color);
	}
}

void Render_PointArray(uint64_t pCanvas, size_t count, SDK::FVector2D* ary, SDK::FLinearColor color) {
	for (size_t i = 1; i < count; ++i)
		SDK::Functions::K2_DrawLine(ary[i - 1], ary[i], 1.0f, color);
}

void Render_CircleOutline(uint64_t pCanvas, SDK::FVector2D pos, float r, SDK::FLinearColor outline) {
	float circum = mpe * 2.f * r;

	int seg;
	int inum = (int)circum;
	if (circum == (float)inum) {
		seg = inum;
	}
	seg = inum + 1;

	if (seg > 180) seg = 180;

	float theta = 0.f;
	float step = 360.f / seg;

	SDK::FVector2D points[180] = {};

	for (size_t i = 0; i < seg; ++i)
	{
		theta = i * step;
		points[i] = SDK::FVector2D(pos.X + SDK::Functions::UKismetMathLibrarySTATIC_Round(r * SDK::Functions::UKismetMathLibrarySTATIC_Sin(D2R(theta))), pos.Y + SDK::Functions::UKismetMathLibrarySTATIC_Round(r * SDK::Functions::UKismetMathLibrarySTATIC_Cos(D2R(theta))));
	}

	Render_PointArray(pCanvas, seg, points, outline);
}

void Render_CircleOutlined(uint64_t pCanvas, SDK::FVector2D pos, float r, SDK::FLinearColor fill, SDK::FLinearColor outline) {
	Render_Circle(pCanvas, pos, r, fill);
	Render_CircleOutline(pCanvas, pos, r, outline);
}

void Render_Toggle(uint64_t pCanvas, SDK::FVector2D& loc_ref, const wchar_t* name, bool* on) {
	auto loc = &loc_ref;

	float mouseX, mouseY;
	SDK::Functions::GetMousePosition(&mouseX, &mouseY);

	bool hover = mouseX > loc->X && mouseX < (loc->X + 64) && mouseY > loc->Y && mouseY < (loc->Y + 20);
	if (hover)
		Target_bIsDragging = false;

	if (SDK::Functions::WasInputKeyJustPressed(defines::PlayerController, defines::LeftClick) && hover)
		*on = !*on;

	SDK::FLinearColor col = *on ? Target_ThemeColor : SDK::FLinearColor(170.f, 170.f, 170.f, 1.f);

	Render_Circle(pCanvas, SDK::FVector2D(loc->X + 10, loc->Y + 10), 6, col);
	Render_Circle(pCanvas, SDK::FVector2D(loc->X + 18, loc->Y + 10), 6, col);
	Render_Clear(pCanvas, SDK::FVector2D(loc->X + 10, loc->Y + 4), SDK::FVector2D(loc->X + 18, loc->Y + 16), col);

	if (*on) {
		Render_CircleOutlined(pCanvas, SDK::FVector2D(loc->X + 18, loc->Y + 10), 5, hover ? SDK::FLinearColor(0.8f * 255, 0.8f * 255, 0.8f * 255, 1.f) : SDK::FLinearColor(255, 255, 255, 1), SDK::FLinearColor(0, 0, 0, 1.f));
		SDK::Functions::K2_DrawLine(SDK::FVector2D(loc->X + 9, loc->Y + 8), SDK::FVector2D(loc->X + 9, loc->Y + 12), 1.f, SDK::FLinearColor(0, 0, 0, 1.f));
	}
	else {
		Render_CircleOutlined(pCanvas, SDK::FVector2D(loc->X + 10, loc->Y + 10), 5, hover ? SDK::FLinearColor(0.8f * 255, 0.8f * 255, 0.8f * 255, 1.f) : SDK::FLinearColor(255, 255, 255, 1), SDK::FLinearColor(0, 0, 0, 1.f));
		Render_CircleOutline(pCanvas, SDK::FVector2D(loc->X + 19, loc->Y + 10), 2, SDK::FLinearColor(0, 0, 0, 1.f));
	}

	SDK::Functions::K2_DrawText(defines::fnFont2, name, SDK::FVector2D(loc->X + 32, loc->Y + 2), SDK::FVector2D(.4f, .4f), SDK::FLinearColor(255.f, 255.f, 255.f, 1.0f), 1.0f, SDK::FLinearColor(), SDK::FVector2D(), false, false, false, SDK::FLinearColor(0, 0, 0, 1.f));

	loc->Y += 25.0f;
}

void Render_Tip(uint64_t pCanvas, SDK::FVector2D& loc_ref, const wchar_t* text) {
	auto loc = &loc_ref;

	SDK::Functions::K2_DrawText(defines::fnFont2, text, SDK::FVector2D(loc->X + 5, loc->Y + 2), SDK::FVector2D(.4f, .4f), SDK::FLinearColor(255.f, 255.f, 255.f, 1.0f), 1.0f, SDK::FLinearColor(), SDK::FVector2D(), false, false, false, SDK::FLinearColor(0, 0, 0, 1.f));

	loc->Y += 25.0f;
}

void Render_Slider(uint64_t pCanvas, const wchar_t* name, float minimum, float maximum, float* val, SDK::FVector2D* loc) {
	auto kinda_white = SDK::FLinearColor(204.f, 204.f, 204.f, 1.f);

	SDK::Functions::K2_DrawText(defines::fnFont2, name, SDK::FVector2D(loc->X + 6, loc->Y), SDK::FVector2D(.4f, .4f), SDK::FLinearColor(255.f, 255.f, 255.f, 1.0f), 1.0f, SDK::FLinearColor(), SDK::FVector2D(), false, false, false, SDK::FLinearColor(0, 0, 0, 1.f));

	loc->X += 6.f;

	float mouseX, mouseY;
	SDK::Functions::GetMousePosition(&mouseX, &mouseY);

	bool hover = mouseX > loc->X && mouseX < (loc->X + 199) && mouseY > loc->Y && mouseY < (loc->Y + 30);
	if (hover)
		Target_bIsDragging = false;

	if (SDK::Functions::IsInputKeyDown(defines::LeftClick) && hover) {
		float ratio = (float)(mouseX - loc->X) / 199;
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
		*val = minimum + ((maximum - minimum) * ratio);
	}

	float xpos = ((*val - minimum) / (maximum - minimum)) * 199;
	loc->Y += 24.f;

	Render_Circle(pCanvas, *loc, 3, Target_ThemeColor);
	Render_Clear(pCanvas, SDK::FVector2D(loc->X, loc->Y - 3), SDK::FVector2D(loc->X + xpos, loc->Y + 3), Target_ThemeColor);

	Render_Clear(pCanvas, SDK::FVector2D(loc->X + xpos, loc->Y - 3), SDK::FVector2D(loc->X + 199, loc->Y + 3), kinda_white);
	Render_Circle(pCanvas, SDK::FVector2D(loc->X + 199, loc->Y), 3, kinda_white);

	Render_CircleOutlined(pCanvas, SDK::FVector2D(loc->X + xpos, loc->Y), 5, SDK::FLinearColor(255.f, 255.f, 255.f, 1.f), SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));

	loc->Y -= 24.f;

	//wchar_t wstr[16] = {};
	//char __str[16] = {};
	SDK::FString wstr = SDK::Functions::STATIC_BuildString_Int(xor_w(L""), xor_w(L""), (float)*val, xor_w(L""));

	//sprintf(__str, xor_a("%0.1f"), *val);
	//AnsiToWide(__str, wstr);
	SDK::Functions::K2_DrawText(defines::fnFont2, wstr, SDK::FVector2D(loc->X + 199 - 30.0f, loc->Y + 33), SDK::FVector2D(.4f, .4f), SDK::FLinearColor(0.f, 0.f, 0.f, 1.f), 1.0f, SDK::FLinearColor(), SDK::FVector2D(), false, false, false, SDK::FLinearColor(0, 0, 0, 1.f));

	loc->X -= 6.f;
	loc->Y += 35.0f;
	loc->Y += 13.0f;
}

void MenuCheckbox(uint64_t pCanvas, SDK::FVector2D sp, const wchar_t* text, bool* shittobind)
{
	SDK::FLinearColor Green = { 0.000000000f, 0.501960814f * 255, 0.000000000f, 1.000000000f };
	SDK::FLinearColor SlateGray = { 0.439215720f * 255, 0.501960814f * 255, 0.564705908f * 255, 1.000000000f };
	auto color = *shittobind ? Green : SlateGray;
	sp.X += 3;
	SDK::FLinearColor gayshit = { 0.06f * 255, 0.06f * 255, 0.06f * 255, 1.000000000f };
	//RegisterButtonControl(sp + g_Clientarea, 15, 15, gayshit, -1, shittobind);
	SDK::Functions::DrawFilledRect(sp + Target_ItemsCalculation + 3, 9, 9, color);
	SDK::Functions::K2_DrawText(defines::fnFont2, text, sp + Target_ItemsCalculation + SDK::FVector2D({ 20, -2 }), SDK::FVector2D(.4f, .4f), SDK::FLinearColor(255, 255, 255, 1), 1.0f, SDK::FLinearColor(0, 0, 0, 1), SDK::FVector2D(), false, false, true, SDK::FLinearColor(0, 0, 0, 0));
}

void MenuDrawItemsFor(uint64_t pCanvas, int index) {
	// draw border
	SDK::Functions::K2_DrawBox(SDK::FVector2D(borderStartPos.X, Target_ItemsCalculation.Y + 4), SDK::FVector2D(borderSize.X, Target_MenuH - 95), 1.f, SDK::FLinearColor(255, 255, 255, 1.f));

	if (index == 0)
	{
		auto loc = Target_ItemsCalculation + SDK::FVector2D(11, 12);
		Render_Toggle(pCanvas, loc, xor_w(L"Enable aimbot (F1)"), &aimbot);
		//Render_Toggle(pCanvas, loc, xor_a(L"Enable Silent"), &Settings::silent);
		Render_Toggle(pCanvas, loc, xor_w(L"FOV circle"), &drawFOV);
		Render_Toggle(pCanvas, loc, xor_w(L"Visiblity check"), &visCheck);
		//Render_Toggle(pCanvas, loc, xor_w(L"Aim Line"), &AimLine);
		//Render_Toggle(pCanvas, loc, xor_w(L"Building weaks spot aimbot"), &Settings::aimWeakspot);
		//Render_Toggle(pCanvas, loc, xor_a(L"Sniper Prediction"), &Settings::sniperPredict);
		Render_Slider(pCanvas, xor_w(L"Aimbot fov"), 25, 1000, &FOVRange, &loc);
		Render_Slider(pCanvas, xor_w(L"Smooth "), 0, 25, &smooth, &loc);
	}
	if (index == 1)
	{
		auto loc = Target_ItemsCalculation + SDK::FVector2D(11, 12);
		Render_Toggle(pCanvas, loc, xor_w(L"Enable Visuals "), &visuals);
		Render_Toggle(pCanvas, loc, xor_w(L"Player Box"), &playerBox);
		if (playerBox) { playerCornerBox = false; }
		Render_Toggle(pCanvas, loc, xor_w(L"Player Corner box"), &playerCornerBox);
		if (playerCornerBox) { playerBox = false; }
		Render_Toggle(pCanvas, loc, xor_w(L"Player Distance"), &playerDistance);
		Render_Toggle(pCanvas, loc, xor_w(L"Player Skeleton"), &Skeletons);
		Render_Toggle(pCanvas, loc, xor_w(L"Player Name"), &playerName);
		Render_Toggle(pCanvas, loc, xor_w(L"Draw Loot"), &drawInteractable);
		Render_Toggle(pCanvas, loc, xor_w(L"Draw Normal Chest"), &drawNormalChest);
		Render_Toggle(pCanvas, loc, xor_w(L"Draw Special Chest"), &drawSpecialChest);
		Render_Toggle(pCanvas, loc, xor_w(L"Draw Traps"), &drawTrap);
		Render_Toggle(pCanvas, loc, xor_w(L"Draw Healing Potion"), &drawHealPot);
		Render_Toggle(pCanvas, loc, xor_w(L"Draw Portals"), &drawPortalz);

		loc.Y = Target_ItemsCalculation.Y + 37;
		loc.X += Target_MenuW / 2 - 1;

		Render_Slider(pCanvas, xor_w(L"Player Overall Distance"), 5, 150, &overallDistance, &loc);
		Render_Slider(pCanvas, xor_w(L"Portal Distance"), 5, 150, &overallPortalDistance, &loc);
		Render_Slider(pCanvas, xor_w(L"Chest Distance"), 5, 50, &overallChestDistance, &loc);
		Render_Slider(pCanvas, xor_w(L"Loot Distance"), 5, 50, &InteractableDistance, &loc);
		Render_Slider(pCanvas, xor_w(L"Trap Distance"), 5, 50, &trapDistance, &loc);
		Render_Slider(pCanvas, xor_w(L"Luck Potion Distance"), 5, 50, &LuckPotDistance, &loc);
		Render_Slider(pCanvas, xor_w(L"Healing Potion Distance"), 5, 50, &HealPotDistance, &loc);
		
	}
	if (index == 2)
	{
		auto loc = Target_ItemsCalculation + SDK::FVector2D(11, 12);
		Render_Toggle(pCanvas, loc, xor_w(L"FOV Changer"), &FOVChanger);

		loc.Y = Target_ItemsCalculation.Y + 37;
		loc.X += Target_MenuW / 2 - 1;
		if (FOVChanger) {
			Render_Slider(pCanvas, xor_w(L"Player FOV"), 80, 150, &FOVSize, &loc);
		}
	}
}

void MenuHandleMouseClicks() {
	if (defines::PlayerController) {
		if (SDK::Functions::WasInputKeyJustPressed(defines::PlayerController, defines::LeftClick)) {
			float locationX, locationY;
			SDK::Functions::GetMousePosition(&locationX, &locationY);
			if (IsInMenu(locationX, locationY)) {
				Target_bIsDragging = true;
				Target_MenuDragStartX = locationX;
				Target_MenuDragStartY = locationY;
			}
		}
		if (SDK::Functions::WasInputKeyJustReleased(defines::LeftClick)) {
			Target_bIsDragging = false;
		}
		if (Target_bIsDragging) {
			float locationX, locationY;
			SDK::Functions::GetMousePosition(&locationX, &locationY);

			int width, height;
			SDK::Functions::GetViewportSize(defines::PlayerController, &width, &height);
			auto newX = Target_MenuInitialPos.X + (locationX - Target_MenuDragStartX);
			auto newY = Target_MenuInitialPos.Y + (locationY - Target_MenuDragStartY);
			if (newX >= width - Target_MenuW)
				newX = width - Target_MenuW;
			if (newY >= height - Target_MenuH)
				newY = height - Target_MenuH;
			if (newX <= 0)
				newX = 0;
			if (newY <= 0)
				newY = 0;

			Target_MenuInitialPos.X = newX;
			Target_MenuInitialPos.Y = newY;
			Target_MenuDragStartX = locationX;
			Target_MenuDragStartY = locationY;
		}
	}
}
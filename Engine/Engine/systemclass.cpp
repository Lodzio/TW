////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"
#include "Pathfinding.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	Command = 0;
	Scroll = 100;
	Center.x = -3600;
	Center.y = -2700;
	m_Timer = 0;
	Commander = 0;
	AmmountofObjectsToRender = 0;
	List_to_render = 0;
	PlayerOneSquadAmmount = 0;
	PlayerOneSquads = 0;
	SparkList = 0;
	RightMouseButtonPressed = 0;
	for (int i = 0; i < 8; i++)
		Soldiers[i] = 0;
	daily_timer = 0;
	monthly_timer = 0;
	n_Shops = 0;
	n_Bakery = 0;
	n_Mill = 0;
	n_Farm = 0;
	n_Employee = 0;
	Employee_list = 0;
	n_Family = 0;
	n_Mine = 0;
	n_Forger = 0;
	n_Workshop = 0;
	CityCenter = Point2L(4000, 3000);
	cost_of_life = 300;
	Graphinput.path = 0;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
	m_Graphics->Mode.JOBSecondary = m_Graphics->Mode.JOB_IDLE;
	m_Graphics->Mode.Unit = 0;

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Timer = new TimerClass;
	if (!m_Timer->Initialize())
	{
		return false;
	}
	Player_Fraction = Fractions::FIRSTARMY;
	CreateNewSquad(OBJTYPES::ENGINEERS, Player_Fraction);
	Object* Buffor;
	PlaceUnit(Point2F(1600, 1600), Buffor, 1, OBJTYPES::COMMANDER, Fractions::GUERILLA);
	Commander = (CommanderClass*)Buffor;
	Commander->SetFractions(Fractions::GUERILLA);
	Commander->SetMaxSpeed(200);
	Commander->SetMaxAcceleration(500);

	QuadTree.Initialize(SizeU(screenWidth * 10, screenHeight * 10));
	SparkList = new SparksClass[2];
	SparkList[0].initialize(SparksClass::ShootFire);
	SparkList[1].initialize(SparksClass::Blood);

	City.init(CityCenter);

	AddEmployee();
	Employee_list[n_Employee - 1]->GiveAdult();
	AddFactory(FactoryClass::factory_type::BAKERY, Employee_list[n_Employee - 1]);
	Employee_list[n_Employee - 1]->GetWallet()->Put_in_money(3000);
	City.GetFactory(0)->GiveProducts(10);

	AddEmployee();
	Employee_list[n_Employee - 1]->GiveAdult();
	AddFactory(FactoryClass::factory_type::MINE, Employee_list[n_Employee - 1]);
	Employee_list[n_Employee - 1]->GetWallet()->Put_in_money(3000);
	City.GetFactory(1)->GiveProducts(10);

	AddEmployee();
	Employee_list[n_Employee - 1]->GiveAdult();
	AddFactory(FactoryClass::factory_type::FARM, Employee_list[n_Employee - 1]);

	AddEmployee();
	Employee_list[n_Employee - 1]->GiveAdult();
	AddFactory(FactoryClass::factory_type::WORKSHOP, Employee_list[n_Employee - 1]);

	AddEmployee();
	Employee_list[n_Employee - 1]->GiveAdult();
	AddFactory(FactoryClass::factory_type::MILL, Employee_list[n_Employee - 1]);
	Employee_list[n_Employee - 1]->GetWallet()->Put_in_money(3000);

	AddEmployee();
	Employee_list[n_Employee - 1]->GiveAdult();
	AddFactory(FactoryClass::factory_type::FORGER, Employee_list[n_Employee - 1]);
	Employee_list[n_Employee - 1]->GetWallet()->Put_in_money(3000);

	lookForJob();
	
	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	bool result;
	QuadTree.Reset();
	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	// Do the frame processing for the graphics object.
	m_Timer->Frame();
	Commander->CalcPosition((int)m_Timer->GetTime());
	for (int i = 0; i < PlayerOneSquadAmmount; i++)
	{
		PlayerOneSquads[i]->CalcPosition((int)m_Timer->GetTime());
		for (int j = 0; j < PlayerOneSquads[i]->GetMembers(); j++)
		{
			QuadTree.PutInObject(PlayerOneSquads[i]->GetUnit(j));
		}
	}

	for (int i = 0; i < n_Employee; i++)
	{
		if (Employee_list[i]->isinfactory())
			continue;
		else
		{
			Employee_list[i]->CalcPosition((int)m_Timer->GetTime());
			QuadTree.PutInObject(Employee_list[i]);
		}
	}

	for (int i = 0; i < City.get_n_of_factores(); i++)
	{
		QuadTree.PutInObject(City.GetFactory(i));
	}
	if (Command)
	{
		QuadTree.PutInObject(Command);
	}
	if (Commander)
	{
		QuadTree.PutInObject(Commander);
	}

	AreaData ListToCollide = QuadTree.GetListToColide();
	QuadTree.CalculateViewArea();
	SparkList[0].UpdateTime(m_Timer->GetTime());
	SparkList[1].UpdateTime(m_Timer->GetTime());
	for (int i = 0; i < PlayerOneSquadAmmount; i++)
	{
		PlayerOneSquads[i]->CalcShooting(m_Timer->GetTime(), SparkList);
	}

	for (int i = 0; i < ListToCollide.Number;)
	{
		if (ListToCollide.ObjectsList[i]->IsMovable())
		{
			((UnitClass*)ListToCollide.ObjectsList[i])->CalcCollide(ListToCollide.ObjectsList[i + 1]);
		}
		else
		{
			((Building*)ListToCollide.ObjectsList[i])->Collide(ListToCollide.ObjectsList[i + 1]);
		}
		i += 2;
	}
	delete[] ListToCollide.ObjectsList;


	daily_timer += (unsigned int)m_Timer->GetTime();
	if (daily_timer > 1000)
	{
		UpdateCity(1000);
		daily_timer -= 1000;
		monthly_timer++;
		if (monthly_timer == 30)
		{
			EndOfMonth();
			monthly_timer = 0;
		}
	}
	Graphinput.AmmountOfObjects = AmmountofObjectsToRender;
	Graphinput.Center = Center;
	Graphinput.city = &City;
	Graphinput.FPS = (int)m_Timer->GetFPS();
	Graphinput.Objects = List_to_render;
	Graphinput.QuadTree = &QuadTree;
	Graphinput.Scroll = Scroll;
	Graphinput.Sparks = SparkList;
	Graphinput.city = &City;
	Graphinput.boxes = &pathsys;
	result = m_Graphics->Frame(Graphinput);
	if(!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}


		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lparam);
			int yPos = GET_Y_LPARAM(lparam);
			if (RightMouseButtonPressed)
			{
				float distance = (float)sqrt(pow(xPos - MouseClickPos.x, 2) + pow(yPos - MouseClickPos.y, 2));
				
				if (m_Graphics->Mode.JOBSecondary == m_Graphics->Mode.JOB_IDLE)
				{
					if (distance > 20)
					{
						m_Graphics->Mode.JOBSecondary = m_Graphics->Mode.JOB_MOVING;
					}
				}
			}
			RECT Screen;
			GetClientRect(m_hwnd, &Screen);
			m_Graphics->UpdateState(xPos, yPos, Scroll, Center);
			if (m_Graphics->Mode.JOBSecondary == m_Graphics->Mode.JOB_MOVING)
			{
				int Deltax = LastMousePos.x - xPos;
				int Deltay = LastMousePos.y - yPos;
				double I = 1000.0 / Scroll;

				Deltax = (int)(Deltax * I);
				Deltay = (int)(Deltay * I);
				Center.x -= Deltax;

				//Prawa i górna krawêdŸ
				int Range = (int)(-Center.x - ((1000.0 / Scroll) - 1) * (Screen.right - Screen.left) / 2);
				if (Range < 0)
				{
					Center.x = (LONG)(-((1000.0 / Scroll) - 1) * (Screen.right - Screen.left) / 2);
				}
				Center.y -= Deltay;
				Range = (int)(-Center.y - ((1000.0 / Scroll) - 1) * (Screen.bottom - Screen.top) / 2);
				if (Range < 0)
				{
					Center.y = (LONG)(-((1000.0 / Scroll) - 1) * (Screen.bottom - Screen.top) / 2);
				}

				//Lewa i dolna krawêdŸ
				Range = (int)(-Center.x + Screen.right - Screen.left + ((Screen.right - Screen.left) * ((1000.0 / Scroll) - 1) / 2));
				if (Range > (int)(m_Graphics->GetMapSize().width))
				{
					Center.x = (LONG)(Screen.right - Screen.left + ((Screen.right - Screen.left) * ((1000.0 / Scroll) - 1) / 2) - m_Graphics->GetMapSize().width);
				}

				Range = (int)(-Center.y + Screen.bottom - Screen.top + ((Screen.bottom - Screen.top) * ((1000.0 / Scroll) - 1) / 2));
				if (Range > (int)(m_Graphics->GetMapSize().height))
				{
					Center.y = (LONG)(Screen.bottom - Screen.top + ((Screen.bottom - Screen.top) * ((1000.0 / Scroll) - 1) / 2) - m_Graphics->GetMapSize().height);
				}
			}
			else
			{
				m_Graphics->UpdateState(xPos, yPos, Scroll, Center);
			}
			LastMousePos.x = xPos;
			LastMousePos.y = yPos;
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			int xPos = GET_X_LPARAM(lparam);
			int yPos = GET_Y_LPARAM(lparam);
			LeftButtonPressed(Point2U(xPos, yPos));
			return 0;
		}
		
		case WM_RBUTTONDOWN:
		{
			int xPos = GET_X_LPARAM(lparam);
			int yPos = GET_Y_LPARAM(lparam);
			MouseClickPos = Point2F((float)xPos, (float)yPos);
			RightMouseButtonPressed = true;
			return 0;
		}

		case WM_RBUTTONUP:
		{
			RightMouseButtonPressed = false;
			if (m_Graphics->Mode.JOBSecondary == m_Graphics->Mode.JOB_MOVING)
			{
				m_Graphics->Mode.JOBSecondary = m_Graphics->Mode.JOB_IDLE;
			}
			else
			{
				int xPos = GET_X_LPARAM(lparam);
				int yPos = GET_Y_LPARAM(lparam);
				RECT Screen;
				GetClientRect(m_hwnd, &Screen);

				if (m_Graphics->Mode.JobPrimary == m_Graphics->Mode.JOB_SELECTED)
				{
					D2D1_POINT_2F Global;
					Global = Point2F((float)(-Center.x - ((Screen.right - Screen.left) * ((1000.0 / Scroll) - 1) / 2) + (xPos * 1000.0 / Scroll)),
						(float)(-Center.y - ((Screen.bottom - Screen.top) * ((1000.0 / Scroll) - 1) / 2) + (yPos * 1000.0 / Scroll)));
					Object* output = QuadTree.GetObjectByCoords(Global);
					if (output == 0)
					{
						if (m_Graphics->Mode.Unit == OBJTYPES::ENGINEERS || m_Graphics->Mode.Unit == OBJTYPES::SOLDIER)
						{
							((SquadClass*)Selected)->SetTargetToMove(Global);
						}
						else if (m_Graphics->Mode.Unit == OBJTYPES::COMMANDER)
						{
							((CommanderClass*)Selected)->SetTarget(Global);
						}
						else if (m_Graphics->Mode.Unit == OBJTYPES::COMMANDCENTER)
						{
							m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
							((Object*)Selected)->Selected = false;
						}
					}
					else
					{
						if (!output->IsMovable() && m_Graphics->Mode.Unit == OBJTYPES::ENGINEERS)
						{
							((SquadClass*)Selected)->SetTargetToRepair(output);
							m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
							((SquadClass*)Selected)->Select(false);
						}
					}
				}
			}
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
			if (zDelta > 0)
			{
				if (Scroll > 100)
				{
					RECT Screen;
					GetClientRect(m_hwnd, &Screen);
					Scroll -= 10;

					//Prawa i górna krawêdŸ
					int Range = (int)(-Center.x - ((1000.0 / Scroll) - 1) * (Screen.right - Screen.left) / 2);
					if (Range < 0)
					{
						Center.x += Range;
					}
					Range = (int)(-Center.x - ((1000.0 / Scroll) - 1) * (Screen.right - Screen.left) / 2);
					Range = (int)(-Center.y - ((1000.0 / Scroll) - 1) * (Screen.bottom - Screen.top) / 2);
					if (Range < 0)
					{
						Center.y += Range;
					}

					//Lewa i dolna krawêdŸ
					Range = (int)(-Center.x + Screen.right - Screen.left + ((Screen.right - Screen.left) * ((1000.0 / Scroll) - 1) / 2));
					if (Range > (int)(m_Graphics->GetMapSize().width))
					{
						Center.x = (LONG)(Screen.right - Screen.left + ((Screen.right - Screen.left) * ((1000.0 / Scroll) - 1) / 2) - m_Graphics->GetMapSize().width);
					}

					Range = (int)(-Center.y + Screen.bottom - Screen.top + ((Screen.bottom - Screen.top) * ((1000.0 / Scroll) - 1) / 2));
					if (Range > (int)(m_Graphics->GetMapSize().height))
					{
						Center.y = (LONG)(Screen.bottom - Screen.top + ((Screen.bottom - Screen.top) * ((1000.0 / Scroll) - 1) / 2) - m_Graphics->GetMapSize().height);
					}
				}
			}
			else if (Scroll < 1000)
			{
				Scroll += 10;
			}

			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

bool SystemClass::PlaceUnit(D2D1_POINT_2F Pos, Object* &Obj, int index, UINT unitType, UINT Fraction, UINT FactoryType, EmployeeClass* own)
{
	RECT Screen;
	GetClientRect(m_hwnd, &Screen);

	D2D1_RECT_U rc;
	D2D1_SIZE_F siz;
	D2D1_SIZE_F scale;
	if (unitType == OBJTYPES::SOLDIER)
	{
		rc.bottom = 100;
		rc.left = 0;
		rc.right = 100;
		rc.top = 0;
		Obj = new SoldierClass;
		scale.height = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		scale.width = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		siz.height = (float)(0.1 * (Screen.bottom - Screen.top));
		siz.width = (float)(0.1 * (Screen.bottom - Screen.top));
		Obj->initialize(index, Pos, 0, siz, scale, 100, 100, rc, Point2F(0.5, 0.5), OBJTYPES::UNIT);
		((UnitClass*)Obj)->SetUnitType(unitType);
		((UnitClass*)Obj)->SetFractions(Fraction);
		((UnitClass*)Obj)->SetPathfinder(&pathsys);
	}
	else if (unitType == OBJTYPES::ENGINEERS)
	{
		rc.bottom = 100;
		rc.left = 100;
		rc.right = 200;
		rc.top = 0;
		Obj = new EngineerClass;
		scale.height = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		scale.width = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		siz.height = (float)(0.1 * (Screen.bottom - Screen.top));
		siz.width = (float)(0.1 * (Screen.bottom - Screen.top));
		Obj->initialize(index, Pos, 0, siz, scale, 100, 100, rc, Point2F(0.5, 0.5), OBJTYPES::UNIT);
		((UnitClass*)Obj)->SetUnitType(unitType);
		((UnitClass*)Obj)->SetFractions(Fraction);
		((UnitClass*)Obj)->SetPathfinder(&pathsys);
	}
	else if (unitType == OBJTYPES::COMMANDER)
	{
		rc.bottom = 100;
		rc.left = 200;
		rc.right = 300;
		rc.top = 0;
		Obj = new CommanderClass;
		scale.height = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		scale.width = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		siz.height = (float)(0.1 * (Screen.bottom - Screen.top));
		siz.width = (float)(0.1 * (Screen.bottom - Screen.top));
		Obj->initialize(index, Pos, 0, siz, scale, 100, 100, rc, Point2F(0.5, 0.5), OBJTYPES::UNIT);
		((UnitClass*)Obj)->SetUnitType(unitType);
		((UnitClass*)Obj)->SetFractions(Fraction);
		((UnitClass*)Obj)->SetPathfinder(&pathsys);
	}
	else if (unitType == OBJTYPES::EMPLOYEE)
	{
		rc.bottom = 100;
		rc.left = 200;
		rc.right = 300;
		rc.top = 0;
		Obj = new EmployeeClass;
		scale.height = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		scale.width = (float)((0.1 * (Screen.bottom - Screen.top)) / 100);
		siz.height = (float)(0.1 * (Screen.bottom - Screen.top));
		siz.width = (float)(0.1 * (Screen.bottom - Screen.top));
		Obj->initialize(index, Pos, 0, siz, scale, 100, 100, rc, Point2F(0.5, 0.5), OBJTYPES::UNIT);
		((UnitClass*)Obj)->SetUnitType(unitType);
		((UnitClass*)Obj)->SetFractions(Fraction);
		((UnitClass*)Obj)->SetPathfinder(&pathsys);
		//City.AddEmployee((EmployeeClass*)Obj);
	}
	else if (unitType == OBJTYPES::COMMANDCENTER)
	{
		Pos.x -= (int)Pos.x % 20;
		Pos.y -= (int)Pos.y % 20;

		rc.bottom = 100;
		rc.left = 0;
		rc.right = 200;
		rc.top = 0;

		siz.height = (float)(0.2 * (Screen.bottom - Screen.top));
		siz.width = (float)(0.4 * (Screen.bottom - Screen.top));
		scale.height = (float)((0.2 * (Screen.bottom - Screen.top)) / 100);
		scale.width = (float)((0.4 * (Screen.bottom - Screen.top)) / 200);

		Obj = new ComCenter;
		Obj->initialize(index, Pos, 0, siz, scale, 1, 1000, rc, Point2F(0.5, 0.5), OBJTYPES::BUILDING);
		Obj->SetFractions(Fraction);
		pathsys.AddBuilding((Building*)Obj);
	}
	else if (unitType == OBJTYPES::FACTORY)
	{
		switch (FactoryType)
		{

		case FactoryClass::FARM:
		{
			rc.bottom = 200;
			rc.left = 200;
			rc.right = 300;
			rc.top = 100;
			break;
		}

		case FactoryClass::MILL:
		{
			rc.bottom = 300;
			rc.left = 200;
			rc.right = 300;
			rc.top = 200;
			break;
		}

		default:
		{
			rc.bottom = 100;
			rc.left = 200;
			rc.right = 300;
			rc.top = 0;
			break;
		}
		}

		siz.height = (float)(0.2 * (Screen.bottom - Screen.top));
		siz.width = (float)(0.2 * (Screen.bottom - Screen.top));
		scale.height = (float)((0.2 * (Screen.bottom - Screen.top)) / 100);
		scale.width = (float)((0.2 * (Screen.bottom - Screen.top)) / 100);

		Obj = new FactoryClass;
		Obj->initialize(index, Pos, 0, siz, scale, 1, 1000, rc, Point2F(0.5, 0.5), OBJTYPES::BUILDING);
		Obj->SetFractions(Fraction);

		float salary = (float)((cost_of_life / 30) * 1.3);

		if (FactoryType == FactoryClass::factory_type::BAKERY)
		{
			((FactoryClass*)Obj)->Init_factory(FactoryClass::Bread, FactoryClass::Flour, 1000, own, FactoryType, salary, &City);
		}
		else if (FactoryType == FactoryClass::factory_type::FARM)
		{
			((FactoryClass*)Obj)->Init_factory(FactoryClass::Wheat, NULL, 750, own, FactoryType, salary, &City);
		}
		else if (FactoryType == FactoryClass::factory_type::MILL)
		{
			((FactoryClass*)Obj)->Init_factory(FactoryClass::Flour, FactoryClass::Wheat, 500, own, FactoryType, salary, &City);
		}
		else if (FactoryType == FactoryClass::factory_type::MINE)
		{
			((FactoryClass*)Obj)->Init_factory(FactoryClass::Iron_ore, NULL, 5000, own, FactoryType, salary, &City);
		}
		else if (FactoryType == FactoryClass::factory_type::FORGER)
		{
			((FactoryClass*)Obj)->Init_factory(FactoryClass::Steel, FactoryClass::Iron_ore, 3000, own, FactoryType, salary, &City);
		}
		else if (FactoryType == FactoryClass::factory_type::WORKSHOP)
		{
			((FactoryClass*)Obj)->Init_factory(FactoryClass::cutlery, FactoryClass::Steel, 8000, own, FactoryType, salary, &City);
		}

		City.AddFactory((FactoryClass*)Obj, Point2L((LONG)Pos.x, (LONG)Pos.y));
		pathsys.AddBuilding((Building*)Obj);
	}

	Add_to_list_to_render(Obj);
	return true;
}

void SystemClass::LeftButtonPressed(D2D1_POINT_2U MousePos)
{
	int xPos = MousePos.x;
	int yPos = MousePos.y;
	RECT Screen;
	GetClientRect(m_hwnd, &Screen);

	if (m_Graphics->Mode.JobPrimary == m_Graphics->Mode.JOB_IDLE)
	{
		Object* output = QuadTree.GetObjectByCoords(Point2F((float)(-Center.x - ((Screen.right - Screen.left) * ((1000.0 / Scroll) - 1) / 2) + (xPos * 1000.0 / Scroll)),
			(float)(-Center.y - ((Screen.bottom - Screen.top) * ((1000.0 / Scroll) - 1) / 2) + (yPos * 1000.0 / Scroll))));
		if (output != 0)
		{
			if (output->GetTextureType() == OBJTYPES::UNIT)
			{
				for (int i = 0; i < PlayerOneSquadAmmount; i++)
				{
					if (PlayerOneSquads[i]->IsObjectMember((UnitClass*)output))
					{
						PlayerOneSquads[i]->Select(true);
						Selected = PlayerOneSquads[i];
						m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_SELECTED;
						m_Graphics->Mode.Unit = output->GetUnitType();
					}
				}
				if (output == Commander)
				{
					Commander->Selected = true;;
					Selected = Commander;
					m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_SELECTED;
					m_Graphics->Mode.Unit = output->GetUnitType();
				}
			}
			else if (output->GetUnitType() == OBJTYPES::COMMANDCENTER || output->GetUnitType() == OBJTYPES::FACTORY)
			{
				m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_SELECTED;
				m_Graphics->Mode.Unit = output->GetUnitType();
				m_Graphics->Mode.selected = output;
				Selected = output;
				output->Selected = true;
			}
		}
	}
	else if (m_Graphics->Mode.JobPrimary == m_Graphics->Mode.JOB_SELECTED && m_Graphics->GetGlowed() == 0)
 	{
		if (m_Graphics->Mode.Unit == OBJTYPES::ENGINEERS || m_Graphics->Mode.Unit == OBJTYPES::SOLDIER)
		{
			m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
			((SquadClass*)Selected)->Select(false);
		}
		else if (m_Graphics->Mode.Unit == OBJTYPES::COMMANDER)
		{
			m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
			((Object*)Selected)->Selected = false;
		}
		else if (m_Graphics->Mode.Unit == OBJTYPES::COMMANDCENTER || m_Graphics->Mode.Unit == OBJTYPES::FACTORY)
		{
			m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
			((Object*)Selected)->Selected = false;
		}
	}


	if (m_Graphics->GetGlowed() == OBJTYPES::COMMANDCENTER && Command == 0)
	{
		m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_PLACING;
		m_Graphics->Mode.Unit = OBJTYPES::COMMANDCENTER;
		m_Graphics->UpdateState(xPos, yPos, Scroll, Center);
	}
	else if (m_Graphics->GetGlowed() == OBJTYPES::SOLDIER)
	{
		CreateNewSquad(OBJTYPES::SOLDIER, Player_Fraction);
	}
	else if (m_Graphics->GetGlowed() == OBJTYPES::ENGINEERS)
	{
		CreateNewSquad(OBJTYPES::ENGINEERS, Player_Fraction);
	}
	else if (m_Graphics->Mode.JobPrimary == m_Graphics->Mode.JOB_PLACING)
	{
		double I = 1000.0 / Scroll;

		D2D1_POINT_2F Position;
		Position.x = (float)(xPos * I - Center.x - (Screen.right * (I - 1) / 2));
		Position.y = (float)(yPos * I - Center.y - (Screen.bottom * (I - 1) / 2));
		Object* Buffor;
		PlaceUnit(Position, Buffor, 1, OBJTYPES::COMMANDCENTER, Fractions::FIRSTARMY);
		Command = (ComCenter*)Buffor;
		m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_SELECTED;
		m_Graphics->Mode.Unit = OBJTYPES::ENGINEERS;
		((SquadClass*)Selected)->SetTargetToRepair(Command);
	}
}

void SystemClass::CreateNewSquad(UINT SquadType, UINT Fraction)
{
	SquadClass** Buffor = new SquadClass*[PlayerOneSquadAmmount + 1];
	for (int i = 0; i < PlayerOneSquadAmmount; i++)
	{
		Buffor[i] = PlayerOneSquads[i];
	}
	if (SquadType == OBJTYPES::ENGINEERS)
	{
		SquadClass* SquadBuf = 0;
		SquadBuf = new SquadClass;
		SquadBuf->initialize(8);
		for (int i = 0; i < 8; i++)
		{
			EngineerClass* UnitBuf = 0;
				Object* Buffer;
			PlaceUnit(Point2F((float)(1000 + (i * 50)), 100), Buffer, 1, SquadType, Fraction);
			UnitBuf = (EngineerClass*)Buffer;
			UnitBuf->SetMaxSpeed(200);
			UnitBuf->SetMaxAcceleration(500);
			SquadBuf->AddMember(UnitBuf);
		}
		Buffor[PlayerOneSquadAmmount] = SquadBuf;
	}
	else if (SquadType == OBJTYPES::SOLDIER)
	{
		SquadClass* SquadBuf = 0;
		SquadBuf = new SquadClass;
		SquadBuf->initialize(8);
		for (int i = 0; i < 8; i++)
		{
			EngineerClass* UnitBuf = 0;
			Object* Buffer;
			PlaceUnit(Point2F((float)(100 + (i * 50)), 100), Buffer, 1, SquadType, Fraction);
			UnitBuf = (EngineerClass*)Buffer;
			UnitBuf->SetMaxSpeed(200);
			UnitBuf->SetMaxAcceleration(500);
			SquadBuf->AddMember(UnitBuf);
		}
		Buffor[PlayerOneSquadAmmount] = SquadBuf;
		Buffor[PlayerOneSquadAmmount]->SetTargetToMove(((Object*)Selected)->GetPosition());
	}
	if (PlayerOneSquads)
	{
		delete[] PlayerOneSquads;
	}
	PlayerOneSquadAmmount++;
	PlayerOneSquads = Buffor;
}

void SystemClass::Add_to_list_to_render(Object * input)
{
	Object** buffor;
	buffor = new Object*[AmmountofObjectsToRender + 1];
	for (int i = 0; i < AmmountofObjectsToRender; i++)
	{
		buffor[i] = List_to_render[i];
	}
	buffor[AmmountofObjectsToRender] = input;
	if (AmmountofObjectsToRender)
		delete[] List_to_render;
	AmmountofObjectsToRender++;
	List_to_render = buffor;
}

void SystemClass::Remove_from_list_to_render(Object * input)
{
	Object** buffor = NULL;
	if (AmmountofObjectsToRender > 1)
		buffor = new Object*[AmmountofObjectsToRender - 1];
	int j = 0;
	for (int i = 0; i < AmmountofObjectsToRender; i++)
	{
		if (input == List_to_render[i])
			continue;
		buffor[j] = List_to_render[i];
		j++;
	}
	delete[] List_to_render;
	AmmountofObjectsToRender--;
	List_to_render = buffor;
}

void SystemClass::UpdateFamily()
{
	for (int i = 0; i < n_Employee; i++)
	{
		if (!Employee_list[i]->IsInFamily() && Employee_list[i]->IsAdult())
		{
			if (rand() % 100 < 20)
			{
				for (int j = 0; j < n_Employee; j++)
				{
					if (i == j)
						continue;
					if (!Employee_list[j]->IsInFamily() && Employee_list[j]->IsAdult())
					{
						AddFamily(Employee_list[j], Employee_list[i]);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < n_Family; i++)
	{
		if (m_Family[i]->Get_n_adults_members() > 2)
		{
			if (rand() % 100 < 80)
			{
				bool found = false;
				for (int j = 0; j < n_Employee; j++)
				{
					if (!Employee_list[j]->IsInFamily() && Employee_list[j]->IsAdult())
					{
						EmployeeClass* fam_memb = m_Family[i]->GetEmployeebyIndex(2);
						if (!fam_memb->IsAdult())
							break;
						m_Family[i]->RemoveMember(fam_memb);
						AddFamily(fam_memb, Employee_list[j]);
						found = true;
						break;
					}
				}

				if (!found)
					for (int j = 0; j < n_Family; j++)
					{
						if (i == j)
							continue;
						if (m_Family[j]->Get_n_adults_members() > 2)
						{
							EmployeeClass* fam_memb1 = m_Family[i]->GetEmployeebyIndex(2);
							if (!fam_memb1->IsAdult())
								break;
							EmployeeClass* fam_memb2 = m_Family[j]->GetEmployeebyIndex(2);
							if (!fam_memb2->IsAdult())
								continue;
							m_Family[i]->RemoveMember(fam_memb1);
							m_Family[j]->RemoveMember(fam_memb2);
							AddFamily(fam_memb1, fam_memb2);
							break;
						}
					}
			}
		}
	}

	for (int i = 0; i < n_Family; i++)
	{
		if (m_Family[i]->Get_n_adults_members() >= 2 && m_Family[i]->life_status > 3)
		{
			if (rand() % 100 < 10)
			{
				AddEmployee();
				m_Family[i]->AddMember(Employee_list[n_Employee - 1]);
				break;
			}
		}
	}
}

void SystemClass::AddFamily(EmployeeClass * one, EmployeeClass * two)
{
	bool found = false;;
	for (int i = 0; i < n_Family; i++)
	{
		if (m_Family[i]->Get_n_members() == 0)
		{
			m_Family[i]->AddMember(one);
			m_Family[i]->AddMember(two);
			found = true;
			break;
		}
	}
	if (found)
		return;

	FamilyClass** buffor = new FamilyClass*[n_Family + 1];
	for (int i = 0; i < n_Family; i++)
	{
		buffor[i] = m_Family[i];
	}
	buffor[n_Family] = new FamilyClass;
	buffor[n_Family]->AddMember(one);
	buffor[n_Family]->AddMember(two);
	m_Family = buffor;
	n_Family++;
}

void SystemClass::AddFactory(UINT type, EmployeeClass* owner, D2D1_POINT_2L index)
{
	Object* new_fact;

	switch (type)
	{

	case FactoryClass::BAKERY:
	{
		PlaceUnit(Point2F(), new_fact, 1, OBJTYPES::FACTORY, NULL, FactoryClass::factory_type::BAKERY, owner);
		((FactoryClass*)new_fact)->SetSuppler(&n_Mill, &Mill_list);
		break;
	}

	case FactoryClass::MILL:
	{
		PlaceUnit(Point2F(), new_fact, 1, OBJTYPES::FACTORY, NULL, FactoryClass::factory_type::MILL, owner);
		((FactoryClass*)new_fact)->SetSuppler(&n_Farm, &Farm_list);
		break;
	}

	case FactoryClass::FARM:
	{
		PlaceUnit(Point2F(), new_fact, 1, OBJTYPES::FACTORY, NULL, FactoryClass::factory_type::FARM, owner);
		break;
	}

	case FactoryClass::MINE:
	{
		PlaceUnit(Point2F(), new_fact, 1, OBJTYPES::FACTORY, NULL, FactoryClass::factory_type::MINE, owner);
		break;
	}

	case FactoryClass::FORGER:
	{
		PlaceUnit(Point2F(), new_fact, 1, OBJTYPES::FACTORY, NULL, FactoryClass::factory_type::FORGER, owner);
		((FactoryClass*)new_fact)->SetSuppler(&n_Mine, &Mine_list);
		break;
	}

	case FactoryClass::WORKSHOP:
	{
		PlaceUnit(Point2F(), new_fact, 1, OBJTYPES::FACTORY, NULL, FactoryClass::factory_type::WORKSHOP, owner);
		((FactoryClass*)new_fact)->SetSuppler(&n_Forger, &Forger_list);
		break;
	}

	}
	Add_Factory_to_sublist((FactoryClass*)new_fact);
}

void SystemClass::RemoveFactory(int i)
{
	if (m_Graphics->Mode.JobPrimary == m_Graphics->Mode.JOB_SELECTED)
	{
		if (m_Graphics->Mode.selected == City.GetFactory(i))
			m_Graphics->Mode.JobPrimary = m_Graphics->Mode.JOB_IDLE;
	}
	Remove_from_list_to_render(City.GetFactory(i));
	City.RemoveFactory(i);
}

float SystemClass::calc_cost_of_life()
{
	float output;
	float price_of_bread = -1;
	for (int i = 0; i < n_Shops; i++)
	{
		if (Shops_list[i]->GetOutputProductsinfo() == FactoryClass::Bread)
		{
			if (Shops_list[i]->GetPriceOfProducts() < price_of_bread || price_of_bread == -1)
			{
				price_of_bread = Shops_list[i]->GetPriceOfProducts();
			}
		}
	}
	output = price_of_bread * 0.6f;
	return output;
}


void SystemClass::Add_Factory_to_sublist(FactoryClass * input)
{
	UINT Type = input->GetFactoryType();
	int n_elements;
	int* add_element;
	FactoryClass*** list;
	if (Type == FactoryClass::BAKERY)
	{
		add_element = &n_Bakery;
		n_elements = n_Bakery + n_Workshop;
		list = &Shops_list;
	}
	else if (Type == FactoryClass::MILL)
	{
		add_element = &n_Mill;
		n_elements = *add_element;
		list = &Mill_list;
	}
	else if (Type == FactoryClass::FARM)
	{
		add_element = &n_Farm;
		n_elements = *add_element;
		list = &Farm_list;
	}
	else if (Type == FactoryClass::MINE)
	{
		add_element = &n_Mine;
		n_elements = *add_element;
		list = &Mine_list;
	}
	else if (Type == FactoryClass::FORGER)
	{
		add_element = &n_Forger;
		n_elements = *add_element;
		list = &Forger_list;
	}
	else if (Type == FactoryClass::WORKSHOP)
	{
		add_element = &n_Workshop;
		n_elements = n_Bakery + n_Workshop;
		list = &Shops_list;
	}
	FactoryClass** Buffor = new FactoryClass*[n_elements + 1];

	for (int i = 0; i < n_elements; i++)
	{
		Buffor[i] = (*list)[i];
	}
	Buffor[n_elements] = input;
	if (n_elements)
		delete[] *list;
	*list = Buffor;
	(*add_element)++;
	n_Shops = n_Workshop + n_Bakery;
}

void SystemClass::Remove_Factory_from_sublist(FactoryClass * input)
{
	UINT Type = input->GetFactoryType();
	int n_elements;
	int* add_element;
	FactoryClass*** list;
	if (Type == FactoryClass::BAKERY)
	{
		add_element = &n_Bakery;
		n_elements = n_Bakery + n_Workshop;
		list = &Shops_list;
	}
	else if (Type == FactoryClass::MILL)
	{
		add_element = &n_Mill;
		n_elements = *add_element;
		list = &Mill_list;
	}
	else if (Type == FactoryClass::FARM)
	{
		add_element = &n_Farm;
		n_elements = *add_element;
		list = &Farm_list;
	}
	else if (Type == FactoryClass::MINE)
	{
		add_element = &n_Mine;
		n_elements = *add_element;
		list = &Mine_list;
	}
	else if (Type == FactoryClass::FORGER)
	{
		add_element = &n_Forger;
		n_elements = *add_element;
		list = &Forger_list;
	}
	else if (Type == FactoryClass::WORKSHOP)
	{
		add_element = &n_Workshop;
		n_elements = n_Bakery + n_Workshop;
		list = &Shops_list;
	}

	FactoryClass** buffor = 0;
	int j = 0;
	if (n_elements > 1)
	{
		buffor = new FactoryClass*[n_elements - 1];
	}

	for (int k = 0; k < n_elements; k++)
	{
		if ((*list)[k] == input)
			continue;
		buffor[j] = (*list)[k];
		j++;
	}

	delete[] *list;
	*list = buffor;
	(*add_element)--;
	n_Shops = n_Workshop + n_Bakery;
}

void SystemClass::AddEmployee()
{
	EmployeeClass** buffor = new EmployeeClass*[n_Employee + 1];
	Object* newWorker;

	for (int i = 0; i < n_Employee; i++)
	{
		buffor[i] = Employee_list[i];
	}
	PlaceUnit(Point2F(), newWorker, 1, OBJTYPES::EMPLOYEE, NULL);
	buffor[n_Employee] = (EmployeeClass*)newWorker;
	buffor[n_Employee]->Init_employee();
	buffor[n_Employee]->SetMaxSpeed(200);
	buffor[n_Employee]->SetMaxAcceleration(500);
	if (n_Employee)
		delete[] Employee_list;
	Employee_list = buffor;
	Employee_list[n_Employee]->SetShops(&Shops_list, &n_Shops);
	n_Employee++;
}

void SystemClass::RemoveEmployee(int i)
{
	Remove_from_list_to_render(Employee_list[i]);
	EmployeeClass** buffor = new EmployeeClass*[n_Employee - 1];
	delete Employee_list[i];
	int j = 0;
	for (int x = 0; x < n_Employee; x++)
	{
		if (x == i)
			continue;
		buffor[j] = Employee_list[x];
		j++;
	}
	delete[] Employee_list;
	Employee_list = buffor;
	n_Employee--;
}

void SystemClass::RemoveEmployee(EmployeeClass* i)
{
	Remove_from_list_to_render(i);
	EmployeeClass** buffor = new EmployeeClass*[n_Employee - 1];
	delete i;
	int j = 0;
	for (int x = 0; x < n_Employee; x++)
	{
		if (Employee_list[x] == i)
			continue;
		buffor[j] = Employee_list[x];
		j++;
	}
	delete[] Employee_list;
	Employee_list = buffor;
	n_Employee--;
}

void SystemClass::Checkdeadpeople()
{
	if (!n_Family)
		return;

	for (int i = 0; i < n_Employee; i++)
	{
		if (Employee_list[i]->GetHungry() < -300)
		{
			if (!Employee_list[i]->IsInFamily())
			{
				int prized_family = rand() % n_Family;
				Employee_list[i]->GetWallet()->Give_money(Employee_list[i]->GetWallet()->GetMoney(), m_Family[prized_family]->GetWallet());
			}
			for (int j = 0; j < City.get_n_of_factores(); j++)
			{
				if (City.GetFactory(j)->GetOwner() == Employee_list[i])
				{
					Remove_Factory_from_sublist(City.GetFactory(j));
					RemoveFactory(j);
					j--;
				}
			}
			if (Employee_list[i]->IsInFamily())
			{
				FamilyClass* family_to_check = Employee_list[i]->GetFamily();
				Employee_list[i]->Dead();
				if (!family_to_check->Get_n_adults_members())
				{
					while (family_to_check->Get_n_members())
					{
						EmployeeClass* children_to_remove = family_to_check->GetEmployeebyIndex(0);
						children_to_remove->Dead();
						RemoveEmployee(children_to_remove);
					}
				}
			}
			else
				Employee_list[i]->Dead();
			for (int j = 0; j < n_Employee; j++)
				Employee_list[j]->remove_borrower(Employee_list[i]);
			RemoveEmployee(i);
			i--;
		}
	}
}

void SystemClass::MakeNewWorkplaces()
{
	for (int i = 0; i < n_Employee; i++)
	{
		int Rand = rand() % 100;
		if (Employee_list[i]->MonthsWithoutJob() >= 4 && Rand < 20)
		{
			FactoryClass* the_best_fact;
			float best_margin = 0;
			float low_salary = -1;

			if (!n_Bakery)
			{
				AddFactory(FactoryClass::BAKERY, Employee_list[i]);
				return;
			}
			else if (!n_Mill)
			{
				AddFactory(FactoryClass::MILL, Employee_list[i]);
				return;
			}
			else if (!n_Farm)
			{
				AddFactory(FactoryClass::FARM, Employee_list[i]);
				return;
			}
			else if (!n_Mine)
			{
				AddFactory(FactoryClass::MINE, Employee_list[i]);
				return;
			}
			else if (!n_Forger)
			{
				AddFactory(FactoryClass::FORGER, Employee_list[i]);
				return;
			}
			else if (!n_Workshop)
			{
				AddFactory(FactoryClass::WORKSHOP, Employee_list[i]);
				return;
			}

			for (int j = 0; j < City.get_n_of_factores(); j++)
			{
				if (City.GetFactory(j)->GetSalary() < low_salary || low_salary == -1)
					low_salary = City.GetFactory(j)->GetSalary();
				if (City.GetFactory(j)->GetMargin() > best_margin)
				{
					the_best_fact = City.GetFactory(j);
					best_margin = City.GetFactory(j)->GetMargin();

				}
			}

			AddFactory(the_best_fact->GetFactoryType(), Employee_list[i]);
			City.GetFactory(City.get_n_of_factores() - 1)->SetParameters(best_margin, low_salary);
			return;
		}
	}
}

void SystemClass::UpdateCity(int timer)
{
	for (int i = 0; i < n_Employee; i++)
		Employee_list[i]->Update(timer);

	for (int i = 0; i < City.get_n_of_factores(); i++)
		City.GetFactory(i)->Update(timer);
}

void SystemClass::EndOfMonth()
{
	float smallest_fee = 0;

	for (int i = 0; i < City.get_n_of_factores(); i++)
		if(smallest_fee < City.GetFactory(i)->GetSalary())
			smallest_fee = City.GetFactory(i)->GetSalary();

	for (int i = 0; i < n_Employee; i++)
		Employee_list[i]->EndOfMonth();

	for (int i = 0; i < City.get_n_of_factores(); i++)
		City.GetFactory(i)->EndOfMonth(smallest_fee);

	for (int i = 0; i < n_Family; i++)
		m_Family[i]->End_of_month();

	cost_of_life = calc_cost_of_life();
	UpdateFamily();
	Checkdeadpeople();
	MakeNewWorkplaces();
	lookForJob();
}

void SystemClass::lookForJob()
{
	FactoryClass** neededEmployee = new FactoryClass*[City.get_n_of_factores()];
	int num_of_needed = 0;

	for (int i = 0; i < City.get_n_of_factores(); i++)
	{
		if (City.GetFactory(i)->IsLookingForEmployee())// && !City.GetFactory(i)->GetOwner()->IsInDebt())
		{
			neededEmployee[num_of_needed] = City.GetFactory(i);
			num_of_needed++;
		}
	}
	if (num_of_needed)
		for (int i = 0; i < n_Employee; i++)
		{
			Employee_list[i]->LookForJob(neededEmployee, num_of_needed, cost_of_life);
		}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
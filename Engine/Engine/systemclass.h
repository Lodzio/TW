////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <Windowsx.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "timerclass.h"
#include "graphicsclass.h"
#include "Squad.h"
#include "Family.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	bool PlaceUnit(D2D1_POINT_2F Pos, Object* &Obj, int index, UINT unitType, UINT Fraction, UINT FactoryType = NULL, EmployeeClass* own = 0);
	void LeftButtonPressed(D2D1_POINT_2U MousePos);
	void CreateNewSquad(UINT SquadType, UINT Fraction);

private:
	D2D1_POINT_2F MouseClickPos;
	bool RightMouseButtonPressed;
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	TimerClass* m_Timer;
	Object** List_to_render;
	void Add_to_list_to_render(Object* input);
	void Remove_from_list_to_render(Object* input);

	D2D1_POINT_2L CityCenter;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	ComCenter* Command;
	CommanderClass* Commander;
	UnitClass* Soldiers[8];
	QuadTreeClass QuadTree;
	void* Selected;
	SquadClass** PlayerOneSquads;
	int PlayerOneSquadAmmount;
	int AmmountofObjectsToRender;
	UINT Player_Fraction;
	SparksClass* SparkList; //0 - DustShoot, 1 - Blood
	unsigned int daily_timer;
	unsigned short int monthly_timer;
	Cityclass City;
	float cost_of_life;

	FamilyClass** m_Family;
	int n_Family;
	void UpdateFamily(); //calculate new partners
	void AddFamily(EmployeeClass* one, EmployeeClass* two);

	int Scroll;
	POINT Center;
	D2D1_POINT_2U LastMousePos;
	void AddFactory(UINT type, EmployeeClass* owner, D2D1_POINT_2L index = Point2L());
	void RemoveFactory(int i);
	float calc_cost_of_life();

	void Add_Factory_to_sublist(FactoryClass* input);
	void Remove_Factory_from_sublist(FactoryClass* input);

	FactoryClass** Shops_list;
	int n_Shops;

	int n_Bakery;

	FactoryClass** Mill_list;
	int n_Mill;

	FactoryClass** Farm_list;
	int n_Farm;

	FactoryClass** Mine_list;
	int n_Mine;

	FactoryClass** Forger_list;
	int n_Forger;

	int n_Workshop;

	EmployeeClass** Employee_list;
	int n_Employee;
	void AddEmployee();
	void RemoveEmployee(int i);
	void RemoveEmployee(EmployeeClass* i);
	void Checkdeadpeople();
	void MakeNewWorkplaces();

	void UpdateCity(int timer);
	void EndOfMonth();
	void lookForJob();
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif
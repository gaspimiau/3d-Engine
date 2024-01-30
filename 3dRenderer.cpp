
#include "App.h"
int CALLBACK WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nShowCmd)
{
	try 
	{
		return App{}.Go();
	}
	catch (const MiExcepcion& e)
	{
		MessageBox(nullptr,e.what(),e.GetType(),MB_OK|MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr,e.what(),"Excepcion Estandar",MB_OK|MB_ICONEXCLAMATION);
	}
	catch(...)
	{
		MessageBox(nullptr,"Sin Detalles","Excepcion Desconocida",MB_OK|MB_ICONEXCLAMATION);
	}
	return -1;
}






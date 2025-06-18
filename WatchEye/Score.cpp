

#include "Score.h"


//פונקציות עבור כל קלאס של תנוחות שזוהה  

std::vector<std::function<void()>> my_void_functions;

//קלאס 1 : OpenMouse פה פתוח
void OpenMouse() 
{
	if(Persons[std::this_thread::get_id()].class1 == false)
	{
		Persons[std::this_thread::get_id()].class1 = true;
		Persons[std::this_thread::get_id()].precent += 10; //הוספת 10% לניקוד של האדם
	}
}

//קלאס 2 : CloseMouse פה סגור
void CloseMouse()
{
	if (Persons[std::this_thread::get_id()].class2 == false)
	{
		Persons[std::this_thread::get_id()].class2 = true;
		Persons[std::this_thread::get_id()].precent += 5; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 3 : OpenEyes עינים פתוחות
void OpenEyes()
{
	if (Persons[std::this_thread::get_id()].class3 == false) 
	{
		Persons[std::this_thread::get_id()].class3 = true;
		Persons[std::this_thread::get_id()].precent += 10; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 4 : CloseEyes עינים סגורות
void CloseEyes()
{
	if (Persons[std::this_thread::get_id()].class4 = false) 
	{
		Persons[std::this_thread::get_id()].class4 = true;
		Persons[std::this_thread::get_id()].precent += 10; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 5 : HairOnFace שיער על הפנים
void HairOnFace()
{
	if (Persons[std::this_thread::get_id()].class5 == false) 
	{
		Persons[std::this_thread::get_id()].class5 = true;
		Persons[std::this_thread::get_id()].precent += 20; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 6 : HeadTitleBack ראש מוטה לאחור
void HeadTitleBack()
{
	if (Persons[std::this_thread::get_id()].class6 == false) 
	{
		Persons[std::this_thread::get_id()].class6 = true;
		Persons[std::this_thread::get_id()].precent += 15; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 7 : HairBack שיער מאחורה
void HairBack()
{
	if (Persons[std::this_thread::get_id()].class7 == false) 
	{
		Persons[std::this_thread::get_id()].class7 = true;
		Persons[std::this_thread::get_id()].precent += 5; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 8 :HairOnEyesשיער על העינים
void HairOnEyes()
{
	if (Persons[std::this_thread::get_id()].class8 == false) 
	{
		Persons[std::this_thread::get_id()].class8 = true;
		Persons[std::this_thread::get_id()].precent += 5; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 9 : HandInTheSide ידים בצדדים
void HandInTheSide()
{
	if (Persons[std::this_thread::get_id()].class9 == false) 
	{
		Persons[std::this_thread::get_id()].class9 = true;
		Persons[std::this_thread::get_id()].precent += 5; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 10 : HandOpen ידים פתוחות
void HandOpen()
{
	if (Persons[std::this_thread::get_id()].class10 == false) 
	{
		Persons[std::this_thread::get_id()].class10 = true;
		Persons[std::this_thread::get_id()].precent += 10; //הוספת 10% לניקוד של האדם
	}
}
//קלאס 11 : waterOTheFace מים על הפנים
void waterOTheFace()
{
	if (Persons[std::this_thread::get_id()].class11 == false) 
	{
		Persons[std::this_thread::get_id()].class11 = true;
		Persons[std::this_thread::get_id()].precent += 5; //הוספת 10% לניקוד של האדם
	}
}
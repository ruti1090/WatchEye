

#include "Score.h"


//�������� ���� �� ���� �� ������ �����  

std::vector<std::function<void()>> my_void_functions;

//���� 1 : OpenMouse �� ����
void OpenMouse() 
{
	if(Persons[std::this_thread::get_id()].class1 == false)
	{
		Persons[std::this_thread::get_id()].class1 = true;
		Persons[std::this_thread::get_id()].precent += 10; //����� 10% ������ �� ����
	}
}

//���� 2 : CloseMouse �� ����
void CloseMouse()
{
	if (Persons[std::this_thread::get_id()].class2 == false)
	{
		Persons[std::this_thread::get_id()].class2 = true;
		Persons[std::this_thread::get_id()].precent += 5; //����� 10% ������ �� ����
	}
}
//���� 3 : OpenEyes ����� ������
void OpenEyes()
{
	if (Persons[std::this_thread::get_id()].class3 == false) 
	{
		Persons[std::this_thread::get_id()].class3 = true;
		Persons[std::this_thread::get_id()].precent += 10; //����� 10% ������ �� ����
	}
}
//���� 4 : CloseEyes ����� ������
void CloseEyes()
{
	if (Persons[std::this_thread::get_id()].class4 = false) 
	{
		Persons[std::this_thread::get_id()].class4 = true;
		Persons[std::this_thread::get_id()].precent += 10; //����� 10% ������ �� ����
	}
}
//���� 5 : HairOnFace ���� �� �����
void HairOnFace()
{
	if (Persons[std::this_thread::get_id()].class5 == false) 
	{
		Persons[std::this_thread::get_id()].class5 = true;
		Persons[std::this_thread::get_id()].precent += 20; //����� 10% ������ �� ����
	}
}
//���� 6 : HeadTitleBack ��� ���� �����
void HeadTitleBack()
{
	if (Persons[std::this_thread::get_id()].class6 == false) 
	{
		Persons[std::this_thread::get_id()].class6 = true;
		Persons[std::this_thread::get_id()].precent += 15; //����� 10% ������ �� ����
	}
}
//���� 7 : HairBack ���� ������
void HairBack()
{
	if (Persons[std::this_thread::get_id()].class7 == false) 
	{
		Persons[std::this_thread::get_id()].class7 = true;
		Persons[std::this_thread::get_id()].precent += 5; //����� 10% ������ �� ����
	}
}
//���� 8 :HairOnEyes���� �� ������
void HairOnEyes()
{
	if (Persons[std::this_thread::get_id()].class8 == false) 
	{
		Persons[std::this_thread::get_id()].class8 = true;
		Persons[std::this_thread::get_id()].precent += 5; //����� 10% ������ �� ����
	}
}
//���� 9 : HandInTheSide ���� ������
void HandInTheSide()
{
	if (Persons[std::this_thread::get_id()].class9 == false) 
	{
		Persons[std::this_thread::get_id()].class9 = true;
		Persons[std::this_thread::get_id()].precent += 5; //����� 10% ������ �� ����
	}
}
//���� 10 : HandOpen ���� ������
void HandOpen()
{
	if (Persons[std::this_thread::get_id()].class10 == false) 
	{
		Persons[std::this_thread::get_id()].class10 = true;
		Persons[std::this_thread::get_id()].precent += 10; //����� 10% ������ �� ����
	}
}
//���� 11 : waterOTheFace ��� �� �����
void waterOTheFace()
{
	if (Persons[std::this_thread::get_id()].class11 == false) 
	{
		Persons[std::this_thread::get_id()].class11 = true;
		Persons[std::this_thread::get_id()].precent += 5; //����� 10% ������ �� ����
	}
}
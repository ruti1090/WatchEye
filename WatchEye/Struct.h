
#ifndef STRUCT_H // �� ������ RECP_H �� ����� �����
#define STRUCT_H // ���� ���� �����

#include <atomic>   // For std::atomic (if needed for concurrency)

 
//���� �� ����� 
struct location
{
	double x;
	double y;
};

struct recP
{
	location location;
	int precent =0;
	int timer =0;
	bool class1 =false;
	bool class2 =false;
	bool class3 = false;
	bool class4 = false;
	bool class5 = false;
	bool class6 = false;
	bool class7 = false;
	bool class8 = false;
	bool class9 = false;
	bool class10 = false;
	bool class11 = false;
};

#endif // STRUCT_H
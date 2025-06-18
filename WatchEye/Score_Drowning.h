#pragma once
#ifndef SCORE_DROWNING_H
#define SCORE_DROWNING_H

#include <map>
#include <thread>    // For std::thread
#include <iostream>   // ���� std::cout
#include <vector>     // ���� std::vector
#include <functional> // ���� std::function (��� ���� ���!)
#include <string>     // ���� ����� �������� ��������� ������
#include <Windows.h> 

#include "Struct.h"


// ������� ������� ����� ����� (���) ���� ����� �����
void TriggerBeepAlert();

extern std::map<int, recP>Persons;
using namespace std;
class Score_Drowning
{
public:
	vector<function<void(int)>>functions_hash;
    Score_Drowning(); // �����������

    // �������� ���� ������ ��� �� �-thread ID ������
    void OpenMouse(int thread_id_for_person);
    void CloseMouse(int thread_id_for_person);
    void OpenEyes(int thread_id_for_person);
    void CloseEyes(int thread_id_for_person);
    void HairOnFace(int thread_id_for_person);
    void HeadTitleBack(int thread_id_for_person);
    void HairBack(int thread_id_for_person);
    void HairOnEyes(int thread_id_for_person);
    void HandInTheSide(int thread_id_for_person);
    void HandOpen(int thread_id_for_person);
    void waterOTheFace(int thread_id_for_person);

    // �������� PlayHash ������ ��� ���� �������
    void PlayHash(int class_id, int thread_id_for_person);


};

#endif
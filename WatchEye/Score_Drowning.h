#pragma once
#ifndef SCORE_DROWNING_H
#define SCORE_DROWNING_H

#include <map>
#include <thread>    // For std::thread
#include <iostream>   // עבור std::cout
#include <vector>     // עבור std::vector
#include <functional> // עבור std::function (הכי חשוב כאן!)
#include <string>     // עבור שימוש במחרוזות בפונקציות הדוגמה
#include <Windows.h> 

#include "Struct.h"


// פונקציה שמפעילה התראה קולית (ביפ) כאשר מתגלה טביעה
void TriggerBeepAlert();

extern std::map<int, recP>Persons;
using namespace std;
class Score_Drowning
{
public:
	vector<function<void(int)>>functions_hash;
    Score_Drowning(); // הקונסטרקטור

    // פונקציות העזר מקבלות כעת את ה-thread ID כפרמטר
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

    // פונקציית PlayHash משתמשת כעת בשני פרמטרים
    void PlayHash(int class_id, int thread_id_for_person);


};

#endif
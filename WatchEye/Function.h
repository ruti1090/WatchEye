#pragma once
#ifndef FUNCTION_H // חשוב לוודא שזה שם ייחודי לקובץ
#define FUNCTION_H
#include <iostream> // ל-std::cout ו-std::cin
#include <fstream>  // לטיפול בקבצים (ifstream)
#include <string>   // לטיפול במחרוזות (std::string)
#include <map>
#include <vector>   // ל-std::vector
#include <thread>   // ל-std::thread
#include <chrono>
#include <cstdlib>  // לצורך system()
#include <Windows.h> // לצורך SetConsoleOutputCP, SetConsoleCP (לטיפול בקידוד עברית)
#include <Python.h>
#include <filesystem>
#include <sstream>   // לצורך std::ostringstream (להמרת thread ID למחרוזת)
#include <cstdio> // נדרש עבור _popen ו- _pclose
#include <regex>      // עבור חילוץ ID משם התיקייה
#include <stdexcept>  // עבור invalid_argument
#include <algorithm> //עבור המיון 

#include "Struct.h"
using namespace std;
namespace fs = std::filesystem; // קיצור עבור std::filesystem

//פונקציה שאחראית לעקוב אחרי השטחים המתים ולבדוק אם נכנס מישהו חדש
bool cheakNewPerson();

//הכרזה על המילון ממקום שונה
extern std::map<int,recP> Persons;
// הצהרה על המשתנים הגלובליים לשליטה בתהליכון
extern std::thread* g_timer_thread_ptr;
extern bool g_run_timer_thread_flag;

//הרצת אנשים ושטחים מתים עבור בדיקה של שינוי או כניסה של מישהו חדש לבריכה
string HelpToCheakNewPerson(string imagePath);

int countLinesInFile(const std::string& filename); //ספירת שורות בקובץ טקסט	

//פונקציה שמריצה את הפונקציה הראשית בפייתון
int runPythonInThread(std::thread::id this_thread_id, std::vector<float>valuesForThread);


//המרה של וקטור מC++ להתאמה לפייתון 
PyObject* convert_vector_to_pylist(const std::vector<float>& valuesForThread);

//פונקציה שמקבלת נתיב לתמונה ומחלקת את התמונה לכל התיקיות של התהליכונים 
void DistributeImageToThreads(string imagePath, string baseFolderPath);

//עוברת על כל התיקיות של התהליכונים קוראת את התוצאות זיהוי מהיולו עבור כל תהליכון ואז מוחקת את הקובץ של הזיהוי
void Drowning_score(std::string folderPath);

//פונקציה מחיקת איבר מהמערך
void Deleting_member(map<int, recP>& person, int thread_id); 

//ניקוי טיימרים
void CleanPoint();

void timer_loop_function(); // פונקציה שתכיל את הלולאה וההשהיה
void StartTimerControl();   // פונקציה שתפעיל את התהליכון
void StopTimerControl();    // פונקציה שתעצור את התהליכון


// פונקציה שמריצה את הפונקציה הראשית בפייתון ניסוייייי
int try_to_run_yolo(thread::id this_thread_id, vector<float>valuesForThread);

//פונקציה לחלוקת הזיהוי הראשון תמונה+ טקסט של התוצאות לכל התהליכונים ולתיקיה של הבדיקה אם נכנס מישהו חדש לבריכה
int givePicInStart(string imagePath,string txtPath , string baseFolderPath);



#endif
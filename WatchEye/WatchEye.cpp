// WatchEye.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>    // For std::vector (dynamic array)
#include <atomic>    // For std::atomic (if truly needed for concurrency)
#include <iostream>  // For std::cout, std::cerr, std::cin
#include <fstream>   // For std::ifstream
#include <string>    // For std::string, std::getline
#include <vector>    // For std::vector
#include <sstream>   // For std::istringstream (to parse numbers from a string)
#include <thread>    // For std::thread
#include <chrono>    // For std::chrono::milliseconds, std::this_thread::sleep_for (optional, for demo)
#include <numeric>   // For std::accumulate (optional, for demo in thread function)
#include <cstdlib> // ל-system()
#include <Windows.h>
#include <Python.h>
#include <map>

#include "Function.h"
#include "Struct.h"
#include "Score.h"
#include "Score_Drowning.h"

using namespace std;


//הגדרת הטבלת גיבוב כשהמפתח הוא מזהה התהליכון והערך הוא מבנה של אדם שזוהה
std::map<int, recP> Persons;
bool g_run_timer_thread_flag;

// פונקציה להתחלת תהליכון הטיימר
void StartTimerControl()
{
	cout << "timer play\n";
	while (g_run_timer_thread_flag)
	{
		for (auto& person : Persons)
		{
			cout << "Person"<<person.first<< endl;
			if (person.second.timer < 10) {
				person.second.timer++;
			}
			else {
				person.second.timer = 0;
				person.second.precent = 0;
				cout << "Person with thread ID " << person.first << " has been reset.\n";
			}
			Sleep(1000);
		}
	}
}

int main()
{
	const wchar_t* pythonHome = L"C:\\Users\\USER\\AppData\\Local\\Programs\\Python\\Python310";
	Py_SetPythonHome(pythonHome);
	Py_Initialize();
	if (!Py_IsInitialized()) {
		std::wcout << L"Error: Failed to initialize Python interpreter!" << std::endl;
		return 1;
	}
	SetConsoleOutputCP(1255);
	SetConsoleCP(1255);
	//קבלת גדול הבריכה מהמשתמש וחישוב מקימום האנשים שיכולים להיות באותו זמן בבריכה
	int height = 0, width = 0;
	cout << "enter height of Pool\n";
	cin >> height;
	cout << "enter width of Pool\n";
	cin >> width;
	int area = height * width;
	int MaxPeople = area / 90;
	cout << "The number of people that can fit in the pool is: " << MaxPeople << " people.\n";


	string path_to_bat_file = "C:\\Users\\USER\\Desktop\\PROJECT\\yoloperson\\CMD.bat";
	int result = system(path_to_bat_file.c_str());
	if (result == 0) {
		std::cout << "קובץ ה-BAT סיים לרוץ בהצלחה." << std::endl;
	}
	else {
		std::cerr << "שגיאה בהרצת קובץ ה-BAT. קוד חזרה: " << result << std::endl;
	}
	string fileOfResualt= "C:\\Users\\USER\\Desktop\\PROJECT\\firstrec\\predict\\labels\\frame_1.txt";

	/*remove("C:/Users/USER/Desktop/PROJECT/PicFromPool/frame_1.jpg");*/

	//הרצת פונקציה לספירת מס' האנשים שיש עכשיו בבריכה 
	int numPeople = countLinesInFile(fileOfResualt);

	cout << "number of people now is: " << numPeople << endl;



	//הצמדת תהליכון לכל אדם שזוהה
	ifstream inputFile(fileOfResualt);
	if (!inputFile.is_open()) {
		std::cerr << "Error: Could not open the folder '" << fileOfResualt << "'." << std::endl;
		return 1;
	}
	string line;
	vector<std::thread> threads;
	while (std::getline(inputFile, line))
	{
		float tempValue;
		std::vector<float> valuesForThread;
		std::istringstream iss(line);
		for (int i = 0; i < 5; ++i)
		{
			if (i == 0) {}
			else
			{
				if (iss >> tempValue)
				{
					valuesForThread.push_back(tempValue);
				}
				else
				{
					std::cerr << "Warning: Skipping line due to malformed data or insufficient values (expected 5): '" << line << "'\n";
					valuesForThread.clear();
					break;
				}
			}
		}
		threads.emplace_back(try_to_run_yolo, this_thread::get_id(), move(valuesForThread));
	}
	givePicInStart("C:\\Users\\USER\\Desktop\\PROJECT\\PicFromPool\\frame_1.jpg", "C:\\Users\\USER\\Desktop\\PROJECT\\firstrec\\predict\\labels\\frame_1.txt", "C:\\Users\\USER\\Desktop\\PROJECT\\Threads");

	inputFile.close(); // סגירת הקובץ
	Py_Initialize();

	if (!Py_IsInitialized()) {
		std::cerr << "Failed to initialize Python interpreter!" << std::endl;
		return 1;
	}
	std::cout << "Python interpreter initialized successfully." << std::endl;

	g_run_timer_thread_flag = true;

	//הפעלת התהליכון לניהול הטיימרים
	thread timerThread(StartTimerControl);

	//בדיקה האם נכנס מישהו חדש ריצה ברקע כל הזמן 
	thread cheakNewPersonThread(cheakNewPerson);

	//הגדרת הנתיב לתוצאות יולו - זיהוי תנוחות
	string folderThreads = "C:/Users/USER/Desktop/PROJECT/Threads";

	//הפעלה ברקע של הפונקצית ניקוד טביעה עבור כל אדם
	thread scorethread(Drowning_score, folderThreads);


	string floderPicFromPool = "C:/Users/USER/Desktop/PROJECT/PicFromPool";

	for (const auto& frame : filesystem::directory_iterator(floderPicFromPool))
	{
		if (filesystem::is_regular_file(frame.path()) && frame.path().extension() == ".jpg")
		{
			DistributeImageToThreads(frame.path().string(), folderThreads);
		}
		else
		{
			std::cout << "No more images to process in the pool." << std::endl;
			break;
		}
		Sleep(500);
	}

	// המתנה שכל התהליכונים יסיימו את עבודתם
	std::cout << "\nMain thread: Waiting for all worker threads to finish...\n";
	for (thread& t : threads)
	{
		t.join(); // חכה שהתהליכון יסיים
	}
	std::cout << "All threads have finished their work. Program exiting.\n";

	g_run_timer_thread_flag = false;
	timerThread.join();

	//לנתק את פונקציית Drowning_score
	scorethread.join();


	//עצירת התהליכון שבודק אם נכנס מישהו חדש 
	cheakNewPersonThread.join();
	Py_Finalize();
	std::cout << "Python interpreter finalized." << std::endl;
	return 0;
}


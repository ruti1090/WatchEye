#pragma once
#ifndef FUNCTION_H // ���� ����� ��� �� ������ �����
#define FUNCTION_H
#include <iostream> // �-std::cout �-std::cin
#include <fstream>  // ������ ������ (ifstream)
#include <string>   // ������ �������� (std::string)
#include <map>
#include <vector>   // �-std::vector
#include <thread>   // �-std::thread
#include <chrono>
#include <cstdlib>  // ����� system()
#include <Windows.h> // ����� SetConsoleOutputCP, SetConsoleCP (������ ������ �����)
#include <Python.h>
#include <filesystem>
#include <sstream>   // ����� std::ostringstream (����� thread ID �������)
#include <cstdio> // ���� ���� _popen �- _pclose
#include <regex>      // ���� ����� ID ��� �������
#include <stdexcept>  // ���� invalid_argument
#include <algorithm> //���� ����� 

#include "Struct.h"
using namespace std;
namespace fs = std::filesystem; // ����� ���� std::filesystem

//������� ������� ����� ���� ������ ����� ������ �� ���� ����� ���
bool cheakNewPerson();

//����� �� ������ ����� ����
extern std::map<int,recP> Persons;
// ����� �� ������� ��������� ������ ��������
extern std::thread* g_timer_thread_ptr;
extern bool g_run_timer_thread_flag;

//���� ����� ������ ���� ���� ����� �� ����� �� ����� �� ����� ��� ������
string HelpToCheakNewPerson(string imagePath);

int countLinesInFile(const std::string& filename); //����� ����� ����� ����	

//������� ������ �� �������� ������ �������
int runPythonInThread(std::thread::id this_thread_id, std::vector<float>valuesForThread);


//���� �� ����� �C++ ������ ������� 
PyObject* convert_vector_to_pylist(const std::vector<float>& valuesForThread);

//������� ������ ���� ������ ������ �� ������ ��� ������� �� ���������� 
void DistributeImageToThreads(string imagePath, string baseFolderPath);

//����� �� �� ������� �� ���������� ����� �� ������� ����� ������ ���� �� ������� ��� ����� �� ����� �� ������
void Drowning_score(std::string folderPath);

//������� ����� ���� ������
void Deleting_member(map<int, recP>& person, int thread_id); 

//����� �������
void CleanPoint();

void timer_loop_function(); // ������� ����� �� ������ �������
void StartTimerControl();   // ������� ������ �� ��������
void StopTimerControl();    // ������� ������ �� ��������


// ������� ������ �� �������� ������ ������� ���������
int try_to_run_yolo(thread::id this_thread_id, vector<float>valuesForThread);

//������� ������ ������ ������ �����+ ���� �� ������� ��� ���������� ������� �� ������ �� ���� ����� ��� ������
int givePicInStart(string imagePath,string txtPath , string baseFolderPath);



#endif
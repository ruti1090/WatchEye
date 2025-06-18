#include "Score_Drowning.h"
#include <chrono> // עבור std::this_thread::sleep_for בדוגמת ה-main אם תחבר

Score_Drowning::Score_Drowning()
{
    // כל למדא מקבלת כעת פרמטר int (ה-thread_id_for_person)
    // ומעבירה אותו לפונקציית החבר המתאימה.
    functions_hash.push_back([this](int thread_id_for_person) { this->OpenMouse(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->CloseMouse(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->OpenEyes(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->CloseEyes(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->HairOnFace(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->HeadTitleBack(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->HairBack(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->HairOnEyes(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->HandInTheSide(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->HandOpen(thread_id_for_person); });
    functions_hash.push_back([this](int thread_id_for_person) { this->waterOTheFace(thread_id_for_person); });
}

void TriggerBeepAlert() {
    Beep(750, 1000); // 750 Hz למשך 1000 אלפיות השנייה (שנייה אחת)
}

void Score_Drowning::OpenMouse(int person_logical_id)
{
    if (Persons[person_logical_id].class1 == false)
    {
        Persons[person_logical_id].class1 = true;
        Persons[person_logical_id].precent += 10;
		if (Persons[person_logical_id].precent > 70)
		{
			TriggerBeepAlert(); 
		}
    }
}

void Score_Drowning::CloseMouse(int person_logical_id)
{
    if (Persons[person_logical_id].class2 == false)
    {
        Persons[person_logical_id].class2 = true;
        Persons[person_logical_id].precent += 5;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::OpenEyes(int person_logical_id)
{
    if (Persons[person_logical_id].class3 == false)
    {
        Persons[person_logical_id].class3 = true;
        Persons[person_logical_id].precent += 10;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::CloseEyes(int person_logical_id)
{
    if (Persons[person_logical_id].class4 == false)
    {
        Persons[person_logical_id].class4 = true;
        Persons[person_logical_id].precent += 10;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::HairOnFace(int person_logical_id)
{
    if (Persons[person_logical_id].class5 == false)
    {
        Persons[person_logical_id].class5 = true;
        Persons[person_logical_id].precent += 20;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::HeadTitleBack(int person_logical_id)
{
    if (Persons[person_logical_id].class6 == false)
    {
        Persons[person_logical_id].class6 = true;
        Persons[person_logical_id].precent += 15;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::HairBack(int person_logical_id)
{
    if (Persons[person_logical_id].class7 == false)
    {
        Persons[person_logical_id].class7 = true;
        Persons[person_logical_id].precent += 5;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::HairOnEyes(int person_logical_id)
{
    if (Persons[person_logical_id].class8 == false)
    {
        Persons[person_logical_id].class8 = true;
        Persons[person_logical_id].precent += 5;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::HandInTheSide(int person_logical_id)
{
    if (Persons[person_logical_id].class9 == false)
    {
        Persons[person_logical_id].class9 = true;
        Persons[person_logical_id].precent += 5;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::HandOpen(int person_logical_id)
{
    if (Persons[person_logical_id].class10 == false)
    {
        Persons[person_logical_id].class10 = true;
        Persons[person_logical_id].precent += 10;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

void Score_Drowning::waterOTheFace(int person_logical_id)
{
    if (Persons[person_logical_id].class11 == false)
    {
        Persons[person_logical_id].class11 = true;
        Persons[person_logical_id].precent += 5;
        if (Persons[person_logical_id].precent > 70)
        {
            TriggerBeepAlert();
        }
    }
}

// הפונקציה PlayHash כעת מפעילה את הלמדא עם ה-ID של האדם
void Score_Drowning::PlayHash(int class_id, int person_logical_id)
{
    if (class_id >= 0 && class_id < functions_hash.size())
    {
        // קורא ללמדא המאוחסנת בווקטור ומעביר לה את ה-person_logical_id
        functions_hash[class_id](person_logical_id);
    }
    else
    {
        std::cerr << "Error: class_id " << class_id << " out of range for functions_hash." << std::endl;
    }
}
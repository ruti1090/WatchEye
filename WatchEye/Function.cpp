
#include "Function.h"
#include "Struct.h"
#include "Score.h"
#include "Score_Drowning.h"


Score_Drowning scoreDrowning;


using namespace std;
using namespace std::filesystem; // כדי להשתמש ב-std::filesystem בקלות


// פונקציה לספירת השורות בקובץ טקסט
int countLinesInFile(const std::string& filename) {
	// יצירת אובייקט ifstream לפתיחת הקובץ לקריאה
	std::ifstream file(filename);

	// בדיקה אם הקובץ נפתח בהצלחה
	if (!file.is_open()) {
		std::cerr << "Error: Could not open the file '" << filename << "'." << std::endl;
		return -1; // החזרת -1 כדי לציין שגיאה
	}

	int lineCount = 0;
	std::string line;

	// קריאת הקובץ שורה אחר שורה
	// getline קורא שורה אחת בכל פעם, כולל רווחים, עד תו מעבר שורה ('\n')
	// הלולאה ממשיכה כל עוד יש שורות לקרוא מהקובץ
	while (std::getline(file, line)) {
		lineCount++;
	}

	// סגירת הקובץ
	// האובייקט std::ifstream סוגר את הקובץ אוטומטית כשהוא יוצא מהסקופ,
	// אך קריאה מפורשת ל-close() היא פרקטיקה טובה.
	file.close();

	return lineCount;
}

//המרה של וקטור מC++ להתאמה לפייתון 
PyObject* convert_vector_to_pylist(const std::vector<float>& valuesForThread)
{
	PyObject* pList = PyList_New(valuesForThread.size());
	if (pList == NULL) {
		PyErr_SetString(PyExc_RuntimeError, "Failed to create Python list.");
		return NULL;
	}
	for (size_t i = 0; i < valuesForThread.size(); ++i) {
		PyObject* pFloat = PyFloat_FromDouble(valuesForThread[i]);
		if (pFloat == NULL) {
			Py_DECREF(pList);
			PyErr_SetString(PyExc_RuntimeError, "Failed to convert double to Python float.");
			return NULL;
		}
		PyList_SetItem(pList, i, pFloat);
	}
	return pList;
}


//פונקציה שמריצה את הפונקציה הראשית בפייתון
//int runPythonInThread(std::thread::id this_thread_id, std::vector<float>valuesForThread)
//{
//	//double X_center = valuesForThread[0];
//	//double Y_center = valuesForThread[1];
//	//double height = valuesForThread[2];
//	//double width = valuesForThread[3];
//	cout << "SSSSSSSSSSSSS" << endl;
//	//PyGILState_STATE gstate; // הצהרה על משתנה מצב ה-GIL
//	//gstate = PyGILState_Ensure();
//
//	//יצירת תיקיה עבור התהליכון הנוכחי
//	std::string base_results_path = "C:/Users/USER/Desktop/PROJECT/Threads";
//	std::ostringstream oss;
//	oss << this_thread_id;
//	std::string thread_id_str = oss.str();
//	std::string thread_folder_name = "thread_data_" + thread_id_str;
//	std::filesystem::path full_thread_folder_path = std::filesystem::path(base_results_path) / thread_folder_name;
//
//	try {
//		if (!std::filesystem::create_directory(full_thread_folder_path)) {
//			std::cerr << "Warning: Directory already exists or could not be created for thread "
//				<< this_thread_id << ": " << full_thread_folder_path << std::endl;
//		}
//		else {
//			std::cout << "Created directory for thread " << this_thread_id << ": "
//				<< full_thread_folder_path << std::endl;
//		}
//	}
//	catch (const std::filesystem::filesystem_error& e) {
//		std::cerr << "Error creating directory for thread " << this_thread_id << ": "
//			<< e.what() << std::endl;
//	//	PyGILState_Release(gstate); // שחרר את ה-GIL
//		return 1;
//	}
//
//	
//
//	std::wcout << L"The Python interpreter was successfully initialized." << std::endl;
//
//	//להוסיף : שליחת ערכים לפייתון 
//	//לשלוח גם את השם של התיקיה 
//	PyObject* pName = NULL;
//	PyObject* pModule = NULL;
//	PyObject* pFunc = NULL;
//
//	std::string script_dir = "C:/Users/USER/Desktop/Rio";
//	std::string module_name = "func";
//
//	PyObject* sysPath = PySys_GetObject("path");
//	if (sysPath == NULL) { PyErr_Print(); Py_Finalize(); return 1; }
//	PyObject* pyScriptDir = PyUnicode_DecodeFSDefault(script_dir.c_str());
//	if (pyScriptDir == NULL) { PyErr_Print(); Py_Finalize(); return 1; }
//	PyList_Append(sysPath, pyScriptDir);
//	Py_XDECREF(pyScriptDir);
//
//	pName = PyUnicode_DecodeFSDefault(module_name.c_str());
//	if (!pName) { PyErr_Print(); Py_Finalize(); return 1; }
//	pModule = PyImport_Import(pName);
//	Py_XDECREF(pName);
//	if (pModule == NULL) { PyErr_Print(); Py_Finalize(); return 1; }
//
//	pFunc = PyObject_GetAttrString(pModule, "Image_processing");
//	if (!pFunc || !PyCallable_Check(pFunc)) {
//		if (PyErr_Occurred()) PyErr_Print();
//		Py_XDECREF(pModule);
//		Py_Finalize();
//		return 1;
//	}
//	PyObject* pArgs = PyTuple_New(5);
//	if (!pArgs) {
//		PyErr_Print();
//		std::cerr << "Error: Failed to create Python tuple for arguments." << std::endl;
//		Py_XDECREF(pFunc); Py_XDECREF(pModule); Py_Finalize(); return 1;
//	}
//	std::string specific_output_path_str = full_thread_folder_path.string();
//	cout << "AAAAAAAAA" << specific_output_path_str << endl;
//	PyTuple_SetItem(pArgs, 0, PyUnicode_DecodeFSDefault(specific_output_path_str.c_str()));
//
//	PyObject* pyValuesForThread = convert_vector_to_pylist(valuesForThread);
//	PyTuple_SetItem(pArgs, 1, pyValuesForThread);
//	PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
//	Py_XDECREF(pArgs);
//	if (pValue == NULL) {
//		PyErr_Print();
//		std::cerr << "Error: Call to Python function 'Image_processing' failed." << std::endl;
//	}
//	else {
//		std::cout << "Python function 'Image_processing' executed successfully." << std::endl;
//	}
//	Py_XDECREF(pValue);
//	Py_XDECREF(pFunc);
//	Py_XDECREF(pModule);
//	
//	return 0;
//}
//////void DrivingScenarios::ConnectKalmanFilter()
//////{
//////
//////	string result = "";
//////	try
//////	{
//////		// Calling the Kalman filter from a Python file with the parameters we recorded
//////		string command = "python.exe kalmanFilter.py " + to_string(GetoldvelosityX()) + " " + to_string(GetoldvelosityY()) + " " +
//////			to_string(GetvelosityX()) + " " + to_string(GetvelosityY());
//////		pipe = _popen(command.c_str(), "r");
//////		while (!feof(pipe))
//////		{
//////			if (fgets(buffer, 128, pipe) != NULL)
//////				result += buffer;
//////
//////		}
//////
//////		stringstream ss(result);
//////		double pos;
//////		while (ss >> pos)
//////		{
//////			filtered_positions.push_back(pos);
//////		}
//////		SetoldvelosityX(GetvelosityX());
//////		SetoldvelosityY(GetvelosityY());
//////		SetvelosityX(filtered_positions[i]);
//////		SetvelosityY(filtered_positions[i + 1]);
//////		print("velosityX:" + to_string(filtered_positions[i]) + " " + "velosityY:" + to_string(filtered_positions[i + 1]));
//////		i = i + 2;
//////		_pclose(pipe);
//////	}
//////	catch (const exception& e)
//////	{
//////		cerr << "Exception caught: " << e.what() << endl;
//////	}
//////}

// פונקציה שמריצה את הפונקציה הראשית בפייתון  
int try_to_run_yolo(thread::id this_thread_id, vector<float>valuesForThread)
{
	// ... קוד ליצירת נתיב התיקייה (ללא שינוי בחלק זה) ...
	std::string base_results_path = "C:\\Users\\USER\\Desktop\\PROJECT\\Threads";
	std::ostringstream oss;
	oss << this_thread_id;
	std::string thread_id_str = oss.str();
	std::string thread_folder_name = "thread_data_" + thread_id_str;
	std::filesystem::path full_thread_folder_path = std::filesystem::path(base_results_path) / thread_folder_name;

	try {
		if (!std::filesystem::create_directory(full_thread_folder_path)) {
			std::cerr << "Warning: Directory already exists or could not be created for thread "
				<< this_thread_id << ": " << full_thread_folder_path << std::endl;
		}
		else {
			std::cout << "Created directory for thread " << this_thread_id << ": "
				<< full_thread_folder_path << std::endl;
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error creating directory for thread " << this_thread_id << ": "
			<< e.what() << std::endl;
		return -1; // החזר שגיאה אם יצירת התיקייה נכשלה לחלוטין
	}
	string specific_output_path_str = full_thread_folder_path.string();
	string model_path = "C:\\Users\\USER\\Desktop\\PROJECT\\yoloperson\\best.pt";
	string model_Sign_path = "C:\\Users\\USER\\Desktop\\PROJECT\\NEWYOLO\\best.pt";
	int result = 0;
	string valuesForThread_str;
	for (size_t i = 0; i < valuesForThread.size(); ++i) {
		valuesForThread_str += std::to_string(valuesForThread[i]);
		if (i < valuesForThread.size() - 1) {
			valuesForThread_str += ",";
		}
	}
	try
	{
		//	string command = "python.exe C:\\Users\\USER\\Desktop\\Rio\\func.py" + specific_output_path_str + " " + valuesForThread_str + " " + model_path + " " + model_Sign_path;
		string command = "python.exe C:\\Users\\USER\\Desktop\\Rio\\func.py \"" +
			specific_output_path_str + "\" \"" +
			valuesForThread_str + "\" \"" +
			model_Sign_path + "\" \"" +
			model_path + "\"";
		this_thread::sleep_for(std::chrono::milliseconds(50));
		std::cout << "Attempting to run Python command: " << command << std::endl; // הדפסת הפקודה לניפוי באגים
		result = system(command.c_str());
		std::cout << "Python script finished with exit code: " << result << std::endl; // הדפסת קוד יציאה
	}
	catch (const exception& e)
	{
		cerr << "Exception caught while running Python script: " << e.what() << endl;
		return -2; // החזר שגיאה אחרת אם הפעלת הפייתון נכשלה
	}

	return result; // החזר את קוד היציאה של הפייתון או 0 להצלחה, או כל ערך אחר כרצונך
}

//int runPythonInThread(std::thread::id this_thread_id, std::vector<float>valuesForThread)
//{
//	//double X_center = valuesForThread[0];
//	//double Y_center = valuesForThread[1];
//	//double height = valuesForThread[2];
//	//double width = valuesForThread[3];
//	
//	cout << "SSSSSSSSSSSSS" << endl;
//	//PyGILState_STATE gstate; // הצהרה על משתנה מצב ה-GIL
//	//gstate = PyGILState_Ensure(); // *** חובה לרכוש את ה-GIL כאן ***
//
//	//יצירת תיקיה עבור התהליכון הנוכחי
//	std::string base_results_path = "C:/Users/USER/Desktop/PROJECT/Threads";
//	std::ostringstream oss;
//	oss << this_thread_id;
//	std::string thread_id_str = oss.str();
//	std::string thread_folder_name = "thread_data_" + thread_id_str;
//	std::filesystem::path full_thread_folder_path = std::filesystem::path(base_results_path) / thread_folder_name;
//
//	try {
//		if (!std::filesystem::create_directory(full_thread_folder_path)) {
//			std::cerr << "Warning: Directory already exists or could not be created for thread "
//				<< this_thread_id << ": " << full_thread_folder_path << std::endl;
//		}
//		else {
//			std::cout << "Created directory for thread " << this_thread_id << ": "
//				<< full_thread_folder_path << std::endl;
//		}
//	}
//	catch (const std::filesystem::filesystem_error& e) {
//		std::cerr << "Error creating directory for thread " << this_thread_id << ": "
//			<< e.what() << std::endl;
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		return 1;
//	}
//
//
//
//	std::wcout << L"The Python interpreter was successfully initialized." << std::endl;
//
//	//להוסיף : שליחת ערכים לפייתון 
//	//לשלוח גם את השם של התיקיה 
//	PyObject* pName = NULL;
//	PyObject* pModule = NULL;
//	PyObject* pFunc = NULL;
//
//	std::string script_dir = "C:/Users/USER/Desktop/Rio";
//	std::string module_name = "func";
//
//	PyObject* sysPath = PySys_GetObject("path");
//	if (sysPath == NULL) {
//		PyErr_Print();
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		// Py_Finalize(); // אין לקרוא ל-Py_Finalize() בתוך תהליכון שאינו התהליכון הראשי
//		return 1;
//	}
//	PyObject* pyScriptDir = PyUnicode_DecodeFSDefault(script_dir.c_str());
//	if (pyScriptDir == NULL) {
//		PyErr_Print();
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		// Py_Finalize(); // אין לקרוא ל-Py_Finalize() בתוך תהליכון שאינו התהליכון הראשי
//		return 1;
//	}
//	PyList_Append(sysPath, pyScriptDir);
//	Py_XDECREF(pyScriptDir);
//
//	pName = PyUnicode_DecodeFSDefault(module_name.c_str());
//	if (!pName) {
//		PyErr_Print();
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		// Py_Finalize(); // אין לקרוא ל-Py_Finalize() בתוך תהליכון שאינו התהליכון הראשי
//		return 1;
//	}
//	pModule = PyImport_Import(pName);
//	Py_XDECREF(pName);
//	if (pModule == NULL) {
//		PyErr_Print();
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		// Py_Finalize(); // אין לקרוא ל-Py_Finalize() בתוך תהליכון שאינו התהליכון הראשי
//		return 1;
//	}
//
//	pFunc = PyObject_GetAttrString(pModule, "Image_processing");
//	if (!pFunc || !PyCallable_Check(pFunc)) {
//		if (PyErr_Occurred()) PyErr_Print();
//		Py_XDECREF(pModule);
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		// Py_Finalize(); // אין לקרוא ל-Py_Finalize() בתוך תהליכון שאינו התהליכון הראשי
//		return 1;
//	}
//	PyObject* pArgs = PyTuple_New(5);
//	if (!pArgs) {
//		PyErr_Print();
//		std::cerr << "Error: Failed to create Python tuple for arguments." << std::endl;
//		Py_XDECREF(pFunc); Py_XDECREF(pModule);
//		//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL במקרה של שגיאה לפני יציאה ***
//		// Py_Finalize(); // אין לקרוא ל-Py_Finalize() בתוך תהליכון שאינו התהליכון הראשי
//		return 1;
//	}
//	std::string specific_output_path_str = full_thread_folder_path.string();
//	cout << "AAAAAAAAA" << specific_output_path_str << endl;
//	PyTuple_SetItem(pArgs, 0, PyUnicode_DecodeFSDefault(specific_output_path_str.c_str()));
//
//	PyObject* pyValuesForThread = convert_vector_to_pylist(valuesForThread);
//	// חשוב: אם convert_vector_to_pylist יכולה להיכשל, יש לטפל בכך ולשחרר את pyValuesForThread במקרה זה.
//	if (!pyValuesForThread) {
//		PyErr_Print();
//		std::cerr << "Error: convert_vector_to_pylist failed." << std::endl;
//		Py_XDECREF(pArgs); Py_XDECREF(pFunc); Py_XDECREF(pModule);
//		//PyGILState_Release(gstate);
//		return 1;
//	}
//
//	PyTuple_SetItem(pArgs, 1, pyValuesForThread);
//	PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
//	Py_XDECREF(pArgs);
//	if (pValue == NULL) {
//		PyErr_Print();
//		std::cerr << "Error: Call to Python function 'Image_processing' failed." << std::endl;
//	}
//	else {
//		std::cout << "Python function 'Image_processing' executed successfully." << std::endl;
//	}
//	Py_XDECREF(pValue);
//	Py_XDECREF(pFunc);
//	Py_XDECREF(pModule);
//
//	//PyGILState_Release(gstate); // *** חובה לשחרר את ה-GIL בסיום העבודה בתהליכון ***
//	return 0;
//}
//הרצת אנשים ושטחים מתים עבור בדיקה של שינוי או כניסה של מישהו חדש לבריכה

//string HelpToCheakNewPerson(string imagePath)
//{
//	string YoloPeopleScriptPath = "C:/Users/USER/Desktop/Rio/YOLOPERSON.py";
//	string YoloPeoplePath = "C:/Users/USER/Desktop/PROJECT/yoloperson/best.pt";
//	string pathToResualt = "C:/Users/USER/Desktop/PROJECT/cheakNewPerson";
//	//string fileOfResualt = "C:/Users/USER/Desktop/PROJECT/PerWithMask/person_detection_results_0.txt";
//	//string imagePath = imagePath;
//	string command = "python \"" + YoloPeopleScriptPath + "\" \"" + imagePath + "\" \"" + YoloPeoplePath + "\" \"" + pathToResualt + "\"";
//	std::cout << "מפעיל את סקריפט הפייתון: " << command << std::endl;
//	int system_result = system(command.c_str());
//	if (system_result != 0) {
//		std::cerr << "שגיאה בהרצת סקריפט הפייתון. קוד חזרה: " << system_result << std::endl;
//		return "1";
//	}
//	std::cout << "סקריפט הפייתון סיים לרוץ בהצלחה." << std::endl;
//	string generatedImagePath = pathToResualt + "/dead_space_image.jpg";
//	return generatedImagePath;
//}

string HelpToCheakNewPerson(string imagePath)
{
	string YoloPeopleScriptPath = "C:/Users/USER/Desktop/Rio/yoloPerson_CheackNew.py";
	string YoloPeopleModelPath = "C:/Users/USER/Desktop/PROJECT/yoloperson/best.pt";
	string checkNewPersonFolder = "C:/Users/USER/Desktop/PROJECT/cheakNewPerson";
	string firstRecLabelsFolder = "C:/Users/USER/Desktop/PROJECT/firstrec/predict/labels";

	// יצירת תיקיית היעד אם אינה קיימת
	if (!fs::exists(checkNewPersonFolder)) {
		try {
			fs::create_directories(checkNewPersonFolder);
			cout << "Created directory: " << checkNewPersonFolder << endl;
		}
		catch (const fs::filesystem_error& e) {
			cerr << "Error creating directory " << checkNewPersonFolder << ": " << e.what() << endl;
			return ""; // מציין כשל
		}
	}

	fs::path original_image_path_obj = imagePath;
	string image_base_name = original_image_path_obj.stem().string(); // שם הקובץ ללא סיומת
	string original_txt_filename = image_base_name + ".txt"; // שם קובץ ה-TXT המקורי
	fs::path original_txt_path_obj = fs::path(firstRecLabelsFolder) / original_txt_filename; // הנתיב המלא ל-TXT המקורי

	fs::path destination_image_path_check = fs::path(checkNewPersonFolder) / "image.jpg"; // שם קבוע לתמונה בתיקיית הבדיקה
	fs::path destination_txt_path_check = fs::path(checkNewPersonFolder) / "image_results.txt"; // שם קבוע ל-TXT בתיקיית הבדיקה

	int max_retries = 5;
	int copy_retry_delay_ms = 100;

	// פונקציית עזר למחיקת קבצים בצורה עמידה
	auto remove_file_robustly = [&](const fs::path& p) {
		if (!fs::exists(p)) return true; // אם לא קיים, נחשב כמחיקה מוצלחת
		for (int j = 0; j < 5; ++j) { // ניסיון מחיקה עד 5 פעמים
			try {
				fs::remove(p);
				return true;
			}
			catch (const fs::filesystem_error& e_remove) {
				// cerr << "Debug: Error removing " << p.string() << " (attempt " << j + 1 << "): " << e_remove.what() << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(50)); // השהיה קצרה לפני ניסיון מחיקה נוסף
			}
		}
		cerr << "Failed to remove file after multiple retries: " << p.string() << endl;
		return false;
		};

	bool image_copied = false;
	for (int i = 0; i < max_retries; ++i) {
		try {
			if (!remove_file_robustly(destination_image_path_check)) {
				cerr << "Failed to prepare destination image for copy. Retrying..." << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(copy_retry_delay_ms));
				continue;
			}
			fs::copy(original_image_path_obj, destination_image_path_check, fs::copy_options::overwrite_existing);
			cout << "Image copied to: \"" << destination_image_path_check.string() << "\"" << endl;
			image_copied = true;
			break;
		}
		catch (const fs::filesystem_error& e) {
			cerr << "Attempt " << i + 1 << "/" << max_retries << ": Error copying image to \"" << destination_image_path_check.string() << "\": " << e.what() << endl;
			if (i < max_retries - 1) {
				std::this_thread::sleep_for(std::chrono::milliseconds(copy_retry_delay_ms));
			}
			else {
				cerr << "Failed to copy image after multiple retries: \"" << destination_image_path_check.string() << "\"" << endl;
				return ""; // מציין כשל קריטי
			}
		}
	}
	if (!image_copied) {
		return "";
	}
	bool txt_copied = false;
	if (fs::exists(original_txt_path_obj)) {
		for (int i = 0; i < max_retries; ++i) {
			try {
				if (!remove_file_robustly(destination_txt_path_check)) {
					cerr << "Failed to prepare destination TXT for copy. Retrying..." << endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(copy_retry_delay_ms));
					continue;
				}
				fs::copy(original_txt_path_obj, destination_txt_path_check, fs::copy_options::overwrite_existing);
				cout << "TXT file copied to: \"" << destination_txt_path_check.string() << "\"" << endl;
				txt_copied = true;
				break;
			}
			catch (const fs::filesystem_error& e) {
				cerr << "Attempt " << i + 1 << "/" << max_retries << ": Error copying TXT file to \"" << destination_txt_path_check.string() << "\": " << e.what() << endl;
				if (i < max_retries - 1) {
					std::this_thread::sleep_for(std::chrono::milliseconds(copy_retry_delay_ms));
				}
				else {
					cerr << "Failed to copy TXT file after multiple retries: \"" << destination_txt_path_check.string() << "\"" << endl;
				}
			}
		}
	}
	else {
		cout << "Original TXT file not found, skipping copy: \"" << original_txt_path_obj.string() << "\"" << endl;
		txt_copied = true;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms


	string python_command = "python \"" + YoloPeopleScriptPath + "\" \"" + destination_image_path_check.string() + "\" \"" + YoloPeopleModelPath + "\" \"" + checkNewPersonFolder + "\" true 0.4"; // הוסף את הפרמטרים save_masked ו-conf_threshold_people
	std::cout << "Attempting to run Python command: " << python_command << std::endl;

	int system_result = system(python_command.c_str());
	if (system_result != 0) {
		std::cerr << "שגיאה בהרצת סקריפט הפייתון. קוד חזרה: " << system_result << std::endl;
	}
	std::cout << "Python script finished with exit code: " << system_result << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (!remove_file_robustly(original_image_path_obj)) {
		cerr << "Warning: Failed to remove original image after processing: \"" << original_image_path_obj.string() << "\"" << endl;
	}
	else {
		cout << "Removed original image: \"" << original_image_path_obj.string() << "\"" << endl;
	}

	if (fs::exists(original_txt_path_obj)) {
		if (!remove_file_robustly(original_txt_path_obj)) {
			cerr << "Warning: Failed to remove original TXT file after processing: \"" << original_txt_path_obj.string() << "\"" << endl;
		}
		else {
			cout << "Removed original TXT file: \"" << original_txt_path_obj.string() << "\"" << endl;
		}
	}
	else {
		cout << "Original TXT file not found (or already removed), skipping explicit removal: \"" << original_txt_path_obj.string() << "\"" << endl;
	}

	return destination_image_path_check.string();
}

//פונקציה שאחראית לעקוב אחרי השטחים המתים ולבדוק אם נכנס מישהו חדש
bool cheakNewPerson()
{
	filesystem::path empty_spaces_image = HelpToCheakNewPerson("C:\\Users\\USER\\Desktop\\PROJECT\\cheakNewPerson\\image.jpg");
	if (!filesystem::exists(empty_spaces_image)) {
		cerr << "Error: The dead space image does not exist at the specified path: " << empty_spaces_image << endl;
		return false;
	}
	cout << "Checking for new people in the dead spaces...\n";
	const wchar_t* pythonHome = L"C:\\Users\\USER\\AppData\\Local\\Programs\\Python\\Python310";
	Py_SetPythonHome(pythonHome);
	Py_Initialize();
	if (!Py_IsInitialized()) {
		wcout << L"Error: Failed to initialize Python interpreter!" << std::endl;
		return false;
	}
	wcout << L"The Python interpreter was successfully initialized." << endl;

	string script_dir = "C:/Users/USER/Desktop/Rio";
	string model_people_file_path = "C:/Users/USER/Desktop/PROJECT/yoloperson/best.pt";
	std::string output_results_directory = "C:/Users/USER/Desktop/PROJECT/cheakNewPerson";
	bool save_detection_images = true;
	double confidence_threshold = 0.4;
	PyObject* sysPath = PySys_GetObject("path");
	if (sysPath == NULL) {
		PyErr_Print();
		std::cerr << "Error: Failed to get sys.path." << std::endl;
		Py_Finalize();
		return false;
	}
	PyObject* pyScriptDir = PyUnicode_DecodeFSDefault(script_dir.c_str());
	if (pyScriptDir == NULL) {
		PyErr_Print();
		std::cerr << "Error: Failed to decode script directory path." << std::endl;
		Py_Finalize();
		return false;
	}
	PyList_Append(sysPath, pyScriptDir);
	Py_XDECREF(pyScriptDir);
	PyObject* pName = PyUnicode_DecodeFSDefault("yoloPerson_CheackNew");
	if (!pName) {
		PyErr_Print();
		std::cerr << "Error: Failed to decode module name." << std::endl;
		Py_Finalize();
		return false;
	}
	PyObject* pModule = PyImport_Import(pName);
	Py_XDECREF(pName);
	if (pModule == NULL) {
		PyErr_Print();
		std::cerr << "Error: Failed to load Python module 'yoloPerson_CheackNew'." << std::endl;
		Py_Finalize();
		return false;
	}
	PyObject* pFunc = PyObject_GetAttrString(pModule, "runYoloCN");
	if (!pFunc || !PyCallable_Check(pFunc)) {
		if (PyErr_Occurred()) PyErr_Print();
		std::cerr << "Error: Cannot find or call Python function 'runYoloCN'." << std::endl;
		Py_XDECREF(pModule);
		Py_Finalize();
		return false;
	}
	PyObject* pArgs = PyTuple_New(5);
	if (!pArgs) {
		PyErr_Print();
		std::cerr << "Error: Failed to create Python tuple for arguments." << std::endl;
		Py_XDECREF(pFunc);
		Py_XDECREF(pModule);
		Py_Finalize();
		return false;
	}
	PyTuple_SetItem(pArgs, 0, PyUnicode_DecodeFSDefault(empty_spaces_image.string().c_str()));
	PyTuple_SetItem(pArgs, 1, PyUnicode_DecodeFSDefault(model_people_file_path.c_str()));
	PyTuple_SetItem(pArgs, 2, PyUnicode_DecodeFSDefault(output_results_directory.c_str()));
	PyTuple_SetItem(pArgs, 3, PyBool_FromLong(save_detection_images ? 1 : 0));
	PyTuple_SetItem(pArgs, 4, PyFloat_FromDouble(confidence_threshold));
	PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
	Py_XDECREF(pArgs);
	bool result_from_python = false;
	if (pValue == NULL) {
		PyErr_Print();
		std::cerr << "Error: Call to Python function 'runYoloCN' failed." << std::endl;
	}
	else if (PyBool_Check(pValue)) {
		result_from_python = (pValue == Py_True);
		std::cout << "Python function 'runYoloCN' returned: "
			<< (result_from_python ? "True" : "False") << std::endl;
	}
	else {
		std::cerr << "Error: Python function 'runYoloCN' returned an unexpected type. Expected boolean." << std::endl;
	}
	Py_XDECREF(pFunc);
	Py_XDECREF(pModule);
	Py_Finalize();
	std::wcout << L"The Python interpreter was successfully finalized." << std::endl;

	return result_from_python;
}

//פונקציה שאחראית לנקד את הטביעה
//עוברת על כל התיקיות של התהליכונים קוראת את התוצאות זיהוי מהיולו עבור כל תהליכון ואז מוחקת את הקובץ של הזיהוי
//void Drowning_score(std::string folderPath)
//{
//	std::filesystem::path found_txt_file_path;
//	bool txt_file_found = false;
//
//	Score_Drowning scoreDrowning; // יצירת מופע של Score_Drowning
//	std::filesystem::path base_folder_path = folderPath;
//
//	// בדיקה ראשונית של תיקיית הבסיס
//	if (!std::filesystem::exists(base_folder_path) || !std::filesystem::is_directory(base_folder_path)) {
//		std::cerr << "שגיאה: התיקייה הראשית לא קיימת או אינה תיקייה: " << folderPath << std::endl;
//		return;
//	}
//
//	try
//	{
//		// מעבר על הפריטים בתיקייה שהתקבלה כקלט תיקיות ברמה 1)
//		for (const auto& item : std::filesystem::directory_iterator(base_folder_path))
//		{
//			// וודא שהפריט הנוכחי הוא תיקייה
//			if (std::filesystem::is_directory(item.path()))
//			{
//				std::filesystem::path current_person_folder = item.path();
//				std::string folder_name = current_person_folder.filename().string();
//
//				// חילוץ ה-ID המספרי משם התיקייה (לדוגמה: thread_data_123 -> 123)
//				std::regex re("thread_data_(\\d+)"); // ביטוי רגולרי למציאת מספר אחרי "thread_data_"
//				std::smatch match;
//				int person_logical_id = -1; // אתחול בערך לא חוקי
//
//				// בדיקה אם שם התיקייה תואם את הפורמט הצפוי
//				if (std::regex_search(folder_name, match, re) && match.size() > 1)
//				{
//					try
//					{
//						person_logical_id = std::stoi(match[1].str());
//					}
//					catch (const std::invalid_argument& e)
//					{
//						std::cerr << "  שגיאה: לא ניתן להמיר את השם '" << folder_name << "' ל-ID מספרי (תוכן לא חוקי). " << e.what() << std::endl;
//						continue; // עבור לתיקייה הבאה
//					}
//				}
//				else
//				{
//					std::cout << "  מתעלם מתיקייה שאינה בפורמט 'thread_data_XXX': " << folder_name << std::endl;
//					continue; // עבור לתיקייה הבאה
//				}
//
//				// כעת, חפש קובץ TXT בתוך תיקיית האדם הנוכחית
//
//				for (const auto& txt_item : std::filesystem::directory_iterator(current_person_folder))
//				{
//					if (std::filesystem::is_regular_file(txt_item.path()) && txt_item.path().extension() == ".txt")
//					{
//						found_txt_file_path = txt_item.path();
//						txt_file_found = true;
//						break; // מצאנו קובץ TXT אחד, אין צורך להמשיך לסרוק בתיקייה זו
//					}
//				}
//
//				if (txt_file_found)
//				{
//					std::cout << "    נמצא קובץ TXT: " << found_txt_file_path.filename() << " בתיקייה: " << folder_name << std::endl;
//					std::ifstream txtFile(found_txt_file_path);
//
//					if (txtFile.is_open()) {
//						std::string line_from_file;
//						// קריאת כל השורות בקובץ ה-TXT
//						while (std::getline(txtFile, line_from_file))
//						{
//							int classs_id = 0; // המשתנה ששומר את ה-sign שנמצא בשורה
//							if (!line_from_file.empty())
//							{
//								// חלץ את המספר מהשורה
//								size_t space_pos = line_from_file.find(' ');
//								std::string number_str;
//								if (space_pos != std::string::npos) {
//									number_str = line_from_file.substr(0, space_pos);
//								}
//								else {
//									number_str = line_from_file; // אם אין רווח, כל השורה היא המספר
//								}
//
//								try {
//									classs_id = std::stoi(number_str);
//								}
//								catch (const std::invalid_argument& e) {
//									std::cerr << "      שגיאה: תוכן לא חוקי בקובץ (לא ניתן להמיר למספר): '" << number_str << "' - " << e.what() << std::endl;
//									continue; // עבור לשורה הבאה בקובץ
//								}
//								catch (const std::out_of_range& e) {
//									std::cerr << "      שגיאה: מספר גדול מדי או קטן מדי בקובץ: '" << number_str << "' - " << e.what() << std::endl;
//									continue; // עבור לשורה הבאה בקובץ
//								}
//
//								// זימון הפונקציה PlayHash בתהליכון חדש
//								// וודא ש-sign_from_file הוא אינדקס חוקי לווקטור functions_hash
//								if (classs_id >= 0 && classs_id < scoreDrowning.functions_hash.size())
//								{
//									// שימוש ב-std::bind כדי לקשור את האובייקט ואת הפרמטרים לפונקציה
//									// ה-person_logical_id הוא ה-ID שחילצנו משם התיקייה
//									std::thread my_new_thread(std::bind(&Score_Drowning::PlayHash, &scoreDrowning, classs_id, person_logical_id));
//									my_new_thread.detach(); // התהליכון ירוץ ברקע באופן עצמאי
//									std::cout << "      הופעל תהליכון עבור PlayHash עם sign: " << classs_id << " ואדם: " << person_logical_id << std::endl;
//								}
//								else
//								{
//									std::cerr << "      שגיאה: sign " << classs_id << " מחוץ לטווח functions_hash." << std::endl;
//								}
//							}
//						}
//						txtFile.close(); // סגירת הקובץ לאחר סיום קריאת כל השורות
//						std::filesystem::remove(found_txt_file_path); // מחיקת הקובץ לאחר עיבוד מלא
//						std::cout << "    קובץ TXT נמחק: " << found_txt_file_path.filename() << std::endl;
//					}
//					else
//					{
//						std::cerr << "    שגיאה: לא ניתן לפתוח את קובץ TXT בנתיב: " << found_txt_file_path << std::endl;
//					}
//				}
//				else
//				{
//					std::cout << "  ayala " << folder_name << std::endl;
//				}
//			}
//		}
//	}
//	catch (const std::filesystem::filesystem_error& e) {
//		std::cerr << "שגיאת מערכת קבצים: " << e.what() << std::endl;
//	}
//	catch (const std::exception& e) {
//		std::cerr << "שגיאה כללית בלתי צפויה: " << e.what() << std::endl;
//	}
//
//	std::cout << "סריקת התיקייה הראשית הסתיימה.\n";
//}
void Drowning_score(std::string folderPath)
{
	std::filesystem::path found_txt_file_path;
	bool txt_file_found = false;

	Score_Drowning scoreDrowning; // יצירת מופע של Score_Drowning
	std::filesystem::path base_folder_path = folderPath;

	// בדיקה ראשונית של תיקיית הבסיס
	if (!std::filesystem::exists(base_folder_path) || !std::filesystem::is_directory(base_folder_path)) {
		std::cerr << "שגיאה: התיקייה הראשית לא קיימת או אינה תיקייה: " << folderPath << std::endl;
		return;
	}

	try
	{
		// מעבר על הפריטים בתיקייה שהתקבלה כקלט (תיקיות ברמה 1)
		for (const auto& item : std::filesystem::directory_iterator(base_folder_path))
		{
			if (std::filesystem::is_directory(item.path()))
			{
				std::filesystem::path current_person_folder = item.path();
				std::string folder_name = current_person_folder.filename().string();

				std::regex re("thread_data_(\\d+)"); // ביטוי רגולרי למציאת מספר אחרי "thread_data_"
				std::smatch match;
				int person_logical_id = -1;

				if (std::regex_search(folder_name, match, re) && match.size() > 1)
				{
					try
					{
						person_logical_id = std::stoi(match[1].str());
					}
					catch (const std::invalid_argument& e)
					{
						std::cerr << "שגיאה: לא ניתן להמיר את השם '" << folder_name << "' ל-ID מספרי (תוכן לא חוקי). " << e.what() << std::endl;
						continue;
					}
				}
				else
				{
					std::cout << "מתעלם מתיקייה שאינה בפורמט 'thread_data_XXX': " << folder_name << std::endl;
					continue;
				}


				found_txt_file_path = ""; // איפוס הנתיב עבור כל תיקיית אדם
				txt_file_found = false; // איפוס הדגל

				for (const auto& txt_item : std::filesystem::directory_iterator(current_person_folder))
				{
					if (std::filesystem::is_regular_file(txt_item.path()) && txt_item.path().extension() == ".txt")
					{
						found_txt_file_path = txt_item.path();
						txt_file_found = true;
						break;
					}
				}

				if (txt_file_found)
				{
					std::cout << "נמצא קובץ TXT: " << found_txt_file_path.filename() << " בתיקייה: " << folder_name << std::endl;
					std::ifstream txtFile(found_txt_file_path);

					if (txtFile.is_open()) {
						std::string line_from_file;
						while (std::getline(txtFile, line_from_file))
						{
							int classs_id = 0;
							if (!line_from_file.empty())
							{
								size_t space_pos = line_from_file.find(' ');
								std::string number_str;
								if (space_pos != std::string::npos) {
									number_str = line_from_file.substr(0, space_pos);
								}
								else {
									number_str = line_from_file;
								}

								try {
									classs_id = std::stoi(number_str);
								}
								catch (const std::invalid_argument& e) {
									std::cerr << "שגיאה: תוכן לא חוקי בקובץ (לא ניתן להמיר למספר): '" << number_str << "' - " << e.what() << std::endl;
									continue;
								}
								catch (const std::out_of_range& e) {
									std::cerr << "שגיאה: מספר גדול מדי או קטן מדי בקובץ: '" << number_str << "' - " << e.what() << std::endl;
									continue;
								}
								if (classs_id >= 0 && classs_id < scoreDrowning.functions_hash.size())
								{ 
									std::thread my_new_thread(std::bind(&Score_Drowning::PlayHash, &scoreDrowning, classs_id, person_logical_id));
									my_new_thread.detach();  
									std::cout << "הופעל תהליכון עבור PlayHash עם sign: " << classs_id << " ואדם: " << person_logical_id << std::endl;
								}
								else
								{
									std::cerr << "שגיאה: sign " << classs_id << " מחוץ לטווח functions_hash." << std::endl;
								}
							}
						}
						txtFile.close(); // סגירת הקובץ לאחר סיום קריאת כל השורות

						// --- התחלת לוגיקת ניסיון חוזר למחיקת קובץ TXT ---
						bool txt_removed = false;
						int max_retries = 5;
						int retry_delay_ms = 100; // 100 מילישניות

						for (int i = 0; i < max_retries && !txt_removed; ++i) {
							try {
								std::filesystem::remove(found_txt_file_path);
								std::cout << "קובץ TXT נמחק: " << found_txt_file_path.filename() << std::endl;
								txt_removed = true;
							}
							catch (const std::filesystem::filesystem_error& e) {
								std::cerr << "שגיאה במחיקת קובץ TXT " << found_txt_file_path.filename() << ": " << e.what() << std::endl;
								std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
							}
						}

						if (!txt_removed) {
							std::cerr << "כשל במחיקת קובץ TXT לאחר ניסיונות חוזרים: " << found_txt_file_path.filename() << std::endl;
						}
						// --- סיום לוגיקת ניסיון חוזר ---

					}
					else
					{
						std::cerr << "שגיאה: לא ניתן לפתוח את קובץ TXT בנתיב: " << found_txt_file_path << std::endl;
					}
				}
				else
				{
					std::cout << "ayala " << folder_name << std::endl; // כנראה הודעת ניפוי שגיאות, ניתן להסיר אם לא נחוץ
				}
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "שגיאת מערכת קבצים כללית: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "שגיאה כללית בלתי צפויה: " << e.what() << std::endl;
	}

	std::cout << "סריקת התיקייה הראשית הסתיימה.\n";
}

// פונקציה למחיקת איבר מהמילון
void Deleting_member(std::map<int, recP>& person, int thread_id) {
	auto it = person.find(thread_id);
	if (it != person.end()) {
		person.erase(it);
		std::cout << "Member with thread_id " << thread_id << " deleted successfully.\n";
	}
	else {
		std::cerr << "Error: Member with thread_id " << thread_id << " not found.\n";
	}
}


//פונקציה שמקבלת נתיב לתמונה ומחלקת את התמונה לכל התיקיות של התהליכונים ולתיקיה של הפונקציה שבודקת אם נכנס מישהו חדש לבריכה
void DistributeImageToThreads(string imagePath, string baseFolderPath)
{
	// בדיקות מקדימות
	if (imagePath.empty()) {
		std::cout << "No new images in " << imagePath << ". Waiting..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}
	if (!std::filesystem::exists(imagePath)) {
		std::cerr << "Error: The selected image file does not exist at " << imagePath << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return;
	}

	// הגדרת נתיב קובץ ה-TXT המשויך לתמונה המקורית
	// בהתבסס על פלט ה-YOLO ומיקום קבצי ה-TXT
	std::string image_name = std::filesystem::path(imagePath).stem().string(); // משיג את שם הקובץ ללא סיומת (לדוגמה: "frame_1")
	std::string image_txt_path = "C:/Users/USER/Desktop/PROJECT/firstrec/predict/labels/" + image_name + ".txt";

	// העתקת התמונה לתיקיות התהליכונים
	for (const auto& folderThread : filesystem::directory_iterator(baseFolderPath))
	{
		if (std::filesystem::is_directory(folderThread.path()))
		{
			filesystem::path thread_folder_specific = folderThread.path();
			filesystem::path destination_image_path = thread_folder_specific / "image.jpg";
			try
			{
				filesystem::copy(imagePath, destination_image_path, filesystem::copy_options::overwrite_existing);
				cout << "Image copied to: " << destination_image_path << endl;
			}
			catch (const std::filesystem::filesystem_error& e) {
				cerr << "Error copying image to " << thread_folder_specific << ": " << e.what() << endl;
			}
		}
	}

	// מכניסה גם לתיקיה של הבדיקה אם נכנס מישהו חדש
	filesystem::path destination_image_path_check = "C:/Users/USER/Desktop/PROJECT/cheakNewPerson/image.jpg";
	try
	{
		filesystem::copy(imagePath, destination_image_path_check, filesystem::copy_options::overwrite_existing);
		cout << "Image copied to: " << destination_image_path_check << endl;
	}
	catch (const std::filesystem::filesystem_error& e) {
		cerr << "Error copying image to " << destination_image_path_check << ": " << e.what() << endl;
	}

	// לולאת ניסיון-חוזר למחיקת קובץ התמונה המקורי
	bool image_removed = false;
	int max_retries = 5;
	int retry_delay_ms = 500;

	for (int i = 0; i < max_retries && !image_removed; ++i) {
		try {
			std::filesystem::remove(imagePath);
			std::cout << "Removed original image: " << imagePath << std::endl;
			image_removed = true;
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "Error removing original image " << imagePath << " (Attempt " << (i + 1) << "/" << max_retries << "): " << e.what() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
		}
	}

	if (!image_removed) {
		std::cerr << "Failed to remove original image after multiple retries: " << imagePath << std::endl;
	}

	// --- התחלת לוגיקת ניסיון חוזר למחיקת קובץ TXT המקורי ---
	bool txt_removed = false;
	// max_retries ו-retry_delay_ms כבר מוגדרים למעלה, ניתן להשתמש בהם שוב.

	for (int i = 0; i < max_retries && !txt_removed; ++i) {
		try {
			// בדוק אם הקובץ TXT קיים לפני שמנסים למחוק אותו
			if (std::filesystem::exists(image_txt_path)) {
				std::filesystem::remove(image_txt_path);
				std::cout << "Removed original TXT file: " << image_txt_path << std::endl;
				txt_removed = true; // הקובץ נמחק בהצלחה
			}
			else {
				// אם הקובץ לא קיים, נניח שהוא כבר נמחק או שלא נוצר, ונחשב את זה כהצלחה
				std::cout << "Original TXT file not found, assuming already removed or not created: " << image_txt_path << std::endl;
				txt_removed = true;
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "Error removing original TXT file " << image_txt_path << " (Attempt " << (i + 1) << "/" << max_retries << "): " << e.what() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms)); // המתן לפני ניסיון חוזר
		}
	}

	if (!txt_removed) {
		std::cerr << "Failed to remove original TXT file after " << max_retries << " retries: " << image_txt_path << std::endl;
	}
}
//פונקציה לחלוקת הזיהוי הראשון תמונה+ טקסט של התוצאות לכל התהליכונים ולתיקיה של הבדיקה אם נכנס מישהו חדש לבריכה
int givePicInStart(string imagePath, string image_txt_path, string baseFolderPath)
{
	// בדיקה אם נתיב התמונה ריק
	if (imagePath.empty()) {
		cout << "No new images in " << imagePath << ". Waiting..." << endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		return 0;
	}
	if (!filesystem::exists(imagePath)) {
		cerr << "Error: The selected image file does not exist at " << imagePath << endl;
		this_thread::sleep_for(chrono::milliseconds(100));
		return 0;
	}
	if (!filesystem::exists(image_txt_path)) {
		cerr << "Error: The selected text file does not exist at " << image_txt_path << endl;
		this_thread::sleep_for(chrono::milliseconds(100));
		return 0;
	}
	for (const auto& folderThread : filesystem::directory_iterator(baseFolderPath))
	{
		if (filesystem::is_directory(folderThread.path()))
		{
			filesystem::path thread_folder_specific = folderThread.path();
			filesystem::path destination_image_path = thread_folder_specific / "image.jpg";
			filesystem::path destination_txt_path = thread_folder_specific / "image_results.txt"; // נתיב יעד לקובץ ה-TXT

			try
			{
				filesystem::copy(imagePath, destination_image_path, filesystem::copy_options::overwrite_existing);
				cout << "Image copied to: " << destination_image_path << endl;
			}
			catch (const filesystem::filesystem_error& e) {
				cerr << "Error copying image to " << thread_folder_specific << ": " << e.what() << endl;
			}

			try
			{
				filesystem::copy(image_txt_path, destination_txt_path, filesystem::copy_options::overwrite_existing);
				cout << "TXT file copied to: " << destination_txt_path << endl;
			}
			catch (const filesystem::filesystem_error& e) {
				cerr << "Error copying TXT file to " << thread_folder_specific << ": " << e.what() << endl;
			}
		}
	}

	// --- העתקה לתיקיית הבדיקה (cheakNewPerson) ---

	filesystem::path destination_image_path_check = "C:/Users/USER/Desktop/PROJECT/cheakNewPerson/image.jpg";
	filesystem::path destination_txt_path_check = "C:/Users/USER/Desktop/PROJECT/cheakNewPerson/image_results.txt"; // נתיב יעד לקובץ ה-TXT

	try
	{
		filesystem::copy(imagePath, destination_image_path_check, filesystem::copy_options::overwrite_existing);
		cout << "Image copied to: " << destination_image_path_check << endl;
	}
	catch (const filesystem::filesystem_error& e) {
		cerr << "Error copying image to " << destination_image_path_check << ": " << e.what() << endl;
	}

	try
	{
		filesystem::copy(image_txt_path, destination_txt_path_check, filesystem::copy_options::overwrite_existing);
		cout << "TXT file copied to: " << destination_txt_path_check << endl;
	}
	catch (const filesystem::filesystem_error& e) {
		cerr << "Error copying TXT file to " << destination_txt_path_check << ": " << e.what() << endl;
	}

	try {
		filesystem::remove(imagePath);
		cout << "Removed original image: " << imagePath << endl;
	}
	catch (const filesystem::filesystem_error& e) {
		cerr << "Error removing original image " << imagePath << ": " << e.what() << endl;
	}

	try {
		filesystem::remove(image_txt_path);
		cout << "Removed original TXT file: " << image_txt_path << endl;
	}
	catch (const filesystem::filesystem_error& e) {
		cerr << "Error removing original TXT file " << image_txt_path << ": " << e.what() << endl;
	}

	return 1;
}

//פונקציה שאחראית על משתנה הטיימר של כל אדם מוסיפה לו 1 כל שניה ואם מגיע ל-10 מתאפס ומאפס גם את הערך של אחוזי הטביעה שלו
void CleanPoint()
{

	for (auto& person : Persons)
	{
		cout << "begin to upload the timer of each person\n";
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

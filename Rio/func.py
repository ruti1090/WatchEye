import sys
from time import sleep
import os

# יבוא קבצים נלווים
import cut_according_to_moving
import increase_mask
import run_yoloP_in_thread
import YOLOPERSON
import run_yolo_signs
import readFolderImage
import yolo_after_mask

def Image_processing(person_specific_output_path, valuesForThread_str, modelSignPath, modelPersonPath):
    print("Begin to run thread in Python")

    sleep(1.0)
    # המרה למערך של float
    try:
        valuesForThread = [float(item) for item in valuesForThread_str.split(',')]
    except ValueError:
        print(f"Error: Could not parse '{valuesForThread_str}' into a list of floats. Exiting.", file=sys.stderr)
        sys.exit(1)

    # קריאת תמונה מתיקיה
    image, imagePath = readFolderImage.find_pic_in_folder(person_specific_output_path)

    # חיתוך לפי תזוזה
    image, imagePath, folder_txt_path = cut_according_to_moving.cut_according_to_moving(imagePath, valuesForThread, person_specific_output_path)

    # זיהוי אנשים אחרי המסכה
    valuesForThread = yolo_after_mask.mainRun(imagePath, modelPersonPath, folder_txt_path, True, 0.4)
    if valuesForThread is None:  # אם לא זוהה אדם
        print("לא זוהה אדם בתמונה הנוכחית על ידי YOLO לאחר המסכה. מדלג על שלבי עיבוד נוספים לאדם זה.")
        return
    # חיתוך נוסף לפי תזוזה (לאחר יולו)
    image, imagePath ,folder_txt_path= cut_according_to_moving.cut_according_to_moving(imagePath, valuesForThread, folder_txt_path)

    # הרצת זיהוי תנוחות
    run_yolo_signs.run_YS_by_thread(imagePath, modelSignPath, 0.0, folder_txt_path)

    # המתנה ממושכת לצורך סנכרון/בדיקה
    sleep(2.0)

    # הגדלת המסכה
    image_name = os.path.basename(imagePath)  # לדוגמה: masked_image_masked_image_frame_1_detection_ _detection_ .jpg
    base_image_name = os.path.splitext(image_name)[0]  # הסרת הסיומת
    expanded_coords_filename = f"expanded_roi_coords_{base_image_name}.txt"
    expanded_coords_filepath = os.path.join(folder_txt_path, expanded_coords_filename)

    # קריאה לפונקציה עם הנתיב המלא לקובץ
    increase_mask.expand_yolo_coords_and_save(valuesForThread, expanded_coords_filepath, 0.25)

    #increase_mask.expand_yolo_coords_and_save(valuesForThread, folder_txt_path, 0.25)


if __name__ == "__main__":
    '''''
    if len(sys.argv) != 5:
        print("Usage: func.py <output_path> <values_str> <sign_model_path> <person_model_path>", file=sys.stderr)
        sys.exit(1)

    person_specific_output_path = sys.argv[1]
    valuesForThread_str = sys.argv[2]
    modelSignPath = sys.argv[3]
    modelPersonPath = sys.argv[4]
    '''''
    person_specific_output_path = r"C:\Users\USER\Desktop\py"  # תיקיית פלט
    valuesForThread_str = "0.432303 , 0.35096 , 0.237041 , 0.698949" # מחרוזת של ערכים מופרדים בפסיקים
    modelSignPath =r"C:\Users\USER\Desktop\PROJECT\NEWYOLO\best.pt"
    modelPersonPath = r"C:\Users\USER\Desktop\PROJECT\yoloperson\best.pt"

    Image_processing(person_specific_output_path, valuesForThread_str, modelSignPath, modelPersonPath)

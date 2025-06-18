import os
import sys
import cv2  # ייבוא ספריית OpenCV
import numpy as np  # ייבוא numpy לטיפול במערכי תמונה


def expand_yolo_coords_and_save(image_path: str, valuesForThread: list, output_filepath_txt: str,output_filepath_img: str, expand_percentage: float = 0.25) -> list:

    if not (isinstance(valuesForThread, (list, tuple)) and len(valuesForThread) == 4):
        print("שגיאה: yolo_coords חייב להיות רשימה/טורפל של 4 ערכים (center_x, center_y, width, height).",
              file=sys.stderr)
        return None

    try:
        x_center_norm, y_center_norm, width_norm, height_norm = valuesForThread

        # הגדלת רוחב וגובה באחוז הנתון
        expanded_width_norm = width_norm * (1 + expand_percentage)
        expanded_height_norm = height_norm * (1 + expand_percentage)

        # המרכז נשאר זהה
        expanded_center_x_norm = x_center_norm
        expanded_center_y_norm = y_center_norm

        # חישוב קואורדינטות קצה עבור תיקון גבולות בטווח נורמלי
        x_min_exp = expanded_center_x_norm - expanded_width_norm / 2
        y_min_exp = expanded_center_y_norm - expanded_height_norm / 2
        x_max_exp = expanded_center_x_norm + expanded_width_norm / 2
        y_max_exp = expanded_center_y_norm + expanded_height_norm / 2

        # תיקון גבולות (לוודא שהטווח נשאר ב-[0,1])
        # אם חורג מגבול תחתון (0), הזז את כל התיבה ימינה/למטה
        if x_min_exp < 0:
            x_max_exp -= x_min_exp  # הגדל את x_max_exp באותו ערך שלילי של x_min_exp
            x_min_exp = 0
        if y_min_exp < 0:
            y_max_exp -= y_min_exp  # הגדל את y_max_exp באותו ערך שלילי של y_min_exp
            y_min_exp = 0

        # אם חורג מגבול עליון (1), הזז את כל התיבה שמאלה/למעלה
        if x_max_exp > 1:
            x_min_exp -= (x_max_exp - 1)  # הקטן את x_min_exp באותו ערך חיובי של החריגה
            x_max_exp = 1
        if y_max_exp > 1:
            y_min_exp -= (y_max_exp - 1)  # הקטן את y_min_exp באותו ערך חיובי של החריגה
            y_max_exp = 1

        # חישוב מחדש של מרכז, רוחב וגובה לאחר תיקון הגבולות
        expanded_center_x_norm = (x_min_exp + x_max_exp) / 2
        expanded_center_y_norm = (y_min_exp + y_max_exp) / 2
        expanded_width_norm = (x_max_exp - x_min_exp)
        expanded_height_norm = (y_max_exp - y_min_exp)

        # ודא שמידות הרוחב והגובה חיוביות (קריטי אם היה תיקון גבולות דרסטי שגרם ל-x_min_exp > x_max_exp למשל)
        expanded_width_norm = max(0.0, expanded_width_norm)
        expanded_height_norm = max(0.0, expanded_height_norm)

        expanded_yolo_coords = [expanded_center_x_norm, expanded_center_y_norm,
                                expanded_width_norm, expanded_height_norm]

        # --- שמירת הקואורדינטות המוגדלות לקובץ TXT ---
        output_dir_txt = os.path.dirname(output_filepath_txt)
        if output_dir_txt and not os.path.exists(
                output_dir_txt):  # בדוק אם output_dir_txt אינו ריק (למקרה שהנתיב הוא רק שם קובץ)
            os.makedirs(output_dir_txt)

        try:
            with open(output_filepath_txt, 'w') as f:  # 'w' דורס את הקובץ
                # פורמט YOLO: class_id x_center y_center width height (class_id 0 for person)
                f.write(
                    f"0 {expanded_center_x_norm:.6f} {expanded_center_y_norm:.6f} {expanded_width_norm:.6f} {expanded_height_norm:.6f}\n")
            print(f"קואורדינטות ה-ROI המוגדל נשמרו ב: {output_filepath_txt}")
        except Exception as e:
            print(f"שגיאה בשמירת קואורדינטות ה-ROI המוגדל לקובץ {output_filepath_txt}: {e}", file=sys.stderr)
            return None  # במקרה של שגיאת כתיבה, נחזיר None

        # --- ציור התיבה המוגדלת על התמונה ושמירתה ---
        try:
            image = cv2.imread(image_path)
            if image is None:
                raise FileNotFoundError(f"שגיאה: לא ניתן לטעון את התמונה בנתיב: {image_path}")

            img_height, img_width = image.shape[:2]

            # המרת קואורדינטות נורמליות לפיקסלים
            x_min_pixel = int((expanded_center_x_norm - expanded_width_norm / 2) * img_width)
            y_min_pixel = int((expanded_center_y_norm - expanded_height_norm / 2) * img_height)
            x_max_pixel = int((expanded_center_x_norm + expanded_width_norm / 2) * img_width)
            y_max_pixel = int((expanded_center_y_norm + expanded_height_norm / 2) * img_height)

            # וודא שהקואורדינטות של הפיקסלים בתוך גבולות התמונה
            x_min_pixel = max(0, x_min_pixel)
            y_min_pixel = max(0, y_min_pixel)
            x_max_pixel = min(img_width, x_max_pixel)
            y_max_pixel = min(img_height, y_max_pixel)

            # ציור התיבה המוגדלת (בצבע כחול לדוגמה, עובי 2 פיקסלים)
            cv2.rectangle(image, (x_min_pixel, y_min_pixel), (x_max_pixel, y_max_pixel), (255, 0, 0), 2)

            # יצירת תיקיית הפלט לתמונה אם אינה קיימת
            output_dir_img = os.path.dirname(output_filepath_img)
            if output_dir_img and not os.path.exists(output_dir_img):
                os.makedirs(output_dir_img)

            # שמירת התמונה עם התיבה המוגדלת
            cv2.imwrite(output_filepath_img, image)
            print(f"תמונה עם תיבת ROI מוגדלת נשמרה ב: {output_filepath_img}")

        except FileNotFoundError as fnf_e:
            print(f"שגיאת קובץ בעת טיפול בתמונה: {fnf_e}", file=sys.stderr)
            return None
        except Exception as e:
            print(f"שגיאה במהלך ציור ושמירת התמונה: {e}", file=sys.stderr)
            import traceback
            traceback.print_exc(file=sys.stderr)
            return None

    except Exception as e:
        print(f"שגיאה במהלך עיבוד קואורדינטות YOLO: {e}", file=sys.stderr)
        import traceback  # ייבוא traceback לטיפול שגיאות מפורט יותר
        traceback.print_exc(file=sys.stderr)
        expanded_yolo_coords = None

    return expanded_yolo_coords


if __name__ == "__main__":
    # *** הגדר כאן את הפרמטרים לבדיקה ישירה ***
    # קואורדינטות YOLO לדוגמה (center_x, center_y, width, height)
    my_values_for_thread = [0.432303, 0.35096, 0.237041, 0.698949]

    # נתיב לתמונה המקורית שעליה נצייר.
    # *** חשוב: שנה את הנתיב הזה לנתיב של תמונה אמיתית וקיימת במערכת שלך! ***
    my_image_path = r"C:\Users\USER\Desktop\py\out_yolo_after_mask_mask_only\masked_image_with_bbox_masked_image_frame_1_detection_ _provided.jpg"

    # נתיב לקובץ הפלט שישמר את הקואורדינטות המוגדלות בפורמט TXT
    output_dir = "C:/Users/USER/Desktop/py/output_expanded_coords"
    output_filename_txt = "expanded_roi_coords_test.txt"
    my_output_filepath_txt = os.path.join(output_dir, output_filename_txt)

    # נתיב לקובץ התמונה החדש שייווצר עם התיבה המוגדלת
    output_filename_img = "expanded_roi_image_with_bbox_test.jpg"
    my_output_filepath_img = os.path.join(output_dir, output_filename_img)

    # אחוז ההגדלה
    my_expand_percentage = 0.25  # 25%


    # קריאה לפונקציה. שימו לב לפרמטרים החדשים!
    expanded_coords = expand_yolo_coords_and_save(
        my_image_path,  # נתיב לתמונה המקורית
        my_values_for_thread,
        my_output_filepath_txt,
        my_output_filepath_img,  # נתיב לשמירת התמונה החדשה עם התיבה
        my_expand_percentage
    )

    if expanded_coords:
        print("\nהגדלת קואורדינטות ושמירת תמונה הסתיימו בהצלחה.")
        print(f"קואורדינטות מוגדלות: {expanded_coords}")
    else:
        print("\nהגדלת קואורדינטות או שמירת תמונה נכשלו.")
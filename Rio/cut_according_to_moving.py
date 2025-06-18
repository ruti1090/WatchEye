import cv2
import numpy as np
import os
import sys
import traceback


def cut_according_to_moving(image_path: str, valuesForThread: list, output_directory: str):
    txt_filepath = None
    output_path = None

    try:
        image = cv2.imread(image_path)
        if image is None:
            raise FileNotFoundError(f"לא ניתן לטעון את התמונה בנתיב: {image_path}")
        img_height, img_width = image.shape[:2]

        if not os.path.exists(output_directory):
            os.makedirs(output_directory, exist_ok=True)

        image_base_name = os.path.splitext(os.path.basename(image_path))[0]

        if not isinstance(valuesForThread, list) or len(valuesForThread) < 4:
            raise ValueError(
                "valuesForThread חייב להיות רשימה עם לפחות 4 ערכים (center_x, center_y, width, height).")

        # *** שימו לב לתיקון כאן: השמות צריכים להתאים לסדר ברשימה valuesForThread ***
        center_x = valuesForThread[0]
        center_y = valuesForThread[1]
        roi_width = valuesForThread[2]  # זהו ה-width
        roi_height = valuesForThread[3] # זהו ה-height

        # המרת קואורדינטות נורמליות לקואורדינטות פיקסל
        x_center_pixel = int(center_x * img_width)
        y_center_pixel = int(center_y * img_height)
        width_pixel = int(roi_width * img_width)
        height_pixel = int(roi_height * img_height)

        # חישוב קואורדינטות הפינות של ה-ROI המקורי בפיקסלים
        x_min_orig = int(x_center_pixel - width_pixel / 2)
        y_min_orig = int(y_center_pixel - height_pixel / 2)
        x_max_orig = int(x_center_pixel + width_pixel / 2)
        y_max_orig = int(y_center_pixel + height_pixel / 2)

        # וודא שהקואורדינטות בתוך גבולות התמונה
        x_min_orig = max(0, x_min_orig)
        y_min_orig = max(0, y_min_orig)
        x_max_orig = min(img_width, x_max_orig)
        y_max_orig = min(img_height, y_max_orig)

        # יצירת מסכה שחורה (0) בגודל התמונה
        mask = np.zeros(image.shape[:2], dtype=np.uint8)

        # צייר מלבן לבן (255) על המסכה באזור ה-ROI
        cv2.rectangle(mask, (x_min_orig, y_min_orig), (x_max_orig, y_max_orig), 255, -1)

        # יישום המסכה על התמונה המקורית
        masked_image = cv2.bitwise_and(image, image, mask=mask)

        # --- שמירת קובץ TXT עם מיקומי ה-ROI המקוריים ---
        txt_filename = f"roi_coords_original_{image_base_name}_detection_.txt"
        txt_filepath = os.path.join(output_directory, txt_filename)

        try:
            # *** שימו לב לתיקון כאן: כתיבת ה-width וה-height בסדר הנכון ***
            with open(txt_filepath, 'w') as f_txt:
                f_txt.write(f"0 {center_x:.6f} {center_y:.6f} {roi_width:.6f} {roi_height:.6f}\n")
            print(f"קואורדינטות ה-ROI המקוריות של זיהוי נשמרו ב: {txt_filepath}")
        except Exception as e:
            print(f"שגיאה בשמירת קואורדינטות ה-ROI המקוריות של זיהוי: {e}", file=sys.stderr)
            traceback.print_exc(file=sys.stderr)

        # --- שמירת התמונה הממוסכת ---
        output_filename = f"masked_image_{image_base_name}_detection_ .jpg"
        output_path = os.path.join(output_directory, output_filename)
        try:
            cv2.imwrite(output_path, masked_image)
            print(f"התמונה הממוסכת של זיהוי נשמרה ב: {output_path}")
        except Exception as e:
            print(f"שגיאה בשמירת התמונה הממוסכת: {e}", file=sys.stderr)
            traceback.print_exc(file=sys.stderr)

        return masked_image, output_path, output_directory


    except FileNotFoundError as e:
        print(f"שגיאה: {e}", file=sys.stderr)
        traceback.print_exc(file=sys.stderr)
        return None, None, None
    except ValueError as ve:
        print(f"שגיאה בערכים: {ve}", file=sys.stderr)
        traceback.print_exc(file=sys.stderr)
        return None, None, None
    except Exception as e:
        print(f"שגיאה כללית בעיבוד התמונה: {e}", file=sys.stderr)
        traceback.print_exc(file=sys.stderr)
        return None, None, None


if __name__ == "__main__":
    my_image_path =r"C:\Users\USER\Desktop\py\frame_6.jpg"
    # הערכים האלה צריכים להתאים לסדר: [center_x, center_y, width, height]
    my_values_for_thread = [0.432303 , 0.436843 , 0.296301 , 0.873686] # ודאי שהסדר כאן נכון
    my_output_directory = "C:/Users/USER/Desktop/py/out"

    print(f"מתחיל עיבוד: תמונה={my_image_path}, ערכים={my_values_for_thread}, תיקיית פלט={my_output_directory}")

    masked_image, new_image_path, new_output_directory = cut_according_to_moving(
        my_image_path, my_values_for_thread, my_output_directory
    )

    if masked_image is not None:
        print("הפעולה 'cut_according_to_moving' הסתיימה בהצלחה.")
        print(f"נתיב התמונה הממוסכת החדש: {new_image_path}")
        print(f"תיקיית הפלט ששימשה: {new_output_directory}")
    else:
        print("הפעולה 'cut_according_to_moving' נכשלה.")
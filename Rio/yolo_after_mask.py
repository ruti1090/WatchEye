import cv2
from ultralytics import YOLO
import numpy as np
import os
import sys

def save_yolo_person_detection_results_txt_simple(person_result, output_dir, image_name, original_image_shape):

    if not person_result or len(person_result.boxes) == 0:
        print(f"לא נמצאו תיבות עבור האדם ב-{image_name}. לא נשמר קובץ TXT של זיהוי אנשים.")
        return

    # ניגש ישירות לנתונים של התיבה התוחמת הראשונה (והיחידה הצפויה)
    box_data = person_result.boxes.data.cpu().numpy()[0]
    x_min, y_min, x_max, y_max, confidence, class_id = box_data

    # בניית נתיב הקובץ (ידרוס קובץ קיים עם אותו שם)
    txt_filename = f"person_detection_results_{os.path.splitext(image_name)[0]}.txt"
    txt_filepath = os.path.join(output_dir, txt_filename)

    try:
        with open(txt_filepath, 'w') as f:
            img_height, img_width = original_image_shape[:2]

            center_x = (x_min + x_max) / 2 / img_width
            center_y = (y_min + y_max) / 2 / img_height
            width = (x_max - x_min) / img_width
            height = (y_max - y_min) / img_height

            f.write(f"{int(class_id)} {center_x:.6f} {center_y:.6f} {width:.6f} {height:.6f} {confidence:.6f}\n")

        print(f"תוצאות זיהוי אדם נשמרו ב: {txt_filepath}")
    except Exception as e:
        print(f"שגיאה בשמירת תוצאות זיהוי אנשים לקובץ TXT: {e}")


def mainRun(image_path, model_people_path, output_dir="output", save_images=True, conf_threshold_people=0.1):

    person_location = None  # אתחול המשתנה שיוחזר

    try:
        # טעינת המודל לזיהוי אנשים
        model_people = YOLO(model_people_path)

        # טעינת התמונה
        image = cv2.imread(image_path)
        if image is None:
            raise FileNotFoundError(f"שגיאה: לא ניתן לטעון את התמונה בנתיב: {image_path}")

        original_image_shape = image.shape  # נשמור את מימדי התמונה המקורית

        # יצירת ספריית פלט אם אינה קיימת
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        print(f"Output directory: {output_dir}")

        # הרצת המודל לזיהוי אנשים (ללא max_det=1, בהנחה שיש אדם בודד)
        results_people = model_people.predict(image, conf=conf_threshold_people, classes=[0], verbose=False)

        # בדיקה אם זוהה אדם כלשהו
        if results_people and len(results_people) > 0 and \
                results_people[0].boxes is not None and len(results_people[0].boxes) > 0:

            person_result = results_people[0]  # ניקח את תוצאת הזיהוי הראשונה (והיחידה הצפויה)

            # שמירת קובץ ה-TXT עם מיקום האדם
            save_yolo_person_detection_results_txt_simple(person_result, output_dir, os.path.basename(image_path),
                                                          original_image_shape)

            # קבלת נתוני התיבה התוחמת של האדם
            box = person_result.boxes[0]  # התיבה של האדם
            x_min, y_min, x_max, y_max = map(int, box.xyxy[0].cpu().numpy())

            # וודא שהקואורדינטות חוקיות בגבולות התמונה
            x_min = max(0, x_min)
            y_min = max(0, y_min)
            x_max = min(image.shape[1], x_max)
            y_max = min(image.shape[0], y_max)

            # חישוב קואורדינטות YOLO לערך המוחזר
            img_height, img_width = original_image_shape[:2]
            center_x = (x_min + x_max) / 2 / img_width
            center_y = (y_min + y_max) / 2 / img_height
            width = (x_max - x_min) / img_width
            height = (y_max - y_min) / img_height

            person_location = [center_x, center_y, width, height]

            # שמירת התמונה המקורית עם תיבת הזיהוי המצוירת (אם save_images True)
            if save_images:
                # יצירת עותק של התמונה המקורית כדי לצייר עליו
                image_with_bbox = image.copy()
                # ציור התיבה התוחמת בצבע כלשהו (לדוגמה, ירוק) ובעובי קו
                cv2.rectangle(image_with_bbox, (x_min, y_min), (x_max, y_max), (0, 255, 0), 2)  # צבע ירוק, עובי 2

                # בניית נתיב הקובץ (ידרוס קובץ קיים)
                output_image_filename = f"detected_person_bbox_{os.path.splitext(os.path.basename(image_path))[0]}.jpg"
                output_image_path = os.path.join(output_dir, output_image_filename)
                cv2.imwrite(output_image_path, image_with_bbox)
                print(f"תמונה עם תיבת זיהוי נשמרה ב: {output_image_path}")
        else:
            print("לא זוהה אף אדם בתמונה, או שהזיהוי לא עמד בסף הביטחון.")
            return None
        # --- לוגיקת "שטחים מתים" (ללא שמירת קבצים, מבוצעת פנימית) ---
        # יצירת מסכה שחורה על הכל חוץ מהאדם - לצורך חישוב השטח המת
        full_mask_for_inverted = np.zeros(image.shape[:2], dtype=np.uint8)
        if person_location:  # אם זוהה אדם, נשתמש בתיבה שלו כדי ליצור את המסכה
            # המרת קואורדינטות YOLO לפיקסלים עבור יצירת המסכה
            x_min_mask = int(person_location[0] * image.shape[1] - person_location[2] * image.shape[1] / 2)
            y_min_mask = int(person_location[1] * image.shape[0] - person_location[3] * image.shape[0] / 2)
            x_max_mask = int(person_location[0] * image.shape[1] + person_location[2] * image.shape[1] / 2)
            y_max_mask = int(person_location[1] * image.shape[0] + person_location[3] * image.shape[0] / 2)

            # ודא שהקואורדינטות של המסכה חוקיות
            x_min_mask = max(0, x_min_mask)
            y_min_mask = max(0, y_min_mask)
            x_max_mask = min(image.shape[1], x_max_mask)
            y_max_mask = min(image.shape[0], y_max_mask)

            cv2.rectangle(full_mask_for_inverted, (x_min_mask, y_min_mask), (x_max_mask, y_max_mask), 255, -1)

        inverted_mask = cv2.bitwise_not(full_mask_for_inverted)  # היפוך המסכה (שחור על האדם, לבן מסביב)

        # חישוב שטח האזורים הלא מזוהים (ללא שמירה לקובץ)
        dead_space_area = np.sum(inverted_mask == 255)
        print(f"שטח האזורים הלא מזוהים (מספר פיקסלים): {dead_space_area} (קבצים הקשורים לשטחים מתים לא נשמרו).")

    except FileNotFoundError as fnf_e:
        print(f"שגיאת קובץ: {fnf_e}", file=sys.stderr)
        person_location = None  # במקרה של שגיאה, נחזיר None
    except Exception as e:
        print(f"שגיאה במהלך עיבוד התמונה: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc(file=sys.stderr)
        person_location = None  # במקרה של שגיאה, נחזיר None

    return person_location

if __name__ == "__main__":
    # *** הגדר כאן את הפרמטרים שלך ***
    my_image_path = "C:/Users/USER/Desktop/py/masked_image_frame_1_detection_ .jpg" # נתיב לתמונת הקלט (כאן: תמונה ממוסכת)
    my_model_people_path = "C:/Users/USER/Desktop/PROJECT/yoloperson/best.pt"  # נתיב למודל YOLO לזיהוי אנשים
    my_output_directory = "C:/Users/USER/Desktop/py/out_yolo_after_mask" # תיקיית פלט ייעודית עבור פלט זה

    # ניתן לשנות את אלה אם צריך
    my_save_images = True
    my_conf_threshold_people = 0.4

    print(f"מתחיל הרצת yolo_after_mask.py ישירות:")
    print(f"  קלט תמונה: {my_image_path}")
    print(f"  מודל אנשים: {my_model_people_path}")
    print(f"  תיקיית פלט: {my_output_directory}")

    # קריאה לפונקציית mainRun
    detected_person_coords = mainRun(
        my_image_path,
        my_model_people_path,
        output_dir=my_output_directory,
        save_images=my_save_images,
        conf_threshold_people=my_conf_threshold_people
    )

    if detected_person_coords:
        print("\nזיהוי אדם הסתיים בהצלחה.")
        print(f"קואורדינטות האדם שזוהה (center_x, center_y, width, height): {detected_person_coords}")
    else:
        print("\nלא זוהה אדם או שהתרחשה שגיאה במהלך הזיהוי.")

#הרצת המודל לזיוהי אנשים בלבד

import cv2
import numpy as np
import os
from ultralytics import YOLO


def run_yp_by_thread(image_path, model_path, conf, classes, output_dir):
    try:
        # טעינת המודל
        model = YOLO(model_path)

        results = model.predict(image_path, conf=conf, classes=classes, save=False)

        # יצירת ספריית הפלט אם אינה קיימת
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        # קביעת שם קובץ ה-TXT
        image_name = os.path.basename(image_path)
        base_image_name = os.path.splitext(image_name)[0]
        txt_filename = f"yolo_detection_results_{base_image_name}.txt"
        txt_filepath = os.path.join(output_dir, txt_filename)

        # פתיחת הקובץ לכתיבה
        with open(txt_filepath, 'w') as f:
            # מעבר על כל אובייקט תוצאה (פריים בודד במקרה של תמונה)
            for r in results:
                # בדיקה אם יש תיבות תוחמות (זיהויים) בתוצאה הנוכחית
                if r.boxes is not None:
                    # מעבר על כל תיבה תוחמת שזוהתה
                    for box in r.boxes:
                        # גישה לנתוני התיבה בפורמט נורמלי (x_center, y_center, width, height)
                        x_center_norm, y_center_norm, width_norm, height_norm = box.xywhn[0].cpu().numpy()

                        # כתיבת הנתונים לקובץ בפורמט YOLO (רק מרכז, רוחב, גובה)
                        # פורמט: x_center y_center width height
                        f.write(f"{x_center_norm:.6f} {y_center_norm:.6f} {width_norm:.6f} {height_norm:.6f}\n")
                else:
                    f.write(f"לא נמצאו זיהויים בתמונה: {image_name}\n")

        print(f"תוצאות זיהוי YOLO (מרכז, רוחב, גובה) נשמרו בהצלחה ב: {txt_filepath}")

        # --- שמירת התמונה עם הזיהויים ---
        # טעינת התמונה המקורית שוב כדי לצייר עליה
        original_image_for_drawing = cv2.imread(image_path)
        if original_image_for_drawing is None:
            print(f"אזהרה: לא ניתן לטעון את התמונה המקורית לצורך ציור: {image_path}")
            return  # יציאה מהפונקציה אם התמונה לא נטענה

        for r in results:
            # r.plot() מצייר את התיבות התוחמות והתוויות על התמונה.
            # הוא מחזיר את התמונה עם הציורים.
            im_with_detections = r.plot()

            # שמירת התמונה עם הזיהויים
            detection_image_filename = f"detected_image_{base_image_name}.jpg"
            detection_image_filepath = os.path.join(output_dir, detection_image_filename)
            cv2.imwrite(detection_image_filepath, im_with_detections)
            print(f"התמונה עם הזיהויים נשמרה ב: {detection_image_filepath}")
            break  # אם מעבדים רק תמונה אחת, אין צורך להמשיך בלולאה

    except FileNotFoundError as e:
        print(f"שגיאה: {e}")
    except Exception as e:
        print(f"שגיאה במהלך הרצת YOLO או שמירת תוצאות: {e}")


if __name__ == '__main__':
    #נתיב לתמונה
    image_path=r"C:\Users\USER\Desktop\1.png"
    #נתיב למודל
    model_path=r"C:\Users\USER\Desktop\PROJECT\yoloperson\best.pt"
    #נתיב לתיקייה לשמירת התוצאות
    output_dir=r"C:\Users\USER\Desktop\PROJECT\YPresults"
    #הגדרת סף הביטחון
    conf=0.4
    #הגדרת הקלאסים לזיהוי
    classes=[0]
    #זימון הפונקציה להרצת המודל
    run_yp_by_thread(image_path,model_path,conf,classes,output_dir)


# הרצה ראשונית של המודל לזיהוי אנשים ושטחים מתים

import cv2
from ultralytics import YOLO
import numpy as np
import os
import sys
import time  # ייבוא חדש לטיפול במחיקה


# עיבוד השטח המת
def process_single_empty_space_region(region_image, region_id, bounding_box_coords, original_image_shape):
    # 2. שמירת האזור החתוך לקובץ נפרד (לצורך בדיקה):
    output_folder = os.path.join("output", "processed_empty_spaces_regions")
    os.makedirs(output_folder, exist_ok=True)
    # לוודא שהתמונה אינה ריקה לפני השמירה
    if region_image.size > 0 and region_image.shape[0] > 0 and region_image.shape[1] > 0:
        cv2.imwrite(os.path.join(output_folder, f"empty_space_region_{region_id}.jpg"), region_image)
    else:
        pass


# שמירת תוצאות זיהוי אנשים ל-TXT
def save_detection_and_unidentified_results_txt(person_results, unidentified_regions_stats, output_dir, image_name,
                                                original_image_shape):
    txt_filename = f"empty_spaces_{os.path.splitext(image_name)[0]}.txt"
    txt_filepath = os.path.join(output_dir, txt_filename)
    try:
        with open(txt_filepath, 'w') as f:
            f.write("אזורים לא מזוהים (שטחים ריקים):\n")
            if unidentified_regions_stats is not None and unidentified_regions_stats[0] > 1:
                img_height, img_width = original_image_shape[:2]

                for i in range(1, unidentified_regions_stats[0]):
                    x, y, w, h, area_pixels = unidentified_regions_stats[2][i]
                    center_x_pixel, center_y_pixel = unidentified_regions_stats[3][i]

                    # לא קיים סף סינון, כל האזורים נשמרים
                    # המרת קואורדינטות לפורמט YOLO (נורמליזציה)
                    center_x_norm = center_x_pixel / img_width
                    center_y_norm = center_y_pixel / img_height
                    width_norm = w / img_width
                    height_norm = h / img_height

                    f.write(
                        f"  {99} {center_x_norm:.6f} {center_y_norm:.6f} {width_norm:.6f} {height_norm:.6f} {int(area_pixels)}\n")

            else:
                f.write("  לא נמצאו אזורים לא מזוהים משמעותיים.\n")

        print(f"תוצאות זיהוי ושטחים לא מזוהים נשמרו ב: {txt_filepath}")
    except Exception as e:
        print(f"שגיאה בשמירת תוצאות ל-TXT: {e}")


# שמירת תוצאות הרצת זיהוי אנשים - מותאמת לאדם בודד
def save_yolo_person_detection_results_txt_simple(person_result, output_dir, image_name, original_image_shape):
    # אם אין תוצאה או אין תיבות, אין מה לשמור.
    # אנחנו מצפים ל-person_result יחיד עם תיבה בודדת.
    if not person_result or len(person_result.boxes) == 0:
        print(f"לא נמצאו תיבות עבור האדם ב-{image_name}. לא נשמר קובץ TXT של זיהוי אנשים.")
        return

    # נקבל את הנתונים מהתיבה התוחמת הראשונה (והיחידה הצפויה)
    box_data = person_result.boxes.data.cpu().numpy()[0]  # **שינוי: ניגש ישירות לאיבר הראשון**
    print(f"person_result type: {type(person_result)}")
    if person_result:
        print(f"person_result has boxes attribute: {'boxes' in dir(person_result)}")
        if person_result.boxes:
            print(f"person_result.boxes type: {type(person_result.boxes)}")
            print(f"Number of boxes: {len(person_result.boxes)}")
            if len(person_result.boxes) > 0:
                print(f"First box data (raw): {person_result.boxes.data}")
    x_min, y_min, x_max, y_max, confidence, class_id = box_data

    txt_filename = f"person_detection_results_{os.path.splitext(image_name)[0]}.txt"  # **שינוי: שם קובץ כללי יותר ללא אינדקס אדם**
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


# הרצת זיהוי אנשים
def run_Y_P_with_mask(image_path, model_people_path, output_dir="output", save_images=True, conf_threshold_people=0.4):
    person_location = None  # משתנה בודד לאחסון מיקום אדם אחד
    dead_space_image_created = False  # משתנה חדש לציון אם קובץ שטח מת נוצר
    try:
        model_people = YOLO(model_people_path)
        image = cv2.imread(image_path)
        if image is None:
            raise FileNotFoundError(f"לא ניתן לטעון את התמונה בנתיב: {image_path}")
        original_image_shape = image.shape

        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        print(f"Output directory: {output_dir}")

        # אתחול המסכה המלאה עם אפסים (שחור), בגודל התמונה המקורית
        full_mask = np.zeros(image.shape[:2], dtype=np.uint8)

        # הגבלת מספר האנשים שזוהו לאחד בלבד
        results_people = model_people.predict(image, conf=conf_threshold_people, classes=[0], max_det=1)

        # וודא שיש תוצאות ושהן מכילות תיבות תוחמות
        if results_people and len(results_people) > 0 and results_people[0].boxes is not None and len(
                results_people[0].boxes) > 0:
            person_result = results_people[0]  # ניקח רק את התוצאה הראשונה (היחידה)

            save_yolo_person_detection_results_txt_simple(person_result, output_dir, os.path.basename(image_path),
                                                          original_image_shape)

            # מכיוון שאנו מצפים לאדם אחד, ניקח את התיבה הראשונה שלו (אם יש מספר תיבות לאדם בודד)
            box = person_result.boxes[0]
            x_min, y_min, x_max, y_max = map(int, box.xyxy[0].cpu().numpy())

            x_min = max(0, x_min)
            y_min = max(0, y_min)
            x_max = min(image.shape[1], x_max)
            y_max = min(image.shape[0], y_max)

            # חישוב קואורדינטות YOLO ועדכון המשתנה person_location
            img_height, img_width = original_image_shape[:2]
            center_x = (x_min + x_max) / 2 / img_width
            center_y = (y_min + y_max) / 2 / img_height
            width = (x_max - x_min) / img_width
            height = (y_max - y_min) / img_height

            person_location = [center_x, center_y, width, height]  # עדכון המשתנה הבודד

            # ציור מלבן על המסכה במיקום האדם (אזור שחור)
            cv2.rectangle(full_mask, (x_min, y_min), (x_max, y_max), 255, -1)  # צובע את אזור האדם בלבן על המסכה

            if save_images:
                person_mask = np.zeros(image.shape[:2], dtype=np.uint8)
                cv2.rectangle(person_mask, (x_min, y_min), (x_max, y_max), 255, -1)
                masked_person_image = cv2.bitwise_and(image, image, mask=person_mask)
                masked_image_path = os.path.join(output_dir,
                                                 f"masked_person_image_{os.path.splitext(os.path.basename(image_path))[0]}.jpg")
                cv2.imwrite(masked_image_path, masked_person_image)
                print(f"תמונה ממוסכת של אדם נשמרה ב: {masked_image_path}")
        else:
            print("לא זוהה אף אדם בתמונה, או שהזיהוי לא עמד בסף הביטחון.")
            # במקרה זה, full_mask נשאר כולו אפסים (שחור), כפי שאותחל.
            # זה אומר שהמסכה ההפוכה (inverted_mask) תהיה כל התמונה לבנה (כלומר, כל התמונה היא "שטח מת").

        # יצירת המסכה ההפוכה:
        # אם זוהה אדם, האזור שלו במסכה יהיה לבן, והיפוך יהפוך אותו לשחור (non-dead space).
        # אם לא זוהה אדם, full_mask הוא כולו שחור, והיפוך יהפוך אותו לכולו לבן (כל התמונה היא dead space).
        inverted_mask = cv2.bitwise_not(full_mask)

        # יצירת תמונת השטח המת:
        # אם זוהה אדם, זה יהיה כל מה שמחוץ למסכה שלו.
        # אם לא זוהה אדם, זה יהיה כל התמונה המקורית.
        dead_space_regions = cv2.bitwise_and(image, image, mask=inverted_mask)

        num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(inverted_mask, 8, cv2.CV_32S)
        unidentified_regions_for_txt = (num_labels, labels, stats, centroids)

        # שמירת קובץ 'dead_space_image.jpg' תמיד, ללא תלות בזיהוי אדם
        if save_images:
            dead_space_image_path = os.path.join(output_dir, "dead_space_image.jpg")
            cv2.imwrite(dead_space_image_path, dead_space_regions)
            print(f"תמונה של שטחים לא מזוהים (או התמונה המלאה) נשמרה ב: {dead_space_image_path}")
            dead_space_image_created = True  # עדכון המשתנה החדש

        dead_space_area = np.sum(inverted_mask == 255)
        print(f"שטח האזורים הלא מזוהים (מספר פיקסלים): {dead_space_area}")

        # save_detection_and_unidentified_results_txt(results_people, unidentified_regions_for_txt, output_dir, os.path.basename(image_path), original_image_shape)
        if num_labels > 1:
            for i in range(1, num_labels):
                x, y, w, h, area = stats[i]
                x = max(0, x)
                y = max(0, y)
                w = min(w, image.shape[1] - x)
                h = min(h, image.shape[0] - y)
                if w <= 0 or h <= 0:
                    continue
                region_of_interest_image = image[y: y + h, x: x + w]
                process_single_empty_space_region(region_of_interest_image, i, (x, y, w, h), original_image_shape)
    except Exception as e:
        print(f"שגיאה במהלך עיבוד התמונה: {e}")
        import traceback
        traceback.print_exc(file=sys.stderr)

    return person_location, dead_space_image_created  # החזר גם את המצב של קובץ השטח המת


'''
if __name__ == "__main__":
    print("begining the python file")
    if len(sys.argv) == 4:
        image_file = sys.argv[1]
        model_people_file = sys.argv[2]
        output_directory = sys.argv[3]
        save_masked = True
        conf_threshold_people = 0.4

        try:
            detected_person_location, dead_space_created = run_Y_P_with_mask(image_file, model_people_file, output_directory, save_masked, conf_threshold_people)
            if detected_person_location:
                print("\nמיקום האדם שזוהה (center_x, center_y, width, height):")
                print(detected_person_location)
            else:
                print("לא זוהה אף אדם בתמונה העומדת בקריטריונים.")

            # עכשיו dead_space_created יהיה תמיד True אם save_images=True ואין שגיאות
            # כי הקובץ dead_space_image.jpg תמיד ייווצר (או עם שטח מת או עם התמונה כולה)

            sys.exit(0)
        except Exception as e:
            print(f"שגיאה קריטית ב-YOLOPERSON.py: {e}", file=sys.stderr)
            sys.exit(1)
    else:
        print("Usage: python YOLOPERSON.py <image_path> <model_people_path> <output_dir>", file=sys.stderr)
        sys.exit(1)
'''
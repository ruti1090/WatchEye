import cv2
from ultralytics import YOLO
import numpy as np
import os
import sys
import glob

def process_single_empty_space_region(region_image, region_id, bounding_box_coords, original_image_shape):
    output_folder = os.path.join("output", "processed_empty_spaces_regions")
    os.makedirs(output_folder, exist_ok=True)
    if region_image.size > 0 and region_image.shape[0] > 0 and region_image.shape[1] > 0:
        cv2.imwrite(os.path.join(output_folder, f"empty_space_region_{region_id}.jpg"), region_image)
    else:
        pass
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

def save_yolo_person_detection_results_txt_simple(person_result, output_dir, image_name, i, original_image_shape):
    if not person_result or len(person_result.boxes) == 0:
        print(f"לא נמצאו תיבות עבור אדם {i} ב-{image_name}. לא נשמר קובץ TXT של זיהוי אנשים.")
        return False

    txt_filename = f"person_detection_results_{i}.txt"
    txt_filepath = os.path.join(output_dir, txt_filename)

    try:
        with open(txt_filepath, 'w') as f:
            for box_data in person_result.boxes.data.cpu().numpy():
                x_min, y_min, x_max, y_max, confidence, class_id = box_data
                img_height, img_width = original_image_shape[:2]
                center_x = (x_min + x_max) / 2 / img_width
                center_y = (y_min + y_max) / 2 / img_height
                width = (x_max - x_min) / img_width
                height = (y_max - y_min) / img_height
                f.write(f"{int(class_id)} {center_x:.6f} {center_y:.6f} {width:.6f} {height:.6f} {confidence:.6f}\n")

        print(f"תוצאות זיהוי אדם {i} נשמרו ב: {txt_filepath}")
        return True
    except Exception as e:
        print(f"שגיאה בשמירת תוצאות זיהוי אנשים לקובץ TXT: {e}")
        return False


def runYoloCN(image_path, model_people_path, output_dir="output", save_images=True, conf_threshold_people=0.4):

    # ניקוי קבצי תוצאות קודמים לפני הרצה זו
    print(f"מנקה קבצים קודמים מ: {output_dir}")
    existing_txt_files = glob.glob(os.path.join(output_dir, "person_detection_results_*.txt"))
    for f in existing_txt_files:
        try:
            os.remove(f)
            print(f"נמחק קובץ תוצאה קודם: {f}")
        except OSError as e:
            print(f"שגיאה במחיקת קובץ {f}: {e}", file=sys.stderr)

    try:
        model_people = YOLO(model_people_path)
        image = cv2.imread(image_path)
        if image is None:
            raise FileNotFoundError(f"לא ניתן לטעון את התמונה בנתיב: {image_path}")
        original_image_shape = image.shape
        results_people = model_people.predict(image, conf=conf_threshold_people, classes=[0])

        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        print(f"Output directory: {output_dir}")

        dead_space_image = np.zeros_like(image)
        full_mask = np.zeros(image.shape[:2], dtype=np.uint8)

        # דגל שיקבל True אם נוצר לפחות קובץ זיהוי אחד
        at_least_one_person_file_created = False

        for i, person_result in enumerate(results_people):
            # שומרים את תוצאת הפונקציה save_yolo_person_detection_results_txt_simple
            file_was_created_for_this_person = save_yolo_person_detection_results_txt_simple(person_result, output_dir, os.path.basename(image_path), i, original_image_shape)
            if file_was_created_for_this_person:
                at_least_one_person_file_created = True

            if person_result.boxes is not None and len(person_result.boxes) > 0:
                for j, box in enumerate(person_result.boxes):
                    x_min, y_min, x_max, y_max = map(int, box.xyxy[0].cpu().numpy())

                    x_min = max(0, x_min)
                    y_min = max(0, y_min)
                    x_max = min(image.shape[1], x_max)
                    y_max = min(image.shape[0], y_max)

                    cv2.rectangle(full_mask, (x_min, y_min), (x_max, y_max), 255, -1)

                    if save_images:
                        person_mask = np.zeros(image.shape[:2], dtype=np.uint8)
                        cv2.rectangle(person_mask, (x_min, y_min), (x_max, y_max), 255, -1)
                        masked_person_image = cv2.bitwise_and(image, image, mask=person_mask)
                        masked_image_path = os.path.join(output_dir, f"masked_person_image_{i}_{j}.jpg")
                        cv2.imwrite(masked_image_path, masked_person_image)
                        print(f"תמונה ממוסכת של אדם {i}-{j} נשמרה ב: {masked_image_path}")

        inverted_mask = cv2.bitwise_not(full_mask)
        dead_space_regions = cv2.bitwise_and(image, image, mask=inverted_mask)
        num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(inverted_mask, 8, cv2.CV_32S)
        unidentified_regions_for_txt = (num_labels, labels, stats, centroids)

        if save_images:
            dead_space_image_path = os.path.join(output_dir, "dead_space_image.jpg")
            cv2.imwrite(dead_space_image_path, dead_space_regions)
            print(f"תמונה של שטחים לא מזוהים נשמרה ב: {dead_space_image_path}")

        dead_space_area = np.sum(inverted_mask == 255)
        print(f"שטח האזורים הלא מזוהים (מספר פיקסלים): {dead_space_area}")

        if num_labels > 1:
            for i in range(1, num_labels):
                x, y, w, h, area = stats[i]
                x = max(0, x)
                y = max(0, y)
                w = min(w, image.shape[1] - x)
                h = min(image.shape[0] - y)
                if w <= 0 or h <= 0:
                    continue
                region_of_interest_image = image[y: y + h, x: x + w]
                process_single_empty_space_region(region_of_interest_image, i, (x, y, w, h), original_image_shape)

        return at_least_one_person_file_created

    except Exception as e:
        print(f"שגיאה במהלך עיבוד התמונה: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc(file=sys.stderr)
        return False

# --- ההפעלה הישירה הממוקדת ---
if len(sys.argv) == 4:
    image_file = sys.argv[1]
    model_people_file = sys.argv[2]
    output_directory = sys.argv[3]
    save_masked = True
    conf_threshold_people = 0.4

    try:
        result = runYoloCN(image_file, model_people_file, output_directory, save_masked, conf_threshold_people)
        print(f"PYTHON_RESULT:{'True' if result else 'False'}")
        sys.exit(0)
    except Exception as e:
        print(f"שגיאה קריטית: {e}", file=sys.stderr)
        sys.exit(1)
else:
    print("Usage: python YOUR_SCRIPT_NAME.py <image_path> <model_people_path> <output_dir>", file=sys.stderr)
    sys.exit(1)
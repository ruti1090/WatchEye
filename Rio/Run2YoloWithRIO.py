import cv2
from ultralytics import YOLO
import numpy as np
import os
import json
import subprocess  # ייבוא מודול subprocess


def save_yolo_pose_results_txt(results, output_dir, image_name, i, j, x_person, y_person, w_person, h_person):
    if not results or len(results) == 0:
        print(
            f"לא נמצאו תוצאות עבור {image_name}, אדם {i}, תיבה {j}. לא נשמר קובץ TXT.")
        return

    # יצירת שם קובץ TXT ייחודי
    txt_filename = f"object_results_{i}_{j}.txt"
    txt_filepath = os.path.join(output_dir, txt_filename)

    try:
        with open(txt_filepath, 'w') as f:
            f.write(f"# Person BBox: {x_person:.6f} {y_person:.6f} {w_person:.6f} {h_person:.6f}\n")

            # לולאה על פני האובייקטים שזוהו
            for result in results:
                if result.boxes is not None:
                    boxes_xywh = result.boxes.xywh.cpu().numpy() if hasattr(result.boxes, 'xywh') else []
                    conf_values = result.conf.cpu().numpy() if hasattr(result, 'conf') else [
                                                                                                0.0] * len(boxes_xywh)
                    classes = result.boxes.cls.cpu().numpy() if hasattr(result.boxes, 'cls') else []

                    # Determine the length of the shortest list
                    min_length = min(len(boxes_xywh), len(conf_values), len(classes))

                    for k in range(min_length):
                        # Access elements using the index k
                        x, y, w, h = boxes_xywh[k] if k < len(boxes_xywh) else (0, 0, 0, 0)
                        conf_value = conf_values[k] if k < len(conf_values) else 0.0
                        class_id = int(classes[k]) if k < len(classes) else -1

                        # כתיבת השורה לקובץ TXT
                        f.write(
                            f"{class_id} {x:.6f} {y:.6f} {w:.6f} {h:.6f} {conf_value:.6f}\n")
                else:
                    print(
                        f"לא נמצאו תיבות עבור {image_name}, אדם {i}, תיבה {j}.")

        print(f"תוצאות זיהוי אובייקטים נשמרו ב: {txt_filepath}")
    except Exception as e:
        print(f"שגיאה בשמירת תוצאות זיהוי אובייקטים לקובץ TXT: {e}")

def save_yolo_person_detection_results_txt_simple(person_result, output_dir, image_name, i, original_image_shape):
    if not person_result or len(person_result.boxes) == 0:
        print(f"לא נמצאו תיבות עבור אדם {i} ב-{image_name}. לא נשמר קובץ TXT של זיהוי אנשים.")
        return

    txt_filename = f"person_detection_results_{i}.txt"  # שם קובץ פשוט לזיהוי אנשים
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
    except Exception as e:
        print(f"שגיאה בשמירת תוצאות זיהוי אנשים לקובץ TXT: {e}")


def run_yolo_with_mask(image_path, model_people_path, model_object_path,output_dir="output", save_images=True, conf_threshold_people=0.3):
    try:
        model_people = YOLO(model_people_path)
        model_object = YOLO(model_object_path)

        image = cv2.imread(image_path)
        if image is None:
            raise FileNotFoundError(
                f"לא ניתן לטעון את התמונה בנתיב: {image_path}")

        original_image_shape = image.shape

        results_people = model_people.predict(image, conf=conf_threshold_people, classes=[0])

        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        print(f"Output directory: {output_dir}")

        for i, person_result in enumerate(results_people):
            save_yolo_person_detection_results_txt_simple(person_result, output_dir,
                                                          os.path.basename(image_path), i, original_image_shape)

            for j, box in enumerate(person_result.boxes):
                x_min, y_min, x_max, y_max = map(int, box.xyxy[0].cpu().numpy())

                img_height, img_width = original_image_shape[:2]
                person_center_x_norm = (x_min + x_max) / 2 / img_width
                person_center_y_norm = (y_min + y_max) / 2 / img_height
                person_width_norm = (x_max - x_min) / img_width
                person_height_norm = (y_max - y_min) / img_height

                mask = np.zeros(image.shape[:2], dtype=np.uint8)
                cv2.rectangle(mask, (x_min, y_min), (x_max, y_max), 255, -1)
                masked_image = cv2.bitwise_and(image, image, mask=mask)

                if save_images:
                    masked_image_path = os.path.join(output_dir, f"masked_image_{i}_{j}.jpg")
                    cv2.imwrite(masked_image_path, masked_image)
                    print(f"תמונה ממוסכת נשמרה ב: {masked_image_path}")

                results_object = model_object.predict(masked_image)
                if results_object and len(results_object) > 0:
                    # *** שינוי כאן: העברת קואורדינטות האדם המנורמלות לפונקציה save_yolo_pose_results_txt ***
                    save_yolo_pose_results_txt(results_object, output_dir, os.path.basename(image_path), i, j,
                                               person_center_x_norm, person_center_y_norm, person_width_norm,
                                               person_height_norm)
                else:
                    print(
                        f"לא נמצאו תוצאות זיהוי תנוחה עבור {os.path.basename(image_path)}, אדם {i}, תיבה {j}")

    except Exception as e:
        print(f"שגיאה במהלך עיבוד התמונה: {e}")


if __name__ == "__main__":
    print("begining the python file")
    image_file = r"C:\Users\USER\Pictures\1.JPG"
    model_people_file = r"C:\Users\USER\Desktop\PROJECT\yoloperson\best.pt"
    model_object_file = r"C:\Users\USER\Desktop\PROJECT\NEWYOLO\best.pt"
    output_directory = r"C:\Users\USER\Desktop\PROJECT\dd"
    save_masked = True
    conf_threshold_people = 0.4

    run_yolo_with_mask(image_file, model_people_file, model_object_file,output_directory, save_masked, conf_threshold_people)
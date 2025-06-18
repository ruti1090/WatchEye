# מעבר על התיקיה שהתקבלה כקלט וחיפוש התמונה
import cv2
import os
import sys
import glob
import time # ודא ש-time מיובא

def find_pic_in_folder(folderOfThread):
    found_image_path = None # השתמש במשתנה עקבי
    supported_extensions = ['*.png', '*.jpg', '*.jpeg', '*.bmp', '*.tiff']
    for ext in supported_extensions:
        image_files = glob.glob(os.path.join(folderOfThread, ext))
        if image_files:
            found_image_path = image_files[0] # הנתיב הנכון נשמר כאן
            print(f"נמצא קובץ תמונה לטיפול: {found_image_path}")
            break # צא מהלולאה ברגע שנמצאה תמונה

    if found_image_path is None:
        print(f"שגיאה: לא נמצא קובץ תמונה (PNG/JPG/JPEG/BMP/TIFF) בתיקייה: {folderOfThread}", file=sys.stderr)
        return None, None

    # לולאת ניסיון-חוזר לקריאת התמונה
    max_read_retries = 5
    read_retry_delay_sec = 0.1 # 100 מילישניות
    for i in range(max_read_retries):
        try:
            image = cv2.imread(found_image_path) # עכשיו משתמשים ב-found_image_path
            if image is not None:
                print(f"התמונה נטענה בהצלחה מ: {found_image_path} לאחר {i+1} ניסיונות.")
                return image, found_image_path
            else:
                print(f"ניסיון {i+1}/{max_read_retries}: לא ניתן לקרוא תמונה מ- {found_image_path}. ממתין ודאי שהקובץ תקין.", file=sys.stderr)
                time.sleep(read_retry_delay_sec)
        except Exception as e:
            print(f"ניסיון {i+1}/{max_read_retries}: שגיאה בעת קריאת התמונה {found_image_path}: {e}. ממתין...", file=sys.stderr)
            time.sleep(read_retry_delay_sec)

    print(f"שגיאה חמורה: נכשל בקריאת תמונה מ- {found_image_path} לאחר ניסיונות מרובים.", file=sys.stderr)
    return None, None # נכשל בקריאת התמונה לאחר כל הניסיונות
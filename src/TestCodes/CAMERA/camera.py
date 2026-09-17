import cv2
import numpy as np

KNOWN_WIDTH = 10.0  
FOCAL_LENGTH = 600.0  

# EMA Filtr uchun o'zgaruvchilar
smoothed_distance = 0.0
alpha = 0.15 # Silliqlash koeffitsiyenti (kichikroq = silliqroq, lekin sekinroq reaksiya)

def get_distance(pixel_width):
    return (KNOWN_WIDTH * FOCAL_LENGTH) / pixel_width

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 1. QIZIL RANG UCHUN "DELTA" BIRLASHTIRISH
    # Qizilning pastki diapazoni (Yorug' tushganda)
    lower_red1 = np.array([0, 100, 50])
    upper_red1 = np.array([10, 255, 255])
    mask1 = cv2.inRange(hsv, lower_red1, upper_red1)

    # Qizilning yuqori diapazoni (Soya tushganda)
    lower_red2 = np.array([160, 100, 50])
    upper_red2 = np.array([180, 255, 255])
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)

    # Ikkala maskani bitta matritsaga birlashtirish (Siz aytgan g'oya!)
    mask = cv2.bitwise_or(mask1, mask2)
    
    # Shovqinlarni tozalash
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    contours, _ = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if contours:
        c = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(c)
        
        if w > 20:
            # Yangi o'lchangan masofa
            raw_distance = get_distance(w)
            
            # 2. EMA FILTRINI QO'LLASH (Sakrashni yo'qotish)
            if smoothed_distance == 0.0:
                smoothed_distance = raw_distance # Boshlang'ich qiymat
            else:
                smoothed_distance = (alpha * raw_distance) + ((1.0 - alpha) * smoothed_distance)
            
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            cv2.putText(frame, f"Masofa: {smoothed_distance:.1f} cm", (x, y - 10), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

    cv2.imshow("WRO Masofa Testi", frame)
    # cv2.imshow("Mask", mask) # Osvisheniya qanday ishlayotganini ko'rish uchun buni yoqib ko'ring

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
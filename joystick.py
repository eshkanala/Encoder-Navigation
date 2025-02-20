import serial
import pyautogui
import time

# --- Configuration ---
arduino_port = "COM4"  # Replace with your Arduino's serial port
baud_rate = 9600

# --- Key Press Rate Limiting (matches Arduino, but can adjust here too) ---
key_press_interval = 0.10  # seconds between key presses

# --- Mouse Click Debouncing ---
click_debounce_delay = 0.2
last_click_time = 0
last_key_press_time = 0 # Not really used with encoder - rate limit in Arduino

try:
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    print(f"Connected to Arduino on port {arduino_port} (Rotary Encoder Mode)")
    print(f"Key Press Interval: {key_press_interval} seconds (controlled in Arduino)")


    while True:
        if ser.in_waiting > 0:
            command = ser.readline().decode('utf-8').strip()
            if command == "L":
                pyautogui.press('left')
            elif command == "R":
                pyautogui.press('right')
            elif command == "U":
                pyautogui.press('up')
            elif command == "D":
                pyautogui.press('down')
            elif command == "C":
                current_time = time.time()
                if current_time - last_click_time > click_debounce_delay:
                    pyautogui.click()
                    last_click_time = current_time
            else:
                print(f"Unknown command received: {command}") # For debugging


except serial.SerialException as e:
    print(f"Error: Could not open serial port {arduino_port}. Check port and connection.")
    print(e)
except KeyboardInterrupt:
    print("\nExiting.")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial port closed.")
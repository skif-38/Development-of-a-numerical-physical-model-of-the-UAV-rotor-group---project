import serial
import time
import msvcrt
import os

PORT = 'COM3'
BAUD = 9600

script_dir = os.path.dirname(os.path.abspath(__file__))

ser = serial.Serial(PORT, BAUD, timeout=0.1)
time.sleep(2)

current_file = None
current_filename = None
file_opened = False
paused = False

def open_new_file():
    global current_file, current_filename, file_opened

    if file_opened and current_file:
        current_file.close()
        print(f"Запись {current_filename} завершена.")

    while True:
        num = input("Номер замера: ")
        num = int(num)
        break
        

    filename = f"zamer_{num}.csv"
    full_path = os.path.join(script_dir, filename)
    current_file = open(full_path, 'w', encoding='utf-8')
    current_file.write('time,cell1,cell2,cell3,cell4,total\n')
    current_file.flush()
    file_opened = True
    current_filename = full_path
    print(f"Запись в файл {full_path}")

open_new_file()

try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(line)
            if not paused and file_opened and current_file:
                current_file.write(line + '\n')
                current_file.flush()

finally:
    if file_opened and current_file:
        current_file.close()
        print(f"Файл {current_filename} закрыт.")

# Encoder-Navigation
Navigation in terms arrow keys translated to encoder movements controlled by an Arduino Uno, and Python.

# How it Works
The Arduino Script detects changes in the position of the encoder and sends it to serial via letters abbreviating the direction or change. 
The Python script reads these serial letters to classify movements and execute them using pyautogui and pyinput.

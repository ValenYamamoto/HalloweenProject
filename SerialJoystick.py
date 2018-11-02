import serial
import pygame
import time

pygame.init()
done = False

x = 0
y = 0
lastx = 0
lasty = 0
lastb = 0
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

pygame.display.set_mode((100, 100))

ser = serial.Serial('COM12', 9600, timeout = 1)
print(ser.portstr)
time.sleep(2)
for x in range(58):
    ser.write(0)

while done == False:
    pygame.event.pump()
    yAxis = int( -joystick.get_axis(1) * 5)
    xAxis = int( joystick.get_axis(4) * 5)
    button = 0;

    if (joystick.get_button(0)) :
        button = 1
        
    #yAxis = -joystick.get_axis(1)
    #xAxis = joystick.get_axis(4)

    
    if(xAxis != lastx or yAxis != lasty or lastb != button):
        data = "<%d>{%d}[%d]" %(xAxis, yAxis, button)
        buffer = 57 - len(data)
        bufferString = ""
        for x in range(buffer):
            bufferString += "0"
        send = bufferString + data
        ser.write((send).encode())
        #print("Axis 1: ", yAxis)
        #print("Axis 4: ", xAxis)

    lastx = xAxis
    lasty = yAxis
    lastb = button

    ser.write(00000000000000000000000000000000000000000000000000)
    
    if (joystick.get_button(1)):
        done = True

    """for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_w:
                y = 1
                print('Forward')
            elif event.key == pygame.K_s:
                y = -1
                print('Backward')
            elif event.key == pygame.K_d:
                x = 1
                print('Right')
            elif event.key == pygame.K_a:
                x = -1
                print('Left')
            else:
                y = 0
                x = 0
        ser.write(("\"X\": \"%f\" \"Y\": \"%f\"\n" %(x, y)).encode())
        """
    
pygame.quit ()
ser.close()

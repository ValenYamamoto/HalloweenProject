import pygame
import time
pygame.init()
done = False
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()
print(joystick.get_numaxes())
while done == False:
    pygame.event.pump()
    print("Axis 1: ", -joystick.get_axis(1))
    print("Axis 4: ", joystick.get_axis(4))

    time.sleep(1)

    if (joystick.get_button(1)):
        done = True

pygame.quit ()
        

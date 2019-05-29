# Used to import the serial terminal for UART communication
import time # to slow stuff down
import serial
import csv
import io
import picamera
import cv2
import numpy

# UART communication
ser = serial.Serial('/dev/ttyACM0', 9600)

# Let's create some global variables
menu_choice = "" # Used to decide where to go in the menu
run = True
ID = ""
incorrect = 0

# From before
userFingerPrint = []
userPassword = []
ItemPairs = []

# Let's create a menu
##1) Read fingerprint/Verify user
##2) Add new user
##3) Delete Individual from both databases (Pi and Elegoo)
##4) Wrong Order signal to Elegoo (e.g. tries to scan barcode first)
##5) Delete entire database

# Lets' define the menu
def menu():
    global menu_choice
    global ID

    # Get the number of fingerprints currently saved
    fing_count = len(userFingerPrint)
    # print("num fings: ", fing_count)
    
    CLS()
    print("1) Read fingerprint")
    print("2) Add user")
    print("3) Delete Individual user")
    print("4) Wrong Order")
    print("5) Delete database")

    menu_choice = input()

    if(menu_choice == "1"):
        if(fing_count > 0):
            if(Read()):
                CLS()
                print(ID)
                print("successful")
                time.sleep(3)
                ID = ""
        else:
            CLS()
            print("Database is empty :(")
            time.sleep(2)
    elif(menu_choice == "2"):
        # func 2
        if(fing_count > 0):
            if(Read()):
                if(ID == "0"):
                    Add()
                else:
                    print("Only the owner can add new fingerprints")
                    time.sleep(2)
        else:
            print("Database is empty")
            print("Please add a fingerprint")
            time.sleep(3)
            Add()
        CLS()
        ID = ""
        time.sleep(3)   # Sleeps for 3 seconds
    elif(menu_choice == "3"):
        # func 3
        if(fing_count > 0):
            del_ind()
        else:
            print("Database is already empty")
        ID = ""
        CLS()
        time.sleep(3)   # Sleeps for 3 seconds
    elif(menu_choice == "4"):
        # func 4
        CLS()
        ser.write(b'4')
        time.sleep(3)   # Sleeps for 3 seconds
    elif(menu_choice == "5"):
        # func
        if(fing_count > 0):
            if(Read()):
                if(ID == "0"):
                    del_data()
                    time.sleep(3)   # Sleeps for 3 seconds
                else:
                    CLS()
                    print("Invalid fingerprint.")
                    print("Only the owner can delete the database")
                    time.sleep(3)
    else:
        CLS()
        print("Incorrect value. Try again")
        time.sleep(3)   # Sleeps for 3 seconds
        return False
    
    menu_choice = ""
    line = ""
    run = True
    return True

def CLS():
    print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")


def Read():
    global ID
    global incorrect
    line = ""
    pw = ""

    # Send the value to the Elegoo
    ser.write(b'1')
    
    # Clear screen and prompt user for fingerprint
    CLS()
    print("Please scan your fingerprint")
    
    while line == "":
        if(ser.in_waiting > 0):
            line = ser.readline().decode().strip('\r\n')
            ID = line
            
    # Send value to Elegoo to shut off sensor
    ser.write(b'6')
    
    # Check for ID number
    if(in_file(ID)):
        CLS()
        print("You have been identified")
        print("Please enter your password: ")
        pw = input()
        if(isMatch(ID, pw)):
            print("Access granted!")
            incorrect = 0
            time.sleep(3)
            return True
        else:
            incorrect +=1 # Used to keep count of incorrect attempts to get in
            print("Access denied!")
            time.sleep(3)
    else:
        print("You have not been identified")
        incorrect += 1
        time.sleep(3)
        
    if(incorrect == 3):
        Intruder()
        incorrect = 0
    else:
        print("Incorrect attempt: ", incorrect)
        time.sleep(2)

    return False

def Add():
    global ID
    pw = ""
    line = ""

    CLS()
    print("Please scan the new fingeprint.")
    ser.write(b'2')
    
    while line == "":
        if(ser.in_waiting > 0):
            line = ser.readline().decode().strip('\r\n')
            #print(line)
            newFingerprint = line;
            
    # Wait for input
    if(newFingerprint != "128"):
        fout = open("Users.txt", 'a')
        userFingerPrint.append(newFingerprint)
        print("ID #: ", newFingerprint)
        print("Add new password: ")
        newPass = input()
        userPassword.append(newPass)
        userData = newFingerprint + "\t" + newPass + "\n"
        fout.write(userData)
        fout.close()
        # ser.write(b'6')
    else:
        print("Invalid fingerprint")
        # ser.write(b'6')
        time.sleep(2)
        
    # Write back to shut sensor off
    ser.write(b'6')
            

def LoadUser():
    with open('Users.txt','r') as f:
        reader=csv.reader(f,delimiter='\t')
        for finger,password in reader:
            userFingerPrint.append(finger)
            userPassword.append(password)
    print("in load user")

    f.close()

def in_file(matchFingerPrint):
    length = len(userFingerPrint)
    for i in range(length):
        if(matchFingerPrint == userFingerPrint[i]):
            global ID
            ID = userFingerPrint[i]
            return True
    return False

def isMatch(matchFingerPrint, matchPassword):
    length = len(userFingerPrint)
    for i in range (length):
        if (matchFingerPrint == userFingerPrint[i] and matchPassword == userPassword[i]):
            return True

    return False

def del_data():
    global ID
    verify = ""
    line = ""

    print("Are you sure [Y/n]")
    verify = input()
    
    if(verify == "Y" or verify == "y"):
        ser.write(b'5')
        f = open('Users.txt', 'w')
        f.close()
    else:
        print("Deleting database failed")
        time.sleep(2)
        
    if(line == "200"):
        print("Deleting database ...")
        time.sleep(2)
#    elif(line == "-200"):
#        print("Database is already empty")
#        time.sleep(2)
        
def del_ind():
    global ID
    
    if(Read()):
        if(ID != "0"):
            # Check to verify it is in the file
            CLS()
            print("Enter your password to verify your fingerprint deletion: ")
            pw = input()
            if(isMatch(ID, pw)):
                del_line = (ID + "\t" + pw)
                del_user(del_line)
                CLS()
                ser.write(b'3')
                print("Your fingerprint has been deleted")
                time.sleep(3)
        else:
            CLS()
            print("Cannot delete the owner")
            print("You must delete the entire database if you want to delete the owner")
            time.sleep(3)
    ID = ""
    
def del_user(delete_person):
    with open("Users.txt", "r") as f:
        lines = f.readlines()
        print(lines)
    with open("Users_replace.txt", "w") as f:
        for line in lines:
            if line.strip("\n") != delete_person:
                f.write(line)
    f.close()
    with open("Users_replace.txt", "r") as f:
        lines = f.readlines()
        print(lines)
    with open("Users.txt", "w") as f:
        for line in lines:
            f.write(line)
            
def Intruder():
    #Create a memory stream so photos doesn't need to be saved in a file
    stream = io.BytesIO()

    #Get the picture (low resolution, so it should be quite fast)
    #Here you can also specify other parameters (e.g.:rotate the image)
    with picamera.PiCamera() as camera:
        camera.resolution = (2592, 1944) # Changed to a much higher quality. It isn't much slower.
        camera.capture(stream, format='jpeg')

    #Convert the picture into a numpy array
    buff = numpy.fromstring(stream.getvalue(), dtype=numpy.uint8)

    #Now creates an OpenCV image
    image = cv2.imdecode(buff, 1)

    #Load a cascade file for detecting faces
    face_cascade = cv2.CascadeClassifier('/home/pi/Desktop/OpenCV_files/faces.xml')

    #Convert to grayscale
    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)

    #Look for faces in the image using the loaded cascade file
    faces = face_cascade.detectMultiScale(gray, 1.1, 5)

    print("Found "+str(len(faces))+" face(s)")

    #Draw a rectangle around every found face
    for (x,y,w,h) in faces:
        cv2.rectangle(image,(x,y),(x+w,y+h),(255,255,0),2)

    #Save the result image
    cv2.imwrite('result.jpg',image)

if __name__ == '__main__':
    LoadUser()
    while run:
        run = menu()

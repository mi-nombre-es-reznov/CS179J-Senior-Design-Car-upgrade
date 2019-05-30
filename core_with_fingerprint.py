import sys
import csv
import time
# import mysql.connector
import RPi.GPIO as GPIO
from hx711 import HX711
# Fingerprint stuff
import serial
import io
import picamera
import cv2
import numpy

# UART communication
ser = serial.Serial('/dev/ttyACM0', 9600)

hx = HX711(5, 6)
hx.set_reading_format("MSB", "MSB")
hx.set_reference_unit(-422.5)
hx.reset()
hx.tare()

GPIO.setmode(GPIO.BCM)
GPIO.setup(27,GPIO.OUT)


userFingerPrint = []
userPassword = []
ItemPairs = []
Finish = ""
Done = ""

# Let's create some global variables
menu_choice = "" # Used to decide where to go in the menu
run = True
ID = ""
incorrect = 0
Scan_state = 0

def menu():
    # Get the number of fingerprints currently saved
    fing_count = len(userFingerPrint)
    global menu_choice
    global ID
    global Scan_state
    
    CLS()
    closeSolenoid()
    print ("Welcome to the secure fridge ")
    print ("1) Add User ")
    print ("2) Remove User ")
    print ("3) Add/Remove Item ")
    print ("4) Add Barcode ")
    print ("5) Remove Barcode ")
    print ("6) Test I/O Menu")
    print ("Choice: ")

    userInput = input()

    if userInput == "9":
        viewBarcodes()
    elif userInput == "0":
        programExit()
    elif userInput == "1":
        # Add User
        if(fing_count > 0):
            if(fingerPrintFunction()):
                if(ID == "0"):
                    addUser()
                else:
                    CLS()
                    print("Only the owner can add new fingerprints")
                    time.sleep(2)
        else:
            CLS()
            print("Database is empty")
            print("Please add a fingerprint")
            time.sleep(3)
            addUser()
        CLS()
        ID = ""        
 
    elif userInput == "2":
        # Remove individual user
        if(fing_count > 0):
           removeUser()
        else:
            CLS()
            print("Database is already empty")
            time.sleep(3)   # Sleeps for 3 seconds
        ID = ""
        CLS()
        
        
    elif userInput == "3":
        Scan_state = 1
        Login()
        Scan_state = 0
    elif userInput == "4":
        addBarcode()
    elif userInput == "5":
        removeBarcode()
    elif userInput == "6":
        testIOMenu()
    elif userInput == "7":
        # Delete Database
        if(fing_count > 0):
            if(Read()):
                if(ID == "0"):
                    del_data()
                else:
                    CLS()
                    print("Invalid fingerprint.")
                    print("Only the owner can delete the database")
                    time.sleep(3)
                    
                    
    elif userInput == "8":
        return False
    else:
        CLS()
        print("Wrong choice")
        time.sleep(1)
    return True

def openSolenoid():
    GPIO.output(27,0)


def closeSolenoid():
    GPIO.output(27,1)

def viewBarcodes():
    CLS()
    print ("In view barcodes")

def addUser():
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
        time.sleep(1)
        
    # Write back to shut sensor off
    ser.write(b'6')

def removeUser():
    global ID
    
    if(fingerPrintFunction()):
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
                time.sleep(2)
        else:
            CLS()
            ser.write(b'3')
            print("Cannot delete the owner")
            print("You must delete the entire database if you want to delete the owner")
            ser.write(b'6')
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

def Login():
    if fingerPrintFunction():
        openSolenoid()
        barCodeFunction()


def addBarcode():
    CLS()
    fout = open("ItemBarcodes.txt", 'a')
    print("Add new barcode: ")
    newBarcodeItem = input()
    print("Add item name: ")
    newItemName = input()
    pair = [newBarcodeItem, newItemName]
    ItemPairs.append(pair)
    userData = newBarcodeItem + "\t" + newItemName + "\n"
    fout.write(userData)
    fout.close()


def removeBarcode():
    CLS()
    DDD = {name: (upc) for name, upc in ItemPairs}
    print("Enter barcode: (-1 to exit)")
    userRemoveBarcode = input()
    if userRemoveBarcode == "-1":
        return
    try:
        barcodeName = DDD[userRemoveBarcode]
        itemToRemove = userRemoveBarcode + "\t" + barcodeName
        with open("ItemBarcodes.txt", "r") as f:
            lines = f.readlines()
        with open("ItemBarcodes.txt", "w") as f:
            for line in lines:
                if line.strip("\n") != itemToRemove:
                    f.write(line)
        print("Item removed")
        return
    except KeyError:
        print("Barcode does not exist")
        removeBarcode()


def testIOMenu():
    CLS()
    print ("Welcome to the I/O Testing Menu ")
    print ("0) Exit ")
    print ("1) Test solenoid open ")
    print ("2) Test solenoid close ")
    print ("Choice: ")


def fingerPrintFunction():
    global ID
    global incorrect
    global Scan_state
    line = ""
    pw = ""
    
    # Only count if trying to get into the system
    if(Scan_state == 0):
        incorrect = 0 # Reset incorrect value to 0


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
            CLS()
            print("Access granted!")
            time.sleep(1)
            if(Scan_state == 1):
                incorrect = 0
            return True
        else:
            if(Scan_state == 1):
                incorrect +=1 # Used to keep count of incorrect attempts to get in
            CLS()
            print("Access denied!")
            time.sleep(2)
    else:
        CLS()
        print("You have not been identified")
        if(Scan_state == 1):
            incorrect += 1
        time.sleep(3)
        
    if(Scan_state == 1):
        if(incorrect == 3):
            Intruder()
            incorrect = 0
        else:
            print("Incorrect attempt: ", incorrect)
            time.sleep(2)

    return False

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

def programExit():
    exit()

def barCodeFunction():
#    CLS()
    D = {upc: (name) for upc, name in ItemPairs}
    DD = {name: (upc) for name, upc in ItemPairs}
    userBarcodeinput = "entry"
    doneCode = DD["Done"]
    while userBarcodeinput != doneCode:
        print("Scan Done if you are finished or \nScan Barcode: ")
        userBarcodeinput = input()
        userBarcodeinput = str(userBarcodeinput)
        if userBarcodeinput == doneCode:
            return
        try:
            foundItem = D[userBarcodeinput]
            print("Desposit/Withdrawl your " + foundItem)
            Transaction(foundItem, userBarcodeinput)

        except KeyError:
            print("Not in database")
            print("Press 'c' to continue")
            userBarcodeinput = input()

    return


def LoadUser():
    with open('Users.txt','r') as f:
        reader=csv.reader(f,delimiter='\t')
        for finger,password in reader:
            userFingerPrint.append(finger)
            userPassword.append(password)
    print("in load user")

    f.close()

def LoadBarcodes():
    with open('ItemBarcodes.txt','r') as f:
        reader=csv.reader(f,delimiter='\t')
        for upc,name in reader:
            pair = [upc, name]
            ItemPairs.append(pair)
        f.close()

    if ItemPairs[0][0] == "Finish":
        Finish = ItemPairs[0][0]
    else:
        print("No finish code exists")
    if ItemPairs[1][0] == "Done":
        Done = ItemPairs[1][0]
    else:
        print("No done code exists")

def Transaction(foundItem, userBarcodeinput):
    mydb = mysql.connector.connect(user='bob', password='dbasdf', host='127.0.0.1', database='dbSecureFridge')
    D = {name: (upc) for name, upc in ItemPairs}
    startWeight = GetWeight()
    startWeight1 = str(startWeight)
    print("Start weight of fridge is: " + startWeight1)
    print("Scan \"Finish\" when you are done")
    userInput = "entry"
    finishCode = D["Finish"]
    while userInput != finishCode:
        userInput = input()
        if (userInput != finishCode):
            print("Wrong code, scan Finish: ")
    endWeight = GetWeight()
    endWeight1 = str(endWeight)
    print("End weight is: " + endWeight1)
    netWeightChange = endWeight - startWeight
    netWeightChange1 = str(netWeightChange)
    print("Net weight change is: " + netWeightChange1)
    
    mycursor = mydb.cursor()
    sql = "SELECT Weight FROM INVENTORY WHERE UPC =" + userBarcodeinput
    mycursor.execute(sql)
    myresult = mycursor.fetchall()  
    if not myresult:
        
        sql = "INSERT INTO INVENTORY (UPC, Name, Weight) VALUES (%s, %s, %s)"
        val = (userBarcodeinput, foundItem, netWeightChange1)
        mycursor.execute(sql, val)
        mydb.commit()
    else:
        mycursor = mydb.cursor()
        sql = "SELECT Weight FROM INVENTORY WHERE UPC=" + userBarcodeinput
        mycursor.execute(sql)
        myresult = mycursor.fetchall()
        newResult = float(myresult[0][0])
        netWeightUpdate = netWeightChange + newResult
        netWeightUpdate1 = str(netWeightChange + newResult)
        mycursor = mydb.cursor()
        if netWeightUpdate <= 0:
            sql = "UPDATE INVENTORY SET Weight = %s WHERE UPC = %s"
            val = ("0", userBarcodeinput)
            mycursor.execute(sql, val)
            mydb.commit()
        else:
            sql = "UPDATE INVENTORY SET Weight = %s WHERE UPC = %s"
            val = (netWeightUpdate1, userBarcodeinput)
            mycursor.execute(sql, val)
            mydb.commit()
        sql = "SELECT Weight FROM INVENTORY WHERE UPC=" + userBarcodeinput
        mycursor.execute(sql)
        myresult = mycursor.fetchall()
        print("Weight in database: " + myresult[0][0])

    barCodeFunction()

def GetWeight():
    while True:
        try:
            val = hx.get_weight(5)
      #      print(val)
            hx.power_down()
            hx.power_up()
            time.sleep(0.1)
            time.sleep(2)
            if val < 0:
                return 0
      #      userGetWeight = input()
            return val

        except(KeyboardInterrupt, SystemExit):
            print ("Cleaning...")
            GPIO.cleanup()
            print ("Bye!")
            print("getweight work")
            sys.exit()

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
        CLS()
        print("Deleting database failed")
        time.sleep(2)
        
#    if(line == "200"):
#        print("Deleting database ...")
#        time.sleep(2)
#    elif(line == "-200"):
#        print("Database is already empty")
#        time.sleep(2)

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
    face_cascade = cv2.CascadeClassifier('/home/pi/Desktop/CS179J-Senior-Design-Car-upgrade/Python Core/faces.xml')

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

def CLS():
    print ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")

if __name__ == '__main__':
    LoadUser()
    LoadBarcodes()
    isRunning = True
    while isRunning:
        isRunning = menu()

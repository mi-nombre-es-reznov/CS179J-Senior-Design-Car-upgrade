import sys
import csv
import time
import mysql.connector
import RPi.GPIO as GPIO
from hx711 import HX711

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



def menu():
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
        addUser()
    elif userInput == "2":
        removeUser()
    elif userInput == "3":
        Login()
    elif userInput == "4":
        addBarcode()
    elif userInput == "5":
        removeBarcode()
    elif userInput == "6":
        testIOMenu()
    elif userInput == "8":
        return False
    else:
        print("Wrong choice")
    return True

def openSolenoid():
    GPIO.output(27,0)


def closeSolenoid():
    GPIO.output(27,1)

def viewBarcodes():
    CLS()
    print ("In view barcodes")

def addUser():
    CLS()
    if(fingerPrintFunction()):
        fout = open("Users.txt", 'a')
        CLS()
        print("Add new fingerprint: ")
        newFingerprint = input()
        userFingerPrint.append(newFingerprint)
        print("Add new password: ")
        newPass = input()
        userPassword.append(newPass)
        userData = newFingerprint + "\t" + newPass + "\n"
        fout.write(userData)
        fout.close()

def removeUser():
    print("Removing user")

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
    userFingerPrint = ""
    while userFingerPrint != "-1":
        CLS()
        print("Enter fingerprint (-1 to exit)")
        userFingerPrint = input()
        if (userFingerPrint == "999" or userFingerPrint == "-1"):
            return False
            programExit()
        print("Enter password: ")
        userPassword = input()
        if (isMatch(userFingerPrint, userPassword)):
            print("Access granted")
            return True
        else:
            print("\n Access denied \n")
            print("Enter 'c' to continue")
            userPassword = input()
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


def CLS():
    print ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")

if __name__ == '__main__':
    LoadUser()
    LoadBarcodes()
    isRunning = True
    while isRunning:
        isRunning = menu()

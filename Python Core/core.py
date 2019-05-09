import sys
import csv

userFingerPrint = []
userPassword = []
ItemPairs = []
Finish = ""
Done = ""

def menu():
    print ("Welcome to the secure fridge \n")
    print ("1) Add User \n")
    print ("2) Remove User \n")
    print ("3) Add/Remove Item \n")
    print ("4) Add Barcode \n")
    print ("5) Remove Barcode \n")
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
    else:
        print("Wrong choice")
        menu()

def viewBarcodes():
    CLS()
    print ("In view barcodes")

def addUser():
    fout = open("Users.txt", 'a')
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
        barCodeFunction()


def addBarcode():
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
    print("Removing barcode")

def testIOMenu():
    CLS()
    print ("Welcome to the I/O Testing Menu \n")
    print ("0) Exit \n")
    print ("1) Test solenoid open \n")
    print ("2) Test solenoid close \n")
    print ("Choice: ")


def fingerPrintFunction():

    userFingerPrint = ""
    while userFingerPrint != "-1":
        print("Enter fingerprint (-1 to exit)")
        userFingerPrint = input()
        if (userFingerPrint == "999" or userFingerPrint == "-1"):
            return False
            programExit()
        print("Enter password: ")
        userPassword = input()
        if (isMatch(userFingerPrint, userPassword)):
            print("Access granted \n")
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
    D = {upc: (name) for upc, name in ItemPairs}
    userBarcodeinput = "entry"
    while userBarcodeinput != Done:
        print("Scan Barcode: ")
        userBarcodeinput = input()
        userBarcodeinput = str(userBarcodeinput)
        try:
            foundItem = D[userBarcodeinput]
            print("Desposit/Withdrawl your " + foundItem)
            print("\n")
            Transaction(foundItem)

        except KeyError:
            print("Not in database")
            print("Press 'c' to continue")
            userBarcodeinput = input()


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

def Transaction(foundItem):
    D = {name: (upc) for name, upc in ItemPairs}
    startWeight = GetWeight()
    print("Scan \"Finish\" when you are done")
    userInput = "entry"
    finishCode = D["Finish"]
    while userInput != finishCode:
        userInput = input()
        if (userInput != finishCode):
            print("Wrong code, scan Finish: ")
    endWeight = GetWeight()
    netWeightChange = endWeight - startWeight
    programExit()

def GetWeight():
    return 1

def CLS():
    print ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")

if __name__ == '__main__':
    LoadUser()
    LoadBarcodes()
    menu()

#import serial
#ser = serial.Serial('/dev/ttyACM0',9600)
#while True:
#    tacos = int(input())
#    encode = b'%d' %tacos
#    ser.write(encode)
#import serial
#ser = serial.Serial('/dev/ttyACM0',9600)
#count = 0
#while count < 10:
#    if ser.in_waiting > 0:
#        line = ser.readline()
#        count_str = str(count)
#        print(count_str)
#        if line == count_str:
#            print("Acess Granted")
#            print("You are using value :", count)
#            print("\n\n")
#            count += 1






#        print(line)
        
#        while count < 10:
#            count_str = str(count)
#            if(line == count_str):
#                print("Access granted!!!")
#                print("You are using value: ", count)
#                print("\n\n")
#            count += 1
#        count = 0;





#while 1:
#    if(ser.in_waiting > 0):
#        line = ser.readline().decode('ascii').strip('\r\n')
#        print(line)



#        if(line == "0"):
#            print("Zero")
#        elif(line == "10"):
#            print("ten")
#        else:
#            print("Invalid number")
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
count = 0
line = ""

while True:
	ins = ""
	print("Enter choice: ")
	ins = input()

	if(ins == "1"):
		print("Scan fingerprint")
		#ser = readline.decode('ascii').strip('\r\n')
		ser.write(b'1')
	elif(ins == "2"):
		print("Add new user")
		ser.write(b'2')
	elif(ins == "3"):
		print("Delete individual")
		ser.write(b'3')
	elif(ins == "4"):
		print("Wrong Order")
		ser.write(b'4')
	elif(ins == "5"):
		print("Delete database")
		ser.write(b'5')

	while line == "":
		line = ser.readline().decode('ascii').strip('\r\n')
		print(line)

	ser.write(b'6')

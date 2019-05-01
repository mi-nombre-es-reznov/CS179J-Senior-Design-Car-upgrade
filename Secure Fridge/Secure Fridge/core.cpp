using namespace std;
Core::Core()
{
	/// 2019 179 9 (0-9) // 9:lock, 0:finish
	isRunning = true;
	GPIOSetup();
	LoadBarcodes();
	LoadUsers();
}

void Core::Start()
{
	while(isRunning)
	{
		Menu();
	}
}
void Core::GPIOSetup()
{
	pinMode(0,OUTPUT);
}
void Core::Menu()
{
	char input = 0;
	CLS();
	cout << "Welcome to the Secure Fridge" << endl;
	cout << "1) Add User" << endl;
	cout << "2) Remove User" << endl;
	cout << "3) Add/Remove Item" << endl;
	cout << "4) Add Barcode" << endl;
	cout << "5) Remove Barcode" << endl;
	cout << "6) Test I/O Menu" << endl;
	cout << "Choice: ";
	cin >> input;
	switch(input) {
		case '9':
			ViewBarcodes();
			break;
		case '0':
			isRunning = false;
			break;
		case '1':
			AddUser();
			break;
		case '2':
			RemoveUser();
			break;
		case '3':
			Login();
			break;
		case '4':
			AddBarcode();
			break;
		case '5':
			RemoveBarcode();
			break;
		case '6':
			TestIOMenu();
			break;
		default:
			break;
	}
}
void Core::Login()
{
	if(FingerPrintFunction()) {
		BarcodeFunction();
	}
}
bool Core::FingerPrintFunction()
{
	string input, pass;
	while(input != "-1") {
		CLS();
		cout << "Enter fingerprint (-1 to exit): ";
		cin >> input;
		if(input == "999") {isRunning = false;}
		if(input == "-1" || input == "999") return false;
		cout << "Enter password: ";
		cin >> pass;
		if(isMatch(input, pass)) {
			cout << "Access granted\n\n";
			return true;
		} else {
			cout << "\nAccess denied\n";
			cout << "Enter 'c' to continue: ";
			cin >> pass;
		}
	}
	return false;
}
void Core::BarcodeFunction()
{
	string input="", name;
	char* ch;
	while(input != Done){
		input="";
		CLS();
		cout << "Scan Barcode: ";
		cin>>input;
		if(Items.find(input) != Items.end()) {
			name = Items.find(input)->second;
			CLS();
			cout << "Deposit/Withdrawl your " << name << endl;
			Transaction(name);
		}
		else if(input != Done) {
			cout << "not in database\n";
			cout << "Enter 'c' to continue: ";
			cin >> name;
		}
	}
}
double Core::GetWeight()
{
	return 1.0; // WIP
}
void Core::OpenLock()
{
	digitalWrite(0, HIGH);
}
void Core::CloseLock()
{
	digitalWrite(0, LOW);
}
void Core::AddBarcode()
{
	string barcode, name;
	cout << "Enter Barcode: ";
	cin >> barcode;

	cout << "Enter Item Name: ";
	cin >> name;

	if(Items.find(barcode) == Items.end())
	{
		Items.insert(pair<string,string>(barcode,name));
		ofstream W;
		W.open("ItemBarcodes.txt",ofstream::trunc);
			W << "Finish\t\t";
			W << Finish;
			W << endl;
			W << "Done\t\t";
			W << Done;
			for(map<string,string>::iterator it = Items.begin(); it != Items.end(); it++)
			{
				W << endl << it->first << "\t" << it->second;
			}
		W.close();
	} else { cout << "Adding Failed... c to continue: "; cin >> name;}
}
void Core::RemoveBarcode()
{
	string barcode;
	cout << "Enter Barcode: ";
	cin >> barcode;

	if(Items.find(barcode) != Items.end())
	{
		Items.erase(barcode);
		ofstream W;
		W.open("ItemBarcodes.txt",ofstream::trunc);
			W << "Finish\t\t";
			W << Finish;
			W << endl;
			W << "Done\t\t";
			W << Done;
			for(map<string,string>::iterator it = Items.begin(); it != Items.end(); it++)
			{
				W << endl << it->first << "\t" << it->second;
			}
		W.close();
	} else { cout << "Remove failed... c to continue: "; cin >> barcode;}
}
void Core::AddUser()
{
	CLS();
	if(FingerPrintFunction())
	{
		string input, pass;
		cout << "Add new fingerprint: ";
		cin >> input;
		cout << "Add new password: ";
		cin >> pass;
		ofstream W;
		UIDs.push_back(input);
		UIDPs.push_back(pass);
		W.open("Users.txt", ofstream::trunc);
		int size = UIDs.size();
		for(int i = 0; i < size; i++)
		{
			W << UIDs[i] << "\t" << UIDPs[i];
			if(i < size - 1) W << endl;
		}
		W.close();
	}
}
void Core::RemoveUser()
{
	CLS();
	if(FingerPrintFunction())
	{
		string fingerprint;
		cout << "Enter Fingerprint: ";
		cin >> fingerprint;

		int size = UIDs.size();
		int i = 0;
		for(; i < size; i++) // find index of fingerprint
			if(UIDs[i] == fingerprint)
				break;
		if(i != size) // if found, remove
		{
			UIDs.erase(UIDs.begin() + i);
			UIDPs.erase(UIDPs.begin() + i);
			ofstream W;
			W.open("Users.txt", ofstream::trunc);
			for(int j = 0; j < size - 1; j++)
			{
				W << UIDs[j] << "\t" << UIDPs[j];
			if(j < size - 2) W << endl;
			}
			W.close();
		} else { cout << "Remove failed\n c to continue: "; cin >> fingerprint;}
	}
}
void Core::LoadBarcodes()
{
	string upc, name, check;
	ifstream R;
	R.open("ItemBarcodes.txt");
	R >> check;
	if(check == "Finish")
	{
		R >> upc;
		Finish = upc;
	} else { cout << "Finish code not found..."; cin >> upc; isRunning = false; return;}
	R >> check;
	if(check == "Done")
	{
		R >> upc;
		Done = upc;
	} else { cout << "Done code not found..."; cin >> upc; isRunning = false; return;}
	while(!R.eof())
	{
		R >> upc;
		R >> name;
		Items.insert(pair<string,string>(upc,name));
	}
	R.close();
}
void Core::LoadUsers()
{
	string fingerprint, password;
	ifstream R;
	R.open("Users.txt");
	while(!R.eof())
	{
		R >> fingerprint;
		R >> password;
		UIDs.push_back(fingerprint);
		UIDPs.push_back(password);
	}
	R.close();
}
void Core::Transaction(string itemName)
{
	string input = "";
	int startWeight,endWeight, netWeightChange;

	startWeight = GetWeight();
	cout << "Scan \"Finish\": ";
	while(input != Finish) {
		cin >> input;
		if(input != Finish) {
			cout << endl << "Wrong code, Scan Finish: ";
		}
	}
	endWeight = GetWeight();

	netWeightChange = endWeight - startWeight;

	// update database
	if(netWeightChange > 0){
		// adding stuff
	} else {
		// removing stuff
	}
}
bool Core::isMatch(string input, string pass)
{
	for (int i = 0; i < UIDs.size(); i++) {
		if(input == UIDs[i] && pass == UIDPs[i]) {
			return true;
		}
	}
	return false;
}
void Core::CLS()
{
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}
void Core::ViewBarcodes()
{
	CLS();
	string placeholder;
	int size = UIDs.size();
	for(map<string,string>::iterator it = Items.begin(); it != Items.end(); it++)
	{
		cout << it->first << "\t" << it->second << endl;
	}
	cout << "Press c to continue: ";
	cin >> placeholder;
}
void Core::TestIOMenu()
{
	while(true)
	{
		char input = 0;
		CLS();
		cout << "Welcome to the I/O Testing Menu" << endl;
		cout << "0) Exit" << endl;
		cout << "1) Test Solenoid Open" << endl;
		cout << "2) Test Solenoid Close" << endl;
		cout << "Choice: ";
		cin >> input;
		switch(input) {
			case '0':
				return;
				break;
			case '1':
				OpenLock();
				break;
			case '2':
				CloseLock();
				break;
			default:
				break;
		}
	}
}
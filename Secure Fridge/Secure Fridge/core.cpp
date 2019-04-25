using namespace std;
Core::Core()
{
	/// 2019 179 9 (0-9) // 9:lock, 0:finish
	isRunning = true;
	Items.insert(pair<string,string>("1","Milk"));
	Items.insert(pair<string,string>("201917991","Egg"));
	Items.insert(pair<string,string>("201917992","Milk"));
	Items.insert(pair<string,string>("201917993","Apple"));
	Items.insert(pair<string,string>("201917994","Juice"));
	Items.insert(pair<string,string>("201917995","Yogurt"));
	Items.insert(pair<string,string>("201917996","Cheese"));
	Items.insert(pair<string,string>("201917997","AppleSauce"));
	Items.insert(pair<string,string>("201917998","Lettuce"));
	
	UIDs.push_back("456");
	UIDPs.push_back("asdf");
	
}

void Core::Start()
{
	while(isRunning)
	{
		Menu();
	}
}
void Core::Menu() {
	char input = 0;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "Welcome to the Secure Fridge" << endl;
	cout << "1) Add User" << endl;
	cout << "2) Add/Remove Item" << endl;
	cout << "Choice: ";
	cin >> input;
	switch(input) {
		case '1':
			AddUser();
			break;
		case '2':
			Login();
			break;
		default:
			break;
	}
}
void Core::AddUser() {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	if(FingerPrintFunction()) { // must have access by existing user
		string input, pass;
		cout << "Add new fingerprint: ";
		cin >> input;
		cout << "Add new password: ";
		cin >> pass;
		UIDs.push_back(input);
		UIDPs.push_back(pass);
	}
}
void Core::Login() {
	if(FingerPrintFunction()) {
		BarcodeFunction();
	}
}
bool Core::FingerPrintFunction()
{
	string input, pass;
	while(input != "-1") {
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
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
	while(input != "201917999"){
		input="";
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		cout << "Scan Barcode: ";
		cin>>input;
		if(Items.find(input) != Items.end()) {
			name = Items.find(input)->second;
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
			cout << "Deposit/Withdrawl your " << name << endl;
			Transaction(name);
		}
		else if(input != "201917999") {
			cout << "not in database\n";
		}
	}
}
double Core::GetWeight()
{

}
void Core::ToggleLock()
{

}
void Core::Transaction(string itemName)
{
	string input = "";
	int startWeight,endWeight, netWeightChange;

	startWeight = GetWeight();
	cout << "Scan \"Finish\": ";
	while(input != "201917990") {
		cin >> input;
		if(input != "201917990") {
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

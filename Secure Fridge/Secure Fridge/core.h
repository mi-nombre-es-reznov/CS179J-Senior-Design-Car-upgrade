#ifndef _core_h_
#define _core_h_

using namespace std;

class Core
{
public:
	Core();

	void Start();
	void GPIOSetup();
	void Login();
	void Menu();
	bool FingerPrintFunction();
	void BarcodeFunction();
	double GetWeight();

	void OpenLock();
	void CloseLock();

	void AddBarcode();
	void RemoveBarcode();

	void AddUser();
	void RemoveUser();

	void LoadBarcodes();
	void LoadUsers();

	void Transaction(string itemName);
	bool isMatch(string input, string pass);

	void CLS();
	void ViewBarcodes();
	void TestIOMenu();

	bool isRunning;
	string Finish, Done;
	map<string,string> Items;
	vector<string> UIDs;
	vector<string> UIDPs;
};

#endif

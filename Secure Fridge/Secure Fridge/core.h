#ifndef _core_h_
#define _core_h_

using namespace std;

class Core
{
public:
	Core();

	void Start();
	void AddUser();
	void Login();
	void Menu();
	bool FingerPrintFunction();
	void BarcodeFunction();
	double GetWeight();
	void ToggleLock();

	void Transaction(string itemName);
	bool isMatch(string input, string pass);

	bool isRunning;
	map<string,string> Items;
	vector<string> UIDs;
	vector<string> UIDPs;
};

#endif

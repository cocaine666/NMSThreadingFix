#include<iostream>
#include<Windows.h>
#include<intrin.h>
#include<fstream>
#include<string>
#include <cstdio>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

using namespace std;


string get_cpu_name()
{
	int cpu_information[4] = { -1 };
	char cpu_string[0x40];
	__cpuid(cpu_information, 0x80000000);
	unsigned int ex_ids = cpu_information[0];
	memset(cpu_string, 0, sizeof(cpu_string));

	for (int i = 0x80000000; i <= ex_ids; ++i)
	{
		__cpuid(cpu_information, i);
		if (i == 0x80000002)
			memcpy(cpu_string, cpu_information, sizeof(cpu_information));
		else if (i == 0x80000003)
			memcpy(cpu_string + 16, cpu_information, sizeof(cpu_information));
		else if (i == 0x80000004)
			memcpy(cpu_string + 32, cpu_information, sizeof(cpu_information));
	}
	return cpu_string;

}

int calculate_threads() {

	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	int threads = system_info.dwNumberOfProcessors; 
	return threads;

}

std::string get_directory() {
	HMODULE hModule = GetModuleHandle(nullptr);
	if (hModule) {
		char path[_MAX_PATH] = "";
		GetModuleFileNameA(hModule, path, sizeof(path));
		PathRemoveFileSpecA(path);
		strcat_s(path, "\\");
		return path;
	}
	return "";
}

int main() {

	string cpu_name = get_cpu_name();
	int available_threads = calculate_threads();
	int low_threads, high_threads;

	low_threads = available_threads / 4;
	high_threads = available_threads - (available_threads / 4);


	string replace_high[] = { "NumHighThreads=0" , "NumHighThreads=1", "NumHighThreads=2", "NumHighThreads=3", "NumHighThreads=4", "NumHighThreads=5", "NumHighThreads=6",
							"NumHighThreads=7","NumHighThreads=8","NumHighThreads=9", "NumHighThreads=10", "NumHighThreads=11", "NumHighThreads=12", "NumHighThreads=13",
							"NumHighThreads=14", "NumHighThreads=15", "NumHighThreads=16" };

	string replace_low[] = { "NumLowThreads=0" , "NumLowThreads=1", "NumLowThreads=2", "NumLowThreads=3", "NumLowThreads=4", "NumLowThreads=5", "NumLowThreads=6",
							"NumLowThreads=7","NumLowThreads=8","NumLowThreads=9", "NumLowThreads=10", "NumLowThreads=11", "NumLowThreads=12", "NumLowThreads=13",
							"NumLowThreads=14", "NumLowThreads=15", "NumLowThreads=16" };

	string low_trd_str = "NumLowThreads=" + to_string(low_threads);
	string high_trd_str = "NumHighThreads=" + to_string(high_threads);

	ifstream filein("SETTINGS\\TKGRAPHICSSETTINGS.MXML");
	ofstream fileout("SETTINGS\\TKGRAPHICSSETTINGS2.MXML");

	if (!filein || !fileout)
	{
		cout << "Error opening files!" << endl;
		return 1;
	}

	string temp_str;

	cout << "Detected CPU: " + cpu_name << endl;
	cout << "Available threads (logical): " + to_string(available_threads) << endl;
	cout << "High threads are set to " + to_string(high_threads) << endl;
	cout << "Low threads are set to " + to_string(low_threads) << endl;

	while (filein >> temp_str)
	{
		for (int i = 0; i < 17; i++) {

			if (temp_str == replace_high[i]) {
				temp_str = "NumHighThreads=" + to_string(high_threads);
			}
			if (temp_str == replace_low[i]) {
				temp_str = "NumLowThreads=" + to_string(low_threads);
			}

		}
		temp_str += "\n";
		fileout << temp_str;
	}
	cout << "Successfully modified config files. Backup file is created in the directory. (File without number 2 is the original.)" << endl;

	fileout.close();
	filein.close();


	string directory = get_directory();
	cout << "Full game directory path: " << directory << endl;


	system("pause");
	return 0;


}
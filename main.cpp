#include <filesystem>
#include <iostream>
#include <fstream>
using namespace std;

string getDir(const std::string& File_Path) {
	size_t pos = File_Path.find_last_of("/\\");
	if (pos != std::string::npos) {
		return File_Path.substr(0, pos);
	}
	return ""; // That's an empty dir if file doesn't exist
}
void verify_dir(const std::string& new_dir) {
	if (filesystem::exists(new_dir)) {
		cout << "Subfolder exists";
	}
	else {
		error_code ec;
		if (filesystem::create_directory(new_dir, ec)) {
			cout << "Subfolder created: " << new_dir << endl;
		}
		else {
			cerr << "Error creating subfolder: " << ec.message() << endl;
		}
	}
}

int main() {
	cout << "Let's splitting \n\n";
	cout << "Enter path file: ";
	string File_Path;
	cin >> File_Path;
	ifstream infile(File_Path, ios::binary);


	infile.seekg(0, std::ios::end);
	size_t file_size = infile.tellg();
	infile.seekg(0, std::ios::beg);

	vector<unsigned char> buffer(file_size);
	infile.read(buffer.data(), file_size);
	if (!infile) {
		std::cerr << "Error reading file: " << File_Path << std::endl;
	}

	infile.close();

	cout << file_size << "\n";
	cout << "How many times do you want to split a file?: ";

	size_t numberFiles;
	cin >> numberFiles;

	std::string dir = getDir(File_Path);
	std::string subfolder = "splittedFiles";
	std::string new_dir = dir + "/" + subfolder;
	verify_dir(new_dir);

	size_t chunks = (file_size + numberFiles - 1) / numberFiles;

	for (size_t i = 0; i < numberFiles; i++) {
		size_t currentChunk = min(chunks, file_size - i * chunks);
		string outputName = new_dir + "/" + "name" + ".part" + to_string(i);

		ofstream outputFile(outputName, ios::binary);
		if (!outputFile) {
			cerr << "Error creating file:" << endl;
			break;
		}
		outputFile.write(buffer.data() + i * chunks, currentChunk);
		if (!outputFile) {
			std::cerr << "Error writing to file: " << std::endl;
		}
		outputFile.close();
	}
}

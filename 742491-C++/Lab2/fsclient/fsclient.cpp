#include "stdafx.h"
#include "fsC.h"

using namespace std;

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

	cout << "resolving naming service" << endl;
	CosNaming::NamingContext_var nc = CosNaming::NamingContext::_nil();
	try {
		ACE_Time_Value timeout(0, 1000000);
		CORBA::Object_var obj = orb->resolve_initial_references("NameService", &timeout);
		nc = CosNaming::NamingContext::_narrow(obj.in());
	}
	catch (CORBA::Exception &) {
		cerr << "could not resolve existing NameService (exeption)" << endl;
		return -1;
	}
	if (CORBA::is_nil(nc.in())) {
		cerr << "NameService: zero ptr" << endl;
		return -1;
	}
	cout << "resolved existing NameService" << endl;

	CosNaming::Name name(1);
	name.length(1);
	name[0].id = CORBA::string_dup("FileManager");
	CORBA::Object_var obj = nc->resolve(name);

	FS::FileManager_var fileManager = FS::FileManager::_narrow(obj.in());
	if (CORBA::is_nil(fileManager.in())) {
		cerr << "FileManager: zero ptr" << endl;
		return -1;
	}
	cout << "resolved FileManager object" << endl;

	FS::Properties ps;
	fileManager->query(ps);
	cout << "got " << ps.length() << " properties" << endl;

	for (uint32_t i = 0; i < ps.length(); ++i) {

	}

	try {
		FS::FileManager::MountSequence_var mounts = fileManager->getMounts();
		cout << "got " << mounts->length() << " mounts:" << endl;
		for (uint32_t i = 0; i < mounts->length(); ++i) {
			const FS::FileManager::MountType& entry = (*mounts)[i];
			cout << " mountPoint " << entry.mountPoint << endl;
			FS::FileSystem_var fs = FS::FileSystem::_duplicate(entry.fs.in());
		}
	}
	catch (...) {
		cerr << "list failed" << endl;
	}

	try {
		FS::FileSystem::FileInformationSequence_var files = fileManager->list(CORBA::string_dup("/*"));
		cout << "got " << files->length() << " entries:" << endl;
		for (uint32_t i = 0; i < files->length(); ++i) {
			const FS::FileSystem::FileInformationType& entry = (*files)[i];
			cout << " name " << entry.name << endl;
		}
	}
	catch (...) {
		cerr << "list failed" << endl;
	}

	char szBuffer[MAX_PATH];
	char szCurrDir[MAX_PATH];
	string line;
	while(true) {
		_getcwd(szCurrDir, MAX_PATH);
		cout << szCurrDir << "> ";
		getline(cin, line);
		if (line == "ls") {
			WIN32_FIND_DATA ffd;
			_stprintf(szBuffer, _T("%s\\*"), szCurrDir);
			HANDLE hFind = FindFirstFile(szBuffer, &ffd);
			if (INVALID_HANDLE_VALUE == hFind) {
				cerr << "Main: BackupDir: FindFirstFile failed: " << GetLastError() << endl;
				break;
			}
			do {
				cout << ffd.cFileName;
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					///if (_tcscmp(_T("."), ffd.cFileName) && _tcscmp(_T(".."), ffd.cFileName)) {
					///	_stprintf(szTarget, _T("%s\\%s"), szTargetDir, ffd.cFileName);
					///	_stprintf(szSource, _T("%s\\"), szSourceDir);
					///	_tcscat(szSource, ffd.cFileName);
					///}
				}
				else {
					//filesize.LowPart = ffd.nFileSizeLow;
					//filesize.HighPart = ffd.nFileSizeHigh;
				///	_stprintf(szSource, _T("%s\\%s"), szSourceDir, ffd.cFileName);
				///	_stprintf(szTarget, _T("%s\\%s"), szTargetDir, ffd.cFileName);
				}
				cout << endl;
			} while (FindNextFile(hFind, &ffd) != 0);
			FindClose(hFind);
		} else if (line == "rls") {
			try {
				FS::FileSystem::FileInformationSequence_var files = fileManager->list(CORBA::string_dup("/*"));
				cout << "got " << files->length() << " entries:" << endl;
				for (uint32_t i = 0; i < files->length(); ++i) {
					const FS::FileSystem::FileInformationType& entry = (*files)[i];
					cout << entry.name << endl;
				}
			}
			catch (...) {
				cerr << "list failed" << endl;
			}
		} else if (line.substr(0, 2) == "cd") {
			string targetDirName = line.substr(3);
			cout << "cd " << targetDirName << endl;
			_chdir(targetDirName.c_str());
		} else if (line.substr(0, 4) == "rdel") {
			string fileName = line.substr(5);
			cout << "rm " << fileName << endl;
			try {
				fileManager->remove(("/" + fileName).c_str());
			}
			catch (...) {
				cerr << "не удалось удалить файл" << endl;
			}
		} else if (line.substr(0, 3) == "get") {
			string sourceFileName = line.substr(4);
			cout << "src " << sourceFileName << endl;
			try {
				FS::File_var fIn(fileManager->open(("/" + sourceFileName).c_str(), true));
				cerr << "1." << endl;
				FS::OctetSequence_var v(new FS::OctetSequence);
				cerr << "2." << endl;
				v->length(fIn->sizeOf());
				cerr << "before read" << endl;
				fIn->read(v, fIn->sizeOf());
				cerr << "after read" << endl;
				fIn->close();
				string::size_type nIndex = sourceFileName.find_last_of(_T('/'));
				if (nIndex == string::npos)
					nIndex = sourceFileName.find_last_of(_T('\\'));
				cerr << "length " << v->length() << " nIndex " << nIndex << endl;
				if (nIndex == string::npos)
					nIndex = 0;
				if (sourceFileName[nIndex] == _T('/'))
					sourceFileName[nIndex] = _T('\\');
				_tcscpy(szBuffer, szCurrDir);
				_tcscat(szBuffer, sourceFileName.substr(nIndex).c_str());
				cerr << "dest: " << szBuffer << endl;

				FILE* fOut = _tfopen(szBuffer, _T("wb"));
				if (fOut) {
					fwrite(v->get_buffer(), 1, v->length(), fOut);
					fclose(fOut); fOut = NULL;
				}
				else {
					cerr << "failed to open output file " << szBuffer << endl;
				}
			} catch (...) {
				cerr << "не удалось получить файл" << endl;
			}
		} else if (line.substr(0, 3) == "put") {
			istringstream ostr(line.substr(4));
			string sourceFileName;// = line.substr(4);
			string destFileName;// = sourceFileName;
			cout << ": " << ostr.str() << endl;
			if (!getline(ostr, sourceFileName, ' ') || !getline(ostr, destFileName, ' ')) {
				cerr << "отсутствует исходный файл, формат команды: put source dest" << endl;
				break;
			}
			cout << "src " << sourceFileName << " dest " << destFileName << endl;

			ifstream file(sourceFileName);
			if (!file) {
				cerr << "Не удалось прочитать файл " << sourceFileName << endl;
				break;
			}
			file.seekg(0, ios::end);
			size_t size = (size_t)file.tellg();
			string content(size, ' ');
			file.seekg(0);
			file.read(&content[0], size);
			cout << "content\n" << content << endl;

			try {
				FS::OctetSequence_var v(new FS::OctetSequence(size, size, (CORBA::Octet*)content.data(), false));
				FS::File_var fOut(fileManager->create(("/" + destFileName).c_str()));
				fOut->write(v.in());
				fOut->close();
			}
			catch (...) {
				cerr << "не удалось отправить файл " << sourceFileName << endl;
			}
		} else if (line == "exit" || line == "quit" || line == "q") {
			break;
		}

	}

	return 0;
}

#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include <cmath>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

namespace pt = boost::property_tree;

using namespace std;
using namespace std::chrono;

/*
Написать программу на языке Си, создающую n процессов, параллельно выполняющих следующую задачу: Подсчет количества слов в файле. 
Входные данные (задаются посредством конфигурационного xml-файла, можно заюзать boost::property_tree):
<Config>
    <NumberOfProcess>10</NumberOfProcess> - количество создаваемых процессов 
    <InputPrefix>/home/am15/input</InputPrefix> - префикс входных файлов 
    <OutputPrefix>/home/am15/output</OutputPrefix> - префикс выходных файлов 
    <MaxFileSize>1000</MaxFileSize> - максимальный размер входного файла (в случае превышения этого значения, в выходной файл записать информацию об ошибке) 
</Config>
Родительский процесс генерирует файлы с произвольным количеством слов и отдает по одному входному файлу каждому из p дочерних процессов, 
каждый из дочерних процессов вызовом семейства exec подменяет свой код кодом программы, 
которая подсчитывает количество слов во входном файле и формирует выходной файл следующего формата: 
Name: *Имя входного файла* 
Count: *Количество слов в файле* Pid: *id процесса, который выполнял работу* Time: *Время выполнения* Error: *В случае превышения MaxFileSize*
*/

class Settings {
public:
    bool read(const string& fileName) {
        try {
            pt::ptree tree;
            pt::read_xml(fileName, tree, pt::xml_parser::no_comments | pt::xml_parser::trim_whitespace);
            for( const auto& root : tree ) { // root - это <Config>
                for( const auto& v : root.second ) {
                    //cout << "data(size is " << v.second.data().size() << "): first \"" << v.first << "\" second \"" << v.second.data() << "\"" << endl;
                    istringstream iss(v.second.data());
                    if(v.first == "NumberOfProcess") {
                        iss >> _procNum;
                    } else if(v.first == "InputPrefix") {
                        iss >> _inPrefix;
                    } else if(v.first == "OutputPrefix") {
                        iss >> _outPrefix;
                    } else if(v.first == "MaxFileSize") {
                        iss >> _maxInSize;
                    }
                }
                break; // considering single root tag
            }
        } catch( const pt::ptree_bad_data& error ) {
            cerr << "Failed to read xml settings (ptree_bad_data): " << error.what() << endl;
            return false;
        } catch (const pt::ptree_bad_path& error) {
            cerr << "Failed to read xml settings (ptree_bad_path): " << error.what() << endl;
            return false;
        }
        return true;
    }
    
    size_t procNum() const {
        return _procNum;
    }
    
    string inPrefix() const {
        return _inPrefix;
    }
    
    string outPrefix() const {
        return _outPrefix;
    }
    
    size_t maxInSize() const {
        return _maxInSize;
    }

private:
    // количество создаваемых процессов
    size_t _procNum = 0;
    // префикс входных файлов
    string _inPrefix = "./";
    // префикс выходных файлов
    string _outPrefix = "./";
    // максимальный размер входного файла
    size_t _maxInSize = 0;
};

string generate_content(size_t max_file_size) {
    const size_t max_word_length = 15;
    string content;
    content.reserve(max_file_size);
    bool space = false; // пробел разрешён
    for(size_t i = 0; i < max_file_size; i++) {
        // вставляем случайным образом пробел (в противном случае - случайный символ)
        int p = rand() % 5;
        if(p > 0 || !space) {
            content.push_back('a' + (rand() % ('z' - 'a' + 1)));
            space = true; // вставили хоть одну букву - разрешаем пробел
        } else {
            content.push_back(' ');
            space = false; // запрещаем подряд идущие пробелы
        }
    }
    return content;
}

vector<string> generate_input_files(const string& prefix, size_t count, size_t max_file_size) {
    vector<string> names;
    cout << "generate_input_files: prefix \"" << prefix << "\" count " << count << " max_file_size " << max_file_size << endl;
    ofstream file;
    for(size_t i = 0; i < count; i++) {
        ostringstream oss;
        oss << prefix << "." << i << ".txt";
        cout << " file: " << oss.str() << endl;
        file.open(oss.str(), fstream::out);
        if(file.is_open()) {
            names.push_back(oss.str());
            file << generate_content(max_file_size);
            file.close();
        } else {
            cout << "не удалось создать файл " << oss.str();
        }
    }
    return names;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    
    Settings settings;
    if(!settings.read("./lab.xml")) {
        return -1;
    }

    vector<string> inFileNames = generate_input_files(settings.inPrefix(), settings.procNum(), settings.maxInSize());
    cout << "generated " << inFileNames.size() << " input files:" << endl;
    for(const auto name: inFileNames)
        cout << " " << name << endl;

    size_t procCount = settings.procNum();
    for (size_t i = 0; i < procCount; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            printf("Невозможно создать процесс\n");
            exit(1);
        } else if (pid == 0) {
            // дочерний процесс
            ostringstream oss;
            ifstream input;
            ofstream output;
            string s;
            // читаем свой файл с исходными данными
            cout << " read input file: " << inFileNames[i] << endl;
            input.open(inFileNames[i], fstream::in);
            if(input.is_open()) {
                // вычисляем его размер
                input.seekg (0, input.end); // перенести указатель на конец файла
                int size = input.tellg();   // получить размер файла
                input.seekg (0, input.beg); // вернуть указатель в начало
                // создаём файл для записи результатов
                oss << settings.outPrefix() << "." << i << ".txt";
                output.open(oss.str(), fstream::out);
                if(output.is_open()) {
                    // сохраняем начальную метку времени
                    auto start = steady_clock::now();
                    // читаем слова и считаем их количество
                    size_t wordCount = 0;
                    while(input >> s) {
                        wordCount++;
                        cout << wordCount << "\t " << s << "\t -> " << input.tellg() << endl;
                    }
                    // вычисляем время подсчёта
                    auto finish = steady_clock::now();
                    auto dur = finish - start;
                    // записываем результаты в файл
                    output << "Name: " << inFileNames[i] << '\n';
                    output << "Count: " << wordCount << '\n';
                    output << "Pid: " << getpid() << '\n';
                    output << "Time: " << duration_cast<milliseconds>(dur).count() << " ms\n";
                    if(size > settings.maxInSize()) {
                        output << "Error: " << size << "/" << settings.maxInSize() << '\n';
                    }
                    output.close();
                    cout << " wrote output file: " << oss.str() << endl;
                }
                input.close();
            }
            // дочерний поток выполнил работу, завершить
            exit(0);
        }
    } // for (цикл по процессам)
    cout << "Ожидание дочерних процессов... ";
    for (size_t i = 0; i < procCount; i++) {
        int status;
        wait(&status);
    }
    cout << "завершено" << endl;
    return 0;
}


//
// Created by dell-pc on 2017/4/5.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <algorithm>

using namespace std;
string fileName;
ofstream out;
map<string, string> my_map;
map<string, string>::iterator iter;
bool judgeIf = true;
ifstream in;
string buf;

void openFile() {
    ifstream in2;
    in2.open(fileName);
    string buf;
    while (getline(in2, buf)) {
        const char *c = buf.c_str();
        if (*c != '#') {
            out << buf << endl;
        }
    }
}

bool compare(string res, string panduan) {
    if (res.find(panduan) < 100) {
        return true;
    } else return false;
}

void judgeCondition(int judge, string res, bool flag) {
    if (compare(res, ">=")) {
        string res2 = res.substr(2, res.length() - 2);
        int num2 = atoi(res2.c_str());
        if (judge < num2) {
            getline(in, buf);
        }
        if (flag) {
            judgeIf = false;
        }
    } else if (compare(res, "<=")) {
        string res2 = res.substr(2, res.length() - 2);
        int num2 = atoi(res2.c_str());
        if (judge > num2) {
            getline(in, buf);
        }
        if (flag) {
            judgeIf = false;
        }
    } else if (compare(res, "<")) {
        string res2 = res.substr(1, res.length() - 1);
        int num2 = atoi(res2.c_str());
        if (judge >= num2) {
            getline(in, buf);
        }
        if (flag) {
            judgeIf = false;
        }
    } else if (compare(res, ">")) {
        string res2 = res.substr(1, res.length() - 1);
        int num2 = atoi(res2.c_str());
        if (judge <= num2) {
            getline(in, buf);
        }
        if (flag) {
            judgeIf = false;
        }
    } else if (compare(res, "==")) {
        string res2 = res.substr(2, res.length() - 2);
        int num2 = atoi(res2.c_str());
        if (judge != num2) {
            getline(in, buf);
        }
        if (flag) {
            judgeIf = false;
        }
    }
}

int main() {
    cout << "input the file name:";
    getline(cin, fileName);
    in.open(fileName);
    if (fileName == "test.cpp") {
        out.open("result.cpp");
    } else if (fileName == "test_2.cpp") {
        out.open("result_2.cpp");
    }
    if (!in) {
        cout << "error!" << buf << endl;
        return 1;
    }
    while (getline(in, buf)) {
        if(buf==""){
            continue;
        }
        const char *firstLetter = buf.c_str();
        string s1, s2, s3, s4;
        istringstream is(buf);
        is >> s1 >> s2 >> s3 >> s4;
        if (*firstLetter == '#') {
            const char *secondFirstLetter = s2.c_str();
            if (s1 == "#include" && *secondFirstLetter == '<') {
                out << buf << endl;
            } else if (s1 == "#include" && *secondFirstLetter == '"') {
                fileName = s2.substr(1, s2.length() - 2);
                openFile();
                fileName=fileName.replace((unsigned int)fileName.find("."),1,"_");
                transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);
                my_map.insert(pair<string, string>(fileName,""));
            } else if (s1 == "#define") {
                if (s2.find("(") < 100) {
                    int position = s2.find("(");
                    s2 = s2.substr(0, (unsigned int) position);
                    while (s3.find("x") < 100) {
                        int position = s3.find("x");
                        s3.replace((unsigned int) position, 1, "a");
                    }
                    for (map<string, string>::iterator i = my_map.begin(); i != my_map.end(); i++) {
                        if (s3.find(i->first) < 100) {
                            int length = (i->first).length();
                            int position = s3.find(i->first);
                            s3.replace((unsigned int) position, (length + 3), i->second);
                        }
                    }
                    my_map.insert(pair<string, string>(s2, s3));
                } else {
                    my_map.insert(pair<string, string>(s2, s3));
                }

            } else if (s1 == "#ifdef") {
                iter = my_map.find(s2);
                if (iter == my_map.end()) {
                    getline(in, buf);
                }
            } else if (s1 == "#undef") {
                my_map.erase(s2);
            } else if (s1 == "#ifndef") {
                iter = my_map.find(s2);
                if (iter != my_map.end()) {
                    getline(in, buf);
                }
            } else if (s1 == "#elif") {
                bool flag = false;
                int judge;
                string contain;
                for (map<string, string>::iterator i = my_map.begin(); i != my_map.end(); i++) {
                    if (s2.find(i->first) < 100) {
                        flag = true;
                        contain = i->first;
                        string s = i->second;
                        judge = atoi(s.c_str());
                    }
                }
                if (flag) {
                    string res = s2.substr(contain.length(), s2.length());
                    judgeCondition(judge, res, false);
                    if (s3 == "&&") {
                        res = s4.substr(contain.length(), s4.length());
                        judgeCondition(judge, res, true);
                    }
                }
            } else if (s1 == "#else") {
                if (!judgeIf) {
                    getline(in, buf);
                }
                judgeIf = true;
            }

        } else {
            string temp;
            if (s4.find("(") < 100) {
                temp = s4.substr(0, s4.length() - 4);
            } else {
                temp = s4.substr(0, s4.length() - 1);
            }
            iter = my_map.find(temp);
            if (iter != my_map.end()) {
                int position = buf.find(s4);
                buf.replace((unsigned int) position, s4.length() - 1, iter->second);
            }
            out << buf << endl;
        }
    }
    in.close();
    out.close();
    return 0;
}


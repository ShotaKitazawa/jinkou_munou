#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

void randinit() { srand((unsigned int)time(NULL)); }

int sinit(char* s, char* v) {
    int count = 0;
    if ((*v & 0xf0) == 0xe0) {
	count = 3;
	for (int i = 0; i < 3; i++) {
	    *s = *v;
	    s++;
	    v++;
	}
    } else if ((*v & 0xe0) == 0xc0) {
	count = 2;
	for (int i = 0; i < 2; i++) {
	    *s = *v;
	    s++;
	    v++;
	}
    } else if ((*v & 0x80) == 0) {
	count = 1;
	*s = *v;
	s++;
	v++;
    }
    *s = '\0';
    return count;
}

int main(int argc, char* argv[]) {
    randinit();

    char* v = *argv;
    while (*v != '\0') v++;
    v++;
    char s[6];
    int n = sinit(s, v);
    char buf[409500];
    char* bp = buf;
    char tmpbuf[6];
    int count = 0;
    int tmp = 0;
    string str;
    string check(s);

    cout << check;
    ifstream ifs_noun("./noun.txt");
    while (ifs_noun.good()) {
	getline(ifs_noun, str);
	if (str.substr(0, n) == s) {
	    count++;
	    for (int i = n; str[i] != '\0'; i++) {
		*bp = str[i];
		bp++;
		tmp++;
	    }
	    for (int i = tmp; i < 3; i++) {
		*bp = ' ';
		bp++;
	    }
	    *bp = ',';
	    bp++;

	    // cout << buf<<endl; //デバッグ用
	}
	tmp = 0;
    }
    *bp = '\0';

    for (int i = 0; i < 6; i++) s[i] = '\0';
    for (int i = 0; i < 6; i++) tmpbuf[i] = '\0';

    int r = rand() % count;
    bp = buf;
    while (r) {
	while (*bp != ',') {
	    bp++;
	}
	bp++;
	r--;
    }

    while (*bp != ',') {
	if ((*bp & 0xff) !=
	    0x20) {  //もし半角スペースでないなら ⇒ 1byte,2byte 文字でないなら
	    cout << *bp;
	}
	bp++;
    }

    cout << "は";

    ifstream ifs_verb("./verb.txt");
    count = 0;
    while (ifs_verb.good()) {
	getline(ifs_verb, str);
	count++;
    }
    ifs_verb.close();
    ifs_verb.open("./verb.txt");
    r = rand() % count;
    while (r) {
	getline(ifs_verb, str);
	r--;
    }
    getline(ifs_verb, str);
    cout << str << "。" << endl;
}

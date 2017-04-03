#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

void randinit() { srand((unsigned int)time(NULL)); }

int arraylen(char* cp) {
    int count = 0;
    while (*cp != '\0') {
	if ((*cp & 0xf0) == 0xe0) {
	    cp++;
	    cp++;
	    cp++;
	} else if ((*cp & 0xe0) == 0xc0) {
	    cp++;
	    cp++;
	} else if ((*cp & 0x80) == 0) {
	    cp++;
	}
	count++;
    }
    return count;
}

void randchoice(char* op, char* ip, int len) {  // outoutpointer , inputpointer
    int n = rand() % len;
    for (int i = 0; i < n; i++) {
	if ((*ip & 0xf0) == 0xe0) {
	    ip++;
	    ip++;
	    ip++;
	} else if ((*ip & 0xe0) == 0xc0) {
	    ip++;
	    ip++;
	} else if ((*ip & 0x80) == 0) {
	    ip++;
	}
    }
    if ((*ip & 0xf0) == 0xe0) {
	for (int i = 0; i < 3; i++) {
	    *op = *ip;
	    op++;
	    ip++;
	}
    } else if ((*ip & 0xe0) == 0xc0) {
	for (int i = 0; i < 2; i++) {
	    *op = *ip;
	    op++;
	    ip++;
	}
    } else if ((*ip & 0x80) == 0) {
	*op = *ip;
	op++;
	ip++;
    }
    *op = '\0';
}

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

int main() {
    randinit();

    char argv[255];
    cout << "> ";
    cin >> argv;
    char* v = argv;
    // while (*v != '\0') v++;
    // v++;
    char s[6];
    char initbuf[255];
    char* ip = initbuf;
    int len;
    int n;
    char buf[409500];
    char* bp = buf;
    char tmpbuf[6];
    int count = 0;
    int errorcount = 0;
    int tmp = 0;
    string str;
    string check;

    do {
	len = arraylen(v);
	randchoice(ip, v, len);
	n = sinit(s, ip);
	bp = buf;
	count = 0;
	tmp = 0;

	check = s;
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
	if (count != 0) break;
	errorcount++;
	if (errorcount > 127) {
	    cerr << "返答できません。" << endl;
	    return 1;
	}
    } while (true);

    cout << check;

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

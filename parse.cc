#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
using namespace std;

ofstream ofs_adj("./adj.txt");
ofstream ofs_verb("./verb.txt");
ofstream ofs_adjv("./adjv.txt");
ofstream ofs_noun("./noun.txt");
char tmpbuf[1023];
char* tmpp = tmpbuf;

void check(char* bp) {
    bp--;
    bp--;
    bp--;
    if ((*bp & 0xff) == 0xe3) {
	bp++;
	if ((*bp & 0xff) == 0x81) {
	    bp++;
	    if ((*bp & 0xff) == 0x84) {  //形容詞
		ofs_adj << tmpbuf << endl;
		// cout << tmpbuf;
		return;
	    } else if ((*bp & 0xff) == 0x86) {  //動詞
		ofs_verb << tmpbuf << endl;
		// cout << tmpbuf;
		return;
	    } else if ((*bp & 0xff) == 0xa0) {  //形容動詞
		ofs_adjv << tmpbuf << endl;
		// cout << tmpbuf;
		return;
	    }
	}
    }
    if (*tmpbuf != '\0') ofs_noun << tmpbuf << endl;
}

void print(char* bp, int& flag, int& preflag, int byte) {
    if (preflag == 0 && flag == 0 && byte == -1) {
	*tmpp = '\0';
	check(bp);
	return;
    }
    for (int i = 0; i < byte; i++) bp--;
    if (preflag == 2 && flag == 4) {
	*tmpp = '\0';
	check(bp);
	tmpp = tmpbuf;
	while (*tmpp != '\0') {
	    *tmpp = '\0';
	    tmpp++;
	}
	tmpp = tmpbuf;
    }

    if ((*bp & 0xff) == 0xe3) {
	bp++;
	if ((*bp & 0xff) == 0x80) {
	    bp++;
	    if ((*bp & 0xff) == 0x81 || (*bp & 0xff) == 0x82) {
		return;
	    }
	    bp--;
	}
	bp--;
    }

    for (int i = 0; i < byte; i++) {
	*tmpp = *bp;
	tmpp++;
	bp++;
    }
}

int main(int argc, char** argv) {
    ifstream ifs("./botchan.txt");
    string str;
    const char* sp;
    char buf[65535];
    char* bp = buf;
    int flag = 0;
    int preflag = 0;

    while (ifs.good()) {
	getline(ifs, str);
	sp = str.c_str();
	if (str.substr(0, 3) == "　" || str.substr(0, 3) == "「") {
	    if (str.substr(0, 3) == "　") {
		sp++;
		sp++;
		sp++;
	    }
	    while (*sp != '\0') {
		if ((*sp & 0xf0) == 0xe0) {  // 3byte
		    sp++;
		    // ひらがな、カタカナ、漢字を判別
		    if ((*sp & 0xff) == 0x81) {  //ひらがな
			sp--;
			flag = 2;
			for (int i = 0; i < 3; i++) {
			    *bp = *sp;
			    bp++;
			    sp++;
			}
		    } else if ((*sp & 0xff) == 0x83) {  //カタカナ
			sp--;
			flag = 3;
			for (int i = 0; i < 3; i++) {
			    *bp = *sp;
			    bp++;
			    sp++;
			}
		    } else if ((*sp & 0xff) == 0x82) {
			sp++;
			if ((*sp & 0xe0) == 0x80) {  //ひらがな
			    flag = 2;
			    sp--;
			    sp--;
			    for (int i = 0; i < 3; i++) {
				*bp = *sp;
				bp++;
				sp++;
			    }
			} else if ((*sp & 0xe0) == 0xa0) {  //カタカナ
			    flag = 3;
			    sp--;
			    sp--;
			    for (int i = 0; i < 3; i++) {
				*bp = *sp;
				bp++;
				sp++;
			    }
			}
		    } else {  // 漢字
			sp--;
			flag = 4;
			for (int i = 0; i < 3; i++) {
			    *bp = *sp;
			    bp++;
			    sp++;
			}
		    }
		} else if ((*sp & 0x80) == 0) {  // 1byte
		    flag = 1;
		    *bp = *sp;
		    bp++;
		    sp++;
		}
		switch (flag) {
		    case 1:  // 1byte
			print(bp, flag, preflag, 1);
			break;
		    case 2:  //ひらがな
			print(bp, flag, preflag, 3);
			break;
		    case 3:  //カタカナ
			print(bp, flag, preflag, 3);
			break;
		    case 4:  //漢字
			print(bp, flag, preflag, 3);
			break;
		}
		preflag = flag;
	    }
	    flag = 0;
	    preflag = 0;
	    print(bp, flag, preflag, -1);
	    bp = buf;
	    while (*bp != '\0') {
		*bp = '\0';
		bp++;
	    }
	    bp = buf;
	}
    }
    ofs_adj.close();
    ofs_verb.close();
    ofs_adjv.close();
    ofs_noun.close();
}

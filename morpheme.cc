#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
using namespace std;

string ifsinit() {
    ifstream ifs("./morpheme.txt");
    string str;
    getline(ifs, str);
    return str;
}

void print(char* bp, int& flag, int& preflag, int byte) {
    for (int i = 0; i < byte; i++) bp--;
    if (preflag) {
	if (flag != preflag) cout << endl;
    }
    cout << bp;
    for (int i = 0; i < byte; i++) bp++;
}

int main(int argc, char** argv) {
    string str = ifsinit();
    const char* sp = str.c_str();
    char buf[65535];
    char* bp = buf;
    int flag = 0;
    int preflag = 0;

    while (*sp != '\0') {
	if ((*sp & 0xf0) == 0xe0) {  // 3byte
	    sp++;		// ひらがな、カタカナ、漢字を判別
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
}

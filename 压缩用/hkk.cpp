#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<string>
#include<vector>
#include<ctime>
#include<Windows.h>
#include<mmsystem.h>
#include<fstream>
#include<thread>
#include<conio.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#undef min
#undef max

#define cint char

#define infi 0x3f3f3f3f
#define e 0.00001
#define N 114514//谱的最长长度
#define MBG 2233//最大歌曲数量
#define MPU 81//最大谱面数量
#define C 233//字符串的最长长度
#define OP 7//显示最多选项
#define CH 127//char最大值

#define sleep 500
#define K 4//键位
#define M 60//一分钟
#define D 3//难度
#define A 220
#define B 240

#define esc 27
#define enter1 13
#define enter2 108
#define left 37
#define up 38
#define right 39
#define down 40
#define backspace 8
#define shift 16
#define capslk 20
#define space ' '
#define dele 46
#define sl Sleep(sleep)
const int namesize = 50;
const int WIDTH = 618, HIGH = 240, BUFSIZE = WIDTH * HIGH;
struct IMAGE {
	char c[BUFSIZE];
	int length;
};
//背景字符画
IMAGE homebackground, startgamebackground, mchoosebackground,rchoosebackground,gamebackground1;
//小型素材
IMAGE kuangkuang, cursor, jiantou, yinfu,click,press,progressbar, progressbar0,longpress;
//字符集
IMAGE number[10] ,alpha[26],alpha1[26],alpha2[15],alpha3[7],alpha4[6],alpha5[4];
//动画
IMAGE animation[4];
struct aninode {
	aninode* next,*last;
	clock_t t;
	cint ind=-1;
};
char backname[] = { "./picture/homebackground(replace)(replace).txt" }, choosename[] = { "./tool/(replace)(replace)()kuangkuang.txt" };
int frame = 0;
FILE *file;
char buffer[BUFSIZE];
char blank[BUFSIZE];
int frontbuffer = 1, backbuffer = 0;
HANDLE hout;HANDLE sout;
HANDLE newconsolebuffer() {
	return CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}
void gotoxy(int x, int y, HANDLE& h) {
	COORD cd = { x,y };
	SetConsoleCursorPosition(h, cd);
}
void clearscreen(HANDLE& h) {
	DWORD r;
	COORD c = { 0,0 };
	FillConsoleOutputAttribute(h, 0, BUFSIZE, c, &r);
	gotoxy(0, 0, h);
}
void hidecursor(HANDLE& h, bool visi) {
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = !visi;
	Cursor.dwSize = sizeof(Cursor);
	SetConsoleCursorInfo(h, &Cursor);
}
void setpicturefont(HANDLE&h) {
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.nFont = 0; font.dwFontSize = { 2,3 };
	font.FontFamily = 54; font.FontWeight = 400;
	wcscpy_s(font.FaceName, L"黑体");
	SetCurrentConsoleFontEx(h, 0, &font);
	system("color 0F");
}
void setwindow(HANDLE& h) {
	COORD size = { 617, 240 };
	SetConsoleScreenBufferSize(h, size);
	//SMALL_RECT rc = { 1,1, 617, 240 };
	//SetConsoleWindowInfo(h, true, &rc);
	system("mode con cols=617 lines=240");
	/*CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(h, &info);
	size = { info.srWindow.Right + 1 ,info.srWindow.Bottom + 1 };
	SetConsoleScreenBufferSize(h, size);*/
}
inline void settoblank(char buffer[BUFSIZE]) {
	for (int i = 0; i < BUFSIZE; i++)
	{
		if ((i + 1) % 618) buffer[i] = ' ';
		else buffer[i] = '\n';
	}
	buffer[BUFSIZE - 1] = '\0';
}
void paint(int x, int y, IMAGE& image, char buffer[BUFSIZE]) {
	int yi = max(-y, 0), xi = max(-x, 0), yb = max(y, 0), xb = max(x, 0);
	int pimag = yi * WIDTH + xi, pbuffer = yb * WIDTH + xb;
	int cnt = 0;
	for (; pbuffer < BUFSIZE - 1 && pimag < image.length; pbuffer++, pimag++)
	{
		if (buffer[pbuffer] == '\n' || buffer[pimag] == '\n') {
			++cnt;
			pimag = (yi + cnt)*WIDTH + xi - 1; pbuffer = (yb + cnt)*WIDTH + xb - 1;
		}
		else if (image.c[pimag] != ' '&&image.c[pimag] != '\n') {
			buffer[pbuffer] = image.c[pimag];
		}
	}
}
void paintlr(int x, int y, IMAGE& image,int l,int r,char buffer[BUFSIZE]) {
	int yi = max(-y, 0), xi = max(-x, 0), yb = max(y, 0), xb = max(x, 0);
	int pimag = yi * WIDTH + xi, pbuffer = yb * WIDTH + xb;
	int cnt = 0;
	for (; pbuffer < BUFSIZE - 1 && pimag < image.length; pbuffer++, pimag++)
	{
		if (buffer[pbuffer] == '\n' || buffer[pimag] == '\n') {
			++cnt;
			pimag = (yi + cnt)*WIDTH + xi - 1; pbuffer = (yb + cnt)*WIDTH + xb - 1;
		}
		else if (image.c[pimag] != ' '&&image.c[pimag] != '\n') {
			if(pbuffer%WIDTH>=l&&pbuffer%WIDTH<=r)
			buffer[pbuffer] = image.c[pimag];
		}
	}
}
void paintud(int x, int y, IMAGE& image, int u, int d, char buffer[BUFSIZE]) {
	int yi = max(-y, 0), xi = max(-x, 0), yb = max(y, 0), xb = max(x, 0);
	int pimag = yi * WIDTH + xi, pbuffer = yb * WIDTH + xb;
	int cnt = 0;
	for (; pbuffer < BUFSIZE - 1 && pimag < image.length; pbuffer++, pimag++)
	{
		if (buffer[pbuffer] == '\n' || buffer[pimag] == '\n') {
			++cnt;
			pimag = (yi + cnt)*WIDTH + xi - 1; pbuffer = (yb + cnt)*WIDTH + xb - 1;
		}
		else if (image.c[pimag] != ' '&&image.c[pimag] != '\n') {
			if (pbuffer/WIDTH >= u && pbuffer/WIDTH <= d)
				buffer[pbuffer] = image.c[pimag];
		}
	}
}
void paintcl(int x, int y, IMAGE& image, char buffer[BUFSIZE]) {
	int yi = max(-y, 0), xi = max(-x, 0), yb = max(y, 0), xb = max(x, 0);
	int pimag = yi * WIDTH + xi, pbuffer = yb * WIDTH + xb;
	int cnt = 0;
	for (; pbuffer < BUFSIZE - 1 && pimag < image.length; pbuffer++, pimag++)
	{
		if (buffer[pbuffer] == '\n' || buffer[pimag] == '\n') {
			++cnt;
			pimag = (yi + cnt)*WIDTH + xi - 1; pbuffer = (yb + cnt)*WIDTH + xb - 1;
		}
		else if (image.c[pimag] != ' '&&image.c[pimag] != '\n') {
			buffer[pbuffer] = ' ';
		}
	}
}
inline void render(char buff[BUFSIZE], HANDLE& showbuffer, HANDLE& hidebuffer) {
	clearscreen(hidebuffer);
	DWORD r;
	WriteConsole(hidebuffer, buff, BUFSIZE, &r, NULL);
}
inline void buffertoview(HANDLE& showbuffer, HANDLE& hidebuffer) {
	SetConsoleActiveScreenBuffer(hidebuffer);
	swap(showbuffer, hidebuffer);
}
void read(IMAGE& image,const char name[]) {
	file = fopen(name, "r");
	fread(image.c, BUFSIZE * sizeof(char), 1, file);
	image.c[BUFSIZE - 1] = '\0';
	int l = 0, m = -1, cnt = 0;
	for (int i = 0; i < BUFSIZE - 1; i++)
	{
		if (image.c[i] != ' ')
		{
			if (image.c[i] != '\n') {
				l = i;
				m = i;
			}
			else {
				image.c[m + 1] = '\n';
				m = (++cnt)*WIDTH - 1;
			}
		}
	}
	image.length = l + 1;
	fclose(file);
}
void initial() {
	settoblank(buffer);
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	setpicturefont(hout); setwindow(hout);
	sout = newconsolebuffer();
	hidecursor(hout, 1); hidecursor(sout, 1);
	read(homebackground, "./picture/homebackground.txt");
	read(startgamebackground, "./picture/startgamebackground.txt");
	read(mchoosebackground, "./picture/mchoosebackground.txt");
	read(rchoosebackground, "./picture/rchoosebackground.txt");
	read(gamebackground1, "./picture/gamebackground1.txt");
	read(jiantou, "./tool/jiantou.txt");
	read(kuangkuang, "./tool/kuangkuang.txt");
	read(yinfu, "./tool/yinfu.txt");
	read(cursor, "./tool/cursor.txt");
	read(click, "./tool/click.txt");
	read(press, "./tool/press.txt");
	read(longpress, "./tool/longpress.txt");
	read(progressbar, "./tool/progressbar.txt");
	read(progressbar0, "./tool/progressbar0.txt");
	char name[] = { "(replace)(replace)0000000000000000000" };
	char tmp[] = { "(replace)" };
	strcpy(name, "./alpha/");
	for (int i = 0; i < 10; i++)
	{
		sprintf(tmp,"%d", i);
		strcat(name, tmp);
		strcat(name, ".txt");
		read(number[i], name);
		strcpy(name, "./alpha/");
	}
	for (int i = 0; i < 26; i++)
	{
		sprintf(tmp, "%c", i+'a');
		strcat(name, tmp);
		strcat(name, ".txt");
		read(alpha[i], name);
		strcpy(name, "./alpha/");
	}
	strcpy(name, "./alpha/alpha/");
	for (int i = 0; i < 26; i++)
	{
		sprintf(tmp, "%c", i + 'A');
		strcat(name, tmp);
		strcat(name, ".txt");
		read(alpha1[i], name);
		strcpy(name, "./alpha/alpha/");
	}
	strcpy(name, "./alpha/specialalpha/");
	for (int i = 0; i < 15; i++)
	{
		sprintf(tmp, "%d", i + 33);
		strcat(name, tmp);
		strcat(name, ".txt");
		read(alpha2[i], name);
		strcpy(name, "./alpha/specialalpha/");
	}
	for (int i = 0; i < 7; i++)
	{
		sprintf(tmp, "%d", i + 58);
		strcat(name, tmp);
		strcat(name, ".txt");
		read(alpha3[i], name);
		strcpy(name, "./alpha/specialalpha/");
	}
	for (int i = 0; i < 6; i++)
	{
		sprintf(tmp, "%d", i + 91);
		strcat(name, tmp);
		strcat(name, ".txt");
		read(alpha4[i], name);
		strcpy(name, "./alpha/specialalpha/");
	}
	for (int i = 0; i < 4; i++)
	{
		sprintf(tmp, "%d", i + 123);
		strcat(name, tmp);
		strcat(name, ".txt");
		read(alpha5[i], name);
		strcpy(name, "./alpha/specialalpha/");
	}
	strcpy(name, "./animation/");
	for (int i = 0; i < 4; i++)
	{
		sprintf(tmp, "%d", i);
		strcat(name, tmp);
		strcat(name, ".txt");
		read(animation[i], name);
		strcpy(name, "./animation/");
	}
}
template<typename T>inline bool pxp(const T* a, const T* b) {//指针排序
	return *a < *b;
}
template<typename T>inline int bsb(const T num[], int l, int r, const T& tar) {//二分查找目标的值或后继(binary search bigger)
	int ans = r + 1;
	for (int m = l + r >> 1; l < r; m = l + r >> 1) {
		if (num[m] < tar)
			l = m + 1;
		else
			ans = r = m;
	}
	return ans;
}

int pow2(const int& n) {//平方(pow(n,2))
	return n * n;
}
inline void fn(const string s[], const int s1, const string& tar, int ans[], int& ans1, int& l, int& r) {//找名字(find name)
	static int kmp[C] = { -1 };
	const int t1 = tar.size();
	for (int x = 1, k1 = -1; x < t1; ++x) {
		for (; k1 != -1 && tar[k1 + 1] != tar[x]; k1 = kmp[k1]);
		kmp[x] = k1 += tar[k1 + 1] == tar[x];
	}
	ans1 = 0;
	long long lat = clock();
	for (int x = 1, y, si1, k1; x <= s1; ++x) {
		for (y = 0, si1 = s[x].size(), k1 = -1; y < si1; ++y) {
			for (; k1 != -1 && tar[k1 + 1] != s[x][y]; k1 = kmp[k1]);
			if (k1 + 1 < t1)
				k1 += tar[k1 + 1] == s[x][y];
			if (k1 == t1 - 1) {
				ans[ans1++] = x;
				break;
			}
		}
		if (clock() - lat > 100)
			break;
	}
	l = -!ans1;
	r = min(OP, ans1) - 1 - !ans1;
}

inline bool pddi(const char& c) {//判断char是否为数字(pd:(pd->pan'duan->判断) digit)
	return '0' <= c && c <= '9';
}
inline bool pda(const char& c) {//判断char是否是小写字母(pd abcdefghijklmnopqrstuvwxyz)
	return 'a' <= c && c <= 'z';
}
inline bool pdA(const char& c) {//判断char是否是大写字母(pd ABCDEFGHIJKLMNOPQRSTUVWXYZ)
	return 'A' <= c && c <= 'Z';
}
inline bool pdAa(const char& c) {//判断char是否是字母(pd ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz)
	return pda(c) || pdA(c);
}
inline bool pdk(const int& c) {//(pd key)键按下为非0
	return GetAsyncKeyState(c) & 0x8000;
}
inline char pdnu(const char* c) {//判断是否是数字(pd number)返回-1则是整数 返回-2则不合法
	const cint c1 = strlen(c);
	cint xp = -1;
	for (cint x = 0; x < c1; ++x) {
		if (!(pddi(c[x]) || c[x] == '.') || xp != -1 && c[x] == '.')
			return -2;
		else if (c[x] == '.')
			xp = x;
	}
	return xp;
}
inline bool pdna(const string& c) {//判断命名是否合法(pd name)
	static const char unc[] = "\'/:*?\"<>|";
	static const int unc1 = 9;
	const int& c1 = c.size();
	for (int x = 0, y; x < c1; ++x)
		for (y = 0; y < unc1; ++y)
			if (c[x] == unc[y])
				return 0;
	return 1;
}

inline int cti(const char* c) {//char to int
	const cint c1 = strlen(c);
	int ans = 0;
	for (cint x = 0; x < c1 && pddi(c[x]) && ans < 100000000; ++x)
		ans = ans * 10 + c[x] - '0';
	return ans;
}
inline int ctik(const char* c, const cint xp) {//char to int with *1000
	if (pdnu(c) == -2)
		return -1;
	const cint c1 = strlen(c);
	int ans = cti(c) * 1000;
	for (cint x = xp + 1, w = 100; x < c1 && w; ++x, w /= 10)
		ans += (c[x] - '0') * w;
	return ans;
}
inline double ctd(const char* c) {//char to double
	if (pdnu(c) == -2)
		return -1;
	const cint c1 = strlen(c);
	static cint xp;
	double ans = 0, w = 0.1;
	for (xp = 0; xp < c1 && c[xp] != '.'; ++xp);
	for (cint x = 0; x < xp; ++x)
		ans = ans * 10 + c[x] - '0';
	for (cint x = xp + 1; x < c1; ++x, w /= 10)
		ans += (c[x] - '0') * w;
	return ans;
}
template<typename T>inline string nts(const T& n) {//integer to string
	if (!n)
		return "0";
	string ans;
	int w = 10;
	for (; n / w >= 1; w *= 10);
	for (w /= 10; w; w /= 10)
		ans += ((int)floor(n) / w) % 10 + '0';
	double dn = floor(n);
	if (abs(dn - n) > e) {
		ans += '.';
		static int nn;
		for (w = 10; abs(dn - n) > e; w *= 10) {
			nn = (int)floor(n * w) % 10;
			dn += (double)nn / w;
			ans += nn + '0';
		}
	}
	return ans;
}
int temt;
inline int stt(const string& st) {//时间字符串（分秒或秒）转时间(string to time) 返回-1则字符串不合法
	const cint c1 = st.size();
	cint mp = c1, xp = c1;
	for (cint x = 0; x < c1; ++x) {
		if (!(pddi(st[x]) || st[x] == ':' || st[x] == '.') || st[x] == ':' && mp != c1 || st[x] == '.' && xp != c1)
			return -1;
		if (st[x] == ':')
			mp = x;
		else if (st[x] == '.')
			xp = x;
	}
	if (mp != c1 && xp != c1 && xp < mp)
		return -1;
	if (mp == c1)
		return temt = ctik(st.c_str(), xp);
	const int& anf = cti(st.substr(0, mp).c_str()), &anm = ctik(st.substr(mp + 1).c_str(), xp - mp - 1);
	if (anm >= M * 1000)
		return -1;
	return temt = anf * M * 1000 + anm;
}
inline string tts(const int& ti) {//时间转时间字符串（分秒）(time to string)
	int anf = ti / 1000 / M, an = ti / 1000 % M;
	if (an < 10)
		return nts(anf) + ":0" + nts(an);
	return nts(anf) + ":" + nts(an);
}
//显示(struct display) 加了---的是有调用的-------------------------------------------------------------------------------------------------------------------------------------------
struct stdi {
	bool inm = 0, ing = 0, ins = 0, inin = 0, inen = 0, inch = 0;
	int totn;
	//inm主菜单(in menu) ing游戏中(in game) ins搜索中(in search) inin输入中(in input) inen结算中(in ending) inch选择音符中(in choosing (note))
	//动画模块

	cint menu, s1, sx;//menu主菜单编号 s1主选项个数 c1副选项个数 x主选项位置（从0开始）
	string s[OP];//主选项
	string ses, ns, diss;//搜索框内容 输入内容 展示需要输入的信息
	cint sex;//搜索光标位置

	int nt, tlen;//现在播放的位置 bgm总时长  （都是ms）
	int gsh, gsbo;//下落高度 底部高度
	double cnt = 1;//练习模式死亡时间
	cint mode;//模式
	struct stpi {
		int sc, pf, gd, ba, com, mac;//分数(score)perfect good bad的数量 combo数
		cint pgb;//现在的判分(perfect good bad)
		char rank;
		aninode head[4], end[4];
		int ntp[K][B], ntk[K][B], nt1[K];//音符的位置(note position) 音符的类型 一条线上音符的数量
		bool nk[4];//在第几条道上在按

	}p1, p2;//Player1 Player2
	int hs;//最高分(highest score)
	cint nx;//选取音符的位置x坐标 若没有可选的则nx==K
	int np;//选取音符的位置第np个
	void inianimation(cint mode) {
		for (int i = 0; i < 4; i++)
		{
			p1.head[i].next = &p1.end[i];
			p1.end[i].last = &p1.head[i];
		}
		if (mode == 2) 
			for (int i = 0; i < 4; i++)
			{
				p2.head[i].next = &p2.end[i];
				p2.end[i].last = &p2.head[i];
			}
	}
	inline void bger() {//歌曲导入失败(bgm error)----------------------------------------------------------------------
		//system("cls");
		printf("歌曲导入失败");
		Sleep(1000);
		disp();
	}
	inline void puer() {//谱面导入失败(pu error)----------------------------------------------------------------------
		//system("cls");
		printf("谱面导入失败");
		Sleep(1000);
		disp();
	}
	inline void nuer() {//数字有问题(number error)----------------------------------------------------------------------
		//system("cls");
		printf("确定宁输入的数字没有问题？");
		Sleep(1000);
		disp();
	}
	inline void ser() {//字符串有问题(number error)----------------------------------------------------------------------
		//system("cls");
		printf("确定宁输入的东西没有问题？");
		Sleep(1000);
		disp();
	}

	int qp[B * 8];
	cint qx[B * 8], qk[B * 8];
	int q1 = 0;
	char t[A][K * 2];//输出图
	inline char ntc() {//单按音符字符
		return '-';
	}
	inline char ntch() {
		return 'O';
	}
	inline char lic() {//判定线字符
		return '_';
	}
	inline char c23() {//长按音符字符
		return '|';
	}
	inline void inid() {//输出图初始化(display initialize)-------------------------------------------------------------------------------------------
		for (int x = 0; x <= gsh + gsbo; ++x)
			for (cint y = 0; y < K * 2; ++y)
				t[x][y] = ' ';
		for (cint x = 0; x < K * 2; ++x)
			t[gsbo][x] = lic();
		q1 = 0;
	}
	inline void disn(const stpi& p, const cint k) {
		for (cint x = 0; x < K; ++x)
			for (int y = 0; y < p.nt1[x]; ++y) {
				++q1;
				if (inch && x == nx && y == np)
					t[qp[q1] = p.ntp[x][y]][qx[q1] = x + k] = ntch();
				else
					t[qp[q1] = p.ntp[x][y]][qx[q1] = x + k] = ntc();
			}
	}
	inline void disp() {//输出画面(display)--------------------------------------------------------
		/*if (ing && !inm && !inch) {
			static long long nt, lasd;
			if ((nt = clock()) - lasd <= 10)
				return;
			lasd = nt;
		}*/
		//system("cls");
		/*static const cint open = 1, star = 2, setu = 4, exi = 0;//open开始界面 star开始游戏 setu设置
		static const cint chbg = 11, chpu = 12, mabg = 13, mcbg = 14, mapu = 15, mcpu = 16, endi = 19;//chbg选择音乐 chpu选择谱面 mabg管理音乐 mcbg管理选择音乐 mapu管理谱面 mcpu管理选择谱面 endi结算界面
		static const cint crea = 21, adva = 22;//crea选择制谱模式 adva选择高级模式
		static const cint paus = 41, ens = 42;//paus暂停界面 ens确认是否删除
		static const cint sdi = 51, svol = 52, sspe = 53, skey = 54;//sdi设置难度 svol设置音量 sspe设置速度 skey设置键位
		static const cint p1k = 61, p2k = 62, spk = 63;//p1k：p1键位  p2k：p2键位  spk：记录点键位*/
		/*bool inm = 0, ing = 0, ins = 0, inin = 0, inen = 0, inch = 0;
		//inm主菜单(in menu) ing游戏中(in game) ins搜索中(in search) inin输入中(in input) inen结算中(in ending) inch选择音符中(in choosing (note))
		cint menu, s1, sx;//menu主菜单编号 s1主选项个数 c1副选项个数 x主选项位置（从0开始）
		string s[OP];//主选项
		string ses, ns, diss;//搜索框内容 输入内容 展示需要输入的信息
		cint sex;//搜索光标位置
		int nt, tlen;//现在播放的位置 bgm总时长  （都是ms）
		int gsh, gsbo;//下落高度 底部高度
		double cnt = 1;//练习模式死亡时间
		cint mode;//模式
		struct stpi {
			int sc, pf, gd, ba, com, mac;//分数(score)perfect good bad的数量 combo数
			cint pgb;//现在的判分(perfect good bad)
			char rank;
			int ntp[K][B], ntk[K][B], nt1[K];//音符的位置(note position) 音符的类型 一条线上音符的数量
			bool nk[4];//在第几条道上在按
		}p1, p2;//Player1 Player2
		int hs;//最高分(highest score)
		cint nx;//选取音符的位置x坐标 若没有可选的则nx==K
		int np;//选取音符的位置第np个
		*/
		if (ing) {
			if (mode != 2) {
				settoblank(buffer);
				//判定块绘制
				for (int k = 0; k < 4; k++)
				{
					for (int i = 0; i < p1.nt1[k]; i++)
					{
						if (p1.ntk[k][i] == 1) {
							paint(k * 63 + 186, HIGH-4 - p1.ntp[k][i], click, buffer);
						}
						else if (p1.ntk[k][i]==2)
						{
							paintud(k * 63 + 186, 0, longpress, HIGH-1-p1.ntp[k][i+1], HIGH - 3 - p1.ntp[k][i], buffer);
							paint(k * 63 + 186, HIGH - 7 - p1.ntp[k][i], press,buffer);
							++i;
						}
						else if (p1.ntk[k][i] == 3)
						{
							paintud(k * 63 + 186, 0, longpress, HIGH - 1 - p1.ntp[k][i], 211, buffer);
						}
					}
				}
				//进度条绘制
				paint(0, 0, gamebackground1, buffer);
				int popos = int(double(nt) / double(tlen) * 497) + 62;
				paint(0, 0, progressbar0, buffer);
				paintlr(0, 0, progressbar, 0, popos, buffer);
				//动画序列插入
				for (int k = 0; k < 4; k++)
				{
					if (p1.nk[k]) {
						aninode* pn = new aninode;
						pn->last = p1.end[k].last; pn->next = &p1.end[k];
						p1.end[k].last->next = pn;
						p1.end[k].last = pn;
						pn->ind = 0;
					}
				}
				//动画绘制
				for (int k = 0; k < 4; k++)
				{
					aninode* pn = p1.head[k].next;
					while (pn->ind != -1) {
						paint(k * 63 + 187, 194, animation[pn->ind], buffer);
						++(pn->ind);
						aninode* tmp = pn->next;
						if (pn->ind == 5) {
							pn->last->next = pn->next;
							pn->next->last = pn->last;
							delete pn;
						}
						pn = tmp;
					}
				}
				//计分板绘制
				string s = nts(p1.sc);
				int i = 0, j = 28;
				for (int k = 0; k < s.length(); k++)
				{
						paint(i, j, number[s[k]-'0'], buffer);
					i += 9;
				}
				render(buffer, hout, sout);
				buffertoview(hout, sout);
			}
		}
		if (inm) {
			pair<int, int> pos[4] = { {73,70},{73,106},{73,141},{73,176} };
			if (menu == 1) {
				settoblank(buffer);
				paint(0, 0, homebackground, buffer);
				paint(pos[sx].first, pos[sx].second, kuangkuang, buffer);
				render(buffer, hout, sout);
				buffertoview(hout, sout);
			}
			else if (menu == 2) {
				settoblank(buffer);
				paint(0, 0, startgamebackground, buffer);
				paint(pos[sx].first, pos[sx].second, kuangkuang, buffer);
				render(buffer, hout, sout);
				buffertoview(hout, sout);
			}
			else if (menu == 11) {
				const int xdis = 16, ydis = 22;
				settoblank(buffer);
				paint(0, 0, mchoosebackground, buffer);
				int i = 73, j = 40;
				for (int k = 0; k <ses.length() ; k++)
				{
					if (ses[k] >= '0'&&ses[k] <= '9') {
						paint(i, j, number[ses[k]-'0'], buffer);
					}
					if (ses[k] >= 'a'&&ses[k] <= 'z') {
						paint(i, j, alpha[ses[k] - 'a'], buffer);
					}
					if (ses[k] >= 'A'&&ses[k] <= 'Z') {
						paint(i, j, alpha1[ses[k] - 'A'], buffer);
					}
					i += xdis;
				}
				i = 98, j = 76;
				for (int k = 0; k < s1; k++)
				{
					for (int x = 0; x < s[k].length(); x++)
					{
						if (s[k][x] >= '0'&&s[k][x] <= '9') {
							paint(i, j, number[s[k][x] - '0'], buffer);
						}
						if (s[k][x] >= 'a'&&s[k][x] <= 'z') {
							paint(i, j, alpha[s[k][x] - 'a'], buffer);
						}
						if (s[k][x] >= 'A'&&s[k][x] <= 'Z') {
							paint(i, j, alpha1[s[k][x] - 'A'], buffer);
						}
						i += xdis;
					}
					i = 98;
					j += ydis;
				}
				if (ins)
				{
					paint(7, 32, yinfu, buffer);
					paint(sex * xdis + 67, 37, cursor, buffer);
				}
				else if(s1) {
					paint(8, 71 + sx * ydis, jiantou, buffer);
				}
				render(buffer, hout, sout);
				buffertoview(hout, sout);
			}
			else if (menu == 12) {
				const int xdis = 16, ydis = 22;
				settoblank(buffer);
				paint(0, 0, rchoosebackground, buffer);
				int i = 73, j = 40;
				for (int k = 0; k < ses.length(); k++)
				{
					if (ses[k] >= '0'&&ses[k] <= '9') {
						paint(i, j, number[ses[k] - '0'], buffer);
					}
					else if (ses[k] >= 'a'&&ses[k] <= 'z') {
						paint(i, j, alpha[ses[k] - 'a'], buffer);
					}
					else if (ses[k] >= 'A'&&ses[k] <= 'Z') {
						paint(i, j, alpha1[ses[k] - 'A'], buffer);
					}
					else if (ses[k] >= 33&&ses[k] <= 47) {
						paint(i, j, alpha2[ses[k] - 33], buffer);
					}
					else if (ses[k] >= 58 && ses[k] <= 64) {
						paint(i, j, alpha3[ses[k] - 58], buffer);
					}
					else if (ses[k] >= 91 && ses[k] <= 96) {
						paint(i, j, alpha4[ses[k] - 33], buffer);
					}
					else if (ses[k] >= 123 && ses[k] <= 126) {
						paint(i, j, alpha1[ses[k] - 123], buffer);
					}
					i += xdis;
				}
				i = 98, j = 76;
				for (int k = 0; k < s1; k++)
				{
					for (int x = 0; x < s[k].length(); x++)
					{
						if (s[k][x] >= '0'&&s[k][x] <= '9') {
							paint(i, j, number[s[k][x] - '0'], buffer);
						}
						else if (s[k][x] >= 'a'&&s[k][x] <= 'z') {
							paint(i, j, alpha[s[k][x] - 'a'], buffer);
						}
						else if (s[k][x] >= 'A'&&s[k][x] <= 'Z') {
							paint(i, j, alpha1[s[k][x] - 'A'], buffer);
						}
						else if (s[k][x] >= 33 && s[k][x] <= 47) {
							paint(i, j, alpha2[s[k][x] - 33], buffer);
						}
						else if (s[k][x] >= 58 && s[k][x] <= 64) {
							paint(i, j, alpha3[s[k][x] - 58], buffer);
						}
						else if (s[k][x] >= 91 && s[k][x] <= 96) {
							paint(i, j, alpha4[s[k][x] - 33], buffer);
						}
						else if (s[k][x] >= 123 && s[k][x] <= 126) {
							paint(i, j, alpha1[s[k][x] - 123], buffer);
						}
						i += xdis;
					}
					i = 98;
					j += ydis;
				}
				if (ins)
				{
					paint(7, 32, yinfu, buffer);
					paint(sex * xdis + 67, 37, cursor, buffer);
				}
				else if(s1){
					paint(8, 71 + sx * ydis, jiantou, buffer);
				}
				render(buffer, hout, sout);
				buffertoview(hout, sout);
			}
		}
		if (inin) {
			printf("%s(", diss.c_str());
			for (cint x = 0; x < sex; ++x)
				printf("%c", ns[x]);
			printf(")(");
			for (cint x = sex; x < ns.size(); ++x)
				printf("%c", ns[x]);
			printf(")");
		}
	}
	inline void c321(double ) {//倒数321(cout 3 2 1)----------------------------------------------------------------------
		/*static double num;
		if (mode == 1)
			num = cnt;
		else
			num = 3;
		disp();
		printf("%d", (int)num);
		const long long bt = clock(), t1 = num * 1000;
		for (long long ncl, lacl = 0; (ncl = clock() - bt) < t1; lacl = ncl) {
			if (floor((double)ncl / 1000) != floor((double)lacl / 1000)) {
				disp();
				printf("%d", (int)(num -= 1));
			}
		}*/
	}
	inline void dise(const cint mode, const int& hs) {
		/*printf("highest score: %d\n\n", hs);

		printf("p1 rank: %c\n", p1.rank);
		printf("p1 score: %d\n", p1.sc);
		printf("p1 maximum combo: %d\n", p1.mac);
		printf("p1 perfect: %d\n", p1.pf);
		printf("p1 good: %d\n", p1.gd);
		printf("p1 bad: %d\n", p1.ba);
		if (mode == 2) {
			printf("\np2 rank: %c\n", p2.rank);
			printf("p2 score: %d\n", p2.sc);
			printf("p2 maximum combo: %d\n", p2.mac);
			printf("p2 perfect: %d\n", p2.pf);
			printf("p2 good: %d\n", p2.gd);
			printf("p2 bad: %d\n", p2.ba);
		}*/
	}
}info;

//输入系统(struct input (system))
struct stin {
	bool cplk = 0, ex, ent;//是否大写(capslock) 是否在输入状态(exist) 是否确认(enter)
	inline char gek() {//得到操作键状态(get key)
		Sleep(5);
		static const short occ1 = CH * 2 + 1;
		static bool nooc[occ1], laoc[occ1];
		static char lan = -1;
		static long long nt, lat;
		static bool con;
		for (short x = 0; x <= occ1; ++x) {
			nooc[x] = pdk(x);
			if (nooc[x] && !laoc[x]) {
				laoc[x] = nooc[x];
				lat = clock();
				return lan = x;
			}
			laoc[x] = nooc[x];
		}

		if (lan != -1 && nooc[lan] && ((nt = clock()) - lat > 400 || con && nt - lat > 50) && nt != shift) {
			lat = nt;
			con = true;
			return lan;
		}
		if (lan != -1 && !nooc[lan]) {
			con = false;
			return lan = -1;
		}
		return -1;
	}
	inline bool pdca() {//判断是否大写(pd capitalize)
		return pdk(shift) ^ cplk;
	}
	inline char pdp(const char& c) {//得到符号(pd punctuation)
		static const char punk[] = { 186,187,188,189,190,191,192,219,220,221,222,48,49,50,51,52,53,54,55,56,57,96,97,98,99,100,101,102,103,104,105,106,107,109,110,111,space };
		static const char puna[] = ";=,-./`[\\]'01234567890123456789*+-./ ", punA[] = ":+<_>?~{|}\")!@#$%^&*(0123456789*+-./ ", pun1 = 37;
		for (cint x = 0; x < pun1; ++x)
			if (c == punk[x]) {
				if (pdca())
					return punA[x];
				return puna[x];
			}
		return 0;
	}

	inline void adc(char s[], cint& s1, cint& sx, const char& c) {//输入字符(add char)
		if (s1 >= C - 1)
			return;
		for (cint x = ++s1; x > sx; --x)
			s[x] = s[x - 1];
		s[sx++] = c;
	}
	inline void dec(char s[], cint& s1, cint& sx) {//删除字符(dele char)
		if (!sx)
			return;
		for (cint x = sx - 1; x < s1; ++x)
			s[x] = s[x + 1];
		--s1;
		--sx;
	}

	inline bool gec(cint nk, char s[], cint& s1, cint& sex) {//得到字符(get char)
		static char np;
		if (nk == esc)
			ex = 0;
		else if (nk == enter1 || nk == enter2)
			ent = 1;
		else if (nk == capslk)
			cplk ^= 1;
		else if (nk == left)
			sex = max(0, sex - 1);
		else if (nk == right)
			sex = min((int)s1, sex + 1);
		else if (nk == dele && sex != s1)
			dec(s, s1, ++sex);
		else if (nk == backspace)
			dec(s, s1, sex);
		else if (pdA(nk)) {
			if (pdca())
				adc(s, s1, sex, nk);
			else
				adc(s, s1, sex, nk - 'A' + 'a');
		}
		else if (np = pdp(nk))
			adc(s, s1, sex, np);
		else
			return 0;
		return 1;
	}
	inline void disp(char s[], const cint sex) {//显示(display)
		info.ns = s;
		info.sex = sex;
		info.disp();
	}
	inline string ges(const string& inpu, const cint mode = 0, const string& inis = "") {//得到字符串(get string) mode: 0名字 1数字 2时间
		info.inin = 1;
		info.diss = inpu;
		static char nk, s[C + 10];
		static cint s1, sex;
		strcpy(s, inis.c_str());
		sex = s1 = inis.size();
		disp(s, sex);
		for (ex = 1; ;) {
			for (ent = 0; ex && !ent;)
				if (gec(nk = gek(), s, s1, sex))
					disp(s, sex);
			if (!ex)
				break;
			if (mode == 0 && !pdna(s) || mode == 3 && s[0] == space)
				info.ser();
			else if (mode == 1 && pdnu(s) == -2 || mode == 2 && stt(s) == -1)
				info.nuer();
			else
				break;
		}
		info.inin = 0;
		info.ns = info.diss = "";
		if (ent)
			return s;
		return "";
	}
	inline void chk(const string& inpu, int& c) {//改按键(change key)
		info.inin = 1;
		info.diss = inpu;
		static char nk, s[2] = " ";
		int nc = c;
		static int np;
		s[0] = c;
		disp(s, 1);
		for (ex = 1, ent = 0; ex && !ent;) {
			nk = gek();
			if (nk == esc)
				ex = 0;
			else if (nk == enter1 || nk == enter2)
				ent = 1;
			else if (pdA(nk))
				s[0] = nc = nk;
			else if ((np = pdp(nk)) && nk != space)
				s[0] = nc = np;
			else
				continue;
			disp(s, 1);
		}
		if (ent)
			c = nc;
		info.inin = 0;
		info.ns = info.diss = "";
	}
}ips;

//全局设置(struct global settings)
struct stgs {

	const short ppf = 80, pg = 160, pb = 300, apf = 80, ag = 160;//初始化：perfect前时间 good前 bad前 perfect后 good后
	const cint psc = 10, gsc = 7;//初始化：perfect加的分(perfect score) good加的分

	double cnt;//练习模式倒数
	cint di = 0;//设置的难度(now difficult)

	string bgm[MBG + 3], pu[MBG + 3][MPU + 3];//已读bgm 已读谱
	int pmsc[MBG][MPU][D], bg1 = 0, pu1[MBG + 3];//谱面最高分(pu maximum score) 已读bgm数量 已读谱的数量

	int bgv = 500, efv = 500;//bgm音量(bem volume) 音效音量(effect volume)

	short h, bo;//可视高度(height) 可视底部高度(bottom)

	inline void inps(const int bgid, const int puid) {//谱面最高分初始化(initialize pu (maximum) score)
		for (cint x = 0; x < D; ++x)
			pmsc[bgid][pu1[bgid]][x] = 0;
	}
}gs;

//播放器(struct player)
struct stpr {

	static const cint id1 = 39;
	string na;//bgm名
	bool pdbg = 0;

	static inline int scmd(const string& c, const cint get = 0) {//输入命令(send cmd)
		static char llc[C];
		if (get == 2)
			return mciSendString(c.c_str(), 0, 0, 0);
		mciSendString(c.c_str(), llc, C, 0);
		if (get == 1)
			return cti(llc);
	}

	inline int vol() {//得到音量(volume)
		if (pdbg)
			return gs.bgv;
		return gs.efv;
	}
	inline void adjv() {//调节音量(adjust volume)
		scmd("setaudio " + na + " volume to " + nts(vol()));
	}

	inline bool op(const string& bgn, const bool mode = 0) {//读bgm(open)
		const cint s1 = bgn.size();
		if (!s1)
			return 0;
		na = "";
		cint x = s1 - 1;
		for (; x > 0 && bgn[x - 1] != '\\'; --x);
		for (; x < s1; ++x)
			if (pdAa(bgn[x]) || pddi(bgn[x]))
				na += bgn[x];
		clo();
		if (!mode)
			return !scmd("open \"" + bgn + ".wav\" alias " + na + " type mpegvideo", 2);
		return !scmd("open \"music\\" + bgn + ".wav\" alias " + na + " type mpegvideo", 2);
	}
	inline int glen() {//bgm的时长(get length)
		return scmd(("Status " + na + " length").c_str(), 1);
	}
	inline int nti() {//获得播放时间(now time)
		return scmd(("Status " + na + " position").c_str(), 1);
	}
	inline void pl(const int st = 0) {//播放(play)
		adjv();
		scmd("play " + na + " from " + nts(st));
	}
	inline void repl() {//重复播放(replay)
		adjv();
		scmd("play " + na + " repeat");
	}
	inline void pl0() {//暂停播放(play=0)
		scmd("pause " + na);
	}
	inline void pl1() {//开始播放(play=1)
		adjv();
		scmd("resume " + na);
	}
	inline void clo() {//关闭bgm(close)
		scmd("close " + na);
	}

	inline void menu(const string& bg) {//菜单播放
		static string labg;
		if (bg == labg)
			return;
		labg = bg;
		clo();
		if (!bg.size())
			return;
		op(bg, 1);
		repl();
	}
	inline void ef() {//敲击音效播放
		static cint nid;
		na = "effect" + nts(nid = (nid + 1) % id1);
		pl();
	}
}bgm, ml, dea, begi, fin, clic, ente;//bgm menuloop finish death

//音符(struct note)
struct stnt {
	int t = 0;//出现时间(time):infi则待删除
	cint k = 0;//音符类型(kind)：1:单点，2:长按开始，3:长按结束
	inline friend bool operator < (const stnt& a, const stnt& b) {
		return a.t < b.t;
	}
};

//谱(struct pu->谱)
struct stpu {

	int nk = 0, nki[K] = {};//音符数量(number key) 每条的音符数量(number key i)
	stnt nt[K][N];//音符(note)
	bool pdrp = 0;//是否成功读入BGM(pd read bgm)

	inline void read(const string& puna, const string& bgna, const bool mode = 0) {//读谱 puna:谱的名字(pu name)
		ifstream fi(("score\\" + bgna + "-" + puna + ".txt").c_str());
		if (!(pdrp = !!fi) && !mode)
			return;
		nk = 0;
		memset(nki, 0, sizeof nki);
		for (int t, x, k; fi >> t >> x >> k; ++nk) {
			nt[x][++nki[x]] = { t,(cint)k };
			nk -= k == 3;
		}
	}
	inline void writ(const string& puna, const string& bgna) {//写谱(write)
		ofstream fi(("score\\" + bgna + "-" + puna + ".txt").c_str());
		for (cint x = 0; x < K; ++x) {
			sort(nt[x] + 1, nt[x] + nki[x] + 1);
			for (int y = 1; y <= nki[x]; ++y)
				if (nt[x][y].t != infi) {
					fi << nt[x][y].t << ' ' << (int)x << ' ' << (int)nt[x][y].k << '\n';
					if (nt[x][y].k == 2)
						for (; nt[x][y + 1].k == 1; ++y);
				}
		}
	}

	inline void ad1(const int t, const cint x) {//加短按音符(add)
		nt[x][++nki[x]] = { t,1 };
	}
	inline void ad2(const int tst, const int ten, const cint x) {//加长按音符(add)
		nt[x][++nki[x]] = { tst,2 };
		nt[x][++nki[x]] = { ten,3 };
	}
	inline void del(const int np, const cint x) {//删音符(dele)
		nt[x][np].t = infi;
		if (nt[x][np].k == 2)
			nt[x][np + 1].t = infi;
	}
}pu, empu, tepu;////empu:空谱 temp临时谱(temporary pu)

int nt, bj;//现在的时间 音符可见标记(bj->biao'ji->标记)
//游玩信息(struct play infomation)
struct stpi {
	bool ex, al;//是否在玩(existence) 是否死亡(alive) 是否中途退出(force exit)
	double spe = 30;//设置的速度 
	int ds, sk[K];//可视秒(delta second)(单位ms) 键位设置(settings key (position))
	int sc, pf, gd, ba, com, mac;//分数(score) perfect good bad的数量 combo数
	cint pgb;//现在的判分(perfect good bad)
	bool k[K], lak[K];//此时的键位(key)，上一时刻的键位(last key)
	int np[K], nr[K];//现在指向的音符(now point) 可视的音符(now right)
	cint lk[K];//长按状态(long key) 0:没按到 1:good 2:perfect
	int ntp[K][N];//音符的位置(note position)
	int ntex[K][N];//音符是否可见(note existence)：如与bj相同则不可见 

	inline bool pdex(const cint x, const int y) {
		return ntex[x][y] != bj && pu.nt[x][y].t - nt <= ds;
	}

	inline void ini(const int st = 0) {//初始化(initialize)
		memset(lak, 0, sizeof lak);
		for (cint x = 0; x < K; ++x)
			np[x] = 1;
		memset(nr, 0, sizeof nr);
		memset(lk, 0, sizeof lk);
		sc = pf = gd = ba = com = mac = 0;
		pgb = 3;
		ex = al = 1;
		ds = gs.h * 1000 / spe;
		if (!st)
			return;

		for (cint x = 0; x < K; ++x) {
			np[x] = bsb(pu.nt[x], 1, pu.nki[x], { st });
			np[x] += pu.nt[x][np[x]].k == 3;
			for (int y = np[x] - 1; y > 0 && st - pu.nt[x][y].t < gs.ag; --y);
		}
	}

	inline void adnp(const cint x) {//自动调节np(adjust np)
		for (; np[x] <= pu.nki[x] && (nt - gs.ag > pu.nt[x][np[x]].t || pu.nt[x][np[x]].k == 3 && !lk[x]); adba())
			ntex[x][np[x]++] = bj;
	}
	inline void adnr(const cint x) {//自动调节nr(adjust nr)
		for (nr[x] = min(max(nr[x], np[x]), pu.nki[x]); nr[x] + 1 <= pu.nki[x] && (pu.nt[x][nr[x] + 1].t <= nt + ds || pu.nt[x][nr[x] + 1].k == 3); ++nr[x]);
	}

	inline void adpf() {//一个perfect(add perfect)
		++pf;
		mac = max(mac, ++com);
		sc += gs.psc;
		pgb = 2;
	}
	inline void adgd() {//一个good(add good)
		if (gs.di == 2) {
			al = 0;
			return;
		}
		++gd;
		mac = max(mac, ++com);
		sc += gs.gsc;
		pgb = 1;
	}
	inline void adba() {//一个bad(add bad)
		if (gs.di) {
			al = 0;
			return;
		}
		++ba;
		com = 0;
		pgb = 0;
	}

	inline void fe(const cint x) {//for end
		for (int y = np[x]; y <= nr[x]; ++y)
			if (ntex[x][y] != bj)
				ntp[x][y] = (pu.nt[x][y].t - nt) * spe / 1000 + gs.bo;
		lak[x] = k[x];
	}

	inline bool puen() {//谱是否完成
		for (cint x = 0; x < K; ++x)
			if (np[x] <= pu.nki[x])
				return 0;
		return 1;
	}

	inline char rank() {//获得分数
		int ma = pu.nk * gs.psc, sc10 = sc * 10;
		if (sc10 == ma * 10)
			return 'S';
		if (sc10 >= ma * 9)
			return 'A';
		if (sc10 >= ma * 8)
			return 'B';
		if (sc10 >= ma * 7)
			return 'C';
		if (sc10 >= ma * 6)
			return 'D';
		return 'E';
	}

	inline void cpp(stdi::stpi& cp) {
		for (cint x = 0; x < K; ++x) {
			cp.nt1[x] = 0;
			for (int y = np[x]; y <= nr[x]; ++y)
				if (ntex[x][y] != bj) {
					cp.ntp[x][cp.nt1[x]] = ntp[x][y];
					cp.ntk[x][cp.nt1[x]] = pu.nt[x][y].k;
					++cp.nt1[x];
				}
		}
	}
}p1, p2;

//记录点(struct record point)
struct strp {
	int sks = 0, skd = 0, sta[N] = {}, st1 = 0;//记录点键位设置(settings key (position) set (position)) 删除记录点键位设置(settings key (position) dele (position))
	bool s = 0, las = 0, d = 0, lad = 0;//此时的sks，上一时刻的sks(last sks) 此时的skd，上一时刻的sks(last skd)
	inline void ini(const int st) {//初始化(initialize)
		sta[st1 = 0] = st;
		las = lad = 0;
	}
	inline void si(const int nt) {//判断记录点情况((record point) situation)
		s = pdk(sks);
		d = pdk(skd);
		if (s && !las)
			sta[++st1] = nt;
		if (d && !lad && st1)
			--st1;
		las = s;
		lad = d;
	}
}rp;

//导入设置(read settings)
inline void rese() {
	ifstream fi("options.txt");
	static bool pd = 0;//是否是第一次导入
	if (!pd) {
		ml.op("effect\\menuloop");
		fin.op("effect\\finish");
		dea.op("effect\\death");
		begi.op("effect\\open");
		ente.op("effect\\enter");
		for (cint x = 0; x < stpr::id1; ++x)
			clic.op("effect\\effect(" + nts(x) + ")");
		pd = 1;
		bgm.pdbg = 1;
	}
	if (!fi)
		return;

	static char noc[C];
	static int num;

	fi >> noc >> num;//difficult:
	gs.di = num;
	fi >> noc >> gs.bgv;//bgm volume:
	fi >> noc >> gs.efv;//sound effect volume:
	fi >> noc >> p1.spe >> p2.spe;//speed:
	fi >> noc >> gs.cnt;//rebirthtime:

	fi.getline(noc, C);
	fi.getline(noc, C);//keyset:
	for (cint x = 0; x < K; ++x)
		fi >> p1.sk[x];
	for (cint x = 0; x < K; ++x)
		fi >> p2.sk[x];
	fi >> rp.sks >> rp.skd;

	fi.getline(noc, C);
	fi.getline(noc, C);//music score:
	fi >> gs.bg1;
	for (int x = 1; fi >> gs.pu1[x]; ++x) {
		gs.pu[x][0] = "temporary";
		fi.getline(noc, C);
		gs.bgm[x] = noc + 1;
		for (int y = 1; y <= gs.pu1[x]; ++y) {
			for (cint z = 0; z < D; ++z)
				fi >> gs.pmsc[x][y][z];
			fi.getline(noc, C);
			gs.pu[x][y] = noc + 1;
		}
		gs.pu[x][gs.pu1[x] + 1] = gs.pu[x][gs.pu1[x] + 2] = "";
	}
	gs.bgm[gs.bg1 + 1] = gs.bgm[gs.bg1 + 2] = "";
}

//导出设置(write settings)
inline void wrse() {
	ofstream fi("options.txt");

	fi << "difficult:\n" << (int)gs.di << '\n';
	fi << "bgmvolume:\n" << gs.bgv << '\n';
	fi << "soundeffectvolume:\n" << gs.efv << '\n';
	fi << "speed:\n" << p1.spe << " " << p2.spe << '\n';
	fi << "rebirthtime:\n" << gs.cnt << '\n';

	fi << "keyset:\n";
	for (cint x = 0; x < K; ++x)
		fi << p1.sk[x] << ' ';
	for (cint x = 0; x < K; ++x)
		fi << p2.sk[x] << ' ';
	fi << rp.sks << ' ' << rp.skd << '\n';

	fi << "musicscore:\n" << gs.bg1 << '\n';
	for (int x = 1, y; x <= gs.bg1 + 1; ++x)
		if (gs.bgm[x].size()) {
			fi << gs.pu1[x] << ' ' << gs.bgm[x] << '\n';
			for (y = 1; y <= gs.pu1[x] + 1; ++y)
				if (gs.pu[x][y].size()) {
					for (cint z = 0; z < D; ++z)
						fi << gs.pmsc[x][y][z] << ' ';
					fi << gs.pu[x][y] << '\n';
				}
		}
}

//菜单(struct menu)                                                            菜单id在这
struct stme {
	static const cint m1 = 20, op1 = 10;

	static const cint open = 1, star = 2, setu = 4, exi = 0;//open开始界面 star开始游戏 setu设置
	static const cint chbg = 11, chpu = 12, mabg = 13, mcbg = 14, mapu = 15, mcpu = 16, endi = 19;//chbg选择音乐 chpu选择谱面 mabg管理音乐 mcbg管理选择音乐 mapu管理谱面 mcpu管理选择谱面 endi结算界面
	static const cint crea = 21, adva = 22;//crea选择制谱模式 adva选择高级模式
	static const cint paus = 41, ens = 42;//paus暂停界面 ens确认是否删除
	static const cint sdi = 51, svol = 52, sspe = 53, skey = 54;//sdi设置难度 svol设置音量 sspe设置速度 skey设置键位
	static const cint p1k = 61, p2k = 62, spk = 63;//p1k:p1键位  p2k:p2键位  spk:记录点键位

	static const cint bgad = -1, bgde = -2, puad = -3, pude = -4, sapu = -5;
	static const cint pl = -10, pm13 = -11, plre = -12, expl = -13;
	static const cint p10 = -20, p11 = -21, p12 = -22, p13 = -23, p20 = -24, p21 = -25, p22 = -26, p23 = -27, adp = -28, dep = -29;
	static const cint norm = -31, hard = -32, mast = -33;
	static const cint bgv = -41, efv = -42, p1s = -43, p2s = -44, rbt = -45;

	string opt[CH][op1];
	cint s1[CH], son[CH][op1];

	stme() {
		static cint p, x;

		/*
		p = ;
		x = -1; opt[p][++x] = "", opt[p][++x] = "", opt[p][++x] = "";
		x = -1; son[p][++x] = , son[p][++x] = , son[p][++x] = ;
		s1[p] = ++x;
		*/

		p = open;
		x = -1; opt[p][++x] = "开始游戏", opt[p][++x] = "资源管理", opt[p][++x] = "设置", opt[p][++x] = "退出游戏";
		x = -1; son[p][++x] = star, son[p][++x] = mabg, son[p][++x] = setu, son[p][++x] = exi;
		s1[p] = ++x;

		p = star;
		x = -1; opt[p][++x] = "单人模式", opt[p][++x] = "练习模式", opt[p][++x] = "多人模式";
		x = -1; son[p][++x] = son[p][++x] = son[p][++x] = chbg;
		s1[p] = ++x;

		p = setu;
		x = -1; opt[p][++x] = "难度设置", opt[p][++x] = "声音设置", opt[p][++x] = "音符下落速度", opt[p][++x] = "练习模式重生时间", opt[p][++x] = "键位设置";
		x = -1; son[p][++x] = sdi, son[p][++x] = svol, son[p][++x] = sspe, son[p][++x] = rbt, son[p][++x] = skey;
		s1[p] = ++x;

		p = mabg;
		x = -1; opt[p][++x] = "选择音乐", opt[p][++x] = "导入音乐";
		x = -1; son[p][++x] = mcbg, son[p][++x] = bgad;
		s1[p] = ++x;

		p = mapu;
		x = -1;  opt[p][++x] = "选择谱面", opt[p][++x] = "导入谱面", opt[p][++x] = "新建谱面";
		x = -1; son[p][++x] = mcpu, son[p][++x] = puad, son[p][++x] = crea;
		s1[p] = ++x;

		p = crea;
		x = -1; opt[p][++x] = "自动模式", opt[p][++x] = "高级模式";
		x = -1; son[p][++x] = pm13, son[p][++x] = adva;
		s1[p] = ++x;

		p = adva;
		x = -1; opt[p][++x] = "短按模式", opt[p][++x] = "长按模式";
		x = -1; son[p][++x] = son[p][++x] = pm13;
		s1[p] = ++x;

		p = paus;
		x = -1; opt[p][++x] = "继续游戏", opt[p][++x] = "重新开始", opt[p][++x] = "声音", opt[p][++x] = "退出游戏";
		x = -1; son[p][++x] = exi, son[p][++x] = plre, son[p][++x] = svol, son[p][++x] = expl;
		s1[p] = ++x;

		p = endi;
		x = -1; opt[p][++x] = "重新开始", opt[p][++x] = "退出游戏";
		x = -1; son[p][++x] = plre, son[p][++x] = expl;
		s1[p] = ++x;

		p = sdi;
		x = -1; opt[p][++x] = "普通", opt[p][++x] = "困难", opt[p][++x] = "大师";
		x = -1; son[p][++x] = norm, son[p][++x] = hard, son[p][++x] = mast;
		s1[p] = ++x;

		p = svol;
		x = -1; opt[p][++x] = "BGM", opt[p][++x] = "音效";
		x = -1; son[p][++x] = bgv, son[p][++x] = efv;
		s1[p] = ++x;

		p = skey;
		x = -1; opt[p][++x] = "P1玩家键位设置", opt[p][++x] = "P2玩家键位设置", opt[p][++x] = "记录点键位设置";
		x = -1; son[p][++x] = p1k, son[p][++x] = p2k, son[p][++x] = spk;
		s1[p] = ++x;

		p = p1k;
		x = -1; opt[p][++x] = "第一列", opt[p][++x] = "第二列", opt[p][++x] = "第三列", opt[p][++x] = "第四列";
		x = -1; son[p][++x] = p10, son[p][++x] = p11, son[p][++x] = p12, son[p][++x] = p13;
		s1[p] = ++x;

		p = p2k;
		x = -1; opt[p][++x] = "第一列", opt[p][++x] = "第二列", opt[p][++x] = "第三列", opt[p][++x] = "第四列";
		x = -1; son[p][++x] = p20, son[p][++x] = p21, son[p][++x] = p22, son[p][++x] = p23;
		s1[p] = ++x;

		p = sspe;
		x = -1; opt[p][++x] = "P1玩家音符下落速度", opt[p][++x] = "P2玩家音符下落速度";
		x = -1; son[p][++x] = p1s, son[p][++x] = p2s;
		s1[p] = ++x;

		p = spk;
		x = -1; opt[p][++x] = "添加记录点", opt[p][++x] = "删除记录点";
		x = -1; son[p][++x] = adp, son[p][++x] = dep;
		s1[p] = ++x;

		p = ens;
		x = -1; opt[p][++x] = "取消", opt[p][++x] = "确认";
		x = -1; son[p][++x] = exi, son[p][++x] = exi;
		s1[p] = ++x;
	}

	struct sts {
		cint p = 1, x = 0, mode = 0, crm = 0;//菜单id 选项位置 侧选项位置 模式 制谱模式
		int bgid = 0, puid = 0;//bgm的id 谱面的id
	}st[m1];//参数栈(stack)

	cint p = 1;//栈顶
	char nk;//输入的字符

	int lbg[MBG], lpu[MPU], lbg1, lpu1;//显示bgm指针 显示谱面指针 显示bgm个数 显示谱面个数
	int bgl = 0, bgr = 0, pul = 0, pur = 0;//显示bgm的左右区间 显示谱面的左右区间
	char sbg[C + 10], spu[C + 10];//bgm搜索串 谱面搜索串
	cint sbg1, spu1, sex;//bgm搜索串长度 谱面搜索串长度 光标位置

	inline void cir(const int s1, cint& np, int& nl, int& nr) {//界面循环(circle)
		if (!s1)
			return;
		if (s1 <= OP)
			np = (np + s1) % s1;
		else if (np < 0) {
			nr = np = s1 - 1;
			nl = nr - OP + 1;
		}
		else if (np == s1) {
			nl = np = 0;
			nr = OP - 1;
		}
		else if (np < nl) {
			--nl;
			--nr;
		}
		else if (np > nr) {
			++nl;
			++nr;
		}
	}
	inline void cir(const int s1, cint& np) {//界面循环(circle)
		np = (np + s1) % s1;
	}

	inline bool pdch(const cint np) {//判断是否在选择界面(pd choose)
		return np == chbg || np == mcbg || np == chpu || np == mcpu;
	}

	inline void sebg() {//搜bgm(search bgm)
		fn(gs.bgm, gs.bg1, sbg, lbg, lbg1, bgl, bgr);
	}
	inline void sepu() {//搜谱面(search pu)
		fn(gs.pu[st[p].bgid], gs.pu1[st[p].bgid], spu, lpu, lpu1, pul, pur);
	}

	inline void disp() {//显示(display)
		info.menu = st[p].p;
		info.sx = st[p].x;
		if (pdch(st[p].p)) {
			info.sex = sex;
			if (st[p].p == chbg || st[p].p == mcbg) {
				if (info.s1 = bgr - bgl + 1)
					for (int x = bgl; x <= bgr; ++x)
						info.s[x - bgl] = gs.bgm[lbg[x]];
				info.ses = sbg;
			}
			else if (st[p].p == chpu || st[p].p == mcpu) {
				if (info.s1 = pur - pul + 1)
					for (int x = pul; x <= pur; ++x)
						info.s[x - pul] = gs.pu[st[p].bgid][lpu[x]];
				info.ses = spu;
			}
		}
		else {
			info.s1 = s1[st[p].p];
			for (cint x = 0; x < info.s1; ++x)
				info.s[x] = opt[st[p].p][x];
			info.sex = 0;
			info.ses = "";
		}
		if (st[p].bgid && !(st[p].p == ens && st[p - 1].p == mcbg)) {
			ml.pl0();
			bgm.menu(gs.bgm[st[p].bgid]);
		}
		else {
			bgm.menu("");
			ml.pl1();
		}
		info.disp();
	}
	inline void sear() {//搜索(search)
		info.ins = 1;
		st[p].x = -1;
		ips.ex = 1;
		ips.ent = 0;
		if (st[p].p == chbg || st[p].p == mcbg) {
			sex = sbg1;
			disp();
			for (; ips.ex && !ips.ent;)
				if (ips.gec(ips.gek(), sbg, sbg1, sex)) {
					sebg();
					disp();
				}
			st[p].x = bgl;
		}
		else if (st[p].p == chpu || st[p].p == mcpu) {
			sex = spu1;
			disp();
			for (; ips.ex && !ips.ent;)
				if (ips.gec(ips.gek(), spu, spu1, sex)) {
					sepu();
					disp();
				}
			st[p].x = pul;
		}
		info.ins = 0;
	}
	inline void mian() {//mian函数
		static string tems;
		static int temi;
		info.inm = 1;
		disp();
		for (; pdk(esc););

		for (; p > 0;) {
			nk = ips.gek();
			if (nk == esc) {
				--p;
				if (st[p].p == pl || st[p].p == pm13)
					return;
			}
			else if (st[p].p != ens && (nk == up || nk == down) || st[p].p == ens && (nk == left || nk == right)) {
				static cint k;
				if (nk == up || nk == left)
					k = -1;
				else
					k = 1;
				if (st[p].p == chbg || st[p].p == mcbg)
					cir(lbg1, st[p].x += k, bgl, bgr);
				else if (st[p].p == chpu || st[p].p == mcpu)
					cir(lpu1, st[p].x += k, pul, pur);
				else
					cir(s1[st[p].p], st[p].x += k);
			}
			else if (st[p].x != -1 && (nk == enter1 || nk == enter2 || nk == dele && (st[p].p == mcbg || st[p].p == mcpu))) {
				++p;
				st[p] = st[p - 1];
				if (st[p].p == chbg || st[p].p == mcbg)
					st[p].bgid = lbg[st[p].x];
				else if (st[p].p == chpu || st[p].p == mcpu)
					st[p].puid = lpu[st[p].x];
				else if (st[p].p == star)
					st[p].mode = st[p].x;
				else if (st[p].p == adva)
					st[p].crm = st[p].x + 1;

				if (st[p].p == chbg)
					st[p].p = chpu;
				else if (st[p].p == mcbg && nk != dele)
					st[p].p = mapu;
				else if (st[p].p == chpu) {
					if (st[p].mode == 1)
						st[p].p = pm13;
					else
						st[p].p = pl;
				}
				else if (st[p].p == mcpu && nk != dele)
					st[p].p = pm13;
				else if (st[p].p == ens && st[p].x == 1) {
					if (st[p - 2].p == mcbg)
						st[p].p = bgde;
					else if (st[p - 2].p == mcpu)
						st[p].p = pude;
				}
				else if (nk == dele)
					st[p].p = ens;
				else
					st[p].p = son[st[p].p][st[p].x];

				if (st[p].p > 0) {
					if (pdch(st[p].p)) {
						if (st[p].p == chbg || st[p].p == mcbg) {
							sbg[sbg1 = 0] = '\0';
							sebg();
							st[p].x = bgl;
							if (st[p].p == mcbg)
								st[p].mode = 3;
						}
						else if (st[p].p == chpu || st[p].p == mcpu) {
							spu[spu1 = 0] = '\0';
							sepu();
							st[p].x = pul;
						}
					}
					else if (st[p].p == sdi)
						st[p].x = gs.di;
					else
						st[p].x = 0;
				}
			}
			else if (nk == space && pdch(st[p].p))
				sear();
			else
				continue;
			if (st[p].p <= 0) {
				bool pd = 0;
				if (st[p].p == pl || st[p].p == pm13) {
					temt = 0;
					if (st[p].p != pm13)
						return;
					ips.ges("请输入开始时间(s): ", 2);
					if (ips.ent)
						return;
				}
				else if (st[p].p == exi || st[p].p == plre || st[p].p == expl || st[p].p == sapu) {
					if (st[p].p == plre)
						p1.ex = 0;
					else if (st[p].p == expl)
						p1.ex = p1.al = 0;
					p -= 2;
					return;
				}
				else if (st[p].p == bgad && (tems = ips.ges("请输入音乐的名字： ")).size()) {
					gs.bgm[++gs.bg1] = tems;
					gs.pu1[gs.bg1] = 0;
					sebg();
				}
				else if (st[p].p == bgde && st[p].x >= 0) {
					gs.bgm[st[p].bgid] = "";
					--gs.bg1;
					ente.pl();
					wrse();
					rese();
					sebg();
					st[p -= 2].x = bgl;
					pd = 1;
				}
				else if (st[p].p == puad && (tems = ips.ges("请输入谱面的名字： ")).size()) {
					gs.pu[st[p].bgid][++gs.pu1[st[p].bgid]] = tems;
					gs.inps(st[p].bgid, gs.pu1[st[p].bgid]);
					sepu();
				}
				else if (st[p].p == pude && st[p].x >= 0) {
					gs.pu[st[p].bgid][st[p].puid] = "";
					--gs.pu1[st[p].bgid];
					ente.pl();
					wrse();
					rese();
					sepu();
					st[p -= 2].x = pul;
					pd = 1;
				}

				else if (st[p].p == norm)
					gs.di = 0;
				else if (st[p].p == hard)
					gs.di = 1;
				else if (st[p].p == mast)
					gs.di = 2;

				else if (st[p].p == bgv && (tems = ips.ges("背景音乐大小（最大100）： ", 1, nts((double)gs.bgv / 10))).size())
					gs.bgv = min(1000, (int)ctd(tems.c_str()) * 10);
				else if (st[p].p == efv && (tems = ips.ges("音效大小（最大100）： ", 1, nts((double)gs.efv / 10))).size())
					gs.efv = min(1000, (int)ctd(tems.c_str()) * 10);
				else if (st[p].p == p1s && (tems = ips.ges("P1音符下落速度（最小2，最大10）： ", 1, nts(p1.spe / 100))).size())
					p1.spe = max(200, min(1000, (int)ctd(tems.c_str()) * 100));
				else if (st[p].p == p2s && (tems = ips.ges("P2音符下落速度（最小2，最大10）： ", 1, nts(p2.spe / 100))).size())
					p2.spe = max(200, min(1000, (int)ctd(tems.c_str()) * 100));
				else if (st[p].p == rbt && (tems = ips.ges("练习模式重生时间（最小0，最大5）： ", 1, nts(gs.cnt))).size())
					gs.cnt = min((double)5, ctd(tems.c_str()));

				else if (st[p].p == p10)
					ips.chk("第一列： ", p1.sk[0]);
				else if (st[p].p == p11)
					ips.chk("第二列： ", p1.sk[1]);
				else if (st[p].p == p12)
					ips.chk("第三列： ", p1.sk[2]);
				else if (st[p].p == p13)
					ips.chk("第四列： ", p1.sk[3]);
				else if (st[p].p == p20)
					ips.chk("第一列： ", p2.sk[0]);
				else if (st[p].p == p21)
					ips.chk("第二列： ", p2.sk[1]);
				else if (st[p].p == p22)
					ips.chk("第三列： ", p2.sk[2]);
				else if (st[p].p == p23)
					ips.chk("第四列： ", p2.sk[3]);
				else if (st[p].p == adp)
					ips.chk("添加记录点： ", rp.sks);
				else if (st[p].p == dep)
					ips.chk("删除记录点： ", rp.skd);

				if (!pd) {
					--p;
					ente.pl();
					wrse();
					rese();
				}
			}
			disp();
		}
		info.inm = 0;
	}


	int ntp[K][B], nt1[K];//音符的位置(note position) 音符的类型 一条线上音符的数量
	inline void chin(cint& nx, int& np, const cint lax = K, const int lat = 0) {//暂停选音符初始化 np:音符在队列中的位置 nx:在第几列
		np = p1.ds;
		nx = K;
		int mit = infi, temi;
		memset(nt1, 0, sizeof nt1);
		for (cint x = 0; x < K; ++x)
			for (int y = p1.np[x]; y <= p1.nr[x]; ++y)
				if (p1.pdex(x, y))
					ntp[x][nt1[x]++] = y;
		if (lax == K) {
			for (cint x = 0; x < K; ++x)
				for (int y = 0; y < nt1[x]; ++y)
					if (pu.nt[x][ntp[x][y]].k != 3 && pu.nt[x][ntp[x][y]].t - nt < mit) {
						nx = x;
						np = y;
						mit = pu.nt[x][ntp[x][y]].t - nt;
						break;
					}
		}
		else
			for (cint x = 0; x < K; ++x)
				for (int y = 0; y < nt1[x]; ++y)
					if (pu.nt[x][ntp[x][y]].k != 3 && (temi = pow2(pu.nt[x][ntp[x][y]].t - lat) * (abs(lax - x) + 1)) < mit) {
						nx = x;
						np = y;
						mit = temi;
					}
	}

	inline int cyud(const cint x, const int y) {//上下循环边界判断(cycle up and down)
		if (y >= nt1[x])
			return 0;
		if (y < 0)
			return nt1[x] - 1;
		return y;
	}
	inline void tud(const cint nx, int& np, const int k) {//上下移(turn up and down)
		int p = np;
		for (; (p = cyud(nx, p + k)) != np && pu.nt[nx][ntp[nx][p]].k == 3;);
		np = p;
	}
	inline void tu(const cint nx, int& np) {//上移(turn up)
		tud(nx, np, 1);
	}
	inline void td(const cint nx, int& np) {//下移(turn down)
		tud(nx, np, -1);
	}

	inline void tlr(cint& nx, int& np, const int k) {//左右移(turn left and right)
		const int lat = pu.nt[nx][ntp[nx][np]].t;
		for (int x = nx, mit = p1.ds; (x = (x + k + K) % K) != nx;) {
			for (int y = 0; y < nt1[x]; ++y)
				if (abs(pu.nt[x][ntp[x][y]].t - lat) <= mit && pu.nt[x][ntp[x][y]].k != 3) {
					mit = abs(pu.nt[x][ntp[x][y]].t - lat);
					nx = x;
					np = y;
				}
			if (nx == x)
				return;
		}
	}
	inline void tl(cint& nx, int& np) {//左移(turn left)
		tlr(nx, np, -1);
	}
	inline void tr(cint& nx, int& np) {//右移(turn right)
		tlr(nx, np, 1);
	}
	inline void del(cint& nx, int& np) {//删除一个音符(delete (note))
		if (nx == K)
			return;
		const int lat = pu.nt[nx][ntp[nx][np]].t;
		p1.ntex[nx][ntp[nx][np]] = bj;
		if (tepu.nt[nx][ntp[nx][np]].k == 2)
			p1.ntex[nx][ntp[nx][np] + 1] = bj;
		tepu.del(ntp[nx][np], nx);
		chin(nx, np, nx, lat);
	}

	inline void dipa(const cint nx, const int np) {
		info.nx = nx;
		info.np = np;
		p1.cpp(info.p1);
		info.disp();
	}
	inline void sppa() {
		info.inch = 1;
		cint nx;
		int np;
		chin(nx, np);
		dipa(nx, np);
		for (; pdk(space););

		for (;;) {
			nk = ips.gek();
			if (nk == esc || nk == space)
				break;
			else if (nk == enter1 || nk == enter2)
				del(nx, np);
			else if (nk == left)
				tl(nx, np);
			else if (nk == right)
				tr(nx, np);
			else if (nk == up)
				tu(nx, np);
			else if (nk == down)
				td(nx, np);
			else
				continue;
			dipa(nx, np);
		}
		info.inch = 0;
		for (; pdk(space) || pdk(esc););
	}
}m;

//核心机制(struct play)
struct stpl {
	inline void pdnt(stpi& p, const cint x) {//判断音符(pd note)
		const int npt = pu.nt[x][p.np[x]].t;//音符的时间(now point time)
		p.k[x] = pdk(p.sk[x]);
		if (p.k[x] && !p.lak[x])
			clic.ef();
		if (p.k[x] && !p.lak[x] && npt <= nt + gs.pb) {//区域内按
			if (pu.nt[x][p.np[x]].k == 1) {//单键
				if (nt - gs.apf <= npt && npt <= nt + gs.ppf)
					p.adpf();
				else if (npt < nt - gs.apf || nt + gs.ppf < npt && npt <= nt + gs.pg)
					p.adgd();
				else
					p.adba();
				p.ntex[x][p.np[x]++] = bj;
			}
			else if (pu.nt[x][p.np[x]].k == 2) {//长键开始
				if (nt - gs.apf <= npt && npt <= nt + gs.ppf)
					p.lk[x] = 2;
				else if (npt < nt - gs.apf || nt + gs.ppf < npt && npt <= nt + gs.pg)
					p.lk[x] = 1;
				else {
					p.adba();
					p.ntex[x][p.np[x]++] = bj;
					p.lk[x] = 0;
				}
				p.ntex[x][p.np[x]++] = bj;
			}
		}
		else if (pu.nt[x][p.np[x]].k == 3 && p.lk[x]) {//长键结束
			if (p.lak[x] && npt <= nt + gs.pg) {
				if (p.lk[x] == 2)
					p.adpf();
				else
					p.adgd();
				p.ntex[x][p.np[x]++] = bj;
				p.lk[x] = 0;
			}
			else if (!p.k[x]) {
				p.adba();
				p.ntex[x][p.np[x]++] = bj;
				p.lk[x] = 0;
			}
		}
	}
	int lat[K];//按下时的时间(last time)
	inline void crnt(stpi& p, const cint x, const cint crm) {//创建音符(create note)
		static const int dt = 200;//判断是否为长按的时间(delta time)
		p.k[x] = pdk(p.sk[x]);
		if (p.k[x] && !p.lak[x]) {
			lat[x] = nt;
			clic.ef();
		}
		else if (!p.k[x] && p.lak[x]) {
			if (!crm && (nt - lat[x] <= dt) || crm == 1)
				tepu.ad1(lat[x], x);
			else
				tepu.ad2(lat[x], nt, x);
		}
	}

	inline void ntsi(stpi& p, const cint mode = 0, const cint crm = 0) {//判断音符情况(note situation)
		for (cint x = 0; x < K; ++x) {
			p.adnp(x);
			p.adnr(x);
			if (mode == 3)
				crnt(p, x, crm);
			else
				pdnt(p, x);
			p.fe(x);
		}
	}

	inline void cpp(stdi::stpi& cp, stpi& p) {//复制玩家信息(copy player)
		cp.sc = p.sc;
		cp.pf = p.pf;
		cp.gd = p.gd;
		cp.ba = p.ba;
		cp.com = p.com;
		cp.mac = p.mac;
		cp.pgb = p.pgb;
		p.cpp(cp);
	}
	inline void disp(const cint mode) {//显示(display)
		cpp(info.p1, p1);
		if (mode == 2)
			cpp(info.p2, p2);
		info.nt = nt;
		info.disp();
	}
	inline void dime(const cint p) {//显示菜单(display menu)
		++m.p;
		m.st[m.p] = m.st[m.p - 1];
		m.st[m.p].x = 0;
		m.st[m.p].p = p;
		m.mian();
		info.inm = 0;
		for (; pdk(esc););
	}

	inline bool puen(const cint mode) {//判断谱面是否完成(pu end)
		return p1.puen() && (mode != 2 || p2.puen());
	}
	inline double c321(const cint mode) {
		if (mode == 1 || mode == 3)
			return gs.cnt;
		return 3;
	}
	inline void pl(const int bgid, const int puid, const cint mode, const int st, const cint crm) {//单人游戏:0表示读曲失败 st:开始时间(start) mode:0是单人模式，1是练习模式，2是双人模式，3是制谱模式 crm:0是自动模式，1是短按，2是长按
		if (!bgm.op(gs.bgm[bgid], 1)) {
			info.bger();
			return;
		}
		if (!puid)
			pu.read(gs.pu[bgid][puid], "");
		else
			pu.read(gs.pu[bgid][puid], gs.bgm[bgid]);
		if (!pu.pdrp) {
			info.puer();
			return;
		}
		const int t1 = bgm.glen();
		info.tlen = t1;
		rp.ini(st);
		if (rp.sta[rp.st1] >= t1) {
			info.nuer();
			return;
		}

		static cint tmdi;
		if (mode == 2 || mode == 3) {
			tmdi = gs.di;
			gs.di = 0;
		}
		if (mode == 3)
			tepu.read(gs.pu[bgid][puid], gs.bgm[bgid], 1);

		for (++bj, p1.ex = 1; p1.ex || p1.al; ++bj) {
			if (mode == 1)
				rp.las = rp.lad = 0;
			else if (mode == 2)
				p2.ini(rp.sta[rp.st1]);
			else if (mode == 3)
				memset(lat, 0, sizeof lat);
			///////////////////////////////////////////////动画模块初始化
			info.inianimation(info.mode);
			info.ing = 1;
			p1.ini(rp.sta[rp.st1]);
			info.inid();
			disp(mode);
			info.c321(c321(mode));

			for (bgm.pl(rp.sta[rp.st1]); ((nt = bgm.nti()) < t1 || !puen(mode)) && p1.al && p1.ex;) {
				ntsi(p1, mode, crm);
				for (cint x = 0; x < K; ++x)
					info.p1.nk[x] = pdk(p1.sk[x]);
				if (mode == 1)
					rp.si(nt);
				else if (mode == 2) {
					ntsi(p2);
					for (cint x = 0; x < K; ++x)
						info.p2.nk[x] = pdk(p2.sk[x]);
				}
				if (mode == 3 && pdk(space)) {
					bgm.pl0();
					m.sppa();
					info.c321(gs.cnt);
					bgm.pl1();
				}
				if (pdk(esc)) {
					bgm.pl0();
					dime(m.paus);
					if (p1.al && p1.ex)
						info.c321(c321(mode));
					bgm.pl1();
				}
				disp(mode);
			}

			bgm.pl0();
			if ((p1.al || !mode) && p1.ex) {
				if (!mode)
					gs.pmsc[bgid][puid][gs.di] = max(gs.pmsc[bgid][puid][gs.di], p1.sc);
				info.hs = gs.pmsc[bgid][puid][gs.di];
				info.ing = 0;
				info.inen = 1;
				info.p1.rank = p1.rank();
				if (mode == 2)
					info.p2.rank = p2.rank();
				info.totn = pu.nk;
				dime(m.endi);
				for (; pdk(enter1) || pdk(enter2););
				info.inen = 0;
			}
		}

		bgm.clo();
		info.ing = 0;
		bgm.menu("");
		if (mode == 2 || mode == 3)
			gs.di = tmdi;
		static string tems;
		if (mode == 3 && (puid && (tems = ips.ges("请输入保存谱面的名字（按esc则不保存）： ", 0, gs.pu[bgid][puid])).size() || !puid && (tems = ips.ges("请输入保存谱面的名字（按esc则不保存）： ", 0)).size())) {
			tepu.writ(tems, gs.bgm[bgid]);
			if (tems != gs.pu[bgid][puid]) {
				gs.pu[bgid][++gs.pu1[bgid]] = tems;
				gs.inps(bgid, gs.pu1[bgid]);
			}
			else
				gs.inps(bgid, puid);
			m.sepu();
		}
	}
}play;

//mian函数
inline void mian() {
	rese();
	for (; m.p > 0;) {
		stme::sts& st = m.st[m.p];
		if (st.p == m.pl || st.p == m.pm13) {
			info.inm = 0;
			begi.pl();

			play.pl(st.bgid, st.puid, info.mode = st.mode, temt, st.crm);
			--m.p;
			wrse();
			rese();
		}
		m.mian();
	}
	wrse();
}
int main() {
	initial();
	std::ios_base::sync_with_stdio(false);
	info.gsh = gs.h = 217;
	info.gsbo = gs.bo = 22;
	mian();
}
//空格暂停
//menuloop
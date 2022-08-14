#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include<time.h>
#include<math.h>
#include"main.h"

struct node_monster* head_m;//���������ͷָ��
BK bk;
HERO hero;
list* zidanA_list = NULL;//�ӵ������ͷָ��
BARRIER* blist = NULL;
int bkpx, bkpy;//��һʱ�̱���������
int vx, vy;//�ӵ��ٶ�

IMAGE img_zidanA[2];
IMAGE img_hero1[2];
IMAGE img_hero2[2];
IMAGE img_move1[5];
IMAGE img_move11[5];
IMAGE img_move2[5];
IMAGE img_move21[5];
IMAGE img_book[5];
IMAGE img_book1[5];
IMAGE img_book2[5];
IMAGE img_book21[5];
IMAGE img_monster[2];
IMAGE img_bk;


int main()
{
	// ������ͼ����
	initgraph(WIDTH, HEIGHT);
	gameInit();

	int num = 0;
	ExMessage m;
	clock_t start_time;


	BeginBatchDraw();
	while (true)
	{
		cleardevice();
		start_time = clock();
		peekmessage(&m, EM_MOUSE | EM_KEY);//��ȡ�����Ϣ
		MouseControl(m, &num);//GOD�������ư�ť����
		bkpx = bk.x, bkpy = bk.y;//��¼����һʱ�̵ı�������

		playerMove(hero.speed, hero.x, hero.y);
		bkDraw(hero.speed, hero.x, hero.y);
		playerAshoot();
		gameDraw();//�������ͼ�ͽ�ɫͼƬ
		DrawButton();
		DrawButtonState(num);
		DrawMonster(hero.x, hero.y, bk.x, bk.y, bkpx, bkpy);//�����ޣ�����һ��Ȧ����ͼƬ���ٸ�
		showAzidan();
		ctrlFps(start_time);
		FlushBatchDraw();
		playerAskill();
	}
	EndBatchDraw();

	//�ͷ������ڴ�
	FreeMonster(head_m);
	FreeZiDan(zidanA_list);

	return 0;
}



//����?�����޵�������
void CreatAMonster(int x, int y, int type) {
	struct node_monster* p;
	if ((p = (struct node_monster*)malloc(sizeof(struct node_monster))) ==
		NULL) {
		printf("�ڴ����ʧ��");
		exit(1);
	}
	else {
		p->inf.x = x;
		p->inf.y = y;
		p->inf.type = type;
		//���ֹ��������ֵ
		switch (type) {
		    case 1:p->inf.Health_point = 1;
				break;
			case 2://p->inf.Health_point = 1;
				break;
			default:
				break;
		}
		if (head_m == NULL) {
			p->next = NULL;
			head_m = p;
		}
		else {
			p->next = head_m;
			head_m = p;
		}
	}
}

//�������������������ı���״̬�������������Ҫ��
void DrawMonster(int herox, int heroy, int bkx, int bky, int bkpx, int bkpy) {
	if (head_m == NULL) {
		;
	}
	else {
		struct node_monster* p;
		p = head_m;
		static int start;
		while (p) {
			if (p->inf.type == 1) {
				if (p->inf.Health_point != 0) {
					start = -1;
					double tsin;
					double tcos;
					int herox1 = herox - bkpx;
					int heroy1 = heroy - bkpy;
					int px = p->inf.x - bkpx;
					int py = p->inf.y - bkpy;
					double t = sqrt((px - herox1) * (px - herox1) + (py - heroy1) * (py - heroy1));
					if (t != 0) {
						tsin = (px - herox1) / t;
						tcos = (py - heroy1) / t;
						px -= (int)(tsin * 5);
						py -= (int)(tcos * 5);
					}
					p->inf.x = px + bkx;
					p->inf.y = py + bky;
					putimage(p->inf.x - 100, p->inf.y - 100, &img_monster[0], NOTSRCERASE);
					putimage(p->inf.x - 100, p->inf.y - 100, &img_monster[1], SRCINVERT);
				}
				if (p->inf.Health_point == 0) {
					if (start == -1) {
						start = clock();
					}
					if (clock() - start >= 3000) {
						p->inf.Health_point = 1;
					}
					p->inf.x = p->inf.x - bkpx + bkx;
					p->inf.y = p->inf.y - bkpy + bky;
					putimage(p->inf.x - 100, p->inf.y - 100, &img_monster[0], NOTSRCERASE);
					putimage(p->inf.x - 100, p->inf.y - 100, &img_monster[1], SRCINVERT);
				}
			}
			if (p->inf.type == 2) {
			}
			if (p->inf.type == 3) {
			}
			if (p->inf.type == 4) {
				p->inf.x = p->inf.x - bkpx + bkx;
				p->inf.y = p->inf.y - bkpy + bky;
				setfillcolor(WHITE);
				fillrectangle(p->inf.x - 20, p->inf.y - 20, p->inf.x + 20, p->inf.y + 20);
			}
			p = p->next;
		}
	}
}

//����������������(ʱͣ��)�������������Ҫ��
void DrawMonster1(int bkx, int bky, int bkpx, int bkpy) {
	if (head_m == NULL) {
		;
	}
	else {

		struct node_monster* p;
		p = head_m;
		while (p) {
			if (p->inf.type == 1) {
				setfillcolor(BLACK);
				p->inf.x = p->inf.x - bkpx + bkx;
				p->inf.y = p->inf.y - bkpy + bky;
				putimage(p->inf.x - 100, p->inf.y - 100, &img_monster[0], NOTSRCERASE);
				putimage(p->inf.x - 100, p->inf.y - 100, &img_monster[1], SRCINVERT);
			}
			if (p->inf.type == 4) {
				p->inf.x = p->inf.x - bkpx + bkx;
				p->inf.y = p->inf.y - bkpy + bky;
				setfillcolor(WHITE);
				fillrectangle(p->inf.x - 20, p->inf.y - 20, p->inf.x + 20, p->inf.y + 20);
			}
			p = p->next;
		}
	}
}

void FreeMonster(node_monster* head_m) {
	node_monster* p;
	if (head_m == NULL)
		return;
	else {
		while (head_m) {
			p = head_m;
			head_m = head_m->next;
			free(p);
		}
	}
}

void FreeZiDan(list* head_m) {
	list* p;
	if (head_m == NULL)
		return;
	else {
		while (head_m) {
			p = head_m;
			head_m = head_m->pnext;
			free(p);
		}
	}
}


//?����ƣ��������Ҳ��Ҫ��
void MouseControl(ExMessage m, int* num) {
	static int flag = 0;
	switch (m.message) {
	case WM_MOUSEMOVE:
		if (*num)
			solidcircle(m.x, m.y, 20);
		break;
	case WM_LBUTTONDOWN:
		*num = InWhichBotton(m.x, m.y);
		if (*num <= 12 && *num != 0) {
			flag = 1;
			solidcircle(m.x, m.y, 20);
		}
		break;
	case WM_LBUTTONUP:
		if (InWhichBotton(m.x, m.y) == 0 && *num <= 12 && flag) {
			CreatAMonster(m.x, m.y, *num);
		}
		flag = 0;
		*num = 0;
		break;
	}
}

//�����ƶ�
void bkDraw(int speed, int x, int y) {
	if (GetAsyncKeyState(87) && y <= 370)
	{
		bk.y += speed;
		hero.dongzuo = 1;
	}
	if (GetAsyncKeyState(83) && y >= 380)
	{
		bk.y -= speed;
		hero.dongzuo = 1;
	}
	if (GetAsyncKeyState(65) && x <= 556)
	{
		bk.x += speed;
		hero.dir = 1;
		hero.dongzuo = 1;
	}
	if (GetAsyncKeyState(68) && x >= 570)
	{
		bk.x -= speed;
		hero.dir = 2;
		hero.dongzuo = 1;
	}
}

//HERO�ƶ�
void playerMove(int speed, int x, int y)
{
	hero.dongzuo = 0;
	if (GetAsyncKeyState(87) && y > 370)//W
	{
		hero.y -= speed;
		hero.dongzuo = 1;
	}
	if (GetAsyncKeyState(83) && y < 380)//S
	{
		hero.y += speed;
		hero.dongzuo = 1;
	}
	if (GetAsyncKeyState(65) && x > 556)//A
	{
		hero.dir = 1;
		hero.x -= speed;
		hero.dongzuo = 1;
	}
	if (GetAsyncKeyState(68) && x < 570)//D
	{
		hero.dir = 2;
		hero.x += speed;
		hero.dongzuo = 1;
	}
}

void listPushBack(list** pplist, list* newNode)
{
	if (*pplist == NULL)//�������Ϊ�գ���ô�����ڵ���ǵ�?��
	{
		*pplist = newNode;
		return;
	}
	list* cur = *pplist;
	while (cur->pnext != NULL)//�ҵ����?���ڵ�
	{
		cur = cur->pnext;
	}
	cur->pnext = newNode;//��?�µĽڵ�
}

list* creatAzidan(int vx, int vy) {
	list* p = (list*)malloc(sizeof(list));
	p->x = hero.x - Azidanwidth / 2;
	p->y = hero.y - Azidanhigh / 2;
	p->vx = vx;
	p->vy = vy;
	p->isExist = 1;
	p->pnext = NULL;
	return p;
}

//?���ƶ�
void listChangeXY(list** pplist) {
	if (*pplist == NULL)
		return;
	list* cur = *pplist;
	while (cur != NULL)
	{
		cur->x += cur->vx - bkpx + bk.x;
		cur->y += cur->vy - bkpy + bk.y;
		//�ж�?���Ƿ���ʧ
		if ((cur->y > hero.y + HEIGHT/2) || (cur->y < hero.y - HEIGHT/2) ||
			(cur->x > hero.x + WIDTH/2) || (cur->x < hero.x - WIDTH/2))
			cur->isExist = 0;
		struct node_monster* cur_m = head_m;
		while (cur_m != NULL) {
			if (cur_m->inf.Health_point) {
				cur->isExist = IfHit(cur_m->inf.x, cur_m->inf.y, cur_m->inf.type,cur->x,cur->y);
				if (cur->isExist == 0)
					cur_m->inf.Health_point--;
			}
			cur_m = cur_m->next;
		}
		cur = cur->pnext;
	}
}

int IfHit(int m_x, int m_y, int type, int z_x, int z_y)
{
	int r = 1;
	switch (type)
	{
	case 1:
		if (z_x <= m_x && z_x >= m_x - 100 && z_y >= m_y - 100 && z_y <= m_y + 100)
			r = 0;
		break;
	default:
		break;
	}
	return r;
}

void listRemoveNode(list** pplist)
{
	if (*pplist == NULL)
		return;
	list* cur = *pplist;
	list* prev = NULL;
	while (cur != NULL)
	{
		if (cur->isExist == 0)
		{
			if (*pplist == cur)
			{
				*pplist = cur->pnext;
				free(cur);
				cur = *pplist;
				prev = *pplist;
			}
			else
			{
				prev->pnext = cur->pnext;
				free(cur);
				cur = prev;
			}
		}
		else
		{
			prev = cur;
			cur = cur->pnext;
		}
	}
}


void showAzidan()
{
	listChangeXY(&zidanA_list);
	listRemoveNode(&zidanA_list);
	for (list* cur = zidanA_list; cur != NULL; cur = cur->pnext) {
		if (cur->isExist) {
			putimage(cur->x, cur->y, &img_zidanA[0], NOTSRCERASE);
			putimage(cur->x, cur->y, &img_zidanA[1], SRCINVERT);
		}
	}
}

void playerAshoot() {
	static int shoot = 1;
	static int start = 0;
	if (GetAsyncKeyState(74))
	{
		if (clock() - start > S_LAG) {
			shoot = 1;
			hero.shoot = 1;
		}
		if (shoot == 1) {
			shoot = 0;
			start = clock();
			vx = 0;
			vy = 0;
			if (GetAsyncKeyState(65))
				vx = -8;
			if (GetAsyncKeyState(68)) {
				vx = 8;
				vy = 0;
			}
			if (GetAsyncKeyState(87)) {
				vy = -8;
			}
			if (GetAsyncKeyState(83))
				vy = 8;
			if (vx == 0 && vy == 0 && hero.dir == 2) {
				vx = 8;
			}
			if (vx == 0 && vy == 0 && hero.dir == 1) {
				vx = -8;
			}
		}
	}
}

void playerAskill() {
	if (GetAsyncKeyState(76))
	{
		int t;
		static int shunyi = 1;
		static clock_t begin = 0;
		static clock_t jishi = 0;
		t = hero.speed;
		if (clock() - begin > SKILL_LAG)
			shunyi = 1;
		if (shunyi == 1) {
			mciSendString(_T("open res/dio2.mp3 alias bk"), NULL, 0, NULL);
			mciSendString(_T("play bk"), NULL, 0, NULL);
			shunyi = 0;
			begin = clock();
			jishi = clock();
			while (clock() - jishi < SKILL_TIME) {
				flushmessage();
				cleardevice();
				hero.speed = 10;
				int bkpx = bk.x, bkpy = bk.y;//��¼��?ʱ�̵ı�������
				playerMove(hero.speed, hero.x, hero.y);
				bkDraw(hero.speed, hero.x, hero.y);
				gameDraw();//�������ͼ��??ͼ?
				DrawButton();
				DrawMonster1(bk.x, bk.y, bkpx, bkpy);
				showAzidan();
				FlushBatchDraw();
				playerAshoot();
				showAzidan();
				Sleep(15);
			}
			mciSendString(_T("close bk"), NULL, 0, NULL);
			hero.speed = t;
		}
	}
}

void ctrlFps(int start_time)
{
	clock_t running_time = clock() - start_time;
	if ((13 - running_time) >= 0)//��?˯�ߺ���ʹ?����
		Sleep(13 - running_time);//��̬˯��
}


//ͼ?���س���
void loadImg()
{
	//���ر���ͼ
	loadimage(&img_bk, _T("res/bk.jpg"));
	//���ﳯ�ҵ�ͼ
	loadimage(&img_hero1[0], _T("res/hero21.jpg"));
	loadimage(&img_hero1[1], _T("res/hero2.jpg"));
	//���ﳯ���ͼ
	loadimage(&img_hero2[0], _T("res/hero11.jpg"));
	loadimage(&img_hero2[1], _T("res/hero1.jpg"));
	//����
	loadimage(&img_monster[0], _T("res/����/����1.jpg"));
	loadimage(&img_monster[1], _T("res/����/����11.jpg"));
	//�ӵ���ͼ
	loadimage(&img_zidanA[0], _T("res/�ӵ�1.jpg"));
	loadimage(&img_zidanA[1], _T("res/�ӵ�.jpg"));
	//�����ƶ���ͼ
	loadimage(&img_move1[0], _T("res/�ܲ�/�ܲ�1.jpg"));
	loadimage(&img_move11[0], _T("res/�ܲ�/�ܲ�11.jpg"));
	loadimage(&img_move1[1], _T("res/�ܲ�/�ܲ�2.jpg"));
	loadimage(&img_move11[1], _T("res/�ܲ�/�ܲ�21.jpg"));
	loadimage(&img_move1[2], _T("res/�ܲ�/�ܲ�3.jpg"));
	loadimage(&img_move11[2], _T("res/�ܲ�/�ܲ�31.jpg"));
	loadimage(&img_move1[3], _T("res/�ܲ�/�ܲ�4.jpg"));
	loadimage(&img_move11[3], _T("res/�ܲ�/�ܲ�41.jpg"));
	loadimage(&img_move1[4], _T("res/�ܲ�/�ܲ�5.jpg"));
	loadimage(&img_move11[4], _T("res/�ܲ�/�ܲ�51.jpg"));
	loadimage(&img_move2[0], _T("res/�ܲ�/�ܲ�1l.jpg"));
	loadimage(&img_move21[0], _T("res/�ܲ�/�ܲ�11l.jpg"));
	loadimage(&img_move2[1], _T("res/�ܲ�/�ܲ�2l.jpg"));
	loadimage(&img_move21[1], _T("res/�ܲ�/�ܲ�21l.jpg"));
	loadimage(&img_move2[2], _T("res/�ܲ�/�ܲ�3l.jpg"));
	loadimage(&img_move21[2], _T("res/�ܲ�/�ܲ�31l.jpg"));
	loadimage(&img_move2[3], _T("res/�ܲ�/�ܲ�4l.jpg"));
	loadimage(&img_move21[3], _T("res/�ܲ�/�ܲ�41l.jpg"));
	loadimage(&img_move2[4], _T("res/�ܲ�/�ܲ�5l.jpg"));
	loadimage(&img_move21[4], _T("res/�ܲ�/�ܲ�51l.jpg"));
	//���ͼƬ
	loadimage(&img_book[0], _T("res/��/��1.jpg"));
	loadimage(&img_book1[0], _T("res/��/��11.jpg"));
	loadimage(&img_book[1], _T("res/��/��2.jpg"));
	loadimage(&img_book1[1], _T("res/��/��21.jpg"));
	loadimage(&img_book[2], _T("res/��/��3.jpg"));
	loadimage(&img_book1[2], _T("res/��/��31.jpg"));
	loadimage(&img_book[3], _T("res/��/��4.jpg"));
	loadimage(&img_book1[3], _T("res/��/��41.jpg"));
	loadimage(&img_book[4], _T("res/��/��5.jpg"));
	loadimage(&img_book1[4], _T("res/��/��51.jpg"));
	loadimage(&img_book2[0], _T("res/��/��1l.jpg"));
	loadimage(&img_book21[0], _T("res/��/��11l.jpg"));
	loadimage(&img_book2[1], _T("res/��/��2l.jpg"));
	loadimage(&img_book21[1], _T("res/��/��21l.jpg"));
	loadimage(&img_book2[2], _T("res/��/��3l.jpg"));
	loadimage(&img_book21[2], _T("res/��/��31l.jpg"));
	loadimage(&img_book2[3], _T("res/��/��4l.jpg"));
	loadimage(&img_book21[3], _T("res/��/��41l.jpg"));
	loadimage(&img_book2[4], _T("res/��/��5l.jpg"));
	loadimage(&img_book21[4], _T("res/��/��51l.jpg"));
}

//��ʼ������
void gameInit() {
	loadImg();
	hero.x = WIDTH / 2;
	hero.y = HEIGHT / 2;
	hero.life = true;
	hero.HP = 5;
	hero.speed = 6;
	hero.dir = 2;
	hero.shoot = 0;
	bk.x = 0;
	bk.y = 0;
}

//��Ϸ���ƺ���
void gameDraw()
{
	//�������ͼ?��ָ��λ��
	putimage(bk.x, bk.y, &img_bk);
	static int count_move;
	static int count_shoot;
	//���??Aͼ?
	if (hero.dir == 2 && hero.dongzuo == 0) {
		count_move = 0;
		putimage(hero.x - 40, hero.y - 42, &img_hero1[0], NOTSRCERASE);
		putimage(hero.x - 40, hero.y - 42, &img_hero1[1], SRCINVERT);
	}
	if (hero.dir == 1 && hero.dongzuo == 0) {
		count_move = 0;
		putimage(hero.x - 40, hero.y - 42, &img_hero2[0], NOTSRCERASE);
		putimage(hero.x - 40, hero.y - 42, &img_hero2[1], SRCINVERT);
	}
	//�ƶ�ʱ�Ķ���
	if (hero.dongzuo == 1  && hero.dir == 2) {
		if (count_move <= 11) {
			putimage(hero.x - 40, hero.y - 40, &img_move11[0], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move1[0], SRCINVERT);
		}
		else if (count_move <= 22) {
			putimage(hero.x - 40, hero.y - 40, &img_move11[1], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move1[1], SRCINVERT);
		}
		else if (count_move <= 33) {
			putimage(hero.x - 40, hero.y - 40, &img_move11[2], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move1[2], SRCINVERT);
		}
		else if (count_move <= 44) {
			putimage(hero.x - 40, hero.y - 40, &img_move11[3], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move1[3], SRCINVERT);
			count_move = 0;
		}
		else if (count_move <= 55) {
			putimage(hero.x - 40, hero.y - 40, &img_move11[4], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move1[4], SRCINVERT);
			count_move = 0;
		}
		count_move++;
	}
	if (hero.dongzuo == 1 && hero.dir == 1) {
		if (count_move <= 11) {
			putimage(hero.x - 40, hero.y - 40, &img_move21[0], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move2[0], SRCINVERT);
		}
		else if (count_move <= 22) {
			putimage(hero.x - 40, hero.y - 40, &img_move21[1], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move2[1], SRCINVERT);
		}
		else if (count_move <= 33) {
			putimage(hero.x - 40, hero.y - 40, &img_move21[2], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move2[2], SRCINVERT);
		}
		else if (count_move <= 44) {
			putimage(hero.x - 40, hero.y - 40, &img_move21[3], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move2[3], SRCINVERT);
			count_move = 0;
		}
		else if (count_move <= 55) {
			putimage(hero.x - 40, hero.y - 40, &img_move21[4], NOTSRCERASE);
			putimage(hero.x - 40, hero.y - 40, &img_move2[4], SRCINVERT);
			count_move = 0;
		}
		count_move++;
	}
	//��Ķ���
	if (hero.dir == 1 && hero.shoot == 0) {
		putimage(hero.x - 11, hero.y + 4, &img_book21[0], NOTSRCERASE);
		putimage(hero.x - 11, hero.y + 4, &img_book2[0], SRCINVERT);
	}
	if (hero.dir == 2 && hero.shoot == 0) {
		putimage(hero.x - 15, hero.y + 4, &img_book1[0], NOTSRCERASE);
		putimage(hero.x - 15, hero.y + 4, &img_book[0], SRCINVERT);
	}
	//ʩ������
	if (hero.dir == 1 && hero.shoot == 1) {
		if (count_shoot <= 5) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[0], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[0], SRCINVERT);
		}
		else if (count_shoot <= 11) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[1], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[1], SRCINVERT);
		}
		else if (count_shoot <= 17) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[2], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[2], SRCINVERT);
		}
		else if (count_shoot <= 24) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[3], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[3], SRCINVERT);
		}
		else if (count_shoot < 30) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[4], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[4], SRCINVERT);
		}
		else if (count_shoot == 30) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[4], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[4], SRCINVERT);
			listPushBack(&zidanA_list, creatAzidan(vx, vy));
		}
		else if (count_shoot <= 34) {
			putimage(hero.x - 11, hero.y + 4, &img_book21[2], NOTSRCERASE);
			putimage(hero.x - 11, hero.y + 4, &img_book2[2], SRCINVERT);
		}
		if (count_shoot == 34) {
			hero.shoot = 0;
			count_shoot = 0;
		}
		count_shoot++;
	}
	if (hero.dir == 2 && hero.shoot == 1) {
		if (count_shoot <= 5) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[0], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[0], SRCINVERT);
		}
	    else if (count_shoot <=11) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[1], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[1], SRCINVERT);
		}
		else if (count_shoot <= 17) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[2], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[2], SRCINVERT);
		}
		else if (count_shoot <= 24) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[3], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[3], SRCINVERT);
		}
		else if (count_shoot < 30) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[4], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[4], SRCINVERT);
		}
		else if (count_shoot == 30) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[4], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[4], SRCINVERT);
			listPushBack(&zidanA_list, creatAzidan(vx, vy));
		}
		else if (count_shoot <= 34) {
			putimage(hero.x - 15, hero.y + 4, &img_book1[2], NOTSRCERASE);
			putimage(hero.x - 15, hero.y + 4, &img_book[2], SRCINVERT);
		}
		if (count_shoot == 34) {
			hero.shoot = 0;
			count_shoot = 0;
		}
		count_shoot++;
	}

	
}

//�ж�?�������ĸ���ť
int InWhichBotton(int x, int y)
{
	int a = 0;
	if (x < 1120 && x > 1060 && y < 98 && y > 50)
		a = 1;
	if (x < 1120 && x > 1060 && y < 148 && y > 100)
		a = 2;
	if (x < 1120 && x > 1060 && y < 198 && y > 150)
		a = 3;
	if (x < 1120 && x > 1060 && y < 248 && y > 200)
		a = 4;
	if (x < 1120 && x > 1060 && y < 298 && y > 250)
		a = 5;
	if (x < 1120 && x > 1060 && y < 348 && y > 300)
		a = 6;
	if (x < 1120 && x > 1060 && y < 398 && y > 350)
		a = 7;
	if (x < 1120 && x > 1060 && y < 448 && y > 400)
		a = 8;
	if (x < 1120 && x > 1060 && y < 498 && y > 450)
		a = 9;
	if (x < 1120 && x > 1060 && y < 548 && y > 500)
		a = 10;
	if (x < 1120 && x > 1060 && y < 598 && y > 550)
		a = 11;
	if (x < 1120 && x > 1060 && y < 648 && y > 600)
		a = 12;
	return a;
}

//����ť
void DrawButton() {
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
	setlinecolor(RGB(0, 150, 200));
	roundrect(1060, 50, 1120, 98, 12, 12);
	outtextxy(1080, 68, _T("M1"));
	roundrect(1060, 100, 1120, 148, 12, 12);
	outtextxy(1080, 118, _T("M2"));
	roundrect(1060, 150, 1120, 198, 12, 12);
	outtextxy(1080, 168, _T("M3"));
	roundrect(1060, 200, 1120, 248, 12, 12);
	outtextxy(1085, 218, _T("Z"));
	roundrect(1060, 250, 1120, 298, 12, 12);
	roundrect(1060, 300, 1120, 348, 12, 12);
	roundrect(1060, 350, 1120, 398, 12, 12);
	roundrect(1060, 400, 1120, 448, 12, 12);
	roundrect(1060, 450, 1120, 498, 12, 12);
	roundrect(1060, 500, 1120, 548, 12, 12);
	roundrect(1060, 550, 1120, 598, 12, 12);
	roundrect(1060, 600, 1120, 648, 12, 12);
}

//������ť�����µ�״̬
void DrawButtonState(int num) {
	setfillcolor(WHITE);
	if (num == 1)
		fillroundrect(1060, 50, 1120, 98, 12, 12);
	if (num == 2)
		fillroundrect(1060, 100, 1120, 148, 12, 12);
	if (num == 3)
		fillroundrect(1060, 150, 1120, 198, 12, 12);
	if (num == 4)
		fillroundrect(1060, 200, 1120, 248, 12, 12);
	if (num == 5)
		fillroundrect(1060, 250, 1120, 298, 12, 12);
	if (num == 6)
		fillroundrect(1060, 300, 1120, 348, 12, 12);
	if (num == 7)
		fillroundrect(1060, 350, 1120, 398, 12, 12);
	if (num == 8)
		fillroundrect(1060, 400, 1120, 448, 12, 12);
	if (num == 9)
		fillroundrect(1060, 450, 1120, 498, 12, 12);
	if (num == 10)
		fillroundrect(1060, 500, 1120, 548, 12, 12);
	if (num == 11)
		fillroundrect(1060, 550, 1120, 598, 12, 12);
	if (num == 12)
		fillroundrect(1060, 600, 1120, 648, 12, 12);
}
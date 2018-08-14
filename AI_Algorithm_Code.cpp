// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI �ڵ� �ۼ� ���]

1. char info[]�� �迭 �ȿ�					"TeamName:�ڽ��� ����,Department:�ڽ��� �Ҽ�"					������ �ۼ��մϴ�.
( ���� ) Teamname:�� Department:�� �� ���� �մϴ�.
"�ڽ��� ����", "�ڽ��� �Ҽ�"�� �����ؾ� �մϴ�.

2. �Ʒ��� myturn() �Լ� �ȿ� �ڽŸ��� AI �ڵ带 �ۼ��մϴ�.

3. AI ������ �׽�Ʈ �Ͻ� ���� "���� �˰����ȸ ��"�� ����մϴ�.

4. ���� �˰��� ��ȸ ���� �����ϱ⿡�� �ٵϵ��� ���� ��, �ڽ��� "����" �� �� �˰����� �߰��Ͽ� �׽�Ʈ �մϴ�.



[���� �� �Լ�]
myturn(int cnt) : �ڽ��� AI �ڵ带 �ۼ��ϴ� ���� �Լ� �Դϴ�.
int cnt (myturn()�Լ��� �Ķ����) : ���� �� �� �־��ϴ��� ���ϴ� ����, cnt�� 1�̸� ���� ���� ��  �� ����  �δ� ��Ȳ(�� ��), cnt�� 2�̸� �� ���� ���� �δ� ��Ȳ(�� ��)
int  x[0], y[0] : �ڽ��� �� ù �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
int  x[1], y[1] : �ڽ��� �� �� �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
void domymove(int x[], int y[], cnt) : �� ������ ��ǥ�� �����ؼ� ���


//int board[BOARD_SIZE][BOARD_SIZE]; �ٵ��� �����Ȳ ��� �־� �ٷλ�� ������. ��, ���������ͷ� ���� �������
// ������ ���� ��ġ�� �ٵϵ��� ������ �ǰ��� ó��.

boolean ifFree(int x, int y) : ���� [x,y]��ǥ�� �ٵϵ��� �ִ��� Ȯ���ϴ� �Լ� (������ true, ������ false)
int showBoard(int x, int y) : [x, y] ��ǥ�� ���� ���� �����ϴ��� �����ִ� �Լ� (1 = �ڽ��� ��, 2 = ����� ��, 3 = ��ŷ)


<-------AI�� �ۼ��Ͻ� ��, ���� �̸��� �Լ� �� ���� ����� �������� �ʽ��ϴ�----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:�źϸ� ������,Department:�ݿ��������б�" };

int* centerchk(int chkx, int chky, int off);
int* linechk(int line[13], int off);
int* findoff(int chkx, int chky, int off);
int showBoard2(int x, int y);
int direction[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } }; // ���⺤��(12�ú��� �ð����)
int board2[BOARD_SIZE][BOARD_SIZE];
int myx[180], myy[180];
int mynum = 0;
int xf, yf;

void myturn(int cnt) {

	int x[2] = { -1, -1 }, y[2] = { -1, -1 };

	// �� �κп��� �˰��� ���α׷�(AI)�� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����Ͻø� �˴ϴ�.
	// ���� Sample code�� AI�� Random���� ���� ���� Algorithm�� �ۼ��Ǿ� �ֽ��ϴ�.

	srand((unsigned)time(NULL));

	if (cnt == 1) {
		do {
			x[0] = 9 + rand() % 4;
			y[0] = 9 + rand() % 4;
			if (terminateAI) return;
		} while (!isFree(x[0], y[0]));
		myx[mynum] = x[0];
		myy[mynum] = y[0];
		mynum += 1;
	}
	else {
		if (mynum == 0) {
			if (abs(lastx[0] - 9) > 6 && abs(lastx[0] - 9) > 6) {
				for (int k = 0; k < cnt; k++)
					do {
						x[k] = 9 + rand() % 4;
						y[k] = 9 + rand() % 4;
					} while (!isFree(x[k], y[k]));
			}

			else {
				do {
					x[0] = lastx[0] + rand() % 3 - 1;
					y[0] = lasty[0] + rand() % 3 - 1;
					x[1] = x[0] + lasty[0] - y[0];
					y[1] = y[0] + lastx[0] - x[0];
				} while (!isFree(x[0], y[0]) || !isFree(x[1], y[1]));
			}


			myx[mynum] = x[0];
			myy[mynum] = y[0];
			myx[mynum + 1] = x[1];
			myy[mynum + 1] = y[1];
			mynum += 2;
		}

		else {
			for (int i = 0; i < 20; i++) for (int j = 0; j < 20; j++) board2[i][j] = board[i][j]; // board2 �ʱ�ȭ
			int danger[3] = { 0 }, advan[3] = { 0 }; // danger:��� advan:���� 0:���赵, 1,2:���� x,y��ǥ
			int xc, yc; // x, y �ĺ�
			int savedx[10] = { 0 }; // 0 : ����
			int savedy[10] = { 0 }; // 0 : �ִ���������

			for (int i = 0; i < mynum; i++) { // ���� ������ ���� ã��
				int *advtemp = findoff(myx[i], myy[i], 1);
				int afindoff[3] = { advtemp[0], advtemp[1], advtemp[2] };

				if (afindoff[2] != 0 && (advan[0] > afindoff[2] || advan[0] == 0)) {
					advan[0] = afindoff[2];
					advan[1] = myx[i] + direction[afindoff[0]][0] * afindoff[1];
					advan[2] = myy[i] + direction[afindoff[0]][1] * afindoff[1];
				}
			}

			for (int i = 0; i < cnt; i++) { // ���� ������ ���� ã��
				int *dantemp = findoff(lastx[i], lasty[i], 2);
				int afindoff[3] = { dantemp[0], dantemp[1], dantemp[2] };

				if (afindoff[2] != 0 && (danger[0] > afindoff[2] || danger[0] == 0)) {
					danger[0] = afindoff[2];
					danger[1] = lastx[i] + direction[afindoff[0]][0] * afindoff[1];
					danger[2] = lasty[i] + direction[afindoff[0]][1] * afindoff[1];
				}
			}

			if (advan[0] != 0 && advan[0] < 4) { // �¸� Ȯ��
				int *temp = centerchk(advan[1], advan[2], 1);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

				if (acenterchk[1] > 1)
					for (int i = 0; i < 2; i++) { // �� �ΰ��� ��� ���Ѵ�
						x[i] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
						y[i] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					}
				else {
					x[0] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5];
					y[0] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5];
				}
			}

			else if (danger[0] != 0 && danger[0] < 4) { // ��ġ�� �й�
				int *temp = centerchk(danger[1], danger[2], 2);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				/*savedx[0] = acenterchk[1];
				for (int i = 0; i < acenterchk[1]; i++) { // ��� �� �ĺ� 1���� �� ���� ���� ���ݼ��� ����
				xc = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
				yc = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
				savedx[i+1] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
				savedy[i+1] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
				if (i == (int)acenterchk[1] / 2) { xf = xc; yf = yc; }

				temp = centerchk(xc, yc, 1);
				int bcenterchk[20];
				for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

				if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
				x[0] = xc;
				y[0] = yc;
				baselevel = bcenterchk[0];
				}
				}
				if (x[0] == -1) { x[0] = xf; y[0] = yf; }
				baselevel = 0;
				for (int i = 0; i < acenterchk[3]; i++) { // ��� �� �ĺ� 2���� �� ���� ���� ���ݼ��� ����
				xc = danger[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + acenterchk[1]];
				yc = danger[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + acenterchk[1]];
				savedx[i + 1] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i + acenterchk[1]];
				savedy[i + 1] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i +acenterchk[1]];
				if (i == (int)acenterchk[3] / 2) { xf = xc; yf = yc; }

				temp = centerchk(xc, yc, 1);
				int bcenterchk[20];
				for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

				if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
				x[1] = xc;
				y[1] = yc;
				baselevel = bcenterchk[0];
				}
				}*/

				if (acenterchk[1] > 1)
					for (int i = 0; i < 2; i++) { // �� �ΰ��� ��� ���Ѵ�
						x[i] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
						y[i] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					}
				else {
					x[0] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5];
					y[0] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5];

					for (int i = 0; i < acenterchk[3]; i++) { // ��� �� �ĺ� 2���� �� ���� ���� ���ݼ��� ����
						xc = danger[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + acenterchk[1]];
						yc = danger[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + acenterchk[1]];
						savedx[i + 1] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i + acenterchk[1]];
						savedy[i + 1] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i + acenterchk[1]];
						if (i == (int)acenterchk[3] / 2) { xf = xc; yf = yc; }

						temp = centerchk(xc, yc, 1);
						int bcenterchk[20];
						for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

						if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
							x[1] = xc;
							y[1] = yc;
							baselevel = bcenterchk[0];
						}
					}
				}


				if (x[1] == -1 && acenterchk[3] != 0 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
			}
			else if (advan[0] != 0 && advan[0] < 6) { // ������ �¸� Ȯ��
				int *temp = centerchk(advan[1], advan[2], 1);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

				for (int i = 0; i < 2; i++) { // �� �ΰ��� ��� ���Ѵ�
					x[i] = advan[1] + direction[acenterchk[2 + 2 * i]][0] * acenterchk[5 + acenterchk[1] * i];
					y[i] = advan[2] + direction[acenterchk[2 + 2 * i]][1] * acenterchk[5 + acenterchk[1] * i];
				}
			}

			else if (danger[0] != 0 && danger[0] < 6) { // ������ �й� Ȯ��
				int limit1 = centerchk(danger[1], danger[2], 2)[1];
				int limit2 = centerchk(danger[1], danger[2], 2)[3]; // 4�� ��� 1���� 2���� �� �� ����

				int *temp = centerchk(danger[1], danger[2], 2);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				for (int i = 0; i < limit1; i++) { // ��� �� �ĺ� 1���� �� ���� ���� ���ݼ��� ����
					xc = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
					yc = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					if (i == (int)limit1 / 2) { xf = xc; yf = yc; }

					temp = centerchk(xc, yc, 1);
					int bcenterchk[20];
					for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

					if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
						x[0] = xc;
						y[0] = yc;
						baselevel = bcenterchk[0];
					}
				}
				if (x[0] == -1) { x[0] = xf; y[0] = yf; }
				baselevel = 0;
				board2[x[0]][y[0]] = 1;
				for (int i = 0; i < limit2; i++) {
					xc = danger[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + limit1];
					yc = danger[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + limit1];
					if (i == (int)limit2 / 2) { xf = xc; yf = yc; }

					temp = centerchk(xc, yc, 1);
					int bcenterchk[20];
					for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

					if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0) && (x[0] != xc || y[0] != yc)) {
						x[1] = xc;
						y[1] = yc;
						baselevel = bcenterchk[0];
					}
				}
				if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
			}
			else if (advan[0] > 5) { // �����Ҹ��� ��
				int *temp = centerchk(advan[1], advan[2], 1);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				for (int i = 0; i < acenterchk[1]; i++) { // ���� �� 1���� �� ���� ���� ���� ����
					xc = advan[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
					yc = advan[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					if (i == (int)acenterchk[1] / 2) { xf = xc; yf = yc; }

					temp = centerchk(xc, yc, 2);
					int bcenterchk[20];
					for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

					if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
						x[0] = xc;
						y[0] = yc;
						baselevel = bcenterchk[0];
					}
				}
				if (x[0] == -1) { x[0] = xf; y[0] = yf; }
				baselevel = 0;

				for (int i = 0; i < acenterchk[3]; i++) { // ���� �� 2���� �� ���� ���� ���� ����
					xc = advan[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + acenterchk[1]];
					yc = advan[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + acenterchk[1]];
					if (i == (int)(acenterchk[3]) / 2) { xf = xc; yf = yc; }

					temp = centerchk(xc, yc, 2);
					int bcenterchk[20];
					for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

					if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0) && (x[0] != xc || y[0] != yc)) {
						x[1] = xc;
						y[1] = yc;
						baselevel = bcenterchk[0];
					}
				}
				if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
			}
			else if (danger[0] > 5) { // ����Ҹ��� ��
				int *temp = centerchk(danger[1], danger[2], 2);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				for (int i = 0; i < acenterchk[1] + acenterchk[3]; i++) { // ��� �� �ĺ� �� ���� ���� ���ݼ��� ����
					if (i < acenterchk[1]) {
						xc = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
						yc = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					}
					else {
						xc = danger[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + acenterchk[1]];
						yc = danger[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + acenterchk[1]];
					}
					if (i == (int)(acenterchk[1] + acenterchk[3]) / 2) { xf = xc; yf = yc; }

					temp = centerchk(xc, yc, 1);
					int bcenterchk[20];
					for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

					if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
						x[0] = xc;
						y[0] = yc;
						baselevel = bcenterchk[0];
					}
				}
				if (x[0] == -1) { x[0] = xf; y[0] = yf; }
			}
			else {
				bool exitloop = false;
				for (int i = 0; i < 9; i++) {
					for (int j = -i; j < i + 1; j++) {
						for (int k = -i; k < i + 1; k++) {
							x[0] = 9 + j;
							y[0] = 9 + k;
							if (!isFree(x[0], y[0])) {
								exitloop = true;
								break;
							}
						}
						if (exitloop = true) break;
					}
					if (exitloop = true) break;
				}
			}

			if (x[1] == -1) { // �� ���� ��� ���� �ʾ��� ��
				board2[x[0]][y[0]] = 1;
				myx[mynum] = x[0];
				myy[mynum] = y[0];
				mynum += 1;
				xc = 0; yc = 0;

				if (savedy[0] == 1 && savedx[0] > 0) { // ����� ���� ���� ��
					int baselevel = 0;

					for (int i = 1; i < savedx[0] + 1; i++) { // ��� �� �ĺ� �� ���� ���� ���ݼ��� ����
						if (savedx[i] != x[0] || savedy[i] != y[0]) {
							xc = savedx[i];
							yc = savedy[i];
							int* temp = centerchk(xc, yc, 1);
							int bcenterchk[20];
							for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

							if ((bcenterchk[0] > 1 && (baselevel > bcenterchk[0] || baselevel == 0)) || (bcenterchk[0] == 0 && baselevel == 0 && bcenterchk[0] != 1)) {
								x[1] = xc;
								y[1] = yc;
								baselevel = bcenterchk[0];
							}
						}
					}
					if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
					myx[mynum] = x[1];
					myy[mynum] = y[1];
					mynum += 1;
				}
				else {
					for (int i = 0; i < 3; i++) { advan[i] = 0; danger[i] = 0; }
					for (int i = 0; i < mynum; i++) { // ���� ������ ���� ã��
						int *temp = findoff(myx[i], myy[i], 1);
						int afindoff[3] = { temp[0], temp[1], temp[2] };

						if (afindoff[2] != 0 && (advan[0] > afindoff[2] || advan[0] == 0)) {
							advan[0] = afindoff[2];
							advan[1] = myx[i] + direction[afindoff[0]][0] * afindoff[1];
							advan[2] = myy[i] + direction[afindoff[0]][1] * afindoff[1];
						}
					}

					for (int i = 0; i < cnt; i++) { // ���� ������ ���� ã��
						int *temp = findoff(lastx[i], lasty[i], 2);
						int afindoff[3] = { temp[0], temp[1], temp[2] };

						if (afindoff[2] != 0 && (danger[0] > afindoff[2] || danger[0] == 0)) {
							danger[0] = afindoff[2];
							danger[1] = lastx[i] + direction[afindoff[0]][0] * afindoff[1];
							danger[2] = lasty[i] + direction[afindoff[0]][1] * afindoff[1];
						}
					}

					if (advan[0] != 0 && advan[0] < 4) { // �¸� Ȯ��
						/*int *temp = centerchk(advan[1], advan[2], 1);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

						x[1] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5];
						y[1] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5];*/
					}

					else if (danger[0] != 0 && danger[0] < 4) { // ��ġ�� �й�
						int *temp = centerchk(danger[1], danger[2], 2);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < acenterchk[1]; i++) { // ��� �� �ĺ� 1���� �� ���� ���� ���ݼ��� ����
							xc = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
							yc = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
							if (i == (int)acenterchk[1] / 2) { xf = xc; yf = yc; }

							temp = centerchk(xc, yc, 1);
							int bcenterchk[20];
							for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

							if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
								x[1] = xc;
								y[1] = yc;
								baselevel = bcenterchk[0];
							}
						}
						if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
					}

					else if (advan[0] != 0 && advan[0] < 6) { // ������ �¸� Ȯ��
						int *temp = centerchk(advan[1], advan[2], 1);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

						x[1] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5];
						y[1] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5];
					}

					else if (danger[0] != 0 && danger[0] < 6) { // ������ �й� Ȯ��
						int limit = centerchk(danger[1], danger[2], 2)[1];
						int limit1 = limit;
						if (danger[0] == 4) limit += centerchk(danger[1], danger[2], 2)[3]; // 4�� ��� 1���� 2���� �� �� ����

						int *temp = centerchk(danger[1], danger[2], 2);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < limit; i++) { // ��� �� �ĺ� 1���� �� ���� ���� ���ݼ��� ����
							if (i < limit1) {
								xc = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
								yc = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
							}
							else {
								xc = danger[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + limit1];
								yc = danger[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + limit1];
							}
							if (i == (int)limit / 2) { xf = xc; yf = yc; }

							temp = centerchk(xc, yc, 1);
							int bcenterchk[20];
							for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

							if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
								x[1] = xc;
								y[1] = yc;
								baselevel = bcenterchk[0];
							}
						}
						if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
					}
					else if (advan[0] > 5) { // �����Ҹ��� ��
						int *temp = centerchk(advan[1], advan[2], 1);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < acenterchk[1] + acenterchk[3]; i++) { // ���� �� �ĺ� �� ���� ���� ���� ����
							if (i < acenterchk[1]) {
								xc = advan[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
								yc = advan[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
							}
							else {
								xc = advan[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + acenterchk[1]];
								yc = advan[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + acenterchk[1]];
							}
							if (i == (int)(acenterchk[1] + acenterchk[3]) / 2) { xf = xc; yf = yc; }

							temp = centerchk(xc, yc, 2);
							int bcenterchk[20];
							for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

							if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
								x[1] = xc;
								y[1] = yc;
								baselevel = bcenterchk[0];
							}
						}
						if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
					}
					else if (danger[0] > 5) { // ����Ҹ��� ��
						int *temp = centerchk(danger[1], danger[2], 2);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < acenterchk[1] + acenterchk[3]; i++) { // ��� �� �ĺ� �� ���� ���� ���ݼ��� ����
							if (i < acenterchk[1]) {
								xc = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
								yc = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
							}
							else {
								xc = danger[1] + direction[acenterchk[4]][0] * acenterchk[5 + i + acenterchk[1]];
								yc = danger[2] + direction[acenterchk[4]][1] * acenterchk[5 + i + acenterchk[1]];
							}
							if (i == (acenterchk[1] + acenterchk[3]) / 2) { xf = xc; yf = yc; }

							temp = centerchk(xc, yc, 1);
							int bcenterchk[20];
							for (int j = 0; j < 20; j++) bcenterchk[j] = temp[j];

							if (bcenterchk[0] != 0 && (baselevel > bcenterchk[0] || baselevel == 0)) {
								x[1] = xc;
								y[1] = yc;
								baselevel = bcenterchk[0];
							}
						}
						if (x[1] == -1 && (x[0] != xf || y[0] != yf)) { x[1] = xf; y[1] = yf; }
					}
					if (x[1] == -1) {
						bool exitloop = false;
						for (int i = 0; i < 9; i++) {
							for (int j = -i; j < i + 1; j++) {
								for (int k = -i; k < i + 1; k++) {
									x[1] = 9 + j;
									y[1] = 9 + k;
									if (!isFree(x[0], y[0])) {
										exitloop = true;
										break;
									}
								}
								if (exitloop = true) break;
							}
							if (exitloop = true) break;
						}
					}

					myx[mynum] = x[1];
					myy[mynum] = y[1];
					mynum += 1;
				}
			}
			else {
				myx[mynum] = x[0];
				myy[mynum] = y[0];
				myx[mynum + 1] = x[1];
				myy[mynum + 1] = y[1];
				mynum += 2;
			}

		}
	}

	if (cnt == 2) while (!isFree(x[0], y[0]) || !isFree(x[1], y[1])) {
		x[0] = rand() % 19;
		y[0] = rand() % 19;
		x[1] = rand() % 19;
		y[1] = rand() % 19;
		if (terminateAI) return;
	}


	/*srand((unsigned)time(NULL));

	for (int i = 0; i < cnt; i++) {
	do {
	x[i] = rand() % width;
	y[i] = rand() % height;
	if (terminateAI) return;
	} while (!isFree(x[i], y[i]));

	if (x[1] == x[0] && y[1] == y[0]) i--;
	}*/

	// �� �κп��� �ڽ��� ���� ���� ����Ͻʽÿ�.
	// �ʼ� �Լ� : domymove(x�迭,y�迭,�迭ũ��)
	// ���⼭ �迭ũ��(cnt)�� myturn()�� �Ķ���� cnt�� �״�� �־���մϴ�.
	if (terminateAI) return;
	domymove(x, y, cnt);
}
// defchk:��ǥ �ֺ� ���¸� �˻���(����)

int* centerchk(int chkx, int chky, int off) { // chkx, chky�� �˻��� ��ǥ, off:���ݵ�
	int lineinfo[4][13]; // �˻��� ��ǥ �߽� 13�� Ȯ��
	static int centerret[20] = { 0 }; // 0:���赵 1,2:1���� ������,���� 3,4:2���� ������,���� 5~:��ǥ
	for (int i = 0; i < 20; i++) centerret[i] = 0;
	int baselevelinfo[4] = { 0 }; // 0~1:1���� ��ġ,����, 2~3:2���� ��ġ,����
	for (int i = 0; i < 4; i++) baselevelinfo[i] = 0;
	int linechk0;

	for (int i = 0; i < 4; i++) { // 4���� üũ i:���⺤�� �ε���
		for (int j = 0; j < 13; j++) lineinfo[i][j] = showBoard2(chkx + direction[i][0] * (j - 6), chky + direction[i][1] * (j - 6));
		linechk0 = linechk(lineinfo[i], off)[0];
		if (linechk0 != 0 && ((baselevelinfo[0] > linechk0) || baselevelinfo[0] == 0)) {
			baselevelinfo[2] = baselevelinfo[0];
			baselevelinfo[3] = baselevelinfo[1];
			baselevelinfo[0] = linechk0;
			baselevelinfo[1] = i;
		}
		else if (linechk0 != 0 && ((baselevelinfo[2] > linechk0) || baselevelinfo[2] == 0) && (linechk0 != baselevelinfo[0] || i != baselevelinfo[1])) {
			baselevelinfo[2] = linechk0;
			baselevelinfo[3] = i;
		}
	}

	if (baselevelinfo[0] == 0) centerret[0] = 0;
	else if (baselevelinfo[0] == 1 || (baselevelinfo[2] != 0 && baselevelinfo[2] < 4)) centerret[0] = 1; // ��ġ 1 1�� +a || ��ġ 2~3 2�� +a
	else if (baselevelinfo[0] == 2 && (baselevelinfo[2] != 0 && baselevelinfo[2] < 6)) centerret[0] = 2; // ��ġ 2 1�� 4~5 1�� +a
	else if (baselevelinfo[0] < 4) centerret[0] = 3; // ��ġ 2~3 1��
	else if (baselevelinfo[2] == 4) centerret[0] = 4; // ��ġ 4 2�� +a
	else if (baselevelinfo[0] == 4 && baselevelinfo[2] == 5) centerret[0] = 5; // ��ġ 4 1�� + 5 1�� +a
	else if (baselevelinfo[2] == 5) centerret[0] = 6; // ��ġ 5 n�� +a || ��ġ 4 1��
	else if (baselevelinfo[2] == 6) centerret[0] = 7; // ��ġ 6 n�� +a
	else if (baselevelinfo[2] == 7) centerret[0] = 8; // ��ġ 7 n��
	else centerret[0] = 0;

	if (centerret[0] != 0) {
		int* linechk1 = linechk(lineinfo[baselevelinfo[1]], off); // 1���� linechk
		centerret[1] = linechk1[1];
		centerret[2] = baselevelinfo[1];
		for (int i = 0; i < centerret[1]; i++) centerret[i + 5] = linechk1[i + 2];

		int* linechk3 = linechk(lineinfo[baselevelinfo[3]], off); // 2���� linechk
		if (centerret[1] != linechk3[1] || centerret[2] != baselevelinfo[3]) {
			centerret[3] = linechk3[1];
			centerret[4] = baselevelinfo[3];
			for (int i = 0; i < centerret[3]; i++) centerret[i + 5 + centerret[1]] = linechk3[i + 2];
			if (centerret[0] == -1) {
				centerret[0] = -1;
			}
		}

	}

	return centerret;
}

int* linechk(int line[13], int off) {
	static int linebaselevel[20]; // 0:���赵 1:������ 2~:����ġ
	for (int i = 0; i < 20; i++) linebaselevel[i] = 0;
	int linein[8];
	for (int i = 0; i < 8; i++) linein[i] = 0;
	int lineout[7] = { 0 };
	for (int i = 0; i < 7; i++) lineout[i] = 0;
	bool valid;
	valid = true;
	int def; // def:��
	if (off == 1) def = 2;
	else def = 1;

	// 0~12�� 0~6���� ��ȯ

	for (int i = 0; i < 6; i++) { // ���ʺ��� �˻�
		for (int j = 0; j < 8; j++) linein[j] = line[i + j];

		valid = true;


		for (int j = 1; j < 7; j++) { // ���迭 �˻�
			if (linein[j] == def || linein[j] == -1) valid = false; // 1~6�� �� ������ ��ȿó��
			else { lineout[j - 1] = linein[j]; }
		}

		if (linein[0] == off || linein[0] == 3 || linein[7] == off || linein[7] == 3) valid = false; // 0,7�� ���ݵ� ������ ��ȿó��

		if (valid == true) {
			if (linein[0] == 0) {
				if (linein[7] == 0) lineout[6] = 0; // ���� �շ����� ��
				else lineout[6] = 6; // �����ʿ� �� ���� ��
			}
			else {
				if (linein[7] == 0) lineout[6] = -1; // ���ʿ� ��� �� ���� ��
				else lineout[6] = 2; // ���ʿ� ��� �� ���� ��
			}
		}

		else for (int i = 0; i < 7; i++) lineout[i] = 0;


		// ����/��ġ ã��


		int ret[10] = { 0 }; // ret (0:��ġ, 1:�ֿ켱������, 2~:�ֿ켱����ġ)
		int offense = 0; // offense:���ݵ� ����
		int left, right; // left:������ ���ݵ� ��ġ, right:�ֿ��� ���ݵ� ��ġ
		int blank[6] = { 0 }; // blank:����� ��ǥ
		int cnt = 0;
		int neighbor; // neighbor:��� �� �� ��ġ
		int opp, oppneighbor;
		if (lineout[6] == -1) { neighbor = 0; opp = 6; oppneighbor = 5; }
		else if (lineout[6] == 6) { neighbor = 5; opp = -1; oppneighbor = 0; }
		else if (lineout[6] == 2) opp = 0;
		else opp = 2;

		for (int j = 0; j < 6; j++) { // ���� �� ����, �� ���� ��ġ �ľ�
			if (lineout[j] != 0) offense++;
			else if (lineout[j] == 0) {
				blank[cnt] = j;
				cnt++;
			}
		}

		for (int j = 0; j< 6; j++) { // ������ �ֿ��� ���ݵ� ��ġ Ȯ��
			left = j;
			if (lineout[j] != 0) break;
		}
		for (int j = 5; j > -1; j--) {
			right = j;
			if (lineout[j] != 0) break;
		}


		if (lineout[6] == 2) { // �� ���� �������� ��
			if (offense == 5 || offense == 4) { ret[0] = 2; ret[1] = 2; ret[2] = blank[0]; ret[3] = blank[1]; }
			else { ret[0] = 0; }
		}

		else if (offense == 5) { // ���� �� 5�� �� ��
			if (blank[0] == 2 || blank[0] == 3) { // ���� ���� 4�� ���� �پ����� �ʴ� ���
				if (lineout[6] != 0 && abs(lineout[6] - blank[0]) == 4) { // ���� 3�� ���� ��� ���� �پ��ִ� ���
					ret[0] = 3;
					ret[1] = 2; ret[2] = blank[0]; ret[3] = opp;
				}
				else {
					ret[0] = 2;
					ret[1] = 1;
					ret[2] = blank[0];
				}
			}
			else if (lineout[6] != 0 && abs(lineout[6] - blank[0]) > 4) { // 4�� ������ ���� �� ���� ��� ���� �ִ� ���
				ret[0] = 3;
				ret[1] = 1;
				ret[2] = blank[0];
			}
			else {
				ret[0] = 1;
				ret[1] = 2;
				ret[2] = blank[0];
				if (blank[0] == 4) ret[3] = -1;
				else ret[3] = 6;
			}
		}



		else if (offense == 4) { // ���� �� 4�� �� ��
			if ((blank[0] + blank[1] == 1) || (blank[0] + blank[1] == 9) || (blank[0] == 0 && blank[1] == 5)) { // 4�� ���� �پ� �ִ� ���
				if (lineout[6] != 0 && (lineout[neighbor] != 0 || (blank[0] + blank[1] == 5))) { // ���� ���� ��� ���� �پ��ְų� 1ĭ�� ������ �ִ� ���
					ret[0] = 3;
					if (blank[0] + blank[1] == 5) {
						ret[1] = 2;	ret[2] = oppneighbor; ret[3] = opp;
					}
					else {
						if (off == 2) { ret[1] = 1; if (lineout[6] == -1) ret[2] = blank[0]; else ret[2] = blank[1]; }
						else { ret[1] = 2; ret[2] = blank[0]; ret[3] = blank[1]; }
					}
				}
				else {
					ret[0] = 1;
					ret[1] = 2;
					ret[2] = left - 1;
					ret[3] = right + 1;
				}
			}
			else if (blank[0] == 1 && blank[1] == 4) { // 101101 �迭�� ���
				if (lineout[6] == 0) { // �շ��ִ°��
					ret[0] = 2;
					ret[1] = 2;
					ret[2] = blank[0];
					ret[3] = blank[1];
				}
				else {
					ret[0] = 3;
					ret[1] = 2;
					if (lineout[6] == 6) { ret[2] = blank[0]; ret[3] = blank[1]; }
					else { ret[2] = blank[1]; ret[3] = blank[0]; }
				}
			}
			else if (blank[0] == 2 || blank[1] == 3) {  // 3���� �پ����� �ʴ� ���
				ret[0] = 3;
				if (off == 1) {
					ret[1] = 2;
					if (blank[0] == 2) { ret[2] = blank[0]; ret[3] = blank[1]; }
					else { ret[2] = blank[1]; ret[3] = blank[0]; }
				}
				else { ret[1] = 1; if (blank[0] == 2) ret[2] = blank[0]; else ret[2] = blank[1]; }
			}

			else if (lineout[6] == 0) { // �� ���� ���
				ret[0] = 2;
				if (off == 1) {
					ret[1] = 2;
					ret[2] = blank[0]; ret[3] = blank[1];
				}
				else { ret[1] = 1; for (int k = left + 1; k < right; k++) if (lineout[k] == 0) ret[2] = k; }
			}

			else if ((lineout[6] == 6 && blank[0] == 3) || (lineout[6] == -1 && blank[1] == 2)) { // �� �ݴ� ���ʿ� 3���� �پ��ִ� ���
				ret[0] = 2;
				if (off == 1) {
					ret[1] = 2;
					ret[2] = blank[0]; ret[3] = opp;
				}
				else { ret[1] = 1; for (int k = left + 1; k < right; k++) if (lineout[k] == 0) ret[2] = k; }
			}
			else {
				if (off == 1) {
					ret[0] = 3;
					ret[1] = 2;
					ret[2] = blank[0]; ret[3] = blank[1];
				}
				else { ret[1] = 1; for (int k = left + 1; k < right; k++) if (lineout[k] == 0) ret[2] = k; }
			}
		}

		else if (offense == 3) { // ���� �� 3�� �� ��
			if (right - left < 4) { // ���� �� ���� ���� 1�� ����
				if (lineout[6] != 0 && lineout[neighbor] != 0) { //��� ���� �پ��ִ� ���
					ret[0] = 5;
					ret[1] = 2;
					if (right - left == 3) {
						for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) ret[2] = blank[j];
						if (lineout[6] == -1) ret[3] = right + 1;
						else ret[3] = left - 1;
					}
					else if (lineout[6] == -1) { ret[2] = right + 1; ret[3] = right + 2; }
					else { ret[2] = left - 1; ret[3] = left - 2; }
				}
				else {
					ret[0] = 4;
					if (off == 1) {
						if (right - left == 3) {
							ret[1] = 3;
							for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) ret[2] = blank[j];
							ret[3] = left - 1;
							ret[4] = right + 1;
						}
						else { ret[1] = 4; ret[2] = left - 1; ret[3] = right + 1; ret[4] = left - 2; ret[5] = right + 2; }
					}
					else {
						if (right - left == 3) {
							ret[1] = 1;
							for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) ret[2] = blank[j];
						}
						else { ret[1] = 2; ret[2] = left - 1; ret[3] = right + 1; }
					}
				}
			}
			else if (lineout[6] != 0 && lineout[neighbor] != 0) {// ���� �� ���� ���� 2�� �̻�, ��� ���� �پ��ִ� ���
				ret[0] = 7;
				ret[1] = 2;
				int temp = 0;
				for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) { ret[2 + temp] = blank[j]; temp++; }
			}
			else if (right - left == 4) { // ���� �� ���� ���� 2��, ��� ���� �پ����� �ʴ� ���
				ret[0] = 5;
				ret[1] = 2;
				int temp = 0;
				for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) { ret[2 + temp] = blank[j]; temp++; }
			}
			else {
				ret[0] = 6;
				int temp = 0;
				for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) { ret[2 + temp] = blank[j]; temp++; }
				ret[1] = temp;
			}
		}

		else if (offense == 2) { // ���� �� 2�� �� ��
			if (right - left < 4) { // ���� �� ���� ���� 2�� ����
				if (lineout[6] == 0 ||
					((lineout[6] == -1 && lineout[1] == 0) || (lineout[6] == 6 && lineout[4] == 0))) { // ���� ���� �� ���� �ּ� ���� 2 �̻�
					ret[0] = 6;
				}
				else {
					ret[0] = 7;
				}
			}
			else {
				ret[0] = 7;
			}

			int tempcnt = 0;
			for (int k = left + 1; k < right; k++) if (lineout[k] == 0) { ret[tempcnt + 2] = k; tempcnt++; }
			if (tempcnt < 3) {
				int tempconst = tempcnt;
				for (int l = 1; l < 4 - tempconst; l++) {
					ret[tempcnt + 2] = left - l;
					tempcnt++;
					ret[tempcnt + 2] = right + l;
					tempcnt++;
				}
			}
			ret[1] = tempcnt;
		}
		else if (offense == 1 && lineout[6] == 0) { // �� �ϳ��� ��������
			ret[0] = 7;
			ret[1] = 4;
			ret[2] = left - 2;
			ret[3] = left - 1;
			ret[4] = right + 1;
			ret[5] = right + 2;
		}

		else ret[0] = 0;

		if (ret[0] != 0 && ((linebaselevel[0] > ret[0]) || linebaselevel[0] == 0)) {
			for (int k = 0; k < ret[1] + 2; k++) linebaselevel[k] = ret[k];
			for (int k = 0; k < ret[1]; k++) linebaselevel[k + 2] += i - 5; // line[13] �迭�� �°� �� ��ġ ����
		}


		if (linebaselevel[1] != 0)  // ���/���� �ĺ� �Ÿ���
			for (int j = 0; j < ret[1]; j++)
				if (line[linebaselevel[2 + j] + 6] != 0) {
					linebaselevel[2 + j] = 0;
					for (int k = 2 + j; k < linebaselevel[1] + 2; k++) linebaselevel[k] = linebaselevel[k + 1];
					linebaselevel[linebaselevel[1] + 2] = 0;
					linebaselevel[1]--;
					if (linebaselevel[1] == 0) break;
				}
		if (linebaselevel[1] == 0) linebaselevel[0] = 0;
	}

	return linebaselevel;
}

int* findoff(int chkx, int chky, int off) { // ���赵 ���� ū ���� ã��
	int* baselevel;
	static int abaselevel[50] = { 0 }; // 0:������ġ 1:���ͺ��� 2:���赵 3,4:1����������,���� 5,6:2���� 7~:����ġ
	for (int i = 0; i < 50; i++) abaselevel[i] = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = -5; j < 6; j++) {
			int adjx = chkx + direction[i][0] * j;
			int adjy = chky + direction[i][1] * j;
			if (adjy == 10 && adjx == 5) {
				adjy = 10;
			}
			int leveltemp = centerchk(adjx, adjy, off)[0];

			if (leveltemp != 0 && ((abaselevel[2] > leveltemp) || abaselevel[2] == 0))  {
				baselevel = centerchk(adjx, adjy, off);
				baselevel[-1] = j;
				baselevel[-2] = i;
				for (int k = 0; k < 50; k++) abaselevel[k] = baselevel[k - 2];
			}
		}
	}

	return abaselevel;
}

int showBoard2(int x, int y) {
	if (x<0 || y<0 || x>18 || y>18) return -1;
	else return board2[x][y];
}
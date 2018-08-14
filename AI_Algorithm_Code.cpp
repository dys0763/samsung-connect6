// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:거북목 갈고리즘,Department:금오공과대학교" };

int* centerchk(int chkx, int chky, int off);
int* linechk(int line[13], int off);
int* findoff(int chkx, int chky, int off);
int showBoard2(int x, int y);
int direction[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } }; // 방향벡터(12시부터 시계방향)
int board2[BOARD_SIZE][BOARD_SIZE];
int myx[180], myy[180];
int mynum = 0;
int xf, yf;

void myturn(int cnt) {

	int x[2] = { -1, -1 }, y[2] = { -1, -1 };

	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

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
			for (int i = 0; i < 20; i++) for (int j = 0; j < 20; j++) board2[i][j] = board[i][j]; // board2 초기화
			int danger[3] = { 0 }, advan[3] = { 0 }; // danger:방어 advan:공격 0:위험도, 1,2:센터 x,y좌표
			int xc, yc; // x, y 후보
			int savedx[10] = { 0 }; // 0 : 개수
			int savedy[10] = { 0 }; // 0 : 있는지없는지

			for (int i = 0; i < mynum; i++) { // 가장 유리한 센터 찾기
				int *advtemp = findoff(myx[i], myy[i], 1);
				int afindoff[3] = { advtemp[0], advtemp[1], advtemp[2] };

				if (afindoff[2] != 0 && (advan[0] > afindoff[2] || advan[0] == 0)) {
					advan[0] = afindoff[2];
					advan[1] = myx[i] + direction[afindoff[0]][0] * afindoff[1];
					advan[2] = myy[i] + direction[afindoff[0]][1] * afindoff[1];
				}
			}

			for (int i = 0; i < cnt; i++) { // 가장 위험한 센터 찾기
				int *dantemp = findoff(lastx[i], lasty[i], 2);
				int afindoff[3] = { dantemp[0], dantemp[1], dantemp[2] };

				if (afindoff[2] != 0 && (danger[0] > afindoff[2] || danger[0] == 0)) {
					danger[0] = afindoff[2];
					danger[1] = lastx[i] + direction[afindoff[0]][0] * afindoff[1];
					danger[2] = lasty[i] + direction[afindoff[0]][1] * afindoff[1];
				}
			}

			if (advan[0] != 0 && advan[0] < 4) { // 승리 확정
				int *temp = centerchk(advan[1], advan[2], 1);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

				if (acenterchk[1] > 1)
					for (int i = 0; i < 2; i++) { // 돌 두개를 모두 정한다
						x[i] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
						y[i] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					}
				else {
					x[0] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5];
					y[0] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5];
				}
			}

			else if (danger[0] != 0 && danger[0] < 4) { // 방치시 패배
				int *temp = centerchk(danger[1], danger[2], 2);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				/*savedx[0] = acenterchk[1];
				for (int i = 0; i < acenterchk[1]; i++) { // 방어 돌 후보 1순위 중 가장 높은 공격성을 선택
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
				for (int i = 0; i < acenterchk[3]; i++) { // 방어 돌 후보 2순위 중 가장 높은 공격성을 선택
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
					for (int i = 0; i < 2; i++) { // 돌 두개를 모두 정한다
						x[i] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5 + i];
						y[i] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5 + i];
					}
				else {
					x[0] = danger[1] + direction[acenterchk[2]][0] * acenterchk[5];
					y[0] = danger[2] + direction[acenterchk[2]][1] * acenterchk[5];

					for (int i = 0; i < acenterchk[3]; i++) { // 방어 돌 후보 2순위 중 가장 높은 공격성을 선택
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
			else if (advan[0] != 0 && advan[0] < 6) { // 다음턴 승리 확정
				int *temp = centerchk(advan[1], advan[2], 1);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

				for (int i = 0; i < 2; i++) { // 돌 두개를 모두 정한다
					x[i] = advan[1] + direction[acenterchk[2 + 2 * i]][0] * acenterchk[5 + acenterchk[1] * i];
					y[i] = advan[2] + direction[acenterchk[2 + 2 * i]][1] * acenterchk[5 + acenterchk[1] * i];
				}
			}

			else if (danger[0] != 0 && danger[0] < 6) { // 다음턴 패배 확정
				int limit1 = centerchk(danger[1], danger[2], 2)[1];
				int limit2 = centerchk(danger[1], danger[2], 2)[3]; // 4일 경우 1순위 2순위 둘 다 선택

				int *temp = centerchk(danger[1], danger[2], 2);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				for (int i = 0; i < limit1; i++) { // 방어 돌 후보 1순위 중 가장 높은 공격성을 선택
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
			else if (advan[0] > 5) { // 공격할만한 곳
				int *temp = centerchk(advan[1], advan[2], 1);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				for (int i = 0; i < acenterchk[1]; i++) { // 공격 돌 1순위 중 가장 높은 방어성을 선택
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

				for (int i = 0; i < acenterchk[3]; i++) { // 공격 돌 2순위 중 가장 높은 방어성을 선택
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
			else if (danger[0] > 5) { // 방어할만한 곳
				int *temp = centerchk(danger[1], danger[2], 2);
				int acenterchk[20];
				for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
				int baselevel = 0;
				xc = 0; yc = 0;

				for (int i = 0; i < acenterchk[1] + acenterchk[3]; i++) { // 방어 돌 후보 중 가장 높은 공격성을 선택
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

			if (x[1] == -1) { // 두 돌을 모두 놓지 않았을 때
				board2[x[0]][y[0]] = 1;
				myx[mynum] = x[0];
				myy[mynum] = y[0];
				mynum += 1;
				xc = 0; yc = 0;

				if (savedy[0] == 1 && savedx[0] > 0) { // 저장된 돌이 있을 때
					int baselevel = 0;

					for (int i = 1; i < savedx[0] + 1; i++) { // 방어 돌 후보 중 가장 높은 공격성을 선택
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
					for (int i = 0; i < mynum; i++) { // 가장 유리한 센터 찾기
						int *temp = findoff(myx[i], myy[i], 1);
						int afindoff[3] = { temp[0], temp[1], temp[2] };

						if (afindoff[2] != 0 && (advan[0] > afindoff[2] || advan[0] == 0)) {
							advan[0] = afindoff[2];
							advan[1] = myx[i] + direction[afindoff[0]][0] * afindoff[1];
							advan[2] = myy[i] + direction[afindoff[0]][1] * afindoff[1];
						}
					}

					for (int i = 0; i < cnt; i++) { // 가장 위험한 센터 찾기
						int *temp = findoff(lastx[i], lasty[i], 2);
						int afindoff[3] = { temp[0], temp[1], temp[2] };

						if (afindoff[2] != 0 && (danger[0] > afindoff[2] || danger[0] == 0)) {
							danger[0] = afindoff[2];
							danger[1] = lastx[i] + direction[afindoff[0]][0] * afindoff[1];
							danger[2] = lasty[i] + direction[afindoff[0]][1] * afindoff[1];
						}
					}

					if (advan[0] != 0 && advan[0] < 4) { // 승리 확정
						/*int *temp = centerchk(advan[1], advan[2], 1);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

						x[1] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5];
						y[1] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5];*/
					}

					else if (danger[0] != 0 && danger[0] < 4) { // 방치시 패배
						int *temp = centerchk(danger[1], danger[2], 2);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < acenterchk[1]; i++) { // 방어 돌 후보 1순위 중 가장 높은 공격성을 선택
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

					else if (advan[0] != 0 && advan[0] < 6) { // 다음턴 승리 확정
						int *temp = centerchk(advan[1], advan[2], 1);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];

						x[1] = advan[1] + direction[acenterchk[2]][0] * acenterchk[5];
						y[1] = advan[2] + direction[acenterchk[2]][1] * acenterchk[5];
					}

					else if (danger[0] != 0 && danger[0] < 6) { // 다음턴 패배 확정
						int limit = centerchk(danger[1], danger[2], 2)[1];
						int limit1 = limit;
						if (danger[0] == 4) limit += centerchk(danger[1], danger[2], 2)[3]; // 4일 경우 1순위 2순위 둘 다 선택

						int *temp = centerchk(danger[1], danger[2], 2);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < limit; i++) { // 방어 돌 후보 1순위 중 가장 높은 공격성을 선택
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
					else if (advan[0] > 5) { // 공격할만한 곳
						int *temp = centerchk(advan[1], advan[2], 1);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < acenterchk[1] + acenterchk[3]; i++) { // 공격 돌 후보 중 가장 높은 방어성을 선택
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
					else if (danger[0] > 5) { // 방어할만한 곳
						int *temp = centerchk(danger[1], danger[2], 2);
						int acenterchk[20];
						for (int j = 0; j < 20; j++) acenterchk[j] = temp[j];
						int baselevel = 0;
						xc = 0; yc = 0;

						for (int i = 0; i < acenterchk[1] + acenterchk[3]; i++) { // 방어 돌 후보 중 가장 높은 공격성을 선택
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

	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	if (terminateAI) return;
	domymove(x, y, cnt);
}
// defchk:좌표 주변 상태를 검사함(방어용)

int* centerchk(int chkx, int chky, int off) { // chkx, chky는 검사할 좌표, off:공격돌
	int lineinfo[4][13]; // 검사할 좌표 중심 13돌 확인
	static int centerret[20] = { 0 }; // 0:위험도 1,2:1순위 돌개수,벡터 3,4:2순위 돌개수,벡터 5~:좌표
	for (int i = 0; i < 20; i++) centerret[i] = 0;
	int baselevelinfo[4] = { 0 }; // 0~1:1순위 가치,벡터, 2~3:2순위 가치,벡터
	for (int i = 0; i < 4; i++) baselevelinfo[i] = 0;
	int linechk0;

	for (int i = 0; i < 4; i++) { // 4방향 체크 i:방향벡터 인덱스
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
	else if (baselevelinfo[0] == 1 || (baselevelinfo[2] != 0 && baselevelinfo[2] < 4)) centerret[0] = 1; // 방어가치 1 1개 +a || 방어가치 2~3 2개 +a
	else if (baselevelinfo[0] == 2 && (baselevelinfo[2] != 0 && baselevelinfo[2] < 6)) centerret[0] = 2; // 방어가치 2 1개 4~5 1개 +a
	else if (baselevelinfo[0] < 4) centerret[0] = 3; // 방어가치 2~3 1개
	else if (baselevelinfo[2] == 4) centerret[0] = 4; // 방어가치 4 2개 +a
	else if (baselevelinfo[0] == 4 && baselevelinfo[2] == 5) centerret[0] = 5; // 방어가치 4 1개 + 5 1개 +a
	else if (baselevelinfo[2] == 5) centerret[0] = 6; // 방어가치 5 n개 +a || 방어가치 4 1개
	else if (baselevelinfo[2] == 6) centerret[0] = 7; // 방어가치 6 n개 +a
	else if (baselevelinfo[2] == 7) centerret[0] = 8; // 방어가치 7 n개
	else centerret[0] = 0;

	if (centerret[0] != 0) {
		int* linechk1 = linechk(lineinfo[baselevelinfo[1]], off); // 1순위 linechk
		centerret[1] = linechk1[1];
		centerret[2] = baselevelinfo[1];
		for (int i = 0; i < centerret[1]; i++) centerret[i + 5] = linechk1[i + 2];

		int* linechk3 = linechk(lineinfo[baselevelinfo[3]], off); // 2순위 linechk
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
	static int linebaselevel[20]; // 0:위험도 1:돌개수 2~:돌위치
	for (int i = 0; i < 20; i++) linebaselevel[i] = 0;
	int linein[8];
	for (int i = 0; i < 8; i++) linein[i] = 0;
	int lineout[7] = { 0 };
	for (int i = 0; i < 7; i++) lineout[i] = 0;
	bool valid;
	valid = true;
	int def; // def:방어돌
	if (off == 1) def = 2;
	else def = 1;

	// 0~12를 0~6으로 변환

	for (int i = 0; i < 6; i++) { // 왼쪽부터 검사
		for (int j = 0; j < 8; j++) linein[j] = line[i + j];

		valid = true;


		for (int j = 1; j < 7; j++) { // 돌배열 검사
			if (linein[j] == def || linein[j] == -1) valid = false; // 1~6에 방어돌 있으면 무효처리
			else { lineout[j - 1] = linein[j]; }
		}

		if (linein[0] == off || linein[0] == 3 || linein[7] == off || linein[7] == 3) valid = false; // 0,7에 공격돌 있으면 무효처리

		if (valid == true) {
			if (linein[0] == 0) {
				if (linein[7] == 0) lineout[6] = 0; // 양쪽 뚫려있을 때
				else lineout[6] = 6; // 오른쪽에 방어돌 있을 때
			}
			else {
				if (linein[7] == 0) lineout[6] = -1; // 왼쪽에 방어 돌 있을 때
				else lineout[6] = 2; // 양쪽에 방어 돌 있을 때
			}
		}

		else for (int i = 0; i < 7; i++) lineout[i] = 0;


		// 공격/방어가치 찾기


		int ret[10] = { 0 }; // ret (0:가치, 1:최우선돌개수, 2~:최우선돌위치)
		int offense = 0; // offense:공격돌 개수
		int left, right; // left:최좌측 공격돌 위치, right:최우측 공격돌 위치
		int blank[6] = { 0 }; // blank:빈공간 좌표
		int cnt = 0;
		int neighbor; // neighbor:방어 돌 옆 위치
		int opp, oppneighbor;
		if (lineout[6] == -1) { neighbor = 0; opp = 6; oppneighbor = 5; }
		else if (lineout[6] == 6) { neighbor = 5; opp = -1; oppneighbor = 0; }
		else if (lineout[6] == 2) opp = 0;
		else opp = 2;

		for (int j = 0; j < 6; j++) { // 공격 돌 개수, 빈 공간 위치 파악
			if (lineout[j] != 0) offense++;
			else if (lineout[j] == 0) {
				blank[cnt] = j;
				cnt++;
			}
		}

		for (int j = 0; j< 6; j++) { // 최좌측 최우측 공격돌 위치 확인
			left = j;
			if (lineout[j] != 0) break;
		}
		for (int j = 5; j > -1; j--) {
			right = j;
			if (lineout[j] != 0) break;
		}


		if (lineout[6] == 2) { // 양 쪽이 막혀있을 시
			if (offense == 5 || offense == 4) { ret[0] = 2; ret[1] = 2; ret[2] = blank[0]; ret[3] = blank[1]; }
			else { ret[0] = 0; }
		}

		else if (offense == 5) { // 공격 돌 5개 일 시
			if (blank[0] == 2 || blank[0] == 3) { // 공격 돌이 4개 연속 붙어있지 않는 경우
				if (lineout[6] != 0 && abs(lineout[6] - blank[0]) == 4) { // 연속 3개 돌이 방어 돌과 붙어있는 경우
					ret[0] = 3;
					ret[1] = 2; ret[2] = blank[0]; ret[3] = opp;
				}
				else {
					ret[0] = 2;
					ret[1] = 1;
					ret[2] = blank[0];
				}
			}
			else if (lineout[6] != 0 && abs(lineout[6] - blank[0]) > 4) { // 4개 연속인 공격 돌 옆에 방어 돌이 있는 경우
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



		else if (offense == 4) { // 공격 돌 4개 일 시
			if ((blank[0] + blank[1] == 1) || (blank[0] + blank[1] == 9) || (blank[0] == 0 && blank[1] == 5)) { // 4개 돌이 붙어 있는 경우
				if (lineout[6] != 0 && (lineout[neighbor] != 0 || (blank[0] + blank[1] == 5))) { // 공격 돌이 방어 돌과 붙어있거나 1칸만 떨어져 있는 경우
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
			else if (blank[0] == 1 && blank[1] == 4) { // 101101 배열인 경우
				if (lineout[6] == 0) { // 뚫려있는경우
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
			else if (blank[0] == 2 || blank[1] == 3) {  // 3개가 붙어있지 않는 경우
				ret[0] = 3;
				if (off == 1) {
					ret[1] = 2;
					if (blank[0] == 2) { ret[2] = blank[0]; ret[3] = blank[1]; }
					else { ret[2] = blank[1]; ret[3] = blank[0]; }
				}
				else { ret[1] = 1; if (blank[0] == 2) ret[2] = blank[0]; else ret[2] = blank[1]; }
			}

			else if (lineout[6] == 0) { // 방어돌 없는 경우
				ret[0] = 2;
				if (off == 1) {
					ret[1] = 2;
					ret[2] = blank[0]; ret[3] = blank[1];
				}
				else { ret[1] = 1; for (int k = left + 1; k < right; k++) if (lineout[k] == 0) ret[2] = k; }
			}

			else if ((lineout[6] == 6 && blank[0] == 3) || (lineout[6] == -1 && blank[1] == 2)) { // 방어돌 반대 끝쪽에 3개가 붙어있는 경우
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

		else if (offense == 3) { // 공격 돌 3개 일 시
			if (right - left < 4) { // 공격 돌 사이 공간 1개 이하
				if (lineout[6] != 0 && lineout[neighbor] != 0) { //방어 돌과 붙어있는 경우
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
			else if (lineout[6] != 0 && lineout[neighbor] != 0) {// 공격 돌 사이 공간 2개 이상, 방어 돌과 붙어있는 경우
				ret[0] = 7;
				ret[1] = 2;
				int temp = 0;
				for (int j = 0; j < 3; j++) if (blank[j] > left && blank[j] < right) { ret[2 + temp] = blank[j]; temp++; }
			}
			else if (right - left == 4) { // 공격 돌 사이 공간 2개, 방어 돌과 붙어있지 않는 경우
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

		else if (offense == 2) { // 공격 돌 2개 일 시
			if (right - left < 4) { // 공격 돌 사이 공간 2개 이하
				if (lineout[6] == 0 ||
					((lineout[6] == -1 && lineout[1] == 0) || (lineout[6] == 6 && lineout[4] == 0))) { // 공격 돌과 방어돌 사이 최소 공간 2 이상
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
		else if (offense == 1 && lineout[6] == 0) { // 돌 하나에 열려있음
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
			for (int k = 0; k < ret[1]; k++) linebaselevel[k + 2] += i - 5; // line[13] 배열에 맞게 돌 위치 조정
		}


		if (linebaselevel[1] != 0)  // 방어/공격 후보 거르기
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

int* findoff(int chkx, int chky, int off) { // 위험도 가장 큰 센터 찾기
	int* baselevel;
	static int abaselevel[50] = { 0 }; // 0:센터위치 1:센터벡터 2:위험도 3,4:1순위돌개수,벡터 5,6:2순위 7~:돌위치
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
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARRAY_MAX 27
#define HAND_MAX 2

// 確率計算の関数
float probability(int sum,int x){
	float p;
	
	p = (float)x/(float)sum;

	return p;
}


int main(void){
	// 変数宣言
	int x,y,z; /*x:コンピュータ　y:プレイヤー　z:勝利判定*/
	int i,j,k,l,m,n,sum; //i:直前のcpu j:直前のユーザ k:予測されるユーザ手
	float h,r;
	FILE *fpHand,*fpA, *fpwHand, *fpwA; 
	int c[3][3][3],b[2];
	float p[3];
	
	while(1){

	// 履歴の読み込み
	fpA = fopen("ARRAY.csv","r");
	if(fpA == NULL){
		printf("ARRAY.csvが開けないため終了します");
		return 1;
	}


	l = 0;
	for(i = 0;i < 3;i++){
		for(j = 0;j < 3;j++){
			for(k = 0;k < 3;k++){
				fscanf(fpA, "%d",&(c[i][j][k]));

				l++;
				if(c[i][j][k] <= 0){
					printf("ARRAY.csv内に不適切な値があるため終了します");//ARRAY.csvの値はすべて1以上
					return 1;
				}else if(feof(fpA) && l < ARRAY_MAX){
					printf("ARRAY.csv内の要素が不足しているため終了します");
					return 1;
				}
				// printf("%d\n",c[i][j][k]);
			}
		}
	}


	fclose(fpA);

	// 直前のじゃんけんの手の読み込み
	fpHand = fopen("HAND.csv","r");
	if(fpHand == NULL){
		printf("HAND.csvが開けないため終了します");
		return 1;
	}

	printf("\n");

	l = 0;
	while(! feof(fpHand) && l < HAND_MAX){
		fscanf(fpHand, "%d",&(b[l]));
		if(b[l] < 0 || b[l] > 2){
			printf("HAND.csv内に不適切な値があるため終了します");
			return 1;
		}
		l++;
	}
	if(l < HAND_MAX){
		printf("HAND.csv内の要素が不足しているため終了します");
		return 1;
	}


	fclose(fpHand);

	i = b[0];//直前のcpu手
	j = b[1];//直前のユーザ手

	// printf("%d\n%d\n",i,j);

	// c[i][j][k]の総和
	sum = 0;
	for(k = 0;k <= 2;k++){
		sum = sum + c[i][j][k];
	}

	// printf("%d\n",sum);
	// printf("%d\n%d\n",i,j);

	// c[i][j][k] k=0,1,2の確率計算
	for(k = 0;k <= 2;k++){
		p[k] = probability(sum,c[i][j][k]);
		// printf("%d\n",c[i][j][k]);
	}

	// 0-1の乱数生成
	srand(time(NULL));
	h = rand()%151;
	r = h / 150;

	// [0,p[0]],(p[0],p[0] + p[1]],(p[0] + p[1],1]で次のユーザの手kをルーレット選択
	if(r >= 0 && r <= p[0]){
		k = 0;
	}else if(r > p[0] && r <= p[0] + p[1]){
		k = 1;
	}else if(r > p[0] + p[1] && r <= 1){
		k = 2;
	}else{
		k = 0;
	}

	// printf("p0 %f\np1 %f\np2 %f\nr %f\nk %d\n",p[0],p[1],p[2],r,k);

	// ↓ゲーム本体

	// 選択したkに勝てるcpuの手xの判定
	if(k < 2){
		x = k +1;
	}else if(k == 2){
		x = 0;
	}

	// ユーザ手入力
	printf("手を入れてください。(グーは0、パーは1、チョキは2、終了は3) ");
	scanf("%d",&y);

	if(y == 0){
		printf("あなたはグーを出しました");

	}else if(y == 1){
		printf("あなたはパーを出しました");

	}else if(y == 2){
		printf("あなたはチョキを出しました");

	}
	printf("\n");

	// printf("x %d:y %d\n",x,y);

	// 終了処理
	if(y >= 3 || y < 0){
		printf("終了します");
		return 0;
	}

	/*CP手判定*/
	if(x == 0){
		printf("コンピュータはグーを出しました");

	}else if(x == 1){
		printf("コンピュータはパーを出しました");

	}else if(x == 2){
		printf("コンピュータはチョキを出しました");

	}
	printf("\n");


	/*勝利判定*/
	z = (x - y + 3) % 3;
	if(z == 0){
		printf("あいこになりました");

	}else if(z == 1){
		printf("あなたの負けです");

	}else if(z == 2){
		printf("あなたの勝ちです");

	}
	printf("\n");

	// 結果の学習
	c[i][j][y] = c[i][j][y] + 1;

	// ↑ゲーム本体

	// 実際に出たcpuとユーザの手の保存
	fpwHand = fopen("HAND.csv","w");
	fprintf(fpwHand,"%d\n%d",x,y);
	fclose(fpwHand);

	// 更新された三次元配列の保存
	fpwA = fopen("ARRAY.csv","w");
	for(l = 0;l < 3;l++){
		for(m = 0;m < 3;m++){
			for(n = 0;n < 3;n++){
				if(l == 2 && m == 2 && n == 2){
					fprintf(fpwA,"%d",c[l][m][n]);
				}else{
					fprintf(fpwA,"%d\n",c[l][m][n]);
				}
			}
		}
	}
	fclose(fpwA);

	}
	
	return 0;
}
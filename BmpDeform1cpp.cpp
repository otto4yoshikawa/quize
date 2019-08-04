/***********************************************************************
*                                                                      *
*         ss                                                             *
*                 BmpDeform (ビットマップ画像の変換)                   *
*                                                                      *
*                                                   (v1.0 / 2019.8.1)  *
***********************************************************************/

#include	<stdio.h>
#include	<string.h>
#include	<malloc.h>
#include	<process.h>
#include <stdlib.h>
#include	<math.h>
#define		IE_MAX	4096	// 出力画像の最大横寸法
#define		JE_MAX	4096	// 出力画像の最大縦寸法
#define		IE3	  IE_MAX*3
/*
	画像の左下隅を原点 (x=0,y=0) とする
	x, i ---- よこ方向をしめす文字（０から）
	y, j ---- たて方向をしめす文字（０から）
	xlen ---- 入力画像の横寸法(４の倍数である)
	ylen ---- 入力画像の縦寸法
	IE   ---- 出力画像の横寸法(４の倍数とする)
	JE   ---- 出力画像の縦寸法
	header -- .bmp画像のヘッダー
	pict_chg[y][x]  ---- 入力画像データの本体
	pict_out[y][x] ---- 出力画像データの本体
*/
	bool options[10];
	char infile[40],outfile[40],rawfile[20];
	int IEin,x1,x2,y1,y2;	
bool parameters(int argc,char *argv[],bool *op){
for(int j=0;j<8;j++) op[j]=false;
	infile[0]=0;
int i=0;char c,*s;
while(++i<argc){

	s=argv[i];

	if(i==1){strcpy(infile,argv[i]); continue;}
		if(i==2){strcpy(outfile,argv[i]); continue;}
	if(*s=='-'){
		s++;c=*s;
		printf("command=%c\n",c);
		if(strncmp("IE=",s,3)==0){IEin=atoi(s+3);continue;}
		if(strncmp("x1=",s,3)==0){x1=atoi(s+3);continue;}
			if(strncmp("x2=",s,3)==0){x2=atoi(s+3);continue;}
			if(strncmp("y1=",s,3)==0){y1=atoi(s+3);continue;}
			if(strncmp("y2=",s,3)==0){y2=atoi(s+3);continue;}
		
			
		switch(c){
		case 'g': op[1]=true;break;
		case 'x': op[2]=true;
	 	if(i==argc) return false;
	 	i++;
	 	s[0]=atoi(argv[i]++);
	 	if(i==argc) return false;
	 	i++;
	 	s[1]=atoi(argv[i]++);break;
	 	case 't':op[3]=true;break;
	 		case 'l':op[4]=true;break;
	 		case 'u':op[5]=true;break;	
	 		case 'k':op[6]=true;break;
	 		case 's':op[7]=true;break;
		}
	}	
}

if(infile[0]==0) return false;
return true;	
}
int main( int argc,char *argv[])
{
	FILE *fp_in;
	FILE *fp_out;

	unsigned char *pict_chg[JE_MAX],*pict_out[JE_MAX],*pict_tmp[JE_MAX];
	unsigned char b,g,r,c,header[1078],colorpalet[1024],swap;
	int i,j,h,w,x,y,xlen,ylen,nlen,IE,JE,m,q;
	int k1,k2,k3,k4,count,offset,conv,turnud,turnlr,xold,yold;
	int nega,outline,patx[4][4],paty[4][4],sumx,sumy,sum;
	float wx,wy,ratio,factor=0.75;
	long fsize_in,fsize_out;
	char cmd[30],strt[7]="start ",*stra,*strb,ans;


	if(argc<2) return 0;
if(!parameters(argc,argv,options)) return 0;
	if(strcmp(".bmp",infile+strlen(infile)-4)!=0 )return 0;
	if(strcmp(".bmp",outfile+strlen(outfile)-4)!=0 )return 0;

//	sizechg=0;
//
if(0){
	printf("\n ************ BmpDeform（ビットマップ画像の変換）************\n");
	printf("\n 入力はビットマップ画像(.bmp)のみです");
	printf("\n 出力はビットマップ画像と汎用フォーマット(.raw)を選択できます\n");
	printf("\n   以下の各処理ができます");
	printf("\n 1.カラー画像のグレースケールへの変換");
	printf("\n 2.カラー画像，グレースケール画像の拡大・縮小");
	printf("\n 3.カラー画像，グレースケール画像のトリミング");
	printf("\n 4.画像を上下に反転");
	printf("\n 5.画像を左右に反転");
	printf("\n 6.グレースケール画像の明度の反転");
	printf("\n 7.グレースケール画像から輪郭抽出");
	printf("\n   以上の各処理を組み合わせて，ワンパスで実行できます\n");
	printf("\n ************************************************************\n");
}
	for(i=0;i<8;i++) if(options[i]) printf("opt %d ",i);
//
//	画像変換処理で使用する配列の宣言
top:for (y=0;y<JE_MAX;y++){pict_chg[y]=(unsigned char *) malloc((long)IE3);}

	for (y=0;y<JE_MAX;y++){pict_out[y]=(unsigned char *) malloc((long)IE3);}
	for (y=0;y<JE_MAX;y++){pict_tmp[y]=(unsigned char *) malloc((long)IE3);}
//
if(0){
	printf("\n 画像ファイルの変換処理を開始します");
s1:	printf("\n InFile.bmp  = ");
s2:	infile[0]='\0';
	scanf("%s",infile);
	if(infile[0]  == '\0' || infile[0] == ' '
	 || infile[0] == '\r' || infile[0] == '\n') goto s2;
	stra=strstr(infile,".BMP");strb=strstr(infile,".bmp");
	if((stra==NULL) && (strb==NULL)) {
		printf("     入力画像ファイルの拡張子は .bmp としてください！\a");
		goto s1;}
}
	fp_in=fopen(infile,"rb");
if(0){	
s3:	printf(" OutFile.bmp = ");
s4:	outfile[0]='\0';
	scanf("%s",outfile);
	if(outfile[0]  == '\0' || outfile[0] == ' '
	 || outfile[0] == '\r' || outfile[0] == '\n') goto s4;
	stra=strstr(outfile,".BMP");strb=strstr(outfile,".bmp");
	if((stra==NULL) && (strb==NULL)) {
		printf("     出力画像ファイルの拡張子は .bmp としてください！\a\n");
		goto s3;}
}
//	入力画像の画面表示
	cmd[0]='\0';
	strcat(cmd,strt);
	strcat(cmd,infile);
	system(cmd);
//
//	ヘッダー(先頭の54バイト)の読み込み
	for (h=0;h<54;h++) header[h]=fgetc(fp_in);
	fsize_in=header[2]+header[3]*256+header[4]*256*256;
	offset=header[10]+header[11]*256;
	xlen=header[18]+header[19]*256;
	ylen=header[22]+header[23]*256;
	x1=0;y1=0;x2=xlen-1;y2=ylen-1;
	if(header[28]==24) count=3;	// Color
	if(header[28]==8)  count=1;	// Grayscale
	
	printf("\n 入力画像ファイルの大きさは %ld バイトです",fsize_in);
	if(count==3) printf("\n 入力画像はカラーです");
	if(count==1) printf("\n 入力画像はグレースケールです");
	printf("\n 入力画像の横寸法は %d ピクセルです",xlen);
	printf("\n 入力画像の縦寸法は %d ピクセルです\n",ylen);
	if(xlen>IE_MAX || ylen>JE_MAX){
	printf("\n 入力画像が大きすぎます！\a\n");fclose(fp_in);goto end;}
//	入力画像の読み込み
	if(count==3) {	// カラー入力
		if(offset>54) for (h=55;h<=offset;h++) c=fgetc(fp_in);
		for (y=0;y<ylen;y++) {
			for (x=0;x<xlen;x++) {
				pict_tmp[y][x*3  ]=fgetc(fp_in);
				pict_tmp[y][x*3+1]=fgetc(fp_in);
				pict_tmp[y][x*3+2]=fgetc(fp_in);
			}
		}
	fclose(fp_in);
	}
	if(count==1) {	// グレー入力
		for (h=55;h<=offset;h++) c=fgetc(fp_in);
		for (y=0;y<ylen;y++) for (x=0;x<xlen;x++) pict_chg[y][x]=fgetc(fp_in);
	fclose(fp_in);
	}
	printf(" 入力画像ファイルの読み込み終了\n");
//
//変換作業の開始
	if(count==3){
		
		printf("\n カラー画像をグレースケールに変換しますか ");
		printf("\n (輪郭抽出をするときには必ず [y] にします) (y/n) = ");
		conv=options[3]?2:1;
		ans='\0';
//		scanf("%c",&ans);
	//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t1;
		conv=0;
		if(ans=='N' || ans=='n') conv=1;	// カラー画像のまま
		if(ans=='Y' || ans=='y') conv=2;	// カラーをグレースケールに変換
	}
	if(count==1) conv=0;					// グレースケールのまま
//
//	カラー画像のまま
	if(conv==1){
	for(y=0;y<ylen;y++)
		for(x=0;x<xlen;x++){
			pict_chg[y][x*3  ]=pict_tmp[y][x*3  ];
			pict_chg[y][x*3+1]=pict_tmp[y][x*3+1];
			pict_chg[y][x*3+2]=pict_tmp[y][x*3+2];}
	}
//	カラー画像のグレースケール画像への変換
	if(count==3 && conv==2){
	for(y=0;y<ylen;y++){
		for(x=0;x<xlen;x++){
			pict_chg[y][x]=(pict_tmp[y][x*3]*29+pict_tmp[y][x*3+1]*150
			+pict_tmp[y][x*3+2]*77)>>8;
		  }
	   }
	printf(" カラー画像のグレー画像への変換をしました\n");
	}

	printf(" 入力画像全体を変換処理対象としますか (y/n) = ");
t2:	ans='\0';
//	scanf("%c",&ans);
	if(options[2])
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t2;
//	if(ans=='Y' || ans=='y') goto s5;
//	if(ans=='N' || ans=='n') 
{
		
		printf(" 変換処理対象領域を指定してください\n");
		printf(" 画像の左下隅が原点 (x=0,y=0) です\n");
//r1:		printf(" 画像の変換処理対象領域の左下隅のｘ座標 x1 = "); scanf("%d",&x1);
		if(x1<0 || x1>=xlen) {printf(" 領域外です！\a\n");return 0;}
//r2:		printf(" 　　　　同　　上　　　　左下隅のｙ座標 y1 = "); scanf("%d",&y1);
		if(y1<0 || y1>=ylen) {printf(" 領域外です！\a\n");return 0;}
//r3:		printf(" 　　　　同　　上　　　　右上隅のｘ座標 x2 = "); scanf("%d",&x2);
		if(x2<0 || x2>=xlen) {printf(" 領域外です！\a\n");return 0;}
//r4:		printf(" 　　　　同　　上　　　　右上隅のｙ座標 y2 = "); scanf("%d",&y2);
		if(y2<0 || y2>=ylen) {printf(" 領域外です！\a\n");return 0;}
	}
//
s5:	printf(" 選んだ領域をズームインあるいはズームアウトしますか (y/n) = ");
//t3:	ans='\0';

//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t3;
	wx=x2-x1+1; wy=y2-y1+1; IE=wx; JE=wy;
if(options[2]){	
//	if(ans=='y' || ans=='Y') 

r5:		printf(" 出力画像の横寸法を４の倍数で指定して下さい(.bmpの仕様)");
		printf("\n 　　　　　　（縦横の比率は変わりません)　 = ");
	//	scanf("%d",&IE); 
	IE=IEin;
	
	JE=IE*wy/wx+0.5;
	
		if(JE>JE_MAX){
			m=IE*JE_MAX/JE;m=m-m%4;
		printf(" 横寸法が大きすぎます！ %d以下にして下さい\a\n",m);
		return 0;}
		if(IE>IE_MAX){
		printf(" 横寸法が大きすぎます！ %d以下にして下さい\a\n",IE_MAX);
		return 0;}
	}
//	if(ans=='N' || ans=='n') 
else 
	{
		JE=y2-y1+1;w=(1.0*JE*wx)/wy+0.5;IE=w-w%4;}
	printf(" 出力画像の寸法は %d×%d となります\n",IE,JE);

//
	printf(" 画像を上下に反転させますか (y/n) = ");
t4:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t4;
//	if(ans=='N' || ans=='n') turnud=0;	// 上下に反転させない
//	if(ans=='Y' || ans=='y') turnud=1;	// 上下に反転させる
turnud=options[5]?1:0;
	printf(" 画像を左右に反転させますか (y/n) = ");
t5:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t5;
//	if(ans=='N' || ans=='n') turnlr=0;	// 左右に反転させない
//  if(ans=='Y' || ans=='y') turnlr=1;	// 左右に反転させる
 turnlr=options[4]?1:0;
	nega=0;
	if(conv!=1) {
	printf(" グレー画像の明度を反転させますか (y/n) = ");
t6:	ans='\0';
//	scanf("%c",&ans);
nega=options[6]?1:0;
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t6;
//	if(ans=='N' || ans=='n') nega=0;	// 明度を反転させない
//	if(ans=='Y' || ans=='y') nega=1; }	// 明度を反転させる
	outline=0;
	if(conv!=1) {
	printf(" グレー画像から輪郭を抽出しますか (y/n) = ");
t7:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t7;
//	if(ans=='N' || ans=='n') outline=0;	// 輪郭を抽出しない
//	if(ans=='Y' || ans=='y') outline=1;}// 輪郭を抽出する

outline=options[7]?1:0;
//	処理対象領域の切り出し
//	(x1,y1)～(x2,y2)の領域を(0,0)～(x2-x1,y2-y1)にコピーする
	if(x1==0 && y1==0) goto s6;
	if(x1!=0 || y1!=0){
	for(y=0;y<y2-y1+1;y++){
		if(conv==1){
			for(x=0;x<x2-x1+1;x++){
				pict_chg[y][x*3  ]=pict_chg[y+y1][(x+x1)*3  ];
				pict_chg[y][x*3+1]=pict_chg[y+y1][(x+x1)*3+1];
				pict_chg[y][x*3+2]=pict_chg[y+y1][(x+x1)*3+2];
			}
		}
		if(count==1 || conv==2){
			for(x=0;x<x2-x1+1;x++){
				pict_chg[y][x]=pict_chg[y+y1][x+x1];
			}
		}
	  }
	}
s6:	printf(" 処理対象領域の取得を終了\n");
//
//	画像を上下に反転
	if(turnud==1){
	for(y=0;y<(y2-y1+1)/2;y++){
		if(conv==1){
			for(x=0;x<x2-x1+1;x++){
				swap=pict_chg[y][x*3];
				pict_chg[y][x*3]=pict_chg[y2-y1-y][x*3];
				pict_chg[y2-y1-y][x*3]=swap;
				swap=pict_chg[y][x*3+1];
				pict_chg[y][x*3+1]=pict_chg[y2-y1-y][x*3+1];
				pict_chg[y2-y1-y][x*3+1]=swap;
				swap=pict_chg[y][x*3+2];
				pict_chg[y][x*3+2]=pict_chg[y2-y1-y][x*3+2];
				pict_chg[y2-y1-y][x*3+2]=swap;
			}
		}
		if(conv==2 || count==1){
			for(x=0;x<x2-x1+1;x++){
				swap=pict_chg[y][x];
				pict_chg[y][x]=pict_chg[y2-y1-y][x];
				pict_chg[y2-y1-y][x]=swap;
			}
		}
	  }
	printf(" 画像を上下に反転しました\n");
	}
//	画像を左右に反転
	if(turnlr==1){
	for(y=0;y<y2-y1+1;y++){
		if(conv==1){
			for(x=0;x<(x2-x1+1)/2;x++){
				swap=pict_chg[y][x*3];
				pict_chg[y][x*3]=pict_chg[y][(x2-x1-x)*3];
				pict_chg[y][(x2-x1-x)*3]=swap;
				swap=pict_chg[y][x*3+1];
				pict_chg[y][x*3+1]=pict_chg[y][(x2-x1-x)*3+1];
				pict_chg[y][(x2-x1-x)*3+1]=swap;
				swap=pict_chg[y][x*3+2];
				pict_chg[y][x*3+2]=pict_chg[y][(x2-x1-x)*3+2];
				pict_chg[y][(x2-x1-x)*3+2]=swap;
			}
		}
		if(conv==2 || count==1){
			for(x=0;x<(x2-x1+1)/2;x++){
				swap=pict_chg[y][x];
				pict_chg[y][x]=pict_chg[y][x2-x1-x];
				pict_chg[y][x2-x1-x]=swap;
			}
		}
	  }
	printf(" 画像を左右に反転しました\n");
	}
//
//	画像サイズの変換処理（全ての場合ここを通る）
	ratio=(IE-1.0)/(x2-x1);			// ratio = New/Old
	if(ratio>=1.0){	// 拡大のとき
//	カラー画像の拡大変換
	if(conv==1){	// カラーのままのとき
		for(y=0;y<JE;y++){
			yold=y/ratio+0.5;
			for(x=0;x<IE;x++){
				xold=x/ratio+0.5;
				pict_out[y][x*3  ]=pict_chg[yold][xold*3  ];
				pict_out[y][x*3+1]=pict_chg[yold][xold*3+1];
				pict_out[y][x*3+2]=pict_chg[yold][xold*3+2];
			}
		}
	}
//	グレースケール画像の拡大変換
	if(conv==2 || count==1){	// カラーからグレー or グレー入力のとき
		for(y=0;y<JE;y++){
			yold=y/ratio+0.5;
			for(x=0;x<IE;x++){
				xold=x/ratio+0.5;
				pict_out[y][x]=pict_chg[yold][xold];
			}
		}
	}
	}
	if(ratio<1.0){	// 縮小のとき
//	カラー画像の縮小変換
	if(conv==1){
		for(y=0;y<JE;y++){
			yold=y/ratio+0.5;
			for(x=0;x<IE;x++){
				xold=x/ratio+0.5;
				b=pict_chg[yold][xold*3  ];
				g=pict_chg[yold][xold*3+1];
				r=pict_chg[yold][xold*3+2];
				pict_out[y][x*3  ]=b;
				pict_out[y][x*3+1]=g;
				pict_out[y][x*3+2]=r;
				if(b==0xff && g==0xff && r==0xff && yold!=0 && yold!=y2){
					q=0;
					if(pict_chg[yold-1][xold*3  ]!=0xff){q=1;
					pict_out[y][x*3  ]=pict_chg[yold-1][xold*3  ];}
					if(pict_chg[yold-1][xold*3+1]!=0xff){q=1;
					pict_out[y][x*3+1]=pict_chg[yold-1][xold*3+1];}
					if(pict_chg[yold-1][xold*3+2]!=0xff){q=1;
					pict_out[y][x*3+2]=pict_chg[yold-1][xold*3+2];}
					if(q==1) continue;
					q=0;
					if(pict_chg[yold+1][xold*3  ]!=0xff){q=1;
					pict_out[y][x*3  ]=pict_chg[yold+1][xold*3  ];}
					if(pict_chg[yold+1][xold*3+1]!=0xff){q=1;
					pict_out[y][x*3+1]=pict_chg[yold+1][xold*3+1];}
					if(pict_chg[yold+1][xold*3+2]!=0xff){q=1;
					pict_out[y][x*3+2]=pict_chg[yold+1][xold*3+2];}
					if(q==1) continue;
				}
				if(b==0xff && g==0xff && r==0xff && xold!=0 && xold!=x2){
					q=0;
					if(pict_chg[yold][(xold-1)*3  ]!=0xff){q=1;
					pict_out[y][x*3  ]=pict_chg[yold][(xold-1)*3  ];}
					if(pict_chg[yold][(xold-1)*3+1]!=0xff){q=1;
					pict_out[y][x*3+1]=pict_chg[yold][(xold-1)*3+1];}
					if(pict_chg[yold][(xold-1)*3+2]!=0xff){q=1;
					pict_out[y][x*3+2]=pict_chg[yold][(xold-1)*3+2];}
					if(q==1) continue;
					if(pict_chg[yold][(xold+1)*3  ]!=0xff)
					pict_out[y][x*3  ]=pict_chg[yold][(xold+1)*3  ];
					if(pict_chg[yold][(xold+1)*3+1]!=0xff)
					pict_out[y][x*3+1]=pict_chg[yold][(xold+1)*3+1];
					if(pict_chg[yold][(xold+1)*3+2]!=0xff)
					pict_out[y][x*3+2]=pict_chg[yold][(xold+1)*3+2];
				}
			}
		}
	}
//	グレースケール画像の縮小変換
	if(conv==2 || count==1){	// カラーからグレー or グレー入力のとき
		for(y=0;y<JE;y++){
			yold=y/ratio+0.5;
			for(x=0;x<IE;x++){
			xold=x/ratio+0.5;
		if(xold!=0 && yold!=0 && xold!=x2 && yold!=y2){sum=
								pict_chg[yold-1][xold]+
		pict_chg[ yold ][xold-1]+pict_chg[ yold ][xold]*2+pict_chg[ yold ][xold+1]+
								pict_chg[yold+1][xold];
		pict_out[y][x]=sum/6;}	// 局所平均値
		else pict_out[y][x]=pict_chg[yold][xold];}
		}
	  }
	}
	printf(" 画像サイズの変換処理をしました\n");
//	sizechg=1;
//
//	グレースケール画像の明度の反転
	if(nega==1 && (conv==2 || count==1)){
	for(y=0;y<JE;y++)
		for(x=0;x<IE;x++)
			pict_out[y][x]=255-pict_out[y][x];
	printf(" グレースケール画像の明度の反転をしました\n");
	}
//
//*** 画像の輪郭抽出（Sobel法）***
//	Sobel's routine
//	if(sizechg==0){printf(" 輪郭の抽出は最後に行って下さい！\a\n");goto out;}
	if(outline==1 && (count==1 || conv==2)){
	for(y=0;y<JE;y++)
		for(x=0;x<IE;x++) pict_tmp[y][x]=0xff;
	patx[1][1]=-1; patx[1][2]= 0; patx[1][3]= 1;	//Pattern for f'(x)
	patx[2][1]=-2; patx[2][2]= 0; patx[2][3]= 2;
	patx[3][1]=-1; patx[3][2]= 0; patx[3][3]= 1;
	paty[1][1]=-1; paty[1][2]=-2; paty[1][3]=-1;	//Pattern for f'(y)
	paty[2][1]= 0; paty[2][2]= 0; paty[2][3]= 0;
	paty[3][1]= 1; paty[3][2]= 2; paty[3][3]= 1;
	for (y=1;y<=JE-2;y++) {
		for (x=1;x<=IE-2;x++) {
			sumx=0;
			sumy=0;
			for (i=1;i<=3;i++) {
				for (j=1;j<=3;j++) {
					sumx=sumx+pict_out[y-2+j][x-2+i]*patx[j][i];
					sumy=sumy+pict_out[y-2+j][x-2+i]*paty[j][i];
				}
			}
			sum=factor*(abs(sumx)+abs(sumy));
			if (sum>255) sum=255;
			pict_tmp[y][x]=255-sum;
		}
	  }
	for (y=0;y<JE;y++)
		for (x=0;x<IE;x++) pict_out[y][x]=pict_tmp[y][x];
	printf(" 輪郭を抽出しました\n");
	}
//
//out:
	printf(" 画像データを出力ファイルに書き込みます\n");
//
//	出力用ファイルヘッダーの作成
	for(i=0;i<1078;i++) header[i]=0x00;
	header[0]=0x42;		// "B"
	header[1]=0x4D;		// "M"
	if(conv==1) fsize_out=54+IE*JE*3+2;	// カラー
	if(conv==2 || count==1) fsize_out=1078+IE*JE+2;// グレー
/*	
	k1=(fsize_out%256);
	k2=(fsize_out/256)%256;
	k3=(fsize_out/65536)%256;
	k4=(fsize_out/16777216)%256;
	header[2]=k1;
	header[3]=k2;
	header[4]=k3;
	header[5]=k4;
*/
*((int*)(header+2))=fsize_out;	
	header[10]=0x36;	// =54
	if(conv!=1)			// グレーならば
 	header[11]=0x04;	// =1024
	header[14]=0x28;	// =40 固定
/*	
	k1=(IE%256);	// 横寸法
	k2=(IE/256)%256;
	k3=(IE/65536)%256;
	k4=(IE/16777216)%256;
	header[18]=k1;
	header[19]=k2;
	header[20]=k3;
	header[21]=k4;
	k1=(JE%256);	// 縦寸法
	k2=(JE/256)%256;
	k3=(JE/65536)%256;
	k4=(JE/16777216)%256;
	header[22]=k1;
	header[23]=k2;
	header[24]=k3;
	header[25]=k4;
*/
 *((int*)(header+18))=IE;
 *((int*)(header+22))=JE;	
	header[26]=0x01;	// 固定
	if(conv==1) header[28]=0x18; // カラー 24 bit/pixel
	if(conv!=1) header[28]=0x08; // グレー  8 bit/pixel
	fsize_out=IE*JE+2;
	/*
	k1=(fsize_out%256);
	k2=(fsize_out/256)%256;
	k3=(fsize_out/65536)%256;
	k4=(fsize_out/16777216)%256;
	
	header[34]=k1;
	header[35]=k2;
	header[36]=k3;
	header[37]=k4;
	*/
 *((int*)(header+34))=fsize_out;
 
 
	header[38]=0x12;	// 横方向解像度 72 dpi
	header[39]=0x0b;
	header[42]=0x12;	// 縦方向解像度 72 dpi
	header[43]=0x0b;
//	Color palette
	if(conv!=1){	// グレーならば
	for(i=0;i<256;i++) {
		colorpalet[4*i  ]=i;
		colorpalet[4*i+1]=i;
		colorpalet[4*i+2]=i;
		colorpalet[4*i+3]=0;}
		for(i=0;i<1024;i++)
			header[54+i]=colorpalet[i];}
//	ヘッダーの終わり
//
//	出力用ファイルを開く
	fp_out=fopen(outfile,"wb");
//
//	出力ファイルへの書き込み
	if(conv==1) {
	for(i=0;i<54;i++){c=header[i];fwrite(&c,1,1,fp_out);}
	for(y=0;y<JE;y++)
		for(x=0;x<IE;x++) {
			c=pict_out[y][x*3  ];fwrite(&c,1,1,fp_out);
			c=pict_out[y][x*3+1];fwrite(&c,1,1,fp_out);
			c=pict_out[y][x*3+2];fwrite(&c,1,1,fp_out);
		}
	}
	if(conv!=1) {
	for(i=0;i<1078;i++) {c=header[i];fwrite(&c,1,1,fp_out);}
	for(y=0;y<JE;y++) {
		for(x=0;x<IE;x++) {c=pict_out[y][x];fwrite(&c,1,1,fp_out);}
	  }
	}
	putc('\0',fp_out);putc('\0',fp_out);
//
//	書き込み用ファイルを閉じる
	fclose(fp_out);
//
//	出力画像の画面表示
	cmd[0]='\0';
	strcat(cmd,strt);
	strcat(cmd,outfile);
	system(cmd);
	printf(" 作成されたビットマップファイル(.bmp)を出力しました\n");
//
//	Make .raw image file
	printf(" 汎用フォーマット(.raw)ファイルを作成しますか (y/n) = ");
t8:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t8;
//	if(ans=='N' || ans=='n') goto s7;
//	if(ans=='Y' || ans=='y')
if(options[8])	
	 {
	nlen=strlen(outfile);
	nlen=nlen-4;
	for(i=0;i<nlen;i++) rawfile[i]=outfile[i];
	rawfile[nlen]='\0';
	strcat(rawfile,".raw");
	fp_out=fopen(rawfile,"w");
  if(conv == 1){
	for(y=0;y<JE;y++) {
		for(x=0;x<IE*3;x=x+3) {
			for(i=0;i<3;i++) {c=pict_out[JE-y-1][x-i+2];fwrite(&c,1,1,fp_out);}
			}
	   }
	printf(" 横寸法%d, 縦寸法%d, 3 byte/pixel の.raw 画像となります\n",IE,JE);
	}
  if(conv != 1){
	for (y=0;y<JE;y++){
		for (x=0;x<IE;x++){
			fputc(pict_out[JE-y-1][x],fp_out);
		}
	}
	printf(" 横寸法%d, 縦寸法%d, 1 byte/pixel の.raw 画像となります\n",IE,JE);
	}
  }
	printf(" 汎用フォーマット(.raw)ファイルを作成しました\n");
end:printf(" 変換処理を終了しました\n");
}}
return 1;
}
/*




　　　　　　　　　　　　　　　　作成者：楠城　力 (Nanjo Tsutomu)
　　　　　　　　　　　　　　　　作成日：2019.8.1
　　　　　　　　　　　　　　　　著作権は上記の作成者にあります。
*/

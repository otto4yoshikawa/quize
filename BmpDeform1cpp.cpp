/***********************************************************************
*                                                                      *
*         ss                                                             *
*                 BmpDeform (�r�b�g�}�b�v�摜�̕ϊ�)                   *
*                                                                      *
*                                                   (v1.0 / 2019.8.1)  *
***********************************************************************/

#include	<stdio.h>
#include	<string.h>
#include	<malloc.h>
#include	<process.h>
#include <stdlib.h>
#include	<math.h>
#define		IE_MAX	4096	// �o�͉摜�̍ő剡���@
#define		JE_MAX	4096	// �o�͉摜�̍ő�c���@
#define		IE3	  IE_MAX*3
/*
	�摜�̍����������_ (x=0,y=0) �Ƃ���
	x, i ---- �悱���������߂������i�O����j
	y, j ---- ���ĕ��������߂������i�O����j
	xlen ---- ���͉摜�̉����@(�S�̔{���ł���)
	ylen ---- ���͉摜�̏c���@
	IE   ---- �o�͉摜�̉����@(�S�̔{���Ƃ���)
	JE   ---- �o�͉摜�̏c���@
	header -- .bmp�摜�̃w�b�_�[
	pict_chg[y][x]  ---- ���͉摜�f�[�^�̖{��
	pict_out[y][x] ---- �o�͉摜�f�[�^�̖{��
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
	printf("\n ************ BmpDeform�i�r�b�g�}�b�v�摜�̕ϊ��j************\n");
	printf("\n ���͂̓r�b�g�}�b�v�摜(.bmp)�݂̂ł�");
	printf("\n �o�͂̓r�b�g�}�b�v�摜�Ɣėp�t�H�[�}�b�g(.raw)��I���ł��܂�\n");
	printf("\n   �ȉ��̊e�������ł��܂�");
	printf("\n 1.�J���[�摜�̃O���[�X�P�[���ւ̕ϊ�");
	printf("\n 2.�J���[�摜�C�O���[�X�P�[���摜�̊g��E�k��");
	printf("\n 3.�J���[�摜�C�O���[�X�P�[���摜�̃g���~���O");
	printf("\n 4.�摜���㉺�ɔ��]");
	printf("\n 5.�摜�����E�ɔ��]");
	printf("\n 6.�O���[�X�P�[���摜�̖��x�̔��]");
	printf("\n 7.�O���[�X�P�[���摜����֊s���o");
	printf("\n   �ȏ�̊e������g�ݍ��킹�āC�����p�X�Ŏ��s�ł��܂�\n");
	printf("\n ************************************************************\n");
}
	for(i=0;i<8;i++) if(options[i]) printf("opt %d ",i);
//
//	�摜�ϊ������Ŏg�p����z��̐錾
top:for (y=0;y<JE_MAX;y++){pict_chg[y]=(unsigned char *) malloc((long)IE3);}

	for (y=0;y<JE_MAX;y++){pict_out[y]=(unsigned char *) malloc((long)IE3);}
	for (y=0;y<JE_MAX;y++){pict_tmp[y]=(unsigned char *) malloc((long)IE3);}
//
if(0){
	printf("\n �摜�t�@�C���̕ϊ��������J�n���܂�");
s1:	printf("\n InFile.bmp  = ");
s2:	infile[0]='\0';
	scanf("%s",infile);
	if(infile[0]  == '\0' || infile[0] == ' '
	 || infile[0] == '\r' || infile[0] == '\n') goto s2;
	stra=strstr(infile,".BMP");strb=strstr(infile,".bmp");
	if((stra==NULL) && (strb==NULL)) {
		printf("     ���͉摜�t�@�C���̊g���q�� .bmp �Ƃ��Ă��������I\a");
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
		printf("     �o�͉摜�t�@�C���̊g���q�� .bmp �Ƃ��Ă��������I\a\n");
		goto s3;}
}
//	���͉摜�̉�ʕ\��
	cmd[0]='\0';
	strcat(cmd,strt);
	strcat(cmd,infile);
	system(cmd);
//
//	�w�b�_�[(�擪��54�o�C�g)�̓ǂݍ���
	for (h=0;h<54;h++) header[h]=fgetc(fp_in);
	fsize_in=header[2]+header[3]*256+header[4]*256*256;
	offset=header[10]+header[11]*256;
	xlen=header[18]+header[19]*256;
	ylen=header[22]+header[23]*256;
	x1=0;y1=0;x2=xlen-1;y2=ylen-1;
	if(header[28]==24) count=3;	// Color
	if(header[28]==8)  count=1;	// Grayscale
	
	printf("\n ���͉摜�t�@�C���̑傫���� %ld �o�C�g�ł�",fsize_in);
	if(count==3) printf("\n ���͉摜�̓J���[�ł�");
	if(count==1) printf("\n ���͉摜�̓O���[�X�P�[���ł�");
	printf("\n ���͉摜�̉����@�� %d �s�N�Z���ł�",xlen);
	printf("\n ���͉摜�̏c���@�� %d �s�N�Z���ł�\n",ylen);
	if(xlen>IE_MAX || ylen>JE_MAX){
	printf("\n ���͉摜���傫�����܂��I\a\n");fclose(fp_in);goto end;}
//	���͉摜�̓ǂݍ���
	if(count==3) {	// �J���[����
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
	if(count==1) {	// �O���[����
		for (h=55;h<=offset;h++) c=fgetc(fp_in);
		for (y=0;y<ylen;y++) for (x=0;x<xlen;x++) pict_chg[y][x]=fgetc(fp_in);
	fclose(fp_in);
	}
	printf(" ���͉摜�t�@�C���̓ǂݍ��ݏI��\n");
//
//�ϊ���Ƃ̊J�n
	if(count==3){
		
		printf("\n �J���[�摜���O���[�X�P�[���ɕϊ����܂��� ");
		printf("\n (�֊s���o������Ƃ��ɂ͕K�� [y] �ɂ��܂�) (y/n) = ");
		conv=options[3]?2:1;
		ans='\0';
//		scanf("%c",&ans);
	//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t1;
		conv=0;
		if(ans=='N' || ans=='n') conv=1;	// �J���[�摜�̂܂�
		if(ans=='Y' || ans=='y') conv=2;	// �J���[���O���[�X�P�[���ɕϊ�
	}
	if(count==1) conv=0;					// �O���[�X�P�[���̂܂�
//
//	�J���[�摜�̂܂�
	if(conv==1){
	for(y=0;y<ylen;y++)
		for(x=0;x<xlen;x++){
			pict_chg[y][x*3  ]=pict_tmp[y][x*3  ];
			pict_chg[y][x*3+1]=pict_tmp[y][x*3+1];
			pict_chg[y][x*3+2]=pict_tmp[y][x*3+2];}
	}
//	�J���[�摜�̃O���[�X�P�[���摜�ւ̕ϊ�
	if(count==3 && conv==2){
	for(y=0;y<ylen;y++){
		for(x=0;x<xlen;x++){
			pict_chg[y][x]=(pict_tmp[y][x*3]*29+pict_tmp[y][x*3+1]*150
			+pict_tmp[y][x*3+2]*77)>>8;
		  }
	   }
	printf(" �J���[�摜�̃O���[�摜�ւ̕ϊ������܂���\n");
	}

	printf(" ���͉摜�S�̂�ϊ������ΏۂƂ��܂��� (y/n) = ");
t2:	ans='\0';
//	scanf("%c",&ans);
	if(options[2])
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t2;
//	if(ans=='Y' || ans=='y') goto s5;
//	if(ans=='N' || ans=='n') 
{
		
		printf(" �ϊ������Ώۗ̈���w�肵�Ă�������\n");
		printf(" �摜�̍����������_ (x=0,y=0) �ł�\n");
//r1:		printf(" �摜�̕ϊ������Ώۗ̈�̍������̂����W x1 = "); scanf("%d",&x1);
		if(x1<0 || x1>=xlen) {printf(" �̈�O�ł��I\a\n");return 0;}
//r2:		printf(" �@�@�@�@���@�@��@�@�@�@�������̂����W y1 = "); scanf("%d",&y1);
		if(y1<0 || y1>=ylen) {printf(" �̈�O�ł��I\a\n");return 0;}
//r3:		printf(" �@�@�@�@���@�@��@�@�@�@�E����̂����W x2 = "); scanf("%d",&x2);
		if(x2<0 || x2>=xlen) {printf(" �̈�O�ł��I\a\n");return 0;}
//r4:		printf(" �@�@�@�@���@�@��@�@�@�@�E����̂����W y2 = "); scanf("%d",&y2);
		if(y2<0 || y2>=ylen) {printf(" �̈�O�ł��I\a\n");return 0;}
	}
//
s5:	printf(" �I�񂾗̈���Y�[���C�����邢�̓Y�[���A�E�g���܂��� (y/n) = ");
//t3:	ans='\0';

//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t3;
	wx=x2-x1+1; wy=y2-y1+1; IE=wx; JE=wy;
if(options[2]){	
//	if(ans=='y' || ans=='Y') 

r5:		printf(" �o�͉摜�̉����@���S�̔{���Ŏw�肵�ĉ�����(.bmp�̎d�l)");
		printf("\n �@�@�@�@�@�@�i�c���̔䗦�͕ς��܂���)�@ = ");
	//	scanf("%d",&IE); 
	IE=IEin;
	
	JE=IE*wy/wx+0.5;
	
		if(JE>JE_MAX){
			m=IE*JE_MAX/JE;m=m-m%4;
		printf(" �����@���傫�����܂��I %d�ȉ��ɂ��ĉ�����\a\n",m);
		return 0;}
		if(IE>IE_MAX){
		printf(" �����@���傫�����܂��I %d�ȉ��ɂ��ĉ�����\a\n",IE_MAX);
		return 0;}
	}
//	if(ans=='N' || ans=='n') 
else 
	{
		JE=y2-y1+1;w=(1.0*JE*wx)/wy+0.5;IE=w-w%4;}
	printf(" �o�͉摜�̐��@�� %d�~%d �ƂȂ�܂�\n",IE,JE);

//
	printf(" �摜���㉺�ɔ��]�����܂��� (y/n) = ");
t4:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t4;
//	if(ans=='N' || ans=='n') turnud=0;	// �㉺�ɔ��]�����Ȃ�
//	if(ans=='Y' || ans=='y') turnud=1;	// �㉺�ɔ��]������
turnud=options[5]?1:0;
	printf(" �摜�����E�ɔ��]�����܂��� (y/n) = ");
t5:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t5;
//	if(ans=='N' || ans=='n') turnlr=0;	// ���E�ɔ��]�����Ȃ�
//  if(ans=='Y' || ans=='y') turnlr=1;	// ���E�ɔ��]������
 turnlr=options[4]?1:0;
	nega=0;
	if(conv!=1) {
	printf(" �O���[�摜�̖��x�𔽓]�����܂��� (y/n) = ");
t6:	ans='\0';
//	scanf("%c",&ans);
nega=options[6]?1:0;
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t6;
//	if(ans=='N' || ans=='n') nega=0;	// ���x�𔽓]�����Ȃ�
//	if(ans=='Y' || ans=='y') nega=1; }	// ���x�𔽓]������
	outline=0;
	if(conv!=1) {
	printf(" �O���[�摜����֊s�𒊏o���܂��� (y/n) = ");
t7:	ans='\0';
//	scanf("%c",&ans);
//	if(ans !='Y' && ans !='y' && ans !='N' && ans !='n') goto t7;
//	if(ans=='N' || ans=='n') outline=0;	// �֊s�𒊏o���Ȃ�
//	if(ans=='Y' || ans=='y') outline=1;}// �֊s�𒊏o����

outline=options[7]?1:0;
//	�����Ώۗ̈�̐؂�o��
//	(x1,y1)�`(x2,y2)�̗̈��(0,0)�`(x2-x1,y2-y1)�ɃR�s�[����
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
s6:	printf(" �����Ώۗ̈�̎擾���I��\n");
//
//	�摜���㉺�ɔ��]
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
	printf(" �摜���㉺�ɔ��]���܂���\n");
	}
//	�摜�����E�ɔ��]
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
	printf(" �摜�����E�ɔ��]���܂���\n");
	}
//
//	�摜�T�C�Y�̕ϊ������i�S�Ă̏ꍇ������ʂ�j
	ratio=(IE-1.0)/(x2-x1);			// ratio = New/Old
	if(ratio>=1.0){	// �g��̂Ƃ�
//	�J���[�摜�̊g��ϊ�
	if(conv==1){	// �J���[�̂܂܂̂Ƃ�
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
//	�O���[�X�P�[���摜�̊g��ϊ�
	if(conv==2 || count==1){	// �J���[����O���[ or �O���[���͂̂Ƃ�
		for(y=0;y<JE;y++){
			yold=y/ratio+0.5;
			for(x=0;x<IE;x++){
				xold=x/ratio+0.5;
				pict_out[y][x]=pict_chg[yold][xold];
			}
		}
	}
	}
	if(ratio<1.0){	// �k���̂Ƃ�
//	�J���[�摜�̏k���ϊ�
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
//	�O���[�X�P�[���摜�̏k���ϊ�
	if(conv==2 || count==1){	// �J���[����O���[ or �O���[���͂̂Ƃ�
		for(y=0;y<JE;y++){
			yold=y/ratio+0.5;
			for(x=0;x<IE;x++){
			xold=x/ratio+0.5;
		if(xold!=0 && yold!=0 && xold!=x2 && yold!=y2){sum=
								pict_chg[yold-1][xold]+
		pict_chg[ yold ][xold-1]+pict_chg[ yold ][xold]*2+pict_chg[ yold ][xold+1]+
								pict_chg[yold+1][xold];
		pict_out[y][x]=sum/6;}	// �Ǐ����ϒl
		else pict_out[y][x]=pict_chg[yold][xold];}
		}
	  }
	}
	printf(" �摜�T�C�Y�̕ϊ����������܂���\n");
//	sizechg=1;
//
//	�O���[�X�P�[���摜�̖��x�̔��]
	if(nega==1 && (conv==2 || count==1)){
	for(y=0;y<JE;y++)
		for(x=0;x<IE;x++)
			pict_out[y][x]=255-pict_out[y][x];
	printf(" �O���[�X�P�[���摜�̖��x�̔��]�����܂���\n");
	}
//
//*** �摜�̗֊s���o�iSobel�@�j***
//	Sobel's routine
//	if(sizechg==0){printf(" �֊s�̒��o�͍Ō�ɍs���ĉ������I\a\n");goto out;}
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
	printf(" �֊s�𒊏o���܂���\n");
	}
//
//out:
	printf(" �摜�f�[�^���o�̓t�@�C���ɏ������݂܂�\n");
//
//	�o�͗p�t�@�C���w�b�_�[�̍쐬
	for(i=0;i<1078;i++) header[i]=0x00;
	header[0]=0x42;		// "B"
	header[1]=0x4D;		// "M"
	if(conv==1) fsize_out=54+IE*JE*3+2;	// �J���[
	if(conv==2 || count==1) fsize_out=1078+IE*JE+2;// �O���[
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
	if(conv!=1)			// �O���[�Ȃ��
 	header[11]=0x04;	// =1024
	header[14]=0x28;	// =40 �Œ�
/*	
	k1=(IE%256);	// �����@
	k2=(IE/256)%256;
	k3=(IE/65536)%256;
	k4=(IE/16777216)%256;
	header[18]=k1;
	header[19]=k2;
	header[20]=k3;
	header[21]=k4;
	k1=(JE%256);	// �c���@
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
	header[26]=0x01;	// �Œ�
	if(conv==1) header[28]=0x18; // �J���[ 24 bit/pixel
	if(conv!=1) header[28]=0x08; // �O���[  8 bit/pixel
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
 
 
	header[38]=0x12;	// �������𑜓x 72 dpi
	header[39]=0x0b;
	header[42]=0x12;	// �c�����𑜓x 72 dpi
	header[43]=0x0b;
//	Color palette
	if(conv!=1){	// �O���[�Ȃ��
	for(i=0;i<256;i++) {
		colorpalet[4*i  ]=i;
		colorpalet[4*i+1]=i;
		colorpalet[4*i+2]=i;
		colorpalet[4*i+3]=0;}
		for(i=0;i<1024;i++)
			header[54+i]=colorpalet[i];}
//	�w�b�_�[�̏I���
//
//	�o�͗p�t�@�C�����J��
	fp_out=fopen(outfile,"wb");
//
//	�o�̓t�@�C���ւ̏�������
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
//	�������ݗp�t�@�C�������
	fclose(fp_out);
//
//	�o�͉摜�̉�ʕ\��
	cmd[0]='\0';
	strcat(cmd,strt);
	strcat(cmd,outfile);
	system(cmd);
	printf(" �쐬���ꂽ�r�b�g�}�b�v�t�@�C��(.bmp)���o�͂��܂���\n");
//
//	Make .raw image file
	printf(" �ėp�t�H�[�}�b�g(.raw)�t�@�C�����쐬���܂��� (y/n) = ");
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
	printf(" �����@%d, �c���@%d, 3 byte/pixel ��.raw �摜�ƂȂ�܂�\n",IE,JE);
	}
  if(conv != 1){
	for (y=0;y<JE;y++){
		for (x=0;x<IE;x++){
			fputc(pict_out[JE-y-1][x],fp_out);
		}
	}
	printf(" �����@%d, �c���@%d, 1 byte/pixel ��.raw �摜�ƂȂ�܂�\n",IE,JE);
	}
  }
	printf(" �ėp�t�H�[�}�b�g(.raw)�t�@�C�����쐬���܂���\n");
end:printf(" �ϊ��������I�����܂���\n");
}}
return 1;
}
/*




�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�쐬�ҁF���@�� (Nanjo Tsutomu)
�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�쐬���F2019.8.1
�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���쌠�͏�L�̍쐬�҂ɂ���܂��B
*/

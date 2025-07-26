#include <stdio.h>
#include <stdlib.h>

long long period(int m, int n) {
	int dir = 0;
	long long *map = calloc(n, sizeof(long long));
	int x = 0;
	int y = 0;
	int dirx[] = {0,1,0,m-1};
	int diry[] = {n-1,0,1,0};
	long long totalit = 0;
	long long mapzero = 0;
	long long saveit = 0;
	char* filename;
	asprintf(&filename, "%d-%d.sav",m,n);
	FILE *backup = fopen(filename,"r");
	if(backup) {
		fscanf(backup, "%d %d %d %lld %d\n", &x, &y, &dir, &totalit, &mapzero);
		for(int i = 0; i < n; i++) fscanf(backup, "%lld ", map+i);
		fclose(backup);
	}
	while(1) {
		do {
			int state = (map[y] >> x)&1;
			if(state) dir = (dir+1)&3;
			else dir = (dir-1)&3;
			map[y] ^= (1L<<x);
			mapzero = (mapzero & ~(1L << y)) | (((long long)(map[y] != 0)) << y);
			x += dirx[dir];
			if(x >= m) x-=m;
			y += diry[dir];
			if(y >= n) y-=n;
			saveit++;
		} while(saveit < 10000000000L && (mapzero != 0));
		totalit += saveit;
		saveit = 0;

		FILE *file = fopen(filename,"w+");
		fprintf(file, "%d %d %d %lld %d\n", x, y, dir, totalit, mapzero);
		for(int i = 0; i < n; i++) fprintf(file, "%lld ", map[i]);
		fprintf(file,"\n");
		fclose(file);
		if(totalit % 1000000000000L == 0) {
			char filecheckpoint[200];
			sprintf(filecheckpoint, "cp %s %s.%d",filename,filename,totalit/1000000000000L);
			system(filecheckpoint);
		}
		printf("%lld iterations\r",totalit);
		fflush(stdout);
		if(mapzero == 0) break;
	}
	printf("Map %dx%d, position: %d, %d, %d, period: %lld\n",m,n,x,y,dir,totalit);
	return totalit;
}

int main(int argc, char* argv[]) {
	if(argc == 3) period(atoi(argv[1]),atoi(argv[2]));
	else if(argc == 1)
	for(int m = 1; m <= 64; m++) {
		for(int n = 1; n <= 64; n++) {
			long long it = period(m,n);
			if(it > 1000000000000L) break;
		}
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

int order(int k, int n) {
	int o = 0;
	int r = 0;
	do {
		r += k;
		if(r >= n) r-=n;
		o++;
	} while(r != 0);
	return o;
}

long long period(int m, int n) {
	long long *map = calloc(n, sizeof(long long));
	int x = 0;
	int y = 0;
	int dir = 0;
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
			if(!(map[y]&(1L<<x))) dir ^= 1;
			map[y] ^= (1L<<x);
			x += dir ? m-1:1;
			if(x >= m) x-=m;
			if(map[y]&(1L<<x)) dir ^= 1;
			map[y] ^= (1L<<x);
			mapzero = (mapzero & ~(1L << y)) | (((long long)(map[y] != 0)) << y);
			y += dir ? 1:n-1;
			if(y >= n) y-=n;
			saveit+=2;
		} while(mapzero && saveit < 10000000000L);
		totalit += saveit;
		saveit = 0;

		FILE *file = fopen(filename,"w+");
		fprintf(file, "%d %d %d %lld %d\n", x, y, dir, totalit, mapzero);
		for(int i = 0; i < n; i++) fprintf(file, "%lld ", map[i]);
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
	printf("Map %dx%d, position: %d, %d, %d, period: %lld, real period: %lld\n",m,n,x,y,dir,totalit,totalit*order(y,n));
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

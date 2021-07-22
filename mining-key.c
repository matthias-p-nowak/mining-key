#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <limits.h>

char alphabet[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char **argv){

  time_t t0,t,tn;
  struct tm *tbuf;
  char tstr[32];
  unsigned char keybuf[256];
  int opt;
  int bits=16;
  int i,s;
  unsigned long trial;
  int nounceStart;
  int nounceVal[32];
  unsigned char md[SHA256_DIGEST_LENGTH];
  int num_keys=INT_MAX;
  int tdiff=0;
  
  t0=time(NULL);
  tbuf=localtime(&t0); // tbuf points to static data
  strftime(tstr,sizeof(tstr),"%H:%M:%S",tbuf);
  printf("%s: starting the program\n",tstr);

  while((opt=getopt(argc,argv,"b:n:t:")) != -1){
    switch(opt){
      case 'b':
        bits=atoi(optarg);
        printf("new required zero bits: %d\n",bits);
        break;
      case 'n':
        num_keys=atoi(optarg);
        break;
      case 't':
        tdiff=atoi(optarg);
        break;
      case 'h':
      default:
        printf("use %s [<options>] <key string> \n"
        " options:\n"
        "  -b <bits>   -- number of required leading zero bits\n"
        "  -n <num>    -- limit the run to get <num> keys\n"
        "  -t <secs>   -- strive to give 1 key per <secs> even with less leading zero bits\n"
        " <key string> -- a string with no spaces including the leading secret\n"
        ,argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }
  if(bits>60)
    bits=60;
  unsigned long threshold=0xffffffffffffffff;
  unsigned long threshold2=0;
  threshold >>=bits;
  if(tdiff>0)
    tn=t0+tdiff;
  if(optind < argc){
    printf("mining for %s\n",argv[optind]);
    memset(keybuf,0,sizeof(keybuf));
    for(i=0;i<sizeof(keybuf) && (argv[optind][i]);++i){
      keybuf[i]=argv[optind][i];
    }
    if(keybuf[i-1]!='-'){
      keybuf[i++]='-';
    }
    nounceStart=i;
    unsigned int printed=0;
    unsigned long bestHash=0xffffffffffffffff;
    for(trial=0;;++trial){
      // creating the next nounce
      for(i=0;i<sizeof(nounceVal);++i){
        s=(nounceVal[i]+1)%sizeof(alphabet);
        nounceVal[i]=s;
        keybuf[nounceStart+i]=alphabet[s];
        if(s)
          break;
      }
      SHA256(keybuf,strlen(keybuf),md);
      unsigned long hash=0;
      for(i=0;i<8;++i){
        hash <<=8;
        hash |= md[i];
      }
      if(hash<bestHash)
        bestHash=hash;
      t=time(NULL);
      if((tdiff>0) && (t>tn)){
        tn+=tdiff;
        if(bestHash>threshold2)
          threshold2=bestHash;
        bestHash=0xffffffffffffffff;
      }
      if((hash<=threshold) || (hash <= threshold2)){
        unsigned long c;
        for(c=0x8000000000000000,i=0;hash<c;++i)
          c>>=1;
        printf("%2d %s --> %016lx\n",i,keybuf,hash);
        fflush(NULL);
        if(++printed >= num_keys)
          break;
      }
      t=time(NULL);
    }
  }

  
  t=time(NULL);
  tbuf=localtime(&t); // tbuf points to static data
  strftime(tstr,sizeof(tstr),"%H:%M:%S",tbuf);  
  printf("%s: program ended, trials: %ld\n",tstr,trial);
  exit(EXIT_SUCCESS);
}

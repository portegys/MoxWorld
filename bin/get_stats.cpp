#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i,j,k;
    char buf[512],buf2[50];
    FILE *fp;
    double d;
    double fbest[30];
    double fave[30];
    double pbest[30];
    double pave[30];
    bool foragers_only;
    int numworlds;

    if (argc != 2)
    {
      fprintf(stderr, "Usage %s <evolve output file prefix string, e.g. evolve_foragers_only_worlds1\n", argv[0]);
      return 1;
    }
    char *prefix = argv[1];

    if (strncmp(prefix, "evolve_foragers_only", 20) == 0)
    {
      foragers_only = true;
    } else if (strncmp(prefix, "evolve_foragers_and_predators", 29) == 0)
    {
      foragers_only = false;
    } else {
      fprintf(stderr, "Invalid prefix\n");
      return 1;
    }
    numworlds = atoi(&prefix[strlen(prefix) - 1]);

    for (i = 0; i < 10; i++)
    {
        sprintf(buf, "./evolve.sh -statistics -input ../work/%s_%d.out > _tmp_stats%d.txt", prefix, i, i);
        system(buf);
    }

    for (i = 0; i < 30; i++)
    {
      fbest[i] = fave[i] = 0.0;
      pbest[i] = pave[i] = 0.0;
    }

    for (i = 0; i < 10; i++)
    {
        sprintf(buf, "grep ^[0-9] _tmp_stats%d.txt", i);
        fp = popen(buf, "r");
        if (foragers_only)
        {
          for (j = 0; j < 60; j++)
          {
             fscanf(fp, "%d %s", &k, buf2);
             d = atof(buf2);
             if (j < 30)
             {
               fbest[j] += d;
             } else {
               fave[j - 30] += d;
             }
          }
        } else {
          for (j = 0; j < 120; j++)
          {
             fscanf(fp, "%d %s", &k, buf2);
             d = atof(buf2);
             if (j < 30)
             {
               fbest[j] += d;
             } else if (j < 60)
             {
               fave[j - 30] += d;
             } else if (j < 90)
             {
               pbest[j - 60] += d;
             } else {
               pave[j - 90] += d;
             }
          }
        }
        pclose(fp);
        sprintf(buf, "/bin/rm -f _tmp_stats%d.txt", i);
        system(buf);
    }

    printf("forager best:\n");
    for (i = 0; i < 30; i++)
    {
      fbest[i] /= (10.0 * (double)numworlds);
      printf("%f\n",fbest[i]);
    }

    printf("forager average:\n");
    for (i = 0; i < 30; i++)
    {
      fave[i] /= (10.0 * (double)numworlds);
      printf("%f\n",fave[i]);
    }
    
    if (!foragers_only)
    {
      printf("predator best:\n");
      for (i = 0; i < 30; i++)
      {
        pbest[i] /= (10.0 * (double)numworlds);
        printf("%f\n",pbest[i]);
      }
  
      printf("predator average:\n");
      for (i = 0; i < 30; i++)
      {
        pave[i] /= (10.0 * (double)numworlds);
        printf("%f\n",pave[i]);
      }
    }

    return 0;
}

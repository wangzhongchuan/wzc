#include <stdio.h>
#include <math.h>
#define N 101
void main()
{
int i,j,line,a[N];
for(i=2;i<N;i++) a=i;
for(i=2;i<sqrt(N);i++)
for(j=i+1;j<N;j++)
{
if(a!=0&&a[j]!=0)
if(a[j]%a==0)
a[j]=0;}
printf(¡°\n¡±);
for(i=2,line=0;i<N;i++)
{
if(a!=0)
{printf(¡°%5d¡±,a);
line++;}
if(line==10)
{printf(¡°\n¡±);
line=0;}
}
}

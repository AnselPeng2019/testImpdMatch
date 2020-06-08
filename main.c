#include "stdio.h"
#include "stdint.h"

uint16_t Impedance_Match(void);

int main(void)
{
  Impedance_Match();
  return 1;
}

void printArray(int *pdata, int size)
{
  for(int i=0;i<size;i++){
    printf(" %4d", pdata[i]);
  }
  printf("\n");
}

uint16_t Impedance_Match()
{
  uint32_t temp = 0;
	uint32_t rev_data[6] = {0  , 10,  1000, 700,  800,  600};
	uint32_t fwd_data[6] = {800, 1100, 1500, 1300, 1000, 750};
	uint8_t  bestID = 0;

  printf("测试数据: \n");
  printf("Rev data: ");
  printArray(rev_data,sizeof(rev_data)/sizeof(uint32_t));
  printf("Fwd data: ");
  printArray(fwd_data,sizeof(fwd_data)/sizeof(uint32_t));

	uint32_t diff_data[6] = {0};

	uint8_t i = 0;

	for(;i<6;i++){

		diff_data[i] = (fwd_data[i]>rev_data[i])?(fwd_data[i]-rev_data[i]):(0);
//		if(temp_data[i] > 2000)
//				temp_data[i] = 0;
	}
//	SetRFPower(chx, 0);//扫描完毕，关掉输出
  //search the max fwd-rev value data
  temp = diff_data[0];
	for(i=1;i<6;i++){
    if(temp < diff_data[i]){
      temp = diff_data[i];
			bestID = i;
		}
	}

  //添加最小反射判断条件，如果出现反射功率在个位数的情况，判断这个阻抗
  //是不是和最佳驻波阻抗是同一个，并且强行作为最佳点
  uint8_t minRevID = 0;
  temp = rev_data[0];
  for(int i=1; i<6;i++){
    if(temp > rev_data[i]){
      temp = rev_data[i];
      minRevID = i;
    }
    //反射值相等的话，取驻波最大的
    else if(temp == rev_data[i] && diff_data[minRevID] < diff_data[i]){
      temp = rev_data[i];
      minRevID = i;
    }
  }
  //当bestID的反射并不足够小 && minRevID的反射足够小 && 两者并不相等
  if(rev_data[minRevID] < 5 && minRevID != bestID && rev_data[bestID] > 5)
    bestID = minRevID;
  

  printf("测试结果: \n");
  printf("Best ID is %d.\n", bestID);
  printf("rev = %4d\n", rev_data[bestID]);
	printf("fwd = %4d\n", fwd_data[bestID]);

	return 1;
}


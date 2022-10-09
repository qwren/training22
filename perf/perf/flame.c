void longa()
{
	int i,j;
	for(i = 0; i < 1000000; i++)
		j=i; //am I silly or crazy? I feel boring and desperate.
}

void longb()
{
	int i,j;
	for(i = 0; i < 2000000; i++)
		j=i; //am I silly or crazy? I feel boring and desperate.
}

void foo2()
{
	int i, j;
	for(i = 0; i < 5000000; i++)
		j=i; //am I silly or crazy? I feel boring and desperate.
	for(i=0 ; i < 10; i++)
		longb();
}

void foo1()
{
	int i, j;
	for(i = 0; i < 20000000; i++)
		j=i; //am I silly or crazy? I feel boring and desperate.
	for(i = 0; i< 100; i++)
		longa();
}

int main(void)
{
	foo1();
	foo2();
}

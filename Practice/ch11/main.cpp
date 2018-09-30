#include"wordTrans.h"

using namespace std;

int main()
{
	ifstream map_file("map_file.txt");
	ifstream input_file("input_file.txt");
	word_transform(map_file, input_file);
	
	system("pause");
	return 0;
  
}

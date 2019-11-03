#include <iostream>
#include "BinaryReader.h"
#include "ZAR.h"
#include "CMBChunk.h"

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		BinaryReader reader(argv[1]);
		ZARHeader header = reader.read<ZARHeader>();
	}
	else
	{
		BinaryReader reader("C:\\Users\\Andrew\\Desktop\\ExtractedRomFS\\actor\\zelda_cow.zar");
		ZARHeader header = reader.read<ZARHeader>();
	}

	std::cin.get();
	return 0;
}
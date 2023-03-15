#include "md5.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Invalid arguments.\n";
		return 1;
	}

	const auto save_path = argv[1];
	std::fstream savefile(save_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	if (savefile.fail())
	{
		std::cerr << "File open failed.\n";
		return 2;
	}

	const auto filesize = savefile.tellg();
	if (filesize != 0x87430 /* NFSHP PC */ && filesize != 0x87021 /* NFSHPR PC */ && filesize != 0x40820 /* NFSMW PC */)
	{
		std::cerr << "Unrecognised save file.\n";
		return 3;
	}

	savefile.seekg(0, std::ios::beg);

	const auto datasize = (size_t)filesize - 16;
	const auto data = std::make_unique_for_overwrite<char[]>(datasize);
	savefile.read(data.get(), datasize);
	if (savefile.fail())
	{
		std::cerr << "File read failed.\n";
		return 4;
	}

	md5_context ctx;
	md5_init(&ctx);
	md5_update(&ctx, data.get(), datasize);

	uint8_t result[16];
	md5_final(result, &ctx);

	auto tmp = result[0];
	result[0] = result[1];
	result[1] = tmp;
	tmp = result[8];
	result[8] = result[9];
	result[9] = tmp;
	tmp = result[14];
	result[14] = result[15];
	result[15] = tmp;

	savefile.seekp(savefile.tellp()); // necessary when switching from read to write - std::ios::cur with 0 doesn't work because https://github.com/microsoft/STL/issues/3572
	savefile.write((char *)result, 16);
	if (savefile.fail())
	{
		std::cerr << "File write failed.\n";
		return 5;
	}

	return 0;
}

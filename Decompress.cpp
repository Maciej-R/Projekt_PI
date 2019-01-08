#include "Decompress.h"

using namespace boost::iostreams::gzip;
using namespace boost::iostreams;
using std::ios_base;

fs::path decompress(std::string& source) {

	fs::path source_path = fs::current_path().append(source);
	if (!fs::exists(source_path)) {
	
		std::cout << "Wrong path";
		return {};
	
	}

	fs::path target(source_path);
	target.replace_extension("");

	//File preparation
	std::ifstream in(source_path, ios_base::in | ios_base::binary);
	std::ofstream of(target, ios_base::out | ios_base::binary, std::ios::trunc);

	try {
	
		filtering_streambuf<input> in_stream;
		in_stream.push(gzip_decompressor());
		in_stream.push(in);
		boost::iostreams::copy(in_stream, of);

		in.close();
		of.close();

	}
	catch (std::exception& e) {
	
		std::cout << e.what();
	
	}

	return target;

}
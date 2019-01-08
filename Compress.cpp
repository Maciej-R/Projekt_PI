#include "Compress.h"

using namespace boost::iostreams::gzip;
using namespace boost::iostreams;
using std::ios_base;

fs::path compress(fs::path& source) {

	fs::path target = fs::current_path().append("temporary");
	
	//Target destination is directory with source filename and .tar.gz extension
	//in directory for temporary files
	target.append(source.filename());
	if (!fs::exists(target)) fs::create_directory(target);

	//Target destination is source with .tar.gz extension
	//else target = source.replace_filename(source.filename().append(".tar.gz"));

	target.append(source.filename().concat(".gz"));

	//File preparation
	std::ifstream in(source, ios_base::in | ios_base::binary);
	std::ofstream of(target, ios_base::out | ios_base::binary);

	//Using Boost's InputFilter with gzip compression
	filtering_streambuf<input> in_stream;
	in_stream.push(gzip_compressor());
	in_stream.push(in);
	boost::iostreams::copy(in_stream, of);

	in.close();
	of.close();
	
	return target;

}
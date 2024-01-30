#include "Exception.h"

MiExcepcion::MiExcepcion(int line, const char * file) noexcept
	:
		line(line),
		file(file)
{
}

const char* MiExcepcion::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * MiExcepcion::GetType() const noexcept
{
	return "Mi Excepcion";
}

int MiExcepcion::GetLine() const noexcept
{
	 return line;
}

const std::string& MiExcepcion::GetFile() const noexcept
{
	return file;
}

std::string MiExcepcion::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File]" << file << std::endl
		<< "[Line]" << line;
	return oss.str();
}


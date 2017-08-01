#include "exceptions.hpp"


LoadFromFileException::LoadFromFileException(const std::string &file_name)
{
    m_err_msg = "Failed to load file: " + file_name;
}


const char *LoadFromFileException::what() const throw()
{
    return m_err_msg.c_str();
}

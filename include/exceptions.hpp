#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class LoadFromFileException: public std::exception
{
public:
    LoadFromFileException(const std::string &filename);

    const char *what() const throw();

private:
    std::string m_err_msg;
};


#endif // EXCEPTIONS_HPP

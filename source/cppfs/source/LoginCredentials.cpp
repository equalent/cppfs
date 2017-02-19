
#include <cppfs/LoginCredentials.h>

#include <string>
#include <istream>
#include <ostream>

#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>


namespace cppfs
{


LoginCredentials::LoginCredentials()
{
}

LoginCredentials::LoginCredentials(const LoginCredentials & loginCredentials)
: m_values(loginCredentials.m_values)
{
}

LoginCredentials::LoginCredentials(const LoginCredentials && loginCredentials)
: m_values(loginCredentials.m_values)
{
}

LoginCredentials::~LoginCredentials()
{
}

LoginCredentials & LoginCredentials::operator=(const LoginCredentials & loginCredentials)
{
    m_values = loginCredentials.m_values;

    return *this;
}

LoginCredentials & LoginCredentials::operator=(const LoginCredentials && loginCredentials)
{
    m_values = loginCredentials.m_values;

    return *this;
}

bool LoginCredentials::load(const std::string & path)
{
    // Open file
    FileHandle file = fs::open(path);
    if (!file.isFile()) return false;

    // Open input stream
    auto in = file.createInputStream();
    if (!in) return false;

    // Get all keys and values
    std::string line;
    while (std::getline(*in, line))
    {
        auto pos = line.find(": ");

        if (pos != std::string::npos)
        {
            std::string key   = line.substr(0, pos);
            std::string value = line.substr(pos + 2);

            setValue(key, value);
        }
    }

    // Close
    delete in;
    return true;
}

bool LoginCredentials::save(const std::string & path) const
{
    // Open file
    FileHandle file = fs::open(path);

    // Open output stream
    auto out = file.createOutputStream();
    if (!out) return false;

    // Get all keys and values
    for (auto it : m_values)
    {
        std::string key   = it.first;
        std::string value = it.second;

        if (!key.empty() && !value.empty())
        {
            (*out) << key << ": " << value << std::endl;
        }
    }

    // Close
    delete out;
    return true;
}

bool LoginCredentials::isSet(const std::string & key) const
{
    return (m_values.find(key) != m_values.end());
}

std::string LoginCredentials::value(const std::string & key) const
{
    auto it = m_values.find(key);

    if (it != m_values.end())
    {
        return it->second;
    }

    return "";
}

void LoginCredentials::setValue(const std::string & key, const std::string & value)
{
    m_values[key] = value;
}


} // namespace cppfs
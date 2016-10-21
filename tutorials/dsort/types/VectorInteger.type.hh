#ifndef VECTOR_INTEGER_TYPE_HH
#define VECTOR_INTEGER_TYPE_HH 1
#include "integer.type.hh" 
#include <cstdio>
#include <vector>
namespace yml
{
typedef std::vector<integer> VectorInteger;
template<>
bool param_import(VectorInteger& param, const char* filename)
{
    FILE* data = fopen(filename, "rb");
    if (data) 
    {
        size_t count;
        integer value;
        int status = fread(reinterpret_cast<char*>(&count), sizeof(count), 1, data);
        if (status == 1)
        {
            int i;
            for (i = 0 ; i < count ; ++i)
            {
                status = fread(reinterpret_cast<char*>(&value), sizeof(value), 1, data);
                if (status == 1)
                {
                    param.push_back(value);
                }
                else 
                {
                    fclose(data);
                    return false;
                }
            }
        }
        else 
        {
            fclose(data);
            return false;
        }
        fclose(data);
        return true;
    }
    return false;
}


template<>
bool param_export(const VectorInteger& param, const char* filename)
{
    FILE* data = fopen(filename, "wb");
    if (data)
    {
        size_t count = param.size();
        int status;
        status = fwrite(reinterpret_cast<char*>(&count), sizeof(count), 1, data);
        if (status == 1)
        {
            size_t i;
            integer value;
            for(i = 0 ; i < count ; ++i)
            {
                value = param[i];
                status = fwrite(reinterpret_cast<char*>(&value), sizeof(value), 1, data);
                if (status != 1)
                {
                    fclose(data);
                    unlink(filename);
                    return false;
                }
            }
            fclose(data);
            return true;
        }
        else 
        {
            fclose(data);
            unlink(filename);
            return false;
        }
        fclose(data);
        return true;
    }
    return false;
}
}
#endif 

#ifndef __ORSIGTOOLS_VERSION_INCLUDE_H_
#define __ORSIGTOOLS_VERSION_INCLUDE_H_


#include <string>

namespace orx
    {
    namespace igtools
        {
        struct Version
            {
            const unsigned int Major = 0;
            const unsigned int Minor = 0;
            const unsigned int Revision = 1;

            std::string to_string() { return std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Revision) }

            };
        }
    }

#endif // __ORSIGTOOLS_VERSION_INCLUDE_H_












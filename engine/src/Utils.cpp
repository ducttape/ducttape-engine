
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Utils.hpp>

namespace dt {

namespace Utils {

std::string toStdString(const QString qstring) {
    return std::string(qstring.toLocal8Bit().data());
}

std::wstring toWString(const QString qstring) {
    std::string str(qstring.toLocal8Bit().data());
    setlocale(LC_ALL, "chs"); 
    const char* _Source = str.c_str();
    size_t _Dsize = str.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

uint32_t mAutoId = 0;

uint32_t autoId() {
    mAutoId++;
    return mAutoId;
}

} // namespace Utils

} // namespace dt

#include <iostream> 
#include <string> 
#include <cstdio> 
#include <stdarg.h>


int main(){


 auto my_debug = [&](int log_lv, int linenum, char* format,  ...) {
          va_list   arg_ptr;
          va_start(arg_ptr, format);
          std::string log_write ;
          std::string del_temp = "echo \"" , seg = " | " ;
          switch (log_lv) {
            case 1: del_temp += "[Info ] ";  break;
            case 2: del_temp += "[Warn ] ";  break;
            case 3: del_temp += "[Error] ";  break;
            case 4: del_temp += "[Fatal] ";  break;
            default:del_temp += "[Debug] ";
          }
          del_temp += __TIME__; del_temp += seg;
          del_temp += "FILE:"; del_temp += __FILE__; del_temp += seg;
          del_temp += "LINE:"; del_temp += std::to_string (linenum); del_temp += seg;
          std::string log_add;
          char buf[1024] = {0};
          vsprintf(buf, format, arg_ptr);
          va_end(arg_ptr);
          del_temp += buf;
          del_temp += "\" >> AAA";
          pclose(popen(del_temp.c_str(), "r"));
    };

    my_debug(3, __LINE__, "%s:%d、%d、%d、%d、%d","We're five minutes away from the start of the fight",7,6,3,2,1);
return 0;
}

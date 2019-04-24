/* #include <iostream> #include <string> #include <cstdio>
 * Debug [0]、Info[1]、Warn[2]、Error[3]、Fatal[4]
 * like my_debug(3, "hello Captain marvel"， __LINE__)
 * g++  -std=c++11 
 */
auto my_debug = [&](const std::string err_str, int log_lv, int linenum) {
      std::string log_write = err_str;
      std::string del_temp = "echo '" , seg = " | " ;
      switch (log_lv) {
        case 0: del_temp += "[Debug] "; break;
        case 1: del_temp += "[Info ] " ; break;
        case 2: del_temp += "[Warn ] " ; break;
        case 3: del_temp += "[Error] "; break;
        case 4: del_temp += "[Fatal] ";
      }
      del_temp += __TIME__; del_temp += seg;
      del_temp += "FILE:"; del_temp += __FILE__; del_temp += seg;
      del_temp += "LINE:"; del_temp += std::to_string (linenum); del_temp += seg;
      del_temp += err_str;
      del_temp += "' >> AAA";
      pclose(popen(del_temp.c_str(), "r"));
};

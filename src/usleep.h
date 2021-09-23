
//#include <wx/types.h>
// #include <wx/defs.h>


#ifdef __WXMSW__
  void usleep(unsigned int usec) ;
#endif

quint64 microseconds (void) ;
void quickSleep (quint64 uSecs) ;

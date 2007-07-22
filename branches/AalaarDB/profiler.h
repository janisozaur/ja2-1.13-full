#pragma once

#include <vector>
#include <set>
#include <ostream>

//#define PROFILER_ENABLED

#define STRINGIZE(x) #x
#define NAMED_	PERFORMANCE_MARKER(x) PerfMarker x(__FILE__, STRINGIZE(x), __LINE__);
#ifdef PROFILER_ENABLED
#define PERFORMANCE_MARKER PerfMarker MARK(__FILE__, __FUNCTION__, __LINE__);
#else
#define PERFORMANCE_MARKER
#endif
#define END_NAMED_	PERFORMANCE_MARKER(x) x.endMark();

struct PerfDatum
{
    PerfDatum(  const char* const fileName, 
                const char* const functionName,
                const int lineNumber,
                __int64 cycles = 0,
                __int64 calls = 0);
    const char* const   _fileName;
    const char* const   _functionName;
    const int           _lineNumber;
    __int64             _cycles;        //Number of CPU cycles used
    __int64             _calls;         //Number of calls to this function
};

class PerfSort
{
public:
    bool operator()(const PerfDatum& lhs, const PerfDatum& rhs) const;
};

class PerfSort2
{
public:
    bool operator()(const PerfDatum& lhs, const PerfDatum& rhs) const;
};


typedef std::set<PerfDatum, PerfSort>       PERF_LOG_T;
typedef std::vector<PerfDatum* >            PERF_STACK_T;

class PerfManager
{
public:
    ~PerfManager(void);
    static PerfManager* instance();
    void enterFunction( const char* const fileName, 
                        const char* const functionName,
                        const int lineNumber);
    void exitFunction();
    void log(std::ostream &os);
private:
    PerfManager(void);
    __int64 getCPUCount(void) const;
    void calibrate(void);
    int getPercision(const double value) const; //used for formatting percentages
    static PerfManager* _instance;
    PERF_LOG_T      _perfLog;
    PERF_STACK_T    _perfStack;
    __int64         _callTime;  //Time it takes to call the getCPUCount function
    __int64         _totalTime;
    __int64         _lastTime;  //The last time we got hte CPU count.
};

class PerfMarker
{
public:
    PerfMarker( const char* const fileName, 
                const char* const functionName,
                const int lineNumber);
    ~PerfMarker();
    void endMark();
private:
    bool _onStack;
};



#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {  
    long active_jiffies = LinuxParser::ActiveJiffies();
    long idle_jiffies = LinuxParser::IdleJiffies();
    long recently_active = active_jiffies - previously_active;
    long recently_idle = idle_jiffies - previously_idle; 
    long recently_total = recently_active + recently_idle;
    float cpu_usage = (float)recently_active / (float)recently_total;  
    previously_active = active_jiffies;
    previously_idle = idle_jiffies;
    return cpu_usage;
 }
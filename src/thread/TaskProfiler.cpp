/* Copyright 2026 teamprof.net@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

#include "../ArduProfApp.h"
#include "../AppContext.h"
#include "../AppEvent.h"
#include "TaskProfiler.h"


#ifdef RUN_PROFILER

#if defined ARDUINO
#else
#include "./AppLog.h"
static const char *TAG = "pf";
#endif


typedef struct {
    TaskHandle_t xHandle;
    char pcTaskName[configMAX_TASK_NAME_LEN];
    uint32_t ulLastRunTimeCounter;
} TaskLastStats_t;


/////////////////////////////////////////////////////////////////////////////
#define PROFILER_REPORT_INTERVAL 5000   // report every ms


#define MAX_TASK_NUMBER 32  // max number of tasks, including buffer for dynamic create tasks


/////////////////////////////////////////////////////////////////////////////
#define TASK_NAME STR(CLASSNAME)
#define TASK_STACK_SIZE (4096 / sizeof(StackType_t))    // Task stack size
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1)  

/////////////////////////////////////////////////////////////////////////////
void CLASSNAME::start(void)
{
    xTaskCreate(run,  
                TASK_NAME,  
                TASK_STACK_SIZE,  
                NULL,  
                TASK_PRIORITY,  
                NULL); 
}
/* brookesia version
#include "brookesia/lib_utils/thread_profiler.hpp"
#include "../peripheral/pm.h"

#define PROFILER_SAMPLING_MS 500        // sampling every ms

void CLASSNAME::start(void)
{
    static auto& profiler = esp_brookesia::lib_utils::ThreadProfiler::get_instance();
    static auto connection = profiler.connect_profiling_signal(
        [](const esp_brookesia::lib_utils::ThreadProfiler::ProfileSnapshot& snapshot) {
            APP_LOGI(TAG, "--- Snapshot ---");
            for (const auto& task : snapshot.tasks) {
                // Print CPU% and state for our tasks
                // if (task.name == "TaskA" || task.name == "TaskB") 
                {
                    APP_LOGI(TAG, "Task: %16.16s| CPU: %2lu%% | State: %d",
                             task.name.c_str(), task.cpu_percent, (int)task.state);
                }
            }
            APP_LOGI(TAG, "----------------");
        }
    );
    esp_brookesia::lib_utils::ThreadProfiler::ProfilingConfig config;
    config.sampling_duration_ms = PROFILER_SAMPLING_MS;
    config.profiling_interval_ms = PROFILER_REPORT_INTERVAL;
    config.enable_auto_logging = false; // We use our custom callback

    profiler.configure_profiling(config);
    
    static auto scheduler = std::make_shared<esp_brookesia::lib_utils::TaskScheduler>();
    if (!scheduler->start()) {
        APP_LOGE(TAG, "Failed to start TaskScheduler");
        return;
    }

    pm::manage_profiler_precision(true);
    if (!profiler.start_profiling(scheduler)) {
        APP_LOGE(TAG, "Failed to start thread profiler");
    } else {
        APP_LOGI(TAG, "Thread profiler configuration applied and started.");
    }

    // profiler.stop_profiling();
    // manage_profiler_precision(false); 
}
*/

/////////////////////////////////////////////////////////////////////////////
void CLASSNAME::run(void *pvParameters)
{
    bool isDone = false;

    TaskStatus_t pxTaskStatusArray[MAX_TASK_NUMBER];
    TaskLastStats_t pxLastStatsArray[MAX_TASK_NUMBER];
    UBaseType_t uxArraySize = 0;
    UBaseType_t uxLastArraySize = 0;
    uint32_t ulTotalRunTime = 0;
    uint32_t ulLastTotalRunTime = 0;

    uxArraySize = uxTaskGetNumberOfTasks() + 8;
    if(uxArraySize >= MAX_TASK_NUMBER)
    {
#if defined ARDUINO
        LOG_TRACE("Adjust MAX_TASK_NUMBER to bigger than ", uxArraySize, "+ 8");
#else
        APP_LOGE(TAG, "Adjust MAX_TASK_NUMBER to bigger than %d + 8", uxArraySize);
#endif
        isDone = true;
    }

    // Initialize and capture the first baseline snapshot
    uxLastArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulLastTotalRunTime);
    for (UBaseType_t i = 0; i < uxLastArraySize; i++) {
        pxLastStatsArray[i].xHandle = pxTaskStatusArray[i].xHandle;
        strncpy(pxLastStatsArray[i].pcTaskName, pxTaskStatusArray[i].pcTaskName, configMAX_TASK_NAME_LEN);
        pxLastStatsArray[i].ulLastRunTimeCounter = pxTaskStatusArray[i].ulRunTimeCounter;
    }

    auto &ctx = getAppContext();

    while(!isDone)
    {
        vTaskDelay(pdMS_TO_TICKS(PROFILER_REPORT_INTERVAL));

        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
        uint32_t ulDeltaTotalTime = ulTotalRunTime - ulLastTotalRunTime;
            
     
        if (ulDeltaTotalTime > 0) {
            int32_t idle0_percentage = -1;
            int32_t idle1_percentage = -1;

#if defined ARDUINO
            LOG_TRACE("=========================================================");
            LOG_TRACE("FreeRTOS ", tskKERNEL_VERSION_NUMBER, " CPU Usage Monitor:", configNUMBER_OF_CORES);
            LOG_TRACE("=========================================================");
            LOG_TRACE("Task Name       CPU Share (%)");
            LOG_TRACE("---------------------------------------------------------");
#else
            APP_LOGI(TAG, "=========================================================");
            APP_LOGI(TAG, "FreeRTOS %s CPU Usage Monitor: %d core(s)", tskKERNEL_VERSION_NUMBER, configNUMBER_OF_CORES);
            APP_LOGI(TAG, "=========================================================");
            APP_LOGI(TAG, "Task Name       CPU Share (%%)");
            APP_LOGI(TAG, "---------------------------------------------------------");
#endif

            for (UBaseType_t x = 0; x < uxArraySize; x++) {
                uint32_t ulDeltaTaskTime = 0;

                // Match current task with its last recorded metric to find runtime delta
                for (UBaseType_t y = 0; y < uxLastArraySize; y++) {
                    if (pxTaskStatusArray[x].xHandle == pxLastStatsArray[y].xHandle) {
                    // if (strcmp(pxTaskStatusArray[x].pcTaskName, pxLastStatsArray[y].pcTaskName) == 0) {
                        ulDeltaTaskTime = pxTaskStatusArray[x].ulRunTimeCounter - pxLastStatsArray[y].ulLastRunTimeCounter;
                        break;
                    }
                }

                // Fallback to current runtime if no historical match was found
                if (ulDeltaTaskTime == 0 && pxTaskStatusArray[x].ulRunTimeCounter > 0) {
                    ulDeltaTaskTime = pxTaskStatusArray[x].ulRunTimeCounter;
                }

                uint32_t ulStatsAsPercentage = (uint32_t)(((uint64_t)ulDeltaTaskTime * 100) / ulDeltaTotalTime);
                // uint32_t ulStatsAsPercentage = (ulDeltaTaskTime * 100) / ulDeltaTotalTime;
#if defined ARDUINO
                char sBuf[128];
                snprintf(sBuf, sizeof(sBuf), "%-20s %-15lu", pxTaskStatusArray[x].pcTaskName, ulStatsAsPercentage);
                LOG_TRACE(sBuf);
                // LOG_TRACE(pxTaskStatusArray[x].pcTaskName, "\t", ulStatsAsPercentage);
#else
                APP_LOGI(TAG, "%-20s %-15lu", pxTaskStatusArray[x].pcTaskName, ulStatsAsPercentage);
#endif

#if SOC_CPU_CORES_NUM > 1
                bool is_idle0 = (strcmp(pxTaskStatusArray[x].pcTaskName, "IDLE0") == 0);
                bool is_idle1 = (strcmp(pxTaskStatusArray[x].pcTaskName, "IDLE1") == 0);
#elif SOC_CPU_CORES_NUM == 1
                bool is_idle0 = (strcmp(pxTaskStatusArray[x].pcTaskName, "IDLE") == 0);
                bool is_idle1 = false;
#endif

                if (is_idle0) {
                    idle0_percentage = ulStatsAsPercentage;
                } else if (is_idle1) {
                    idle1_percentage = ulStatsAsPercentage;
                }
            }
#if defined ARDUINO
            LOG_TRACE("=========================================================");
#else
            APP_LOGI(TAG, "=========================================================");
#endif


            if(idle0_percentage >= 0)
            {
                if(ctx.threadApp)
                {
                    ctx.threadApp->postEvent(EventSystem, SysCpuLoad, 0, 100-idle0_percentage);
                }

// #if defined ARDUINO
//                 LOG_TRACE("IDLE0: ", idle0_percentage, "%");
// #else
//                 APP_LOGD(TAG, "IDLE0: %d%%", idle0_percentage);
// #endif
            }


#if SOC_CPU_CORES_NUM > 1
            if(idle1_percentage >= 0)
            {
                if(ctx.threadApp)
                {
                    ctx.threadApp->postEvent(EventSystem, SysCpuLoad, 1, 100-idle1_percentage);
                }

// #if defined ARDUINO
//                 LOG_TRACE("IDLE1: ", idle1_percentage, "%");
// #else
//                 APP_LOGD(TAG, "IDLE1: %d%%", idle1_percentage);
// #endif
            }
#endif

            // Update snapshots for the next computation window
            ulLastTotalRunTime = ulTotalRunTime;
            uxLastArraySize = uxArraySize;
            for (UBaseType_t i = 0; i < uxArraySize; i++) {
                pxLastStatsArray[i].xHandle = pxTaskStatusArray[i].xHandle;
                strncpy(pxLastStatsArray[i].pcTaskName, pxTaskStatusArray[i].pcTaskName, configMAX_TASK_NAME_LEN);
                pxLastStatsArray[i].ulLastRunTimeCounter = pxTaskStatusArray[i].ulRunTimeCounter;
            }
        }
    }

    vTaskDelete(NULL);
}

#endif
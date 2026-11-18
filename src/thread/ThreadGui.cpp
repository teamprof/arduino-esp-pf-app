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
#include "./ThreadGui.h"
#include "../AppContext.h"
#include "../AppVersion.h"
#include "../util/os.h"
#include "../pins.h"


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define TEXT_APP_VER_X 0
#define TEXT_APP_VER_Y 0

#define TEXT_CPU_LOAD_X 0
#define TEXT_CPU_LOAD_Y 10

#define PROGRESS_BAR_X 0
#define PROGRESS_BAR_Y 22
#define PROGRESS_BAR_W 128
#define PROGRESS_BAR_H 10


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// #define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// #define NUMFLAKES     10 // Number of snowflakes in the animation example

// #define LOGO_HEIGHT   16
// #define LOGO_WIDTH    16
// static const unsigned char PROGMEM logo_bmp[] =
// { 0b00000000, 0b11000000,
//   0b00000001, 0b11000000,
//   0b00000001, 0b11000000,
//   0b00000011, 0b11100000,
//   0b11110011, 0b11100000,
//   0b11111110, 0b11111000,
//   0b01111110, 0b11111111,
//   0b00110011, 0b10011111,
//   0b00011111, 0b11111100,
//   0b00001101, 0b01110000,
//   0b00011011, 0b10100000,
//   0b00111111, 0b11100000,
//   0b00111111, 0b11110000,
//   0b01111100, 0b11110000,
//   0b01110000, 0b01110000,
//   0b00000000, 0b00110000 };


// void testdrawline() {
//   int16_t i;

//   display.clearDisplay(); // Clear display buffer

//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
//     display.display(); // Update screen with each newly-drawn line
//     delay(1);
//   }
//   for(i=0; i<display.height(); i+=4) {
//     display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   delay(250);

//   display.clearDisplay();

//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   for(i=display.height()-1; i>=0; i-=4) {
//     display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   delay(250);

//   display.clearDisplay();

//   for(i=display.width()-1; i>=0; i-=4) {
//     display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   for(i=display.height()-1; i>=0; i-=4) {
//     display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   delay(250);

//   display.clearDisplay();

//   for(i=0; i<display.height(); i+=4) {
//     display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000); // Pause for 2 seconds
// }

// void testdrawrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=2) {
//     display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=3) {
//     // The INVERSE color is used so rectangles alternate white/black
//     display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawcircle(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
//     display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillcircle(void) {
//   display.clearDisplay();

//   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
//     // The INVERSE color is used so circles alternate white/black
//     display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
//     display.display(); // Update screen with each newly-drawn circle
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawroundrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2-2; i+=2) {
//     display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
//       display.height()/4, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillroundrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2-2; i+=2) {
//     // The INVERSE color is used so round-rects alternate white/black
//     display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
//       display.height()/4, SSD1306_INVERSE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawtriangle(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
//     display.drawTriangle(
//       display.width()/2  , display.height()/2-i,
//       display.width()/2-i, display.height()/2+i,
//       display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfilltriangle(void) {
//   display.clearDisplay();

//   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
//     // The INVERSE color is used so triangles alternate white/black
//     display.fillTriangle(
//       display.width()/2  , display.height()/2-i,
//       display.width()/2-i, display.height()/2+i,
//       display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawchar(void) {
//   display.clearDisplay();

//   display.setTextSize(1);      // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE); // Draw white text
//   display.setCursor(0, 0);     // Start at top-left corner
//   display.cp437(true);         // Use full 256 char 'Code Page 437' font

//   // Not all the characters will fit on the display. This is normal.
//   // Library will draw what it can and the rest will be clipped.
//   for(int16_t i=0; i<256; i++) {
//     if(i == '\n') display.write(' ');
//     else          display.write(i);
//   }

//   display.display();
//   delay(2000);
// }

// void testdrawstyles(void) {
//   display.clearDisplay();

//   display.setTextSize(1);             // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE);        // Draw white text
//   display.setCursor(0,0);             // Start at top-left corner
//   display.println(F("Hello, world!"));

//   display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
//   display.println(3.141592);

//   display.setTextSize(2);             // Draw 2X-scale text
//   display.setTextColor(SSD1306_WHITE);
//   display.print(F("0x")); display.println(0xDEADBEEF, HEX);

//   display.display();
//   delay(2000);
// }

// void testscrolltext(void) {
//   display.clearDisplay();

//   display.setTextSize(2); // Draw 2X-scale text
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(10, 0);
//   display.println(F("scroll"));
//   display.display();      // Show initial text
//   delay(100);

//   // Scroll in various directions, pausing in-between:
//   display.startscrollright(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrollleft(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrolldiagright(0x00, 0x07);
//   delay(2000);
//   display.startscrolldiagleft(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
// }

// void testdrawbitmap(void) {
//   display.clearDisplay();

//   display.drawBitmap(
//     (display.width()  - LOGO_WIDTH ) / 2,
//     (display.height() - LOGO_HEIGHT) / 2,
//     logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
//   display.display();
//   delay(1000);
// }

// #define XPOS   0 // Indexes into the 'icons' array in function below
// #define YPOS   1
// #define DELTAY 2

// void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
//   int8_t f, icons[NUMFLAKES][3];

//   // Initialize 'snowflake' positions
//   for(f=0; f< NUMFLAKES; f++) {
//     icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
//     icons[f][YPOS]   = -LOGO_HEIGHT;
//     icons[f][DELTAY] = random(1, 6);
//     Serial.print(F("x: "));
//     Serial.print(icons[f][XPOS], DEC);
//     Serial.print(F(" y: "));
//     Serial.print(icons[f][YPOS], DEC);
//     Serial.print(F(" dy: "));
//     Serial.println(icons[f][DELTAY], DEC);
//   }

//   for(;;) { // Loop forever...
//     display.clearDisplay(); // Clear the display buffer

//     // Draw each snowflake:
//     for(f=0; f< NUMFLAKES; f++) {
//       display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
//     }

//     display.display(); // Show the display buffer on the screen
//     delay(200);        // Pause for 1/10 second

//     // Then update coordinates of each flake...
//     for(f=0; f< NUMFLAKES; f++) {
//       icons[f][YPOS] += icons[f][DELTAY];
//       // If snowflake is off the bottom of the screen...
//       if (icons[f][YPOS] >= display.height()) {
//         // Reinitialize to a random position, just off the top
//         icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
//         icons[f][YPOS]   = -LOGO_HEIGHT;
//         icons[f][DELTAY] = random(1, 6);
//       }
//     }
//   }
// }



////////////////////////////////////////////////////////////////////////////////////////////

#if defined ARDUPROF_FREERTOS

#if defined ARDUINO_ARCH_RP2040
////////////////////////////////////////////////////////////////////////////////////////////
// Thread for Pi Pico/Pico2 (RP2040/RP2350) FreeRTOS
////////////////////////////////////////////////////////////////////////////////////////////
static constexpr UBaseType_t uxCoreAffinityMask = ((1 << 0)); // task only run on core 0
// static constexpr UBaseType_t uxCoreAffinityMask = ((1 << 1)); // task only run on core 1
// static constexpr uxCoreAffinityMask = ( ( 1 << 0 ) | ( 1 << 2 ) );  // e.g. task can only run on core 0 and core 2

#elif defined ESP_PLATFORM
////////////////////////////////////////////////////////////////////////////////////////////
// Thread for ESP32
////////////////////////////////////////////////////////////////////////////////////////////
#define RUNNING_CORE ARDUINO_RUNNING_CORE
// #define RUNNING_CORE 0 // dedicate core 0 for Thread
// #define RUNNING_CORE 1 // dedicate core 1 for Thread

#endif

#define TASK_NAME STR(CLASSNAME)
#define TASK_STACK_SIZE (4096 / sizeof(StackType_t))
#define TASK_PRIORITY 6    // Priority, (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
#define TASK_QUEUE_SIZE 16 // message queue size for app task
static_assert(TASK_PRIORITY <= configMAX_PRIORITIES, "TASK_PRIORITY exceeds configMAX_PRIORITIES");

#define TASK_INIT_NAME "taskDelayInit"
#define TASK_INIT_STACK_SIZE (4096 / sizeof(StackType_t))
#define TASK_INIT_PRIORITY 0
static_assert(TASK_INIT_PRIORITY <= configMAX_PRIORITIES, "TASK_INIT_PRIORITY exceeds configMAX_PRIORITIES");

static uint8_t ucQueueStorageArea[TASK_QUEUE_SIZE * sizeof(Message)];
static StaticQueue_t xStaticQueue;

static StackType_t xStack[TASK_STACK_SIZE];
static StaticTask_t xTaskBuffer;

///////////////////////////////////////////////////////////////////////
void CLASSNAME::start(void *ctx)
{
    auto core = os::getRunningCore();
    auto prio = os::getPriority();
    auto freeHeapSize = os::getFreeHeapSize();
#if defined ARDUINO
    LOG_TRACE("core", core, ", priority=", prio, ", free heap size=", freeHeapSize);
#else
    APP_LOGD(TAG, "%s: core %d, priority=%d, free heap size=%lu", __func__, core, prio, freeHeapSize);
#endif 

    ThreadBase::start(ctx);

#if defined ARDUINO_ARCH_RP2040
    _taskHandle = xTaskCreateStatic(
        [](void *instance)
        { static_cast<ThreadBase *>(instance)->run(); },
        TASK_NAME,
        TASK_STACK_SIZE,    // This stack size can be checked & adjusted by reading the Stack Highwater
        this,
        TASK_PRIORITY,      // Priority, (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        xStack,
        &xTaskBuffer);
    configASSERT(_taskHandle);
    vTaskCoreAffinitySet(_taskHandle, uxCoreAffinityMask); // Set the core affinity mask for the task, i.e. set task on running core

#elif defined ESP_PLATFORM
    _taskHandle = xTaskCreateStaticPinnedToCore(
        [](void *instance) { static_cast<ThreadBase *>(instance)->run(); },
        TASK_NAME,
        TASK_STACK_SIZE,    // This stack size can be checked & adjusted by reading the Stack Highwater
        this,
        TASK_PRIORITY,      // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        xStack, &xTaskBuffer, RUNNING_CORE);
#endif
}

#elif defined ARDUPROF_MBED && defined ARDUINO_ARCH_MBED_RP2040
////////////////////////////////////////////////////////////////////////////////////////////
// Thread for Pi Pico (RP2040) Mbed OS
////////////////////////////////////////////////////////////////////////////////////////////
#define THREAD_QUEUE_SIZE (64 * EVENTS_EVENT_SIZE) // message queue size for app thread
#include "../util/util.h"
/////////////////////////////////////////////////////////////////////////////
// use static threadQueue instead of heap
static events::EventQueue threadQueue(THREAD_QUEUE_SIZE);

/////////////////////////////////////////////////////////////////////////////
void CLASSNAME::start(void *ctx)
{
    LOG_TRACE("core", get_core_num(), ", ctx=(hex)", DebugLogBase::HEX, (uint32_t)ctx);
    ThreadBase::start(ctx);
}

#endif

/////////////////////////////////////////////////////////////////////////////
CLASSNAME& CLASSNAME::getInstance()
{
    static CLASSNAME instance; // Guaranteed thread-safe initialization in C++ 11+
    return instance;
}

CLASSNAME::CLASSNAME() :
#if defined ARDUPROF_FREERTOS
                        ardufreertos::ThreadBase(TASK_QUEUE_SIZE, ucQueueStorageArea, &xStaticQueue),
                        _timer1Hz("Timer 1Hz", pdMS_TO_TICKS(1000),
                            [](TimerHandle_t xTimer) {
                                getInstance().postEvent(EventSystem, SysSoftwareTimer, 0, (uint32_t)xTimer);
                            }),
#elif defined ARDUPROF_MBED && defined ARDUINO_ARCH_MBED_RP2040
                         ardumbedos::ThreadBase(&threadQueue),
                        //  ardumbedos::ThreadBase(THREAD_QUEUE_SIZE),  // threadQueue is dynamically allocate from heap
                         _timer1Hz(queue(), 1000ms, [](int id)
                                   {
                                        getInstance().postEvent(EventSystem, SysSoftwareTimer, 0, (uint32_t)xTimer);
                                   }),
#endif
                         _oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
                         //  _buttonFac(queue()),
                         //  _debounceTimer(queue(), EventSystem, SysSoftwareTimer),
                         _handlerMap()
{
    _handlerMap = {
        __EVENT_MAP(CLASSNAME, EventApp),
        __EVENT_MAP(CLASSNAME, EventSystem),
        __EVENT_MAP(CLASSNAME, EventNull), // {EventNull, &CLASSNAME::handlerEventNull},
    };
}

void CLASSNAME::setup(void)
{
    auto core = os::getRunningCore();
    auto prio = os::getPriority();
    auto freeHeapSize = os::getFreeHeapSize();
#if defined ARDUINO
    LOG_TRACE("core", core, ", priority=", prio, ", free heap size=", freeHeapSize);
#else
    APP_LOGD(TAG, "%s: core %d, priority=%d, free heap size=%lu", __func__, core, prio, freeHeapSize);
#endif 

    ThreadBase::setup();
    
    if(!initOled())
    {
        _isDone = true;
        return;
    }

    displayAppVersion();
    updateCpuLoad(0, -1);


  // // Draw a single pixel in white
  // display.drawPixel(10, 10, SSD1306_WHITE);

  // // Show the display buffer on the screen. You MUST call display() after
  // // drawing commands to make them visible on screen!
  // display.display();
  // delay(2000);
  // // display.display() is NOT necessary after every single drawing command,
  // // unless that's what you want...rather, you can batch up a bunch of
  // // drawing operations and then update the screen all at once by calling
  // // display.display(). These examples demonstrate both approaches...

  // testdrawline();      // Draw many lines

  // testdrawrect();      // Draw rectangles (outlines)

  // testfillrect();      // Draw rectangles (filled)

  // testdrawcircle();    // Draw circles (outlines)

  // testfillcircle();    // Draw circles (filled)

  // testdrawroundrect(); // Draw rounded rectangles (outlines)

  // testfillroundrect(); // Draw rounded rectangles (filled)

  // testdrawtriangle();  // Draw triangles (outlines)

  // testfilltriangle();  // Draw triangles (filled)

  // testdrawchar();      // Draw characters of the default font

  // testdrawstyles();    // Draw 'stylized' characters

  // testscrolltext();    // Draw scrolling text

  // testdrawbitmap();    // Draw a small bitmap image

  // // Invert and restore display, pausing in-between
  // display.invertDisplay(true);
  // delay(1000);
  // display.invertDisplay(false);
  // delay(1000);

  // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps



    _timer1Hz.start();
    // _timer1Hz.stop();

    // vTaskDelay(pdMS_TO_TICKS(1000));
}

/////////////////////////////////////////////////////////////////////////////
void CLASSNAME::onMessage(const Message &msg)
{
    // LOG_TRACE("event=", msg.event, ", iParam=", msg.iParam, ", uParam=", msg.uParam, ", lParam=", msg.lParam);
    auto func = _handlerMap[msg.event];
    if (func)
    {
        (this->*func)(msg);
    }
    else
    {
        LOG_TRACE("Unsupported event=", msg.event, ", iParam=", msg.iParam, ", uParam=", msg.uParam, ", lParam=", msg.lParam);
    }
}

/////////////////////////////////////////////////////////////////////////////
__EVENT_FUNC_DEFINITION(CLASSNAME, EventApp, msg) // void CLASSNAME::handlerEventApp(const Message &msg)
{
    auto src = static_cast<AppTriggerSource>(msg.iParam);
    switch (src)
    {
    default:
        // DBGLOG(Debug, "Unsupported src=%d, uParam=%u, lParam=%lu", src, msg.uParam, msg.lParam);
        LOG_TRACE("Unsupported src=", src, ", uParam=", msg.uParam, ", lParam=", msg.lParam);
        break;
    }
}

__EVENT_FUNC_DEFINITION(CLASSNAME, EventSystem, msg) // void CLASSNAME::handlerEventSystem(const Message &msg)
{
    // LOG_TRACE("EventSystem(", msg.event, "), iParam = ", msg.iParam, ", uParam = ", msg.uParam, ", lParam = ", msg.lParam);
    enum SystemTriggerSource src = static_cast<SystemTriggerSource>(msg.iParam);
    switch (src)
    {
    case SysSoftwareTimer:
        handlerSoftwareTimer((TimerHandle_t)(msg.lParam));
        break;
    case SysCpuLoad:
    {
        handlerCpuLoad(msg);
        break;
    }
    // case SysButtonClick:
    // {
    //     handlerButtonClick(msg);
    //     break;
    // }
    // case SysButtonDoubleClick:
    // {
    //     handlerButtonDoubleClick(msg);
    //     break;
    // }
    // case SysButtonLongPress:
    // {
    //     handlerButtonLongPress(msg);
    //     break;
    // }
    default:
        LOG_TRACE("unsupported SystemTriggerSource=", src);
        break;
    }
}

// define EventNull handler
__EVENT_FUNC_DEFINITION(CLASSNAME, EventNull, msg) // void CLASSNAME::handlerEventNull(const Message &msg)
{
    LOG_TRACE("EventNull(", msg.event, "), iParam=", msg.iParam, ", uParam=", msg.uParam, ", lParam=", msg.lParam);
}
/////////////////////////////////////////////////////////////////////////////
void CLASSNAME::handlerSoftwareTimer(TimerHandle_t xTimer)
{
    // if (xTimer == _debounceTimer.timer())
    // {
    //     // LOG_TRACE("_debounceTimer::timer()");
    //     // _debounceTimer.onEventTimer();
    // }
    // else
    if (xTimer == _timer1Hz.timer())
    {
        // LOG_TRACE("_timer1Hz");

        static uint16_t percentage = 0;
        updateProgressBar(percentage++);

        // auto ctx = reinterpret_cast<AppContext *>(context());
        // postEvent(ctx->queueMain, EventNull);
    }
    else
    {
        LOG_WARN("unsupported timer handle=(hex)", DebugLogBase::HEX, (uint32_t)(xTimer));
    }
}

void CLASSNAME::handlerCpuLoad(const Message &msg)
{
    int16_t cpu = msg.uParam;
    int32_t load = (int32_t)(msg.lParam);
    updateCpuLoad(cpu, load);
}

/////////////////////////////////////////////////////////////////////////////
bool CLASSNAME::initOled(void)
{
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
//   Wire.setSDA(PIN_I2C_SDA);
//   Wire.setSCL(PIN_I2C_SCL);
//   Wire.begin();

    // Wait for display
    delay(100);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!_oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        LOG_ERROR("init OLED failed");
        return false;
    }

    // Clear the buffer
    _oled.clearDisplay();
    _oled.display();

    _oled.setTextSize(1);      // Normal 1:1 pixel scale
    _oled.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    _oled.setCursor(0, 0);     // Start at top-left corner
    _oled.cp437(true);         // Use full 256 char 'Code Page 437' font

    return true;
}

void CLASSNAME::updateCpuLoad(int16_t cpu, int32_t load)
{
    LOG_TRACE("cpu=", cpu, ",load=", load, "%");

    char sBuf[32];
    if(load >= 0)
    {
        snprintf(sBuf, sizeof(sBuf), "CPU%1d:%3lu%%", cpu, load);
    }
    else
    {
        snprintf(sBuf, sizeof(sBuf), "CPU%1d:---%%", cpu);
    }
    // LOG_TRACE(sBuf);
      
    _oled.setCursor(TEXT_CPU_LOAD_X, TEXT_CPU_LOAD_Y);
    _oled.println(sBuf);
    _oled.display();

}

void CLASSNAME::displayAppVersion(void)
{
    char sBuf[32];

    _oled.setCursor(TEXT_APP_VER_X, TEXT_APP_VER_Y);
    snprintf(sBuf, sizeof(sBuf), "App version %s", AppVersion::getFirmwareVersionString());
    _oled.println(sBuf);
    _oled.display();
}

void CLASSNAME::updateProgressBar(uint16_t percentage)
{
    int x = PROGRESS_BAR_X, y = PROGRESS_BAR_Y;
    int width = PROGRESS_BAR_W, height = PROGRESS_BAR_H;

    // Clamp percentage to 0 - 100
    if (percentage > 100) percentage = 100;

    // Draw outer border
    _oled.drawRect(x, y, width, height, SSD1306_WHITE);

    // Calculate inner dimensions (leaving a 1px border + 1px padding)
    int innerX = x + 2;
    int innerY = y + 2;
    int maxInnerWidth = width - 4;
    int innerHeight = height - 4;

    // Calculate filled width
    int filledWidth = (maxInnerWidth * percentage) / 100;

    // Clear the entire inner area first so shrinking bars erase cleanly
    _oled.fillRect(innerX, innerY, maxInnerWidth, innerHeight, SSD1306_BLACK);

    // Draw the filled progress bar
    if (filledWidth > 0)
    {
      _oled.fillRect(innerX, innerY, filledWidth, innerHeight, SSD1306_WHITE);
    }

    _oled.display();
}

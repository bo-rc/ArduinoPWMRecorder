/* Refer: Wireless RC Adapter v2.1
 *  http://wireless-rc-adapter.github.io
 */

// >>> Input signal modulation (uncomment only one!) <<<
#define PWM_RECEIVER  // Enable Pulse Width Modulation receiver
//#define PPM_RECEIVER  // Enable Pulse Position Modulation receiver

#define CHANNELS 2  // Override the default 6 channels (PPM max: 8, PWM max: 6)
#define HERTZ 60

// >>> Serial-Debug options for troubleshooting <<<
// #define SERIAL_DEBUG  // Enable Serial Debug by uncommenting this line
#define SERIAL_SPD 115200  // Set debug bitrate between 9600-115200 bps (default: 9600)

// >>> Serial-reader options for recording PWM inputs <<<
// #define SERIAL_DEBUG  // Enable Serial Debugger by uncommenting this line
#define SERIAL_READER  // Enable Serial Reader by uncommenting this line
#define PAYLOAD_START 'H'

// Global variables
const uint8_t FLAGS[8] = {1, 2, 4, 8, 16, 32, 64, 128};  // Channel value position flags
volatile uint16_t rc_values[CHANNELS] = {0};  // Actual channel values
uint16_t rc_min_values[CHANNELS], rc_max_values[CHANNELS];  // Calibration data arrays
uint8_t tx_shared_flags = 0;

// Setup function
void setup() {
  initLed();  // Configure and init the leds on the board
  readMem();  // Read calibration data from eeprom

  initSerial();  // Start serial debug output
  rcSetupPwm();  // Attach interrupt timers to PWM pins

  delay(250);  // Give signals some time to settle...
}

void loop() {
  #if defined(PWM_RECEIVER)
    rcProcessPwm();  // Measure channels pwm timing values.
  #endif

  #if defined(SERIAL_READER)
    rcSendChannels();  // Print RAW channel values on serial terminal.
  #endif

  #if defined(SERIAL_DEBUG)
    rcPrintChannels();
  #endif
}
